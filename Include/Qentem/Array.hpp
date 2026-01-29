/**
 * @file Array.hpp
 * @brief Implements a dynamic, contiguous array container for Qentem Engine.
 *
 * Provides a resizable, ownership-aware array for elements of type `Type_T`,
 * supporting efficient insertion, removal, and direct indexed access.
 * Array stores elements in a contiguous memory block, with compile-time
 * expansion policy, optional default initialization, and safe adoption of
 * external buffers. It is designed for high performance, minimal overhead,
 * and compatibility with embedded or native environments.
 *
 * Key features:
 *  - Contiguous memory storage for fast iteration and cache-friendly access.
 *  - Dynamic resizing with compile-time expansion multiplier for predictable growth.
 *  - Move- and copy-aware insertion, assignment, and buffer adoption.
 *  - STL-style iteration with `begin()` and `end()`.
 *  - Optional range construction for uninitialized or default-initialized elements.
 *
 * This container is a core component for managing collections of data
 * throughout the Qentem Engine library.
 *
 * @author Hani
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_ARRAY_H
#define QENTEM_ARRAY_H

#include "Qentem/Reserver.hpp"

namespace Qentem {

/**
 * @brief Dynamic, contiguous array with compile-time growth control.
 *
 * Array provides a flexible, resizable container for elements of type `Type_T`,
 * storing them in contiguous memory for efficient access, iteration, and manipulation.
 *
 * Key features:
 *  - Contiguous storage: elements are stored in a single block of memory.
 *  - Dynamic resizing: grows automatically when inserting beyond capacity.
 *  - Compile-time growth policy: the expansion multiplier determines how capacity scales
 *    when resizing, avoiding runtime branching and overhead.
 *  - Ownership management: supports moving, copying, adopting external buffers, and
 *    safe memory release.
 *  - STL-compatible iteration: provides `begin()` and `end()` methods for const and
 *    mutable access.
 *
 * @tparam Type_T
 *         The element type stored in the array. Must be copyable and/or movable.
 * @tparam Expansion_Multiplier_T
 *         Compile-time factor controlling capacity growth when resizing.
 *         Defaults to 2 (doubling). Must be greater than 1. Different multipliers
 *         result in distinct container types with independent growth policies.
 *
 * Example:
 *   Array<int> numbers;             // Default expansion multiplier (2)
 *   Array<int, 4> large_growth;     // Expands 4× when capacity is exceeded
 */
template <typename Type_T, SizeT Expansion_Multiplier_T = 2>
struct Array {
    QENTEM_INLINE Array() noexcept : storage_{nullptr}, capacity_{0}, size_{0} {};

    explicit Array(SizeT capacity, bool initialize = false) : storage_{nullptr}, capacity_{0}, size_{0} {
        if (capacity != 0) {
            reserve(capacity);

            if (initialize) {
                setSize(Capacity());
                MemoryUtils::ConstructRange(Storage(), (Storage() + Capacity()));
            }
        }
    }

    QENTEM_INLINE Array(Array &&src) noexcept : storage_{src.storage_}, capacity_{src.capacity_}, size_{src.size_} {
        src.clearStorage();
        src.setCapacity(0);
        src.setSize(0);
    }

    QENTEM_INLINE Array(const Array &src) : storage_{nullptr}, capacity_{0}, size_{src.Size()} {
        if (src.IsNotEmpty()) {
            copyArray(src);
        }
    }

    ~Array() {
        MemoryUtils::Destruct(Storage(), End());
        release(Storage(), Capacity());
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T     *old_storage  = Storage();
            const SizeT old_capacity = Capacity();
            const SizeT old_size     = Size();

            setStorage(src.Storage());
            setCapacity(src.Capacity());
            setSize(src.Size());

            src.clearStorage();
            src.setCapacity(0);
            src.setSize(0);

            if (old_storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                MemoryUtils::Destruct(old_storage, (old_storage + old_size));
                release(old_storage, old_capacity);
            }
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_T     *old_storage  = Storage();
            const SizeT old_capacity = Capacity();
            const SizeT old_size     = Size();

            clearStorage();
            setSize(src.Size());

            if (src.IsNotEmpty()) {
                copyArray(src);
            }

            if (old_storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                MemoryUtils::Destruct(old_storage, (old_storage + old_size));
                release(old_storage, old_capacity);
            }
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == 0) {
            setStorage(src.Storage());
            setCapacity(src.Capacity());
            setSize(src.Size());
        } else {
            const SizeT n_size = (Size() + src.Size());

            if (n_size > Capacity()) {
                expand(n_size);
            }

            MemoryUtils::CopyTo((Storage() + Size()), src.Storage(), src.Size());
            release(src.Storage(), src.Capacity());
            setSize(n_size);
        }

        src.clearStorage();
        src.setCapacity(0);
        src.setSize(0);
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            expand(n_size);
        }

        size_ += src.Size();

        Type_T       *storage  = Storage();
        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            MemoryUtils::Construct(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_T &&item) {
        if (Size() == Capacity()) {
            expand((Capacity() | SizeT{1}) * Expansion_Multiplier_T);
        }

        MemoryUtils::Construct((Storage() + Size()), QUtility::Move(item));
        ++size_;
    }

    void operator+=(const Type_T &item) {
        if (Size() == Capacity()) {
            expand((Capacity() | SizeT{1}) * Expansion_Multiplier_T);
        }

        MemoryUtils::Construct((Storage() + Size()), item);
        ++size_;
    }

    /**
     * @brief Adopts (takes ownership of) a pre-reserved buffer and assumes responsibility for its release.
     *
     * @warning The buffer must have been reserved using a compatible strategy,
     *          and must not be accessed elsewhere after adoption.
     *
     * @param ptr       Pointer to the pre-reserved memory block.
     * @param size      The current logical size (number of constructed elements).
     * @param capacity  The total capacity of the buffer (number of available slots).
     */
    void Adopt(Type_T *ptr, SizeT size, SizeT capacity) {
        // Clean up any existing storage
        Reset();

        // Take ownership of the new buffer
        setStorage(ptr);
        setCapacity(capacity);
        setSize(size);
    }

    QENTEM_INLINE void Insert(Array &&src) {
        *this += QUtility::Move(src);
    }

    QENTEM_INLINE void Insert(const Array &src) {
        *this += src;
    }

    QENTEM_INLINE Type_T &Insert(Type_T &&item) {
        const SizeT size = Size();

        *this += QUtility::Move(item);

        return Storage()[size];
    }

    QENTEM_INLINE Type_T &Insert(const Type_T &item) {
        const SizeT size = Size();

        *this += item;

        return Storage()[size];
    }

    QENTEM_INLINE void Clear() noexcept {
        MemoryUtils::Destruct(Storage(), End());

        setSize(0);
    }

    void Reset() noexcept {
        MemoryUtils::Destruct(Storage(), End());
        release(Storage(), Capacity());

        clearStorage();
        setCapacity(0);
        setSize(0);
    }

    Type_T *Detach() noexcept {
        Type_T *tmp = Storage();

        clearStorage();
        setCapacity(0);
        setSize(0);

        return tmp;
    }

    void Reserve(const SizeT capacity, bool initialize = false) {
        if (capacity != 0) {
            Clear();

            if (capacity > Capacity()) {
                expand(capacity);
            } else if (capacity < Capacity()) {
                Reserver::Shrink(Storage(), Capacity(), capacity);
                setCapacity(capacity);
            }

            if (initialize) {
                MemoryUtils::ConstructRange(Storage(), (Storage() + capacity));
                setSize(capacity);
            }
        } else {
            Reset();
        }
    }

    void Resize(SizeT new_capacity) {
        if (new_capacity != 0) {
            if (new_capacity < Capacity()) {
                if (Size() > new_capacity) {
                    MemoryUtils::Destruct((Storage() + new_capacity), End());
                    setSize(new_capacity);
                }

                Reserver::Shrink(Storage(), Capacity(), new_capacity);
                setCapacity(new_capacity);
            } else if (new_capacity > Capacity()) {
                expand(new_capacity);
            }
        } else {
            Reset();
        }
    }

    void ResizeWithDefaultInit(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_T *current = Storage();
            MemoryUtils::ConstructRange((current + Size()), (current + new_size));
            setSize(new_size);
        }
    }

    template <typename... Values_T>
    void ResizeInit(SizeT new_size, Values_T &&...values) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_T *current = Storage();
            MemoryUtils::ConstructRange((current + Size()), (current + new_size),
                                        QUtility::Forward<Values_T>(values)...);
            setSize(new_size);
        }
    }

    QENTEM_INLINE void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            expand(n_size);
        }
    }

    QENTEM_INLINE void Swap(Type_T &item1, Type_T &item2) noexcept {
        QUtility::Swap(item1, item2);
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        if (ascend) {
            QUtility::Sort<true>(Storage(), SizeT{0}, Size());
        } else {
            QUtility::Sort<false>(Storage(), SizeT{0}, Size());
        }
    }

    /**
     * @brief Shrinks the internal buffer to match the current element count.
     *
     * If the reserved capacity exceeds the number of stored elements,
     * this method creates a new buffer with the exact required size,
     * transfers all elements into it, and releases the surplus memory.
     */
    QENTEM_INLINE void Compress() {
        if (Size() < Capacity()) {
            Resize(Size());
        }
    }

    void Drop(const SizeT size) noexcept {
        if (size <= Size()) {
            const SizeT new_size = (Size() - size);

            MemoryUtils::Destruct((Storage() + new_size), End());
            setSize(new_size);
        }
    }

    QENTEM_INLINE void DropFast(const SizeT count) noexcept {
        size_ -= count;
    }

    QENTEM_INLINE Type_T *Storage() noexcept {
        return storage_;
    }

    QENTEM_INLINE const Type_T *Storage() const noexcept {
        return storage_;
    }

    QENTEM_INLINE SizeT Size() const noexcept {
        return size_;
    }

    QENTEM_INLINE SizeT Capacity() const noexcept {
        return capacity_;
    }

    QENTEM_INLINE const Type_T *First() const noexcept {
        return storage_;
    }

    QENTEM_INLINE Type_T *Last() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Type_T *End() const noexcept {
        return (First() + Size());
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    QENTEM_INLINE bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    QENTEM_INLINE const Type_T *begin() const noexcept {
        return First();
    }

    QENTEM_INLINE const Type_T *end() const noexcept {
        return End();
    }

    QENTEM_INLINE Type_T *begin() noexcept {
        return Storage();
    }

    QENTEM_INLINE Type_T *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    QENTEM_INLINE void setStorage(Type_T *ptr) noexcept {
        storage_ = ptr;
    }

    QENTEM_INLINE void clearStorage() noexcept {
        storage_ = nullptr;
    }

    QENTEM_INLINE void setSize(SizeT new_size) noexcept {
        size_ = new_size;
    }

    QENTEM_INLINE void setCapacity(SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    void copyArray(const Array &src) {
        reserve(src.Size());

        Type_T *storage = Storage();

        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            MemoryUtils::Construct(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void expand(SizeT new_capacity) {
        if (Reserver::TryExpand(Storage(), Capacity(), new_capacity)) {
            setCapacity(new_capacity);
            return;
        }

        Type_T     *old_storage  = Storage();
        const SizeT old_capacity = Capacity();

        reserve(new_capacity);

        if (old_storage != nullptr) {
            MemoryUtils::CopyTo(Storage(), old_storage, Size());
            release(old_storage, old_capacity);
        }
    }

    QENTEM_INLINE void reserve(SizeT capacity) {
        setStorage(Reserver::Reserve<Type_T>(capacity));
        setCapacity(capacity);
    }

    QENTEM_INLINE static void release(Type_T *storage, SizeT capacity) {
        Reserver::Release(storage, capacity);
    }

    Type_T *storage_{nullptr};
    SizeT   capacity_{0};
    SizeT   size_{0};
};

} // namespace Qentem

#endif
