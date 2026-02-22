/**
 * @file LiteStream.hpp
 * @brief Lightweight, page-expandable character stream.
 *
 * `LiteStream` is a minimal character stream backed by a page-aligned expandable buffer,
 * designed for efficient sequential writes. It supports single-character and block
 * writes, explicit null termination, and controlled capacity growth.
 *
 * Capacity increases only when required and is rounded to system page boundaries,
 * providing predictable memory usage and avoiding excessive reallocation.
 *
 * Unlike `StringStream`, this class is intended for internal diagnostics, file logging,
 * and console output where simplicity, low overhead, and controlled expansion are
 * preferred over rich string manipulation.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_LITE_STREAM_H
#define QENTEM_LITE_STREAM_H

#include "Qentem/SystemMemory.hpp"
#include "Qentem/StringUtils.hpp"

namespace Qentem {

struct LiteStream {
    using CharType = char;

    LiteStream(LiteStream &&)                 = delete;
    LiteStream(const LiteStream &)            = delete;
    LiteStream &operator=(LiteStream &&)      = delete;
    LiteStream &operator=(const LiteStream &) = delete;

    QENTEM_INLINE LiteStream() noexcept = default;

    QENTEM_INLINE explicit LiteStream(SizeT32 capacity) noexcept {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    QENTEM_INLINE ~LiteStream() noexcept {
        release(storage_, capacity_);
    }

    void Write(char ch) noexcept {
        const SizeT32 new_length = (length_ + 1U);

        if (Capacity() == length_) {
            expand(new_length);
        }

        storage_[length_] = ch;
        length_           = new_length;
    }

    void Write(const char *str, const SizeT32 length) noexcept {
        const SizeT32 new_length = (length_ + length);

        if (Capacity() < new_length) {
            expand(new_length);
        }

        char   *des    = (storage_ + length_);
        SizeT32 offset = 0;

        while (offset < length) {
            des[offset] = str[offset];
            ++offset;
        }

        length_ = new_length;
    }

    void InsertNull() {
        if (capacity_ < length_) {
            expand(capacity_ + 1U);
        }

        storage_[length_] = char{0};
    }

    QENTEM_INLINE void Expect(SizeT32 length) {
        length += length_;

        if (Capacity() < length) {
            expand(length);
        }
    }

    QENTEM_INLINE void SetLength(SizeT32 length) noexcept {
        if (capacity_ < length) {
            expand(length);
        }

        length_ = length;
    }

    QENTEM_INLINE QENTEM_INLINE void Clear() noexcept {
        length_ = 0;
    }

    QENTEM_INLINE char *Storage() noexcept {
        return storage_;
    }

    QENTEM_INLINE const char *Storage() const noexcept {
        return storage_;
    }

    QENTEM_INLINE const char *First() const noexcept {
        return storage_;
    }

    QENTEM_INLINE char *Last() noexcept {
        if (Capacity() != 0) {
            return (storage_ + (length_ - 1U));
        }

        return nullptr;
    }

    QENTEM_INLINE const char *Last() const noexcept {
        if (Capacity() != 0) {
            return (storage_ + (length_ - 1U));
        }

        return nullptr;
    }

    QENTEM_INLINE SizeT32 Length() const noexcept {
        return length_;
    }

    QENTEM_INLINE SizeT32 Capacity() const noexcept {
        return capacity_;
    }

  private:
    void expand(const SizeT32 new_capacity) noexcept {
        constexpr SizeT32 long_m1        = (sizeof(SystemLong) - 1U);
        SizeT32           aligned_length = (((length_ + long_m1) & ~long_m1) / sizeof(SystemLong));

        char         *str          = storage_;
        const SizeT32 old_capacity = capacity_;

        reserve(new_capacity);

        SystemLong *dst = reinterpret_cast<SystemLong *>(storage_);
        SystemLong *src = reinterpret_cast<SystemLong *>(str);

        SizeT32 offset = 0;
        while (offset < aligned_length) {
            dst[offset] = src[offset];
            ++offset;
        }

        release(str, old_capacity);
    }

    QENTEM_INLINE static void release(char *storage, SizeT32 size) noexcept {
        if (storage != nullptr) {
            SystemMemory::Release(storage, size);
        }
    }

    void reserve(SizeT32 capacity) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        if (capacity > SystemMemory::GetPageSize()) {
            capacity = SystemMemory::AlignToPageSize(capacity);
        } else {
            capacity = SystemMemory::GetPageSize();
        }
#else
        capacity = (capacity + sizeof(SystemLong) - 1U) & ~(sizeof(SystemLong) - 1U);
        capacity *= 2;
#endif

        storage_  = static_cast<char *>(SystemMemory::Reserve(capacity));
        capacity_ = capacity;
    }

    char   *storage_;
    SizeT32 capacity_;
    SizeT32 length_;
};

} // namespace Qentem

#endif // QENTEM_LITE_STREAM_H
