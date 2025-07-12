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

    LiteStream()                              = default;
    LiteStream(LiteStream &&)                 = delete;
    LiteStream(const LiteStream &)            = delete;
    LiteStream &operator=(LiteStream &&)      = delete;
    LiteStream &operator=(const LiteStream &) = delete;

    explicit LiteStream(SizeT32 capacity) {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    ~LiteStream() {
        release(storage_, capacity_);
    }

    void Write(char ch) {
        const SizeT32 new_length = (length_ + 1U);

        if (Capacity() == length_) {
            expand(new_length * ExpandFactor);
        }

        storage_[length_] = ch;
        length_           = new_length;
    }

    void Write(const char *str, const SizeT32 length) {
        if (length != 0) {
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
    }

    void operator<<(char ch) {
        Write(ch);
    }

    void operator<<(const char *str) {
        Write(str, StringUtils::Count(str));
    }

    void Clear() noexcept {
        length_ = 0;
    }

    void StepBack(const SizeT32 length) noexcept {
        if (length <= length_) {
            length_ -= length;
        }
    }

    char *Storage() noexcept {
        return storage_;
    }

    const char *Storage() const noexcept {
        return storage_;
    }

    const char *First() const noexcept {
        return storage_;
    }

    char *Last() noexcept {
        if (Capacity() != 0) {
            return (storage_ + (length_ - 1U));
        }

        return nullptr;
    }

    const char *Last() const noexcept {
        if (Capacity() != 0) {
            return (storage_ + (length_ - 1U));
        }

        return nullptr;
    }

    SizeT32 Length() const noexcept {
        return length_;
    }

    SizeT32 Capacity() const noexcept {
        return capacity_;
    }

    void InsertNull() {
        if (Capacity() == length_) {
            expand(length_ + 1U);
        }

        storage_[length_] = char{0};
    }

    void InsertAt(char ch, SizeT32 index) {
        if (index < length_) {
            const SizeT32 new_length = (length_ + 1U);

            if (new_length <= Capacity()) {
                char   *data = storage_;
                SizeT32 i    = length_;

                while (i > index) {
                    data[i] = data[i - 1U];
                    --i;
                }

                data[index] = ch;
                length_     = new_length;
            } else {
                char *new_storage = static_cast<char *>(SystemMemory::Reserve(new_length));

                SizeT32 i = 0;
                while (i < index) {
                    new_storage[i] = storage_[i];
                    ++i;
                }

                new_storage[index] = ch;

                SizeT32 j = index;
                while (j < length_) {
                    new_storage[j + 1] = storage_[j];
                    ++j;
                }

                release(storage_, capacity_);
                storage_  = new_storage;
                capacity_ = new_length;
                length_   = new_length;
            }
        }
    }

  private:
    void expand(const SizeT32 new_capacity) {
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

    void release(char *storage, SizeT32 size) {
        if (storage != nullptr) {
            SystemMemory::Release(storage, size);
        }
    }

    void reserve(SizeT32 capacity) {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        const SizeT32 page_size    = static_cast<SizeT32>(SystemMemory::PageSize());
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

    char   *storage_{nullptr};
    SizeT32 length_{0};
    SizeT32 capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_LITE_STREAM_H
