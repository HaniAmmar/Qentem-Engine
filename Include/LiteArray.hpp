/**
 * @file LiteArray.hpp
 * @brief Lightweight dynamic array with minimal overhead, used internally in memory subsystems.
 *
 * `LiteArray` is a simplified dynamic array intended for internal use by the `Reserver` memory allocator
 * and related components. It provides basic storage, ownership, and resizing logic for trivially
 * constructible and destructible types. It assumes move semantics and does not support copy semantics.
 *
 * This array is not intended for general-purpose use and is excluded from the public Qentem API.
 * Use only within low-level memory components where minimal dependencies and overhead are required.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_LITE_ARRAY_H
#define QENTEM_LITE_ARRAY_H

#include "SystemMemory.hpp"
#include "MemoryUtils.hpp"

namespace Qentem {

template <typename Type_T>
struct LiteArray {
    QENTEM_INLINE LiteArray() noexcept = default;

    LiteArray(const LiteArray &)            = delete;
    LiteArray &operator=(const LiteArray &) = delete;

    QENTEM_INLINE explicit LiteArray(SizeT capacity, bool initialize = false) noexcept {
        if (capacity != 0) {
            reserve(capacity);

            if (initialize) {
                MemoryUtils::InitializeRange(storage_, (storage_ + capacity_));
                size_ = capacity_;
            }
        }
    }

    QENTEM_INLINE LiteArray(LiteArray &&src) noexcept
        : storage_{src.storage_}, size_{src.size_}, capacity_{src.capacity_} {
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        raw_storage_     = src.raw_storage_;
        src.raw_storage_ = nullptr;
#endif

        src.storage_  = nullptr;
        src.size_     = 0;
        src.capacity_ = 0;
    }

    QENTEM_INLINE LiteArray &operator=(LiteArray &&src) noexcept {
        if (this != &src) {
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
            void *old_raw_storage = raw_storage_;
            raw_storage_          = src.raw_storage_;
            src.raw_storage_      = nullptr;
#endif
            Type_T     *old_storage  = storage_;
            const SizeT old_size     = size_;
            const SizeT old_capacity = capacity_;

            storage_  = src.storage_;
            size_     = src.size_;
            capacity_ = src.capacity_;

            src.storage_  = nullptr;
            src.size_     = 0;
            src.capacity_ = 0;

            if (old_storage != nullptr) {
                // Just in case the copied array is not a child array, do this last.
                MemoryUtils::Dispose(old_storage, (old_storage + old_size));
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
                release(old_raw_storage, old_capacity);
#else
                release(old_storage, old_capacity);
#endif
            }
        }

        return *this;
    }

    QENTEM_INLINE ~LiteArray() noexcept {
        if (storage_ != nullptr) {
            MemoryUtils::Dispose(storage_, End());

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
            release(raw_storage_, capacity_);
#else
            release(storage_, capacity_);
#endif
        }
    }

    QENTEM_INLINE void operator+=(Type_T &&item) noexcept {
        if (size_ == capacity_) {
            expand((capacity_ != 0) ? (capacity_ * SizeT{2}) : SizeT{1});
        }

        MemoryUtils::Initialize((storage_ + size_), QUtility::Move(item));
        ++size_;
    }

    QENTEM_INLINE Type_T &Insert(Type_T &&item) noexcept {
        const SizeT size = size_;

        *this += QUtility::Move(item);

        return storage_[size];
    }

    QENTEM_INLINE void Drop(const SizeT size) noexcept {
        if (size <= Size()) {
            const SizeT new_size = (Size() - size);

            MemoryUtils::Dispose((Storage() + new_size), End());
            size_ = new_size;
        }
    }

    QENTEM_INLINE void DropFast(const SizeT size) noexcept {
        size_ -= size;
    }

    QENTEM_INLINE void Clear() noexcept {
        MemoryUtils::Dispose(storage_, End());
        size_ = 0;
    }

    QENTEM_INLINE void Reset() noexcept {
        if (storage_ != nullptr) {
            MemoryUtils::Dispose(storage_, End());

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
            release(raw_storage_, capacity_);
#else
            release(storage_, capacity_);
#endif
        }

        storage_  = nullptr;
        size_     = 0;
        capacity_ = 0;
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    QENTEM_INLINE bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    QENTEM_INLINE Type_T *Storage() noexcept {
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
            return (Storage() + (size_ - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Type_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (size_ - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Type_T *End() const noexcept {
        return (First() + size_);
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

  private:
    QENTEM_INLINE void expand(SizeT new_capacity) noexcept {
        Type_T *old_storage = storage_;

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
        void *old_raw_storage = raw_storage_;
#endif

        const SizeT old_capacity = Capacity();

        reserve(new_capacity);

        if (old_storage != nullptr) {
            MemoryUtils::CopyTo(storage_, old_storage, size_);
#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
            release(old_raw_storage, old_capacity);
#else
            release(old_storage, old_capacity);
#endif
        }
    }

    /**
     * @brief Reserves raw memory for element storage, rounded to page size.
     *
     * This reserves in **bytes**, but stores capacity as number of elements.
     * Ensures proper alignment and page efficiency.
     */
    QENTEM_INLINE void reserve(SizeT capacity) noexcept {
        SizeT capacity_bytes = (capacity * sizeof(Type_T));

#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        const SizeT32 page_size    = static_cast<SizeT32>(SystemMemory::PageSize());
        const SizeT32 page_size_m1 = (page_size - 1U);

        if (capacity_bytes < page_size) {
            capacity_bytes = page_size;
        } else {
            // Round up to next page boundary
            capacity_bytes += page_size_m1;
            capacity_bytes &= ~page_size_m1;
        }

        storage_ = static_cast<Type_T *>(SystemMemory::Reserve(capacity_bytes));
#else
        raw_storage_                        = SystemMemory::Reserve(capacity_bytes + 64U);
        const SystemIntType raw_address     = reinterpret_cast<SystemIntType>(raw_storage_);
        const SystemIntType aligned_address = ((raw_address + SystemIntType{63}) & ~SystemIntType{63});
        storage_                            = reinterpret_cast<Type_T *>(aligned_address);
#endif

        capacity_ = (capacity_bytes / sizeof(Type_T));
    }

    static void release(void *storage, SizeT capacity) noexcept {
        SystemMemory::Release(storage, (capacity * sizeof(Type_T)));
    }

#ifdef QENTEM_SYSTEM_MEMORY_FALLBACK
    void *raw_storage_{nullptr};
#endif

    Type_T *storage_{nullptr};
    SizeT   size_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_LITE_ARRAY_H
