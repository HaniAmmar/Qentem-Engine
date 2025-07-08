/**
 * @file Array.hpp
 * @brief Defines a dynamic array container for Qentem Engine.
 *
 * This header implements the Array container, providing dynamic, contiguous storage
 * for elements with efficient resizing, insertion, and removal. Array.hpp is a core
 * component for managing collections of data throughout the Qentem Engine library.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_ARRAY_H
#define QENTEM_ARRAY_H

#include "QAllocator.hpp"

namespace Qentem {

template <typename Type_T>
struct Array {
    Array() noexcept = default;

    Array(Array &&src) noexcept : storage_{src.storage_}, size_{src.size_}, capacity_{src.capacity_} {
        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
    }

    explicit Array(SizeT capacity, bool initialize = false) : capacity_{capacity} {
        if (capacity != 0) {
            Type_T *current = allocate();

            if (initialize) {
                QAllocator::InitializeRange(current, (current + capacity));
                setSize(capacity);
            }
        }
    }

    Array(const Array &src) : capacity_{src.Size()} {
        if (src.IsNotEmpty()) {
            setSize(src.Size());
            copyArray(src);
        }
    }

    ~Array() {
        if (Storage() != nullptr) {
            QAllocator::Dispose(Storage(), End());
            QAllocator::Deallocate(Storage());
        }
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T     *storage = Storage();
            const SizeT size    = Size();

            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());

            src.clearStorage();
            src.setSize(0);
            src.setCapacity(0);

            if (storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                QAllocator::Dispose(storage, (storage + size));
                QAllocator::Deallocate(storage);
            }
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_T     *storage = Storage();
            const SizeT size    = Size();

            clearStorage();
            setSize(src.Size());
            setCapacity(src.Size());

            if (IsNotEmpty()) {
                copyArray(src);
            }

            if (storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                QAllocator::Dispose(storage, (storage + size));
                QAllocator::Deallocate(storage);
            }
        }

        return *this;
    }

    void operator+=(Array &&src) {
        if (Capacity() == 0) {
            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());
        } else {
            const SizeT n_size = (Size() + src.Size());

            if (n_size > Capacity()) {
                expand(n_size);
            }

            MemoryUtils::CopyTo((Storage() + Size()), src.Storage(), src.Size());
            QAllocator::Deallocate(src.Storage());
            setSize(n_size);
        }

        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
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
            QAllocator::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_T &&item) {
        if (Size() == Capacity()) {
            expand((Capacity() | SizeT{1}) * SizeT{2});
        }

        QAllocator::Initialize((Storage() + Size()), QUtility::Move(item));
        ++size_;
    }

    inline void operator+=(const Type_T &item) {
        if (Size() == Capacity()) {
            expand((Capacity() | SizeT{1}) * SizeT{2});
        }

        QAllocator::Initialize((Storage() + Size()), item);
        ++size_;
    }

    /**
     * @brief Adopts (takes ownership of) a pre-allocated buffer and manages its lifetime.
     *
     * The Array takes responsibility for the provided memory block (pointer, capacity, and length).
     * When the Array is destroyed or cleared, it will properly destroy and deallocate all elements.
     *
     * @warning The buffer must have been allocated with the appropriate allocator,
     *          and must not be used elsewhere after being adopted.
     * @param ptr   Pointer to the pre-allocated memory block.
     * @param size  The current logical size (number of constructed elements).
     * @param capacity   The capacity of the buffer (number of slots allocated).
     */
    void Adopt(Type_T *ptr, SizeT size, SizeT capacity) {
        // Clean up any existing storage
        Reset();

        // Take ownership of the new buffer
        setStorage(ptr);
        setSize(size);
        setCapacity(capacity);
    }

    inline void Insert(Array &&src) {
        *this += QUtility::Move(src);
    }

    inline void Insert(const Array &src) {
        *this += src;
    }

    inline Type_T &Insert(Type_T &&item) {
        const SizeT size = Size();

        *this += QUtility::Move(item);

        return Storage()[size];
    }

    inline Type_T &Insert(const Type_T &item) {
        const SizeT size = Size();

        *this += item;

        return Storage()[size];
    }

    void Clear() noexcept {
        QAllocator::Dispose(Storage(), End());

        setSize(0);
    }

    void Reset() noexcept {
        QAllocator::Dispose(Storage(), End());
        QAllocator::Deallocate(Storage());

        clearStorage();
        setSize(0);
        setCapacity(0);
    }

    Type_T *Detach() noexcept {
        Type_T *tmp = Storage();

        clearStorage();
        setSize(0);
        setCapacity(0);

        return tmp;
    }

    void Reserve(const SizeT size, bool initialize = false) {
        Reset();

        if (size != 0) {
            setCapacity(size);
            Type_T *current = allocate();

            if (initialize) {
                QAllocator::InitializeRange(current, (current + size));
                setSize(size);
            }
        }
    }

    void Resize(SizeT new_size) {
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink
                QAllocator::Dispose((Storage() + new_size), End());
                setSize(new_size);
            }

            expand(new_size);
            return;
        }

        Reset();
    }

    void ResizeWithDefaultInit(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_T *current = Storage();
            QAllocator::InitializeRange((current + Size()), (current + new_size));
        }

        setSize(Capacity());
    }

    inline void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            expand(n_size);
        }
    }

    void Swap(Type_T &item1, Type_T &item2) noexcept {
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
     * If the allocated capacity exceeds the number of stored elements,
     * this method reallocates the buffer to the exact size, copies
     * all elements into the new storage and frees the excess memory.
     */
    void Compress() {
        if (Size() < Capacity()) {
            Resize(Size());
        }
    }

    inline void Drop(const SizeT size) noexcept {
        if (size <= Size()) {
            const SizeT new_size = (Size() - size);

            QAllocator::Dispose((Storage() + new_size), End());
            setSize(new_size);
        }
    }

    inline Type_T *Storage() noexcept {
        return storage_;
    }

    inline const Type_T *Storage() const noexcept {
        return storage_;
    }

    inline SizeT Size() const noexcept {
        return size_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline const Type_T *First() const noexcept {
        return storage_;
    }

    inline Type_T *Last() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Size() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Type_T *End() const noexcept {
        return (First() + Size());
    }

    inline bool IsEmpty() const noexcept {
        return (Size() == 0);
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Type_T *begin() const noexcept {
        return First();
    }

    inline const Type_T *end() const noexcept {
        return End();
    }

    inline Type_T *begin() noexcept {
        return Storage();
    }

    inline Type_T *end() noexcept {
        return (Storage() + Size());
    }

    //////////// Private ////////////

  private:
    void setStorage(Type_T *ptr) noexcept {
        storage_ = ptr;
    }

    Type_T *allocate() {
        Type_T *new_storage = QAllocator::Allocate<Type_T>(Capacity());
        setStorage(new_storage);
        return new_storage;
    }

    void clearStorage() noexcept {
        storage_ = nullptr;
    }

    void setSize(SizeT new_size) noexcept {
        size_ = new_size;
    }

    void setCapacity(SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    void expand(SizeT new_capacity) {
        Type_T *src = Storage();

        setCapacity(new_capacity);

        Type_T *des = allocate();

        if (src != nullptr) {
            MemoryUtils::CopyTo(des, src, Size());
            QAllocator::Deallocate(src);
        }
    }

    void copyArray(const Array &src) {
        Type_T       *new_storage = allocate();
        const Type_T *src_item    = src.First();
        const Type_T *src_end     = (src_item + src.Size());

        while (src_item < src_end) {
            QAllocator::Initialize(new_storage, *src_item);
            ++new_storage;
            ++src_item;
        }
    }

    Type_T *storage_{nullptr};
    SizeT   size_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif
