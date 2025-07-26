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

#include "Reserver.hpp"

namespace Qentem {

template <typename Type_T>
struct Array {
    QENTEM_INLINE Array() noexcept = default;

    QENTEM_INLINE Array(Array &&src) noexcept : storage_{src.storage_}, size_{src.size_}, capacity_{src.capacity_} {
        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
    }

    explicit Array(SizeT capacity, bool initialize = false) : capacity_{capacity} {
        if (capacity != 0) {
            reserve();

            if (initialize) {
                MemoryUtils::InitializeRange(Storage(), (Storage() + Capacity()));
                setSize(Capacity());
            }
        }
    }

    QENTEM_INLINE Array(const Array &src) : size_{src.Size()}, capacity_{src.Size()} {
        if (src.IsNotEmpty()) {
            copyArray(src);
        }
    }

    ~Array() {
        MemoryUtils::Dispose(Storage(), End());
        Reserver::Release(Storage(), Capacity());
    }

    Array &operator=(Array &&src) noexcept {
        if (this != &src) {
            Type_T     *old_storage  = Storage();
            const SizeT old_size     = Size();
            const SizeT old_capacity = Capacity();

            setStorage(src.Storage());
            setSize(src.Size());
            setCapacity(src.Capacity());

            src.clearStorage();
            src.setSize(0);
            src.setCapacity(0);

            if (old_storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                MemoryUtils::Dispose(old_storage, (old_storage + old_size));
                Reserver::Release(old_storage, old_capacity);
            }
        }

        return *this;
    }

    Array &operator=(const Array &src) {
        if (this != &src) {
            Type_T     *old_storage  = Storage();
            const SizeT old_size     = Size();
            const SizeT old_capacity = Capacity();

            clearStorage();
            setSize(src.Size());
            setCapacity(src.Size());

            if (src.IsNotEmpty()) {
                copyArray(src);
            }

            if (old_storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                MemoryUtils::Dispose(old_storage, (old_storage + old_size));
                Reserver::Release(old_storage, old_capacity);
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
                resize(n_size);
            }

            MemoryUtils::CopyTo((Storage() + Size()), src.Storage(), src.Size());
            Reserver::Release(src.Storage(), src.Capacity());
            setSize(n_size);
        }

        src.clearStorage();
        src.setSize(0);
        src.setCapacity(0);
    }

    void operator+=(const Array &src) {
        const SizeT n_size = (Size() + src.Size());

        if (n_size > Capacity()) {
            resize(n_size);
        }

        size_ += src.Size();

        Type_T       *storage  = Storage();
        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            MemoryUtils::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    void operator+=(Type_T &&item) {
        if (Size() == Capacity()) {
            resize((Capacity() | SizeT{1}) * SizeT{2});
        }

        MemoryUtils::Initialize((Storage() + Size()), QUtility::Move(item));
        ++size_;
    }

    void operator+=(const Type_T &item) {
        if (Size() == Capacity()) {
            resize((Capacity() | SizeT{1}) * SizeT{2});
        }

        MemoryUtils::Initialize((Storage() + Size()), item);
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
        setSize(size);
        setCapacity(capacity);
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
        MemoryUtils::Dispose(Storage(), End());

        setSize(0);
    }

    void Reset() noexcept {
        MemoryUtils::Dispose(Storage(), End());
        Reserver::Release(Storage(), Capacity());

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
            reserve();

            if (initialize) {
                MemoryUtils::InitializeRange(Storage(), (Storage() + size));
                setSize(size);
            }
        }
    }

    void Resize(SizeT new_size) {
        if (new_size != 0) {
            if (Size() > new_size) {
                // Shrink
                MemoryUtils::Dispose((Storage() + new_size), End());
                setSize(new_size);
            }

            resize(new_size);
            return;
        }

        Reset();
    }

    void ResizeWithDefaultInit(SizeT new_size) {
        Resize(new_size);

        if (new_size > Size()) {
            Type_T *current = Storage();
            MemoryUtils::InitializeRange((current + Size()), (current + new_size));
        }

        setSize(Capacity());
    }

    QENTEM_INLINE void Expect(SizeT size) {
        const SizeT n_size = (size + Size());

        if (n_size > Capacity()) {
            resize(n_size);
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

            MemoryUtils::Dispose((Storage() + new_size), End());
            setSize(new_size);
        }
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

    QENTEM_INLINE void reserve() {
        storage_ = Reserver::Reserve<Type_T>(Capacity());
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

    void resize(SizeT new_capacity) {
        Type_T     *old_storage  = Storage();
        const SizeT old_capacity = Capacity();

        setCapacity(new_capacity);

        reserve();

        if (old_storage != nullptr) {
            MemoryUtils::CopyTo(Storage(), old_storage, Size());
            Reserver::Release(old_storage, old_capacity);
        }
    }

    void copyArray(const Array &src) {
        reserve();

        Type_T *storage = Storage();

        const Type_T *src_item = src.First();
        const Type_T *src_end  = (src_item + src.Size());

        while (src_item < src_end) {
            MemoryUtils::Initialize(storage, *src_item);
            ++storage;
            ++src_item;
        }
    }

    Type_T *storage_{nullptr};
    SizeT   size_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif
