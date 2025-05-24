/*
 * Copyright (c) 2025 Hani Ammar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QENTEM_DEQUE_HPP
#define QENTEM_DEQUE_HPP

#include "Memory.hpp"

namespace Qentem {

/**
 * @brief Double-ended queue with power-of-two capacity, backed by a ring buffer.
 *
 * Supports amortized O(1) insertion/removal at both front and back using
 * bitmask wraparound. Capacity is always a power of two.
 *
 * @tparam Type_T The element type stored in the deque.
 */
template <typename Type_T>
struct Deque {
    /**
     * @brief Default constructor: creates an empty deque.
     */
    Deque() noexcept = default;

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
    explicit Deque(SizeT cap) {
        allocate(cap);
    }

    /**
     * @brief Move constructor: takes ownership of another deque's storage.
     * @param src Source deque to move from.
     */
    Deque(Deque &&src) noexcept
        : storage_{src.storage_}, index_{src.index_}, head_{src.head_}, capacity_{src.capacity_} {
        src.clearStorage();
        src.setSize(0);
        src.setHead(0);
        src.setCapacity(0);
    }

    /**
     * @brief Move assignment: disposes current contents and takes over src's.
     * @param src Source deque to move from.
     * @return Reference to this deque.
     */
    Deque &operator=(Deque &&src) noexcept {
        if (this != &src) {
            dispose();
            setStorage(src.Storage());
            setSize(src.Size());
            setHead(src.head());
            setCapacity(src.Capacity());

            src.clearStorage();
            src.setSize(0);
            src.setHead(0);
            src.setCapacity(0);
        }
        return *this;
    }

    /**
     * @brief Destructor: disposes all elements and deallocates buffer.
     */
    ~Deque() {
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
     * @complexity Amortized O(1), occasional O(n) when reallocation occurs.
     */
    void operator+=(Type_T &&item) {
        if (IsFull()) {
            resize(capacity_ * SizeT{2});
        }

        Memory::Initialize((Storage() + tail()), Memory::Move(item));

        setSize(Size() + SizeT{1});
    }

    /**
     * @brief Inserts a copy of the given element at the back of the deque.
     *
     * If the deque is full, its capacity is automatically doubled (power-of-two)
     * before inserting. The element is then copy-constructed into the tail slot.
     *
     * @complexity Amortized O(1), with occasional O(n) when a reallocation occurs.
     */
    inline void operator+=(const Type_T &item) {
        if (IsFull()) {
            resize(capacity_ * SizeT{2});
        }

        Memory::Initialize((Storage() + tail()), item);

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
     * @complexity Amortized O(1), occasional O(n) when reallocation occurs.
     */
    inline Type_T &Insert(Type_T &&item) {
        const SizeT index = tail();

        *this += Memory::Move(item);

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
     * @complexity Amortized O(1), with occasional O(n) when a reallocation occurs.
     */
    inline Type_T &Insert(const Type_T &item) {
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
    void Dequeue() {
        if (IsNotEmpty()) {
            Memory::Dispose(First());

            setHead((head() + SizeT{1}) & (Capacity() - SizeT{1}));
            setSize(Size() - SizeT{1});
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
    void DequeueBack() {
        if (IsNotEmpty()) {
            // Destroy the very last element in place
            Memory::Dispose(Last());
            // Decrement logical size
            setSize(Size() - SizeT{1});
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
     * @note noexcept: no heap allocations or exceptions are thrown.
     */
    const Type_T *First() const noexcept {
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
     * @note noexcept: no heap allocations or exceptions are thrown.
     */
    Type_T *First() noexcept {
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
     * until the deque is modified (e.g., via push, pop, or resize).
     *
     * @complexity O(1)
     * @note noexcept: no heap allocations or exceptions are thrown.
     */
    const Type_T *Last() const noexcept {
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
     * until the deque is modified (e.g., via push, pop, or resize).
     *
     * @complexity O(1)
     * @note noexcept: no heap allocations or exceptions are thrown.
     */
    Type_T *Last() noexcept {
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
    Type_T *Get(SizeT index) noexcept {
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
     * @note noexcept: no allocations or exceptions are thrown.
     */
    const Type_T *Get(SizeT index) const noexcept {
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
    inline void Expect(SizeT additional) {
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

                if (count > 0) {
                    if (end <= Capacity()) {
                        Memory::Dispose((Storage() + start), (Storage() + end));
                    } else {
                        // Wrapped range
                        Memory::Dispose((Storage() + start), (Storage() + Capacity()));
                        Memory::Dispose(Storage(), (Storage() + (count - (Capacity() - start))));
                    }
                }

                setSize(new_size);
            }

            resize(new_size);
        } else {
            // Completely clear and deallocate
            Reset();
        }
    }

    /**
     * @brief Resizes the deque and default-initializes any newly added slots.
     *
     * Behavior:
     *  - If new_size == 0: fully resets the deque and deallocates storage.
     *  - If new_size > current Size(): grows capacity if needed, then default-
     *    constructs elements in the new slots, and updates logical size.
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

            // 3) Initialize contiguous or wrapped range
            if (end <= Capacity()) {
                Memory::Initialize((Storage() + start), (Storage() + end));
            }

            // 4) Update logical size to include the new default-initialized elements
            setSize(new_size);
        }
    }

    /**
     * @brief Shrinks the internal buffer to match the current element count.
     *
     * If the allocated capacity exceeds the number of stored elements,
     * this method reallocates the buffer to the exact size, copies
     * all elements into the new storage (resetting head to zero),
     * and frees the excess memory.
     */
    void Compress() {
        const SizeT n_size = Memory::AlignSize(Size());

        if (Memory::AlignSize(n_size) < Capacity()) {
            Resize(n_size);
        }
    }

    /**
     * @brief Reserves a fresh buffer of exactly `cap` capacity, discarding existing data.
     *
     * This operation resets the deque to empty, deallocates any prior storage,
     * and then allocates a new buffer of power-of-two capacity ≥ `cap`.
     *
     * @param cap Minimum number of elements the new buffer should hold.
     * @post Size() == 0 and Capacity() == AlignSize(cap)
     */
    void Reserve(SizeT cap) {
        // Clear and deallocate the old buffer
        Reset();

        // Allocate a new buffer with the requested minimum capacity
        allocate(cap);
    }

    /**
     * @brief Destroys all elements in the deque while keeping the existing buffer.
     *
     * Invokes the destructor on each element from the current head to the tail,
     * correctly handling wrap-around, but does not free or reallocate the underlying
     * storage. After calling Clear(), Size() == 0 and Head() == 0, but Capacity()
     * remains unchanged for immediate reuse.
     *
     * @complexity O(n), where n is the number of elements.
     */
    void Clear() {
        clear();
        setSize(0);
        setHead(0);
    }

    /**
     * @brief Completely clears the deque and releases all memory.
     *
     * Destroys every element, deallocates the underlying buffer,
     * and resets head, size, and capacity to zero. After calling
     * Reset(), the deque is equivalent to a freshly default-constructed
     * instance.
     *
     * @complexity O(n) for element destruction plus O(1) for deallocation.
     */
    void Reset() {
        dispose();
        clearStorage();
        setSize(0);
        setHead(0);
        setCapacity(0);
    }

    // Accessors
    inline Type_T *Storage() const noexcept {
        return storage_;
    }

    SizeT Capacity() const noexcept {
        return capacity_;
    }

    SizeT Size() const noexcept {
        return index_;
    }

    bool IsEmpty() const noexcept {
        return Size() == 0;
    }

    bool IsNotEmpty() const noexcept {
        return Size() != 0;
    }

    bool IsFull() const noexcept {
        return (Size() == Capacity());
    }

  private:
    inline SizeT head() const noexcept {
        return head_;
    }

    /**
     * @brief Computes the next insertion index (tail) = (head+size) & (capacity-1).
     */
    inline SizeT tail() const noexcept {
        return ((head() + Size()) & (Capacity() - SizeT{1}));
    }

    inline void setSize(SizeT new_size) noexcept {
        index_ = new_size;
    }

    inline void setHead(SizeT new_head) noexcept {
        head_ = new_head;
    }

    inline void setCapacity(SizeT new_cap) noexcept {
        capacity_ = new_cap;
    }

    void setStorage(Type_T *ptr) noexcept {
        storage_ = ptr;
    }

    inline void clearStorage() noexcept {
        setStorage(nullptr);
    }

    /**
     * @brief Allocates a buffer of power-of-two capacity >= new_cap.
     */
    void allocate(SizeT new_cap) {
        setCapacity(Memory::AlignSize(new_cap));
        setStorage(Memory::Allocate<Type_T>(Capacity()));
    }

    /**
     * @brief Ensures the internal buffer has capacity ≥ new_cap while preserving existing elements.
     *
     * When new_cap is greater than the current capacity, this method:
     *   1. Allocates a new power-of-two buffer of size ≥ new_cap.
     *   2. Moves existing elements in-order from the old buffer to the new one.
     *   3. Deallocates the old buffer and resets head to 0.
     *   4. Leaves the logical size unchanged.
     *
     * @param new_cap Desired minimum capacity for the deque's buffer.
     * @complexity Amortized O(n) when growing (due to element moves); O(1) otherwise.
     */
    void resize(SizeT new_cap) {
        constexpr SizeT32 type_size = sizeof(Type_T);

        // 1) Bulk‐copy, two-segment style to preserve order
        const SizeT head_to_end = (capacity_ - head());
        const SizeT firstCount  = ((Size() < head_to_end) ? Size() : head_to_end);
        // Copy the wrapped remainder (if any)
        const SizeT secondCount = (Size() - firstCount);

        // 2) Capture old storage
        Type_T *old_storage = storage_;

        // 3) Allocate new buffer (updates storage_, capacity_)
        allocate(new_cap);

        // Copy the first contiguous block
        Memory::Copy(storage_, (old_storage + head()), (firstCount * type_size));

        if (secondCount != 0) {
            Memory::Copy((storage_ + firstCount), old_storage, (secondCount * type_size));
        }

        // 4) Clean up old buffer and reset head/index
        Memory::Deallocate(old_storage);
        setHead(0);
    }

    void clear() noexcept {
        if (Size() > 0) {
            if ((head() + Size()) <= Capacity()) {
                Memory::Dispose(Storage() + head(), Storage() + head() + Size());
            } else {
                Memory::Dispose((Storage() + head()), (Storage() + Capacity()));
                Memory::Dispose(Storage(), (Storage() + (Size()) - (Capacity() - head())));
            }
        }
    }

    /**
     * @brief Disposes elements and deallocates the buffer.
     */
    void dispose() {
        clear();
        Memory::Deallocate(storage_);
    }

    // Underlying buffer pointer and indices
    Type_T *storage_{nullptr};
    SizeT   index_{0};
    SizeT   head_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_DEQUE_HPP
