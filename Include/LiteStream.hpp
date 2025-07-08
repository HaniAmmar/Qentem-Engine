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

#include "StringUtils.hpp"

#if defined(_MSC_VER)
#define QENTEM_LITE_STREAM_ALLOCATE(size) malloc(size)
#define QENTEM_LITE_STREAM_DEALLOCATE(ptr) free(ptr)
#else
#define QENTEM_LITE_STREAM_ALLOCATE(size) __builtin_malloc(size)
#define QENTEM_LITE_STREAM_DEALLOCATE(ptr) __builtin_free(ptr)
#endif

namespace Qentem {

struct LiteStream {
    using CharType = char;
    static constexpr SizeT ExpandFactor{2};

    LiteStream()                              = default;
    LiteStream(LiteStream &&)                 = delete;
    LiteStream(const LiteStream &)            = delete;
    LiteStream &operator=(LiteStream &&)      = delete;
    LiteStream &operator=(const LiteStream &) = delete;

    inline explicit LiteStream(SizeT capacity) {
        if (capacity != 0) {
            allocate(capacity);
        }
    }

    ~LiteStream() {
        QENTEM_LITE_STREAM_DEALLOCATE(Storage());
    }

    inline void Write(char ch) {
        const SizeT new_length = (Length() + SizeT{1});

        if (Capacity() == Length()) {
            expand(new_length * ExpandFactor);
        }

        Storage()[Length()] = ch;
        length_             = new_length;
    }

    inline void Write(const char *str, const SizeT length) {
        if (length != 0) {
            const SizeT new_length = (Length() + length);

            if (Capacity() < new_length) {
                expand(new_length * ExpandFactor);
            }

            char *des    = (Storage() + Length());
            SizeT offset = 0;

            while (offset < length) {
                des[offset] = str[offset];
                ++offset;
            }

            length_ = new_length;
        }
    }

    inline void operator<<(char ch) {
        Write(ch);
    }

    inline void operator<<(const char *str) {
        Write(str, StringUtils::Count(str));
    }

    inline void Clear() noexcept {
        length_ = 0;
    }

    inline void StepBack(const SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
        }
    }

    inline char *Storage() noexcept {
        return storage_;
    }

    inline const char *Storage() const noexcept {
        return storage_;
    }

    inline const char *First() const noexcept {
        return storage_;
    }

    inline char *Last() noexcept {
        if (Capacity() != 0) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline const char *Last() const noexcept {
        if (Capacity() != 0) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline SizeT Length() const noexcept {
        return length_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    void InsertNull() {
        if (Capacity() == Length()) {
            expand(Length() + SizeT{1});
        }

        Storage()[Length()] = char{0};
    }

    inline void InsertAt(char ch, SizeT index) {
        if (index < Length()) {
            const SizeT new_length = (Length() + SizeT{1});

            if (new_length <= Capacity()) {
                char *data = Storage();
                SizeT i    = Length();

                while (i > index) {
                    data[i] = data[i - SizeT{1}];
                    --i;
                }

                data[index] = ch;
                length_     = new_length;
            } else {
                char *new_storage = static_cast<char *>(QENTEM_LITE_STREAM_ALLOCATE(new_length));

                SizeT i = 0;
                while (i < index) {
                    new_storage[i] = Storage()[i];
                    ++i;
                }

                new_storage[index] = ch;

                SizeT j = index;
                while (j < Length()) {
                    new_storage[j + 1] = Storage()[j];
                    ++j;
                }

                QENTEM_LITE_STREAM_DEALLOCATE(Storage());
                storage_  = new_storage;
                capacity_ = new_length;
                length_   = new_length;
            }
        }
    }

  private:
    void expand(const SizeT new_capacity) {
        char *str = Storage();

        allocate(new_capacity);

        SizeT offset = 0;

        while (offset < Length()) {
            Storage()[offset] = str[offset];
            ++offset;
        }

        QENTEM_LITE_STREAM_DEALLOCATE(str);
    }

    void allocate(SizeT capacity) {
        storage_  = static_cast<char *>(QENTEM_LITE_STREAM_ALLOCATE(capacity));
        capacity_ = capacity;
    }

    char *storage_{nullptr};
    SizeT length_{0};
    SizeT capacity_{0};
};

} // namespace Qentem

#endif // QENTEM_LITE_STREAM_H
