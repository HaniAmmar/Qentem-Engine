/**
 * @file QPool.hpp
 * @brief Lightweight, alignment-aware object pool with page-sized memory blocks and LIFO reuse.
 *
 * A fixed-size object pool for efficient allocation and reuse of objects.
 *
 * This pool allocates memory in fixed-size, page-aligned blocks, each containing
 * multiple items of type `Type_T`. Objects are constructed lazily, on demand,
 * the first time they are retrieved via `Get()`. Returned objects are recycled
 * without destruction and reused in subsequent allocations, reducing heap churn
 * and improving performance in high-frequency allocation scenarios.
 *
 * Key Features:
 * - Constant-time allocation and recycling
 * - Lazy construction: objects are constructed only upon first retrieval
 * - Memory allocation bypasses libc via `SystemMemory::Reserve`
 * - Page-aware layout: each pool block contains a fixed number of aligned objects
 * - Grows automatically when exhausted, or explicitly via `Expand()`
 *
 * Template Parameters:
 * @tparam Type_T        The object type managed by the pool
 * @tparam TYPE_ALIGN_T  Required alignment for each object (defaults to alignof(Type_T))
 * @tparam DESTRUCT_CB_T Optional destruction callback invoked when the pool is destroyed
 *
 * Example:
 * @code
 * QPool<MyStruct> pool;
 * MyStruct* ptr = pool.Get();
 * pool.Recycle(ptr);
 * @endcode
 *
 * @note The pool retains ownership of all objects at all times.
 *       Destructors are invoked only when the pool itself is destroyed.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_OBJECT_POOL_H
#define QENTEM_OBJECT_POOL_H

#include "Qentem/SystemMemory.hpp"
#include "Qentem/MemoryUtils.hpp"

#ifdef QENTEM_ENABLE_MEMORY_RECORD
#include "Qentem/MemoryRecord.hpp"
#endif

namespace Qentem {

/**
 * @brief Callback type for constructing an object in the pool.
 *
 * Invoked when a previously unconstructed item is retrieved via `Get()`.
 * The callback is responsible for initializing the object in-place at the
 * provided pointer, using the forwarded constructor arguments.
 *
 * @tparam Type_T  The type of object to construct.
 * @tparam Args_T  Constructor argument types.
 */
template <typename Type_T, typename... Args_T>
using QPoolConstructCB_T = void (*)(Type_T *, Args_T...);

/**
 * @brief Callback type for destroying an object in the pool.
 *
 * Invoked by the pool when objects are destructed, either during
 * `Reset()` or the pool's destructor. The callback must destroy
 * the object at the given pointer but should not free the memory,
 * as the pool retains ownership of the storage.
 *
 * @tparam Type_T The type of object to destroy.
 */
template <typename Type_T>
using QPoolDestructCB_T = void (*)(Type_T *);

template <typename Type_T, SizeT32 TYPE_ALIGN_T = alignof(Type_T), QPoolDestructCB_T<Type_T> DESTRUCT_CB_T = nullptr>
struct QPool {
    QPool(const QPool &)            = delete; //!< Non-copyable.
    QPool &operator=(const QPool &) = delete; //!< Non-copyable.

    static_assert((TYPE_ALIGN_T >= alignof(Type_T)), "TYPE_ALIGN_T must be >= alignof(Type_T)");

    /**
     * @brief A single pool item — wraps storage for `Type_T` and a free-list link.
     *
     * The contained object is constructed lazily on first use and remains
     * constructed until the pool itself is destroyed.
     */
    struct alignas(TYPE_ALIGN_T) Item {
        Type_T Storage; //!< In-place storage for `Type_T` (constructed lazily).
        Item  *Next;    //!< Link to the next free item (LIFO order).
    };

    /// Size of a single item, rounded up to the required alignment.
    static constexpr SizeT32 ITEM_SIZE = QUtility::AlignUp<SizeT32>(sizeof(Item), TYPE_ALIGN_T);
    /**
     * @brief A single contiguous allocation block backing the pool.
     *
     * Each block contains a `Pool` header followed by a fixed number of `Item`s.
     * Blocks are linked together to form the pool's backing storage.
     */
    struct Pool {
        Pool *Next; //!< Link to the next allocated pool block.
    };

    /**
     * Pool header is padded so that the first Item is always aligned
     * to TYPE_ALIGN_T. Do not assume sizeof(Pool) is sufficient.
     */
    static constexpr SizeT32 POOL_HEADER_SIZE = QUtility::AlignUp<SizeT32>(sizeof(Pool), TYPE_ALIGN_T);

    /**
     * @brief Constructs a pool, optionally preallocating storage blocks.
     *
     * @param count Number of pool blocks to preallocate.
     *
     * If `count` is zero, no storage is allocated eagerly and the pool
     * expands automatically on first use.
     */
    explicit QPool(SizeT32 count = 0) noexcept {
        if (count != 0) {
            expand(count);
        }
    }

    /**
     * @brief Move constructor — transfers pool state and managed storage.
     *
     * After the move, the source pool is left in an empty, non-owning state
     * and will not destroy or release any storage.
     */
    QPool(QPool &&src) noexcept
        : list_{src.list_}, storage_{src.storage_}, in_use_{src.in_use_}, constructed_{src.constructed_} {
        src.list_        = nullptr;
        src.storage_     = nullptr;
        src.in_use_      = 0;
        src.constructed_ = 0;
    }

    /**
     * @brief Move assignment — releases current storage and transfers pool state.
     *
     * Any storage currently managed by this pool is destroyed before the state
     * of the source pool is adopted. The source pool is left empty.
     */
    QPool &operator=(QPool &&src) noexcept {
        if (this != &src) {
            destruct();

            list_        = src.list_;
            storage_     = src.storage_;
            in_use_      = src.in_use_;
            constructed_ = src.constructed_;

            src.list_        = nullptr;
            src.storage_     = nullptr;
            src.in_use_      = 0;
            src.constructed_ = 0;
        }

        return *this;
    }

    /**
     * @brief Destructor — destroys all constructed objects and releases all pool storage.
     *
     * All objects ever constructed by the pool are destructed, including those
     * currently checked out. Ownership and lifetime are fully controlled by the pool.
     */
    ~QPool() {
        destruct();
    }

    /**
     * @brief Retrieves an object from the pool, constructing it if needed.
     *
     * Allocates a new pool block if no free items are available. The returned object
     * is taken from the free list.
     *
     * If this retrieval exceeds the number of objects previously constructed,
     * the object is constructed in-place using the forwarded arguments.
     *
     * @tparam Args_T Argument types for `Type_T`'s constructor.
     * @param args    Constructor arguments forwarded to `Type_T`.
     * @return Pointer to a constructed object owned by the pool.
     */
    template <typename... Args_T>
    Type_T *Get(Args_T &&...args) {
        if (list_ == nullptr) {
            expand(1U);
        }

        Item *item = list_;
        list_      = item->Next;

        ++in_use_;

        if (in_use_ > constructed_) {
            MemoryUtils::Construct(&(item->Storage), QUtility::Forward<Args_T>(args)...);
            ++constructed_;
        }

        return &(item->Storage);
    }

    /**
     * @brief Retrieves an object from the pool using a custom construction callback.
     *
     * Behaves identically to `Get()` except that object construction is delegated
     * to the provided callback when the object has not been constructed before.
     *
     * @tparam Args_T Argument types forwarded to the construction callback.
     * @param construct_cb Callback responsible for constructing the object.
     * @param args         Arguments forwarded to the callback.
     * @return Pointer to a constructed object owned by the pool.
     */
    template <typename... Args_T>
    Type_T *Get(QPoolConstructCB_T<Type_T, Args_T...> construct_cb, Args_T &&...args) {
        if (list_ == nullptr) {
            expand(1U);
        }

        Item *item = list_;
        list_      = item->Next;

        ++in_use_;

        if (in_use_ > constructed_) {
            construct_cb(&(item->Storage), QUtility::Forward<Args_T>(args)...);
            ++constructed_;
        }

        return &(item->Storage);
    }

    /**
     * @brief Returns an object to the pool without invoking its destructor.
     *
     * @param item Pointer to an object previously obtained via `Get()`.
     *
     * The object remains fully constructed and is placed back into the free list
     * for reuse by future allocation requests.
     *
     * The in-use counter is decremented accordingly.
     */
    void Recycle(Type_T *item) noexcept {
        Item *r_item = reinterpret_cast<Item *>(item);
        r_item->Next = list_;
        list_        = r_item;
        --in_use_;
    }

    /**
     * @brief Returns the number of objects currently checked out from the pool.
     *
     * This count reflects objects obtained via `Get()` that have not yet been
     * returned through `Recycle()`.
     */
    SizeT32 InUseCount() const noexcept {
        return in_use_;
    }

    /**
     * @brief Returns the total number of objects ever constructed by the pool.
     *
     * This value includes both in-use objects and recycled objects that remain
     * constructed and available for reuse.
     */
    SizeT32 ConstructedCount() const noexcept {
        return constructed_;
    }

    /**
     * @brief Preallocates one or more pool storage blocks.
     *
     * @param count Number of additional pool blocks to reserve.
     *
     * This does not construct objects eagerly; it only expands the pool's
     * backing storage and free list capacity.
     */
    void Expand(SizeT32 count) {
        expand(count);
    }

    /**
     * @brief Resets the pool by destroying all constructed objects and releasing memory.
     *
     * After this call, the pool is returned to its initial empty state.
     * All backing storage is freed, and internal counters are reset.
     */
    void Reset() {
        destruct();
        list_   = nullptr;
        in_use_ = 0;
    }

  private:
    /**
     * @brief Releases all allocated memory and destroys constructed objects.
     *
     * Traverses all allocated pool blocks and invokes destructors only for objects
     * that were actually constructed. Destruction proceeds block by block, starting
     * with the most recently allocated block.
     *
     * For the first block, only the remaining subset of constructed objects
     * (constructed_ % storage_size_) is destroyed. All earlier blocks are known
     * to be fully constructed and are therefore fully destructed.
     *
     * After destruction, each pool block is released back to the system.
     */
    void destruct() {
        SizeT32 count = (constructed_ % storage_size_);

        while (storage_ != nullptr) {
            Pool *next_pool = storage_->Next;
            Item *items     = reinterpret_cast<Item *>(reinterpret_cast<unsigned char *>(storage_) + POOL_HEADER_SIZE);

            SizeT32 index = 0;

            while (index < count) {
                if constexpr (DESTRUCT_CB_T == nullptr) {
                    MemoryUtils::Destruct(&(items[index].Storage));
                } else {
                    DESTRUCT_CB_T(&(items[index].Storage));
                }

                ++index;
            }

            constructed_ -= count;
            count = storage_size_;

            SystemMemory::Release(storage_, reserve_size_);

#ifdef QENTEM_ENABLE_MEMORY_RECORD
            MemoryRecord::Released(reserve_size_);
            MemoryRecord::ReleasedBlock(reserve_size_);
#endif

            storage_ = next_pool;
        }
    }

    /**
     * @brief Allocates and links new pool blocks, populating the free list.
     *
     * Each allocated block is reserved as a contiguous memory region consisting of
     * a pool header followed immediately by an array of items. The items are pushed
     * onto the free list in reverse index order, forming a LIFO free-list.
     *
     * Newly allocated pool blocks are linked into the internal storage chain and
     * fully owned by the pool for its entire lifetime.
     *
     * @param count Number of new pool blocks to allocate.
     */
    void expand(SizeT32 count) {
        do {
            Pool *pool = static_cast<Pool *>(SystemMemory::Reserve(reserve_size_));

#ifdef QENTEM_ENABLE_MEMORY_RECORD
            MemoryRecord::ReservedBlock(reserve_size_);
            MemoryRecord::Reserved(reserve_size_);
#endif

            Item   *items = reinterpret_cast<Item *>(reinterpret_cast<unsigned char *>(pool) + POOL_HEADER_SIZE);
            SizeT32 index = storage_size_;

            // Link the newly allocated pool block into the storage chain
            pool->Next = storage_;
            storage_   = pool;

            // Populate the free list with all items from this block (LIFO order)
            do {
                --index;
                Item &item = items[index];
                item.Next  = list_;
                list_      = &item;
            } while (index != 0);

            --count;
        } while (count != 0);
    }

    /**
     * @brief Calculates how many items fit into a single storage block.
     *
     * The count is derived from the system page size after subtracting the
     * aligned pool header. The remaining space is divided into fixed-size,
     * properly aligned items.
     *
     * @return Number of items that fit in one storage block.
     */
    static SizeT32 getStorageCount() noexcept {
        const SizeT32 payload = (SystemMemory::GetPageSize() - POOL_HEADER_SIZE);

        return (payload / ITEM_SIZE);
    }

    /**
     * @brief Computes the total size, in bytes, of a single storage block.
     *
     * The storage block consists of:
     *  - one pointer-sized header used for internal bookkeeping
     *  - a contiguous array of items determined by getStorageCount()
     *
     * When system memory fallback is disabled, the resulting size is rounded up
     * to the nearest system page boundary to ensure page-aligned allocation.
     *
     * @return Size in bytes to reserve for one storage block.
     */
    static SystemLong getStorageSize() noexcept {
        // static_assert((STORAGE_SIZE > 0), "Object size exceeds page size — cannot fit even one item.");

        SystemLong reserve_size = static_cast<SystemLong>(POOL_HEADER_SIZE + (ITEM_SIZE * getStorageCount()));

#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        const SizeT32 page_size    = SystemMemory::GetPageSize();
        const SizeT32 page_size_m1 = (page_size - 1U);

        if (reserve_size < page_size) {
            // Ensure at least one full page is reserved
            reserve_size = page_size;
        } else {
            // Round up to the next page boundary
            reserve_size += page_size_m1;
            reserve_size &= ~page_size_m1;
        }
#endif

        return reserve_size;
    }

    inline static SystemLong reserve_size_{getStorageSize()};
    inline static SizeT32    storage_size_{getStorageCount()};

    Item   *list_{nullptr};    ///< Head of the free-list (LIFO order).
    Pool   *storage_{nullptr}; ///< Singly-linked list of owned memory blocks backing the pool.
    SizeT32 in_use_{0};        ///< Number of objects currently in use.
    SizeT32 constructed_{0};   ///< Total number of objects constructed over the lifetime of the pool.
};

} // namespace Qentem

#endif
