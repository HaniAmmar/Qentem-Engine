/**
 * @file Deque.hpp
 * @brief Double-ended queue with power-of-two capacity, backed by a ring buffer.
 *
 * This Deque implements the usual front/back push and pop in O(1) time via
 * power-of-two ring buffering. In scenarios where you must store “absolute”
 * indices (e.g. for a name→index map) and still have them remain valid after
 * arbitrary front-pops, you can enable the built-in pop-count biasing:
 *
 *   - pop_count_ tracks the total number of times Dequeue() has been called
 *     since the last buffer resized or reset.
 *   - To record an element’s absolute index on insertion, compute:
 *         absolute = current_size + PopCount();
 *   - To recover its current logical index for Get(), compute:
 *         logical = absolute - PopCount();
 *
 * Because pop_count_ is reset to zero whenever the underlying storage is refreshed
 * (during reserve expansion, resizing, or reset), all biased indices remain consistent
 * without requiring a full rebuild of stored mappings. This design ensures
 * true O(1) insertion, eviction, and index lookup — even in dynamic table scenarios.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_DEQUE_HPP
#define QENTEM_DEQUE_HPP

#include "Reserver.hpp"

namespace Qentem {

/**
 *
 * @tparam Type_T The element type stored in the deque.
 */
template <typename Type_T>
struct Deque {
    /**
     * @brief Default constructor: creates an empty deque.
     */
    QENTEM_INLINE Deque() noexcept = default;

    /**
     * @brief Deleted copy constructor: Deque is non-copyable.
     */
    Deque(const Deque &) = delete;

    /**
     * @brief Deleted copy assignment: Deque is non-copyable.
     */
    Deque &operator=(const Deque &) = delete;

    /**
     * @brief Constructs with an initial capacity (rounded to power-of-two).
     * @param cap Minimum number of elements to support without resizing.
     */
    QENTEM_INLINE explicit Deque(SizeT capacity) {
        reserve(capacity);
    }

    /**
     * @brief Move constructor: takes ownership of another deque's storage.
     * @param src Source deque to move from.
     */
    Deque(Deque &&src) noexcept
        : storage_{src.storage_}, size_{src.size_}, head_{src.head_}, pop_count_{src.pop_count_},
          capacity_{src.capacity_} {
        src.clearStorage();
        src.setSize(0);
        src.setHead(0);
        src.setPopCount(0);
        src.setCapacity(0);
    }

    /**
     * @brief Move assignment: disposes current contents and takes over src's.
     * @param src Source deque to move from.
     * @return Reference to this deque.
     */
    Deque &operator=(Deque &&src) noexcept {
        if (this != &src) {
            this->~Deque();
            MemoryUtils::Construct(this, QUtility::Move(src));
        }

        return *this;
    }

    /**
     * @brief Destructor: disposes all elements and release buffer.
     */
    QENTEM_INLINE ~Deque() {
        dispose();
    }

    /**
     * @brief Inserts a new element at the back by moving the provided rvalue.
     *
     * Moves `item` into the next free slot at the tail. If the deque is full,
     * the buffer capacity is doubled before insertion.
     *
     * @param item Rvalue of type `Type_T` to be moved into the deque.
     *
     * @complexity Amortized O(1), occasional O(n) when resize occurs.
     */
    void operator+=(Type_T &&item) {
        if (IsFull()) {
            resize(capacity_ * SizeT{2});
        }

        MemoryUtils::Construct((Storage() + tail()), QUtility::Move(item));

        setSize(Size() + SizeT{1});
    }

    /**
     * @brief Inserts a copy of the given element at the back of the deque.
     *
     * If the deque is full, its capacity is automatically doubled (power-of-two)
     * before inserting. The element is then copy-constructed into the tail slot.
     *
     * @complexity Amortized O(1), with occasional O(n) when a resize occurs.
     */
    void operator+=(const Type_T &item) {
        if (IsFull()) {
            resize(capacity_ * SizeT{2});
        }

        MemoryUtils::Construct((Storage() + tail()), item);

        setSize(Size() + SizeT{1});
    }

    /**
     * @brief Inserts a new element at the back by moving the provided rvalue.
     *
     * Moves `item` into the next free slot at the tail. If the deque is full,
     * the buffer capacity is doubled before insertion. Returns a reference
     * to the newly inserted element.
     *
     * @param item Rvalue of type `Type_T` to be moved into the deque.
     * @return Reference to the element now stored at the back.
     *
     * @complexity Amortized O(1), occasional O(n) when resize occurs.
     */
    QENTEM_INLINE Type_T &Insert(Type_T &&item) {
        const SizeT index = tail();

        *this += QUtility::Move(item);

        return Storage()[index];
    }

    /**
     * @brief Inserts a copy of the given element at the back of the deque.
     *
     * If the deque is full, its capacity is automatically doubled (power-of-two)
     * before inserting. The element is then copy-constructed into the tail slot.
     *
     * @param item  Const reference to the element to copy into the deque.
     *
     * @complexity Amortized O(1), with occasional O(n) when a resize occurs.
     */
    QENTEM_INLINE Type_T &Insert(const Type_T &item) {
        const SizeT index = tail();

        *this += item;

        return Storage()[index];
    }

    /**
     * @brief Removes and destroys the element at the front of the deque (FIFO).
     *
     * If the deque is non-empty, this destroys the object at the current head,
     * advances the head index by one (wrapping around via bitmask), and
     * decrements the logical size. Calling on an empty deque is a no-op.
     *
     * @complexity Amortized O(1)
     */
    QENTEM_INLINE void Dequeue() {
        if (IsNotEmpty()) {
            MemoryUtils::Destruct(First());

            setHead((head() + SizeT{1}) & (Capacity() - SizeT{1}));
            --size_;
            ++pop_count_;
        }
    }

    /**
     * @brief Removes and destroys the element at the back of the deque (LIFO).
     *
     * If the deque contains at least one element, this invokes the destructor
     * of the element at the tail position and decrements the logical size.
     * Calling on an empty deque has no effect.
     *
     * @complexity O(1)
     */
    QENTEM_INLINE void DequeueBack() {
        if (IsNotEmpty()) {
            // Destroy the very last element in place
            MemoryUtils::Destruct(Last());
            // Decrement logical size
            --size_;
        }
    }

    /**
     * @brief Provides a pointer to the first (oldest) element in the deque.
     *
     * Returns a direct pointer to the element at the front without
     * removing it. If the deque is empty, returns nullptr. The pointer remains
     * valid until the deque is modified (e.g., by insertion, removal, or resize).
     *
     * @complexity O(1)
     */
    QENTEM_INLINE const Type_T *First() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + head());
        }

        return nullptr;
    }

    /**
     * @brief Provides a pointer to the first (oldest) element in the deque.
     *
     * Returns a direct, mutable pointer to the element at the front without
     * removing it. If the deque is empty, returns nullptr. The pointer remains
     * valid until the deque is modified (e.g., by insertion, removal, or resize).
     *
     * @complexity O(1)
     */
    QENTEM_INLINE Type_T *First() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + head());
        }

        return nullptr;
    }

    /**
     * @brief Provides a pointer to the last (most recently added) element.
     *
     * Returns a direct pointer to the back of the deque without removing it.
     * If the deque is empty, returns nullptr. The pointer remains valid
     * until the deque is modified (e.g., by insertion, removal, or resize).
     *
     * @complexity O(1)
     */
    QENTEM_INLINE const Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + ((head() + (Size() - SizeT{1})) & (Capacity() - SizeT{1})));
        }

        return nullptr;
    }

    /**
     * @brief Provides a pointer to the last (most recently added) element.
     *
     * Returns a direct, mutable pointer to the back of the deque without removing it.
     * If the deque is empty, returns nullptr. The pointer remains valid
     *until the deque is modified (e.g., by insertion, removal, or resize).
     *
     * @complexity O(1)
     */
    QENTEM_INLINE Type_T *Last() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + ((head() + (Size() - SizeT{1})) & (Capacity() - SizeT{1})));
        }

        return nullptr;
    }

    /**
     * @brief Returns a pointer to the element at logical index, or nullptr if out of range.
     * @param index Offset from front (0-based).
     * @return Pointer to Storage()[(head + index) & mask] if index < Size(), else nullptr.
     */
    QENTEM_INLINE Type_T *Get(SizeT index) noexcept {
        if (index < Size()) {
            return (Storage() + ((head() + index) & (Capacity() - SizeT{1})));
        }

        return nullptr;
    }

    /**
     * @brief Returns a pointer to the element at the specified logical index.
     *
     * Provides direct access without removal. Computes the physical position via
     * wraparound: (head + index) & (capacity - 1). If `index` is out of range,
     * returns nullptr.
     *
     * @param index Zero-based offset from the front of the deque.
     * @return Pointer to the element at that index, or nullptr if index ≥ Size().
     *
     * @complexity O(1)
     */
    QENTEM_INLINE const Type_T *Get(SizeT index) const noexcept {
        if (index < Size()) {
            return (Storage() + ((head() + index) & (Capacity() - SizeT{1})));
        }

        return nullptr;
    }

    /**
     * @brief Returns a pointer to the element at the specified logical index.
     *
     * Provides direct access without removal. Computes the physical position via
     * wraparound: (head + index) & (capacity - 1). If `index` is out of range,
     * returns nullptr.
     *
     * @param index Zero-based offset from the front of the deque.
     * @return Pointer to the element at that index, or nullptr if index ≥ Size().
     */
    QENTEM_INLINE void Expect(SizeT additional) {
        SizeT needed = (additional + Size());

        if (needed > Capacity()) {
            resize(needed);
        }
    }

    /**
     * @brief Adjusts capacity or logical size:
     *        - new_size == 0: full reset.
     *        - new_size >= Size(): grow capacity if needed, leave Size unchanged.
     *        - new_size <  Size(): shrink Size and destroy excess elements.
     * @param new_size Target size or capacity.
     */
    void Resize(SizeT new_size) {
        if (new_size != 0) {
            if (new_size < Size()) {
                // Shrinking: destroy elements beyond new_size
                const SizeT start = ((head() + new_size) & (Capacity() - SizeT{1}));
                const SizeT count = (Size() - new_size);
                const SizeT end   = (start + count);

                if (count != 0) {
                    if (end <= Capacity()) {
                        MemoryUtils::Destruct((Storage() + start), (Storage() + end));
                    } else {
                        // Wrapped range
                        MemoryUtils::Destruct((Storage() + start), (Storage() + Capacity()));
                        MemoryUtils::Destruct(Storage(), (Storage() + (count - (Capacity() - start))));
                    }
                }

                setSize(new_size);
            }

            resize(new_size);
        } else {
            // Fully clear and return reserved memory
            Reset();
        }
    }

    /**
     * @brief Resizes the deque and default-initializes any newly added slots.
     *
     * Behavior:
     *  - If new_size == 0: fully resets the deque and releases all reserved memory.
     *  - If new_size > current Size(): expands capacity if needed, default-constructs
     *    elements in the new slots, and updates the logical size.
     *  - If new_size <= current Size(): behaves like a shrink-only Resize, destroying
     *    elements beyond new_size and updating logical size.
     *
     * @param new_size Desired new logical size.
     */
    void ResizeWithDefaultInit(SizeT new_size) {
        // 1) Perform capacity-adjusting Resize without growing logical size
        Resize(new_size);

        // 2) Only default-initialize when actually grown beyond old Size()
        if (new_size > Size()) {
            // Number of new slots that need initialization
            const SizeT count = (new_size - Size());
            // Starting position for new elements (tail of old data)
            const SizeT start = tail();
            // Unwrapped end index (may exceed Capacity())
            const SizeT end = (tail() + count);

            // 3) Initialize contiguous range
            if (end <= Capacity()) {
                MemoryUtils::ConstructRange((Storage() + start), (Storage() + end));
            }

            // 4) Update logical size to include the new default-initialized elements
            setSize(new_size);
        }
    }

    /**
     * @brief Shrinks the internal buffer to match the current element count.
     *
     * If the reserved capacity exceeds the number of stored elements,
     * this method requests a tighter buffer sized exactly to fit,
     * moves all elements into the new region (resetting head to zero),
     * and returns any surplus memory.
     */
    void Compress() {
        const SizeT n_size = MemoryUtils::AlignToPow2(Size());

        if (n_size < Capacity()) {
            Resize(n_size);
        }
    }

    /**
     * @brief Reserves a fresh buffer of exactly `cap` capacity, discarding existing data.
     *
     * This operation resets the deque to an empty state, releases any previously
     * reserved memory, and secures a new buffer with a power-of-two capacity ≥ `cap`.
     *
     * @param cap Minimum number of elements the new buffer should support.
     * @post Size() == 0 and Capacity() == AlignToPow2(cap)
     */
    QENTEM_INLINE void Reserve(SizeT cap) {
        // Release old buffer and reset state
        Reset();

        // Acquire new buffer with requested minimum capacity
        reserve(cap);
    }

    /**
     * @brief Destructs all elements in the deque while preserving the underlying buffer.
     *
     * Calls the destructor on each element from head to tail, correctly handling wrap-around.
     * The reserved memory is retained for future use—no memory is released or requested.
     * After Clear(), Size() == 0 and Head() == 0, but Capacity() remains unchanged.
     *
     * @complexity O(n), where n is the current number of elements.
     */
    QENTEM_INLINE void Clear() {
        clear();
        setSize(0);
        setPopCount(0);
        setHead(0);
    }

    /**
     * @brief Resets the deque to an empty state and returns its reserved buffer.
     *
     * All elements are destroyed, and the internal memory block is released
     * back to the reserver or system. The logical size and position are reset.
     */
    QENTEM_INLINE void Reset() {
        dispose();
        clearStorage();
        setSize(0);
        setHead(0);
        setPopCount(0);
        setCapacity(0);
    }

    // Accessors
    QENTEM_INLINE Type_T *Storage() const noexcept {
        return storage_;
    }

    QENTEM_INLINE SizeT Capacity() const noexcept {
        return capacity_;
    }

    QENTEM_INLINE SizeT Size() const noexcept {
        return size_;
    }

    /**
     * @brief Returns the number of elements removed from the front since the last buffer refresh.
     *
     * This pop-count is useful for tracking relative positions or maintaining offset-based
     * mappings that remain valid across pop-front operations. The counter resets to zero
     * whenever the internal storage is refreshed—either by resizing, reserving new space,
     * or a full reset—ensuring consistent indexing semantics.
     *
     * @return The current value of pop_count_.
     */
    QENTEM_INLINE SizeT PopCount() const noexcept {
        return pop_count_;
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return Size() == 0;
    }

    QENTEM_INLINE bool IsNotEmpty() const noexcept {
        return Size() != 0;
    }

    QENTEM_INLINE bool IsFull() const noexcept {
        return (Size() == Capacity());
    }

  private:
    QENTEM_INLINE SizeT head() const noexcept {
        return head_;
    }

    /**
     * @brief Computes the next insertion index (tail) = (head+size) & (capacity-1).
     */
    QENTEM_INLINE SizeT tail() const noexcept {
        return ((head() + Size()) & (Capacity() - SizeT{1}));
    }

    QENTEM_INLINE void setSize(SizeT new_size) noexcept {
        size_ = new_size;
    }

    QENTEM_INLINE void setHead(SizeT new_head) noexcept {
        head_ = new_head;
    }

    QENTEM_INLINE void setPopCount(SizeT new_pop_count) noexcept {
        pop_count_ = new_pop_count;
    }

    QENTEM_INLINE void setCapacity(SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    QENTEM_INLINE void setStorage(Type_T *ptr) noexcept {
        storage_ = ptr;
    }

    QENTEM_INLINE void clearStorage() noexcept {
        setStorage(nullptr);
    }

    /**
     * @brief Reserves a buffer with power-of-two capacity ≥ new_capacity.
     *
     * The capacity is rounded up to the next power of two to maintain efficient
     * wrapping and indexing. A fresh buffer is obtained via the Reserver, and any
     * previous pop history is cleared.
     *
     * @param capacity Minimum number of elements to support.
     */
    void reserve(SizeT capacity) {
        setCapacity(MemoryUtils::AlignToPow2(capacity));
        setStorage(Reserver::Reserve<Type_T>(Capacity()));
        setPopCount(0);
    }

    /**
     * @brief Expands the buffer to accommodate at least `new_capacity` elements, preserving contents.
     *
     * If the requested capacity exceeds the current one, this method:
     *   1. Reserves a new power-of-two buffer of size ≥ `new_capacity`.
     *   2. Moves existing elements into the new buffer in correct order, handling wrap-around.
     *   3. Releases the old buffer and resets internal indexing.
     *   4. Leaves the logical size unchanged.
     *
     * This ensures stable element layout during capacity growth, while preserving
     * deque semantics and minimizing disruptions.
     *
     * @param new_capacity Minimum number of elements to support.
     * @complexity Amortized O(n) when growing (due to data movement), O(1) otherwise.
     */
    void resize(SizeT new_capacity) {
        // 1) Bulk‐copy, two-segment style to preserve order
        const SizeT head_to_end = (capacity_ - head());
        const SizeT first_count = ((Size() < head_to_end) ? Size() : head_to_end);
        // Copy the wrapped remainder (if any)
        const SizeT second_count = (Size() - first_count);

        // 2) Capture old storage
        Type_T     *old_storage  = storage_;
        const SizeT old_capacity = capacity_;

        // 3) Reserve a new buffer (updates storage_, capacity_)
        reserve(new_capacity);

        // Copy the first contiguous block
        MemoryUtils::CopyTo(storage_, (old_storage + head()), first_count);

        if (second_count != 0) {
            MemoryUtils::CopyTo((storage_ + first_count), old_storage, second_count);
        }

        // 4) Clean up old buffer and reset head/index
        Reserver::Release(old_storage, old_capacity);
        setHead(0);
    }

    void clear() noexcept {
        if (Size() != 0) {
            if ((head() + Size()) <= Capacity()) {
                MemoryUtils::Destruct(Storage() + head(), Storage() + head() + Size());
            } else {
                MemoryUtils::Destruct((Storage() + head()), (Storage() + Capacity()));
                MemoryUtils::Destruct(Storage(), (Storage() + (Size()) - (Capacity() - head())));
            }
        }
    }

    /**
     * @brief Destructs all elements and returns the buffer to the memory Reserver.
     *
     */
    QENTEM_INLINE void dispose() {
        clear();
        Reserver::Release(storage_, capacity_);
    }

    // Underlying buffer pointer and indices
    Type_T *storage_{nullptr};
    SizeT   size_{0};
    SizeT   head_{0};
    SizeT   pop_count_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_DEQUE_HPP
