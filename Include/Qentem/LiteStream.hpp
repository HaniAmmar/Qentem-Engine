/**
 * @file LiteStream.hpp
 * @brief Lightweight, expandable character stream for internal logging and output.
 *
 * `LiteStream` is a minimal, dynamically resizing buffer designed for efficient
 * character streaming. It supports single-character and block writes, backstepping,
 * null termination, and insertion.
 *
 * Unlike `StringStream`, this class is optimized for internal diagnostics, file
 * logging, and console output.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_LITE_STREAM_H
#define QENTEM_LITE_STREAM_H

#include "SystemMemory.hpp"
#include "StringUtils.hpp"

namespace Qentem {

struct LiteStream {
    using CharType = char;
    static constexpr SizeT32 ExpandFactor{2};

    LiteStream(LiteStream &&)                 = delete;
    LiteStream(const LiteStream &)            = delete;
    LiteStream &operator=(LiteStream &&)      = delete;
    LiteStream &operator=(const LiteStream &) = delete;

    QENTEM_INLINE LiteStream() noexcept : storage_{nullptr}, capacity_{0}, length_{0} {};

    QENTEM_INLINE explicit LiteStream(SizeT32 capacity) noexcept : storage_{nullptr}, capacity_{0}, length_{0} {
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
            expand(new_length * ExpandFactor);
        }

        storage_[length_] = ch;
        length_           = new_length;
    }

    void Write(const char *str, const SizeT32 length) noexcept {
        const SizeT32 new_length = (length_ + length);

        if (Capacity() < new_length) {
            expand(new_length * ExpandFactor);
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
            expand(capacity_ + SizeT{1});
        }

        storage_[length_] = char{0};
    }

    QENTEM_INLINE void SetLength(SizeT length) noexcept {
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
        char         *str          = storage_;
        const SizeT32 old_capacity = capacity_;

        reserve(new_capacity);

        SizeT32 offset = 0;

        while (offset < length_) {
            storage_[offset] = str[offset];
            ++offset;
        }

        release(str, old_capacity);
    }

    QENTEM_INLINE void release(char *storage, SizeT32 size) noexcept {
        if (storage != nullptr) {
            SystemMemory::Release(storage, size);
        }
    }

    void reserve(SizeT32 capacity) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        const SizeT32 page_size    = SystemMemory::GetPageSize();
        const SizeT32 page_size_m1 = (page_size - 1U);

        if (capacity < page_size) {
            capacity = page_size;
        } else {
            // Round up to next page boundary
            capacity += page_size_m1;
            capacity &= ~page_size_m1;
        }
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
