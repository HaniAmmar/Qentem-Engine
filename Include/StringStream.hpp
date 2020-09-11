/*
 * Copyright (c) 2020 Hani Ammar
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

#include "String.hpp"

#ifndef QENTEM_STRINGSTREAM_H_
#define QENTEM_STRINGSTREAM_H_

#ifndef QENTEM_STRINGSTREAM_INITIALSIZE_
#define QENTEM_STRINGSTREAM_INITIALSIZE_ 8U
#endif

namespace Qentem {

/*
 * Resizable string container with null terminator.
 */
class StringStream {
  public:
    StringStream() = default;

    ~StringStream() {
        Clear();
    }

    explicit StringStream(ULong size) : capacity_(size) {
        if (size != 0) {
            str_ = HAllocator::Allocate<char>(size + 1U);
        }
    }

    StringStream(StringStream &&ss) noexcept
        : offset_(ss.offset_), capacity_(ss.capacity_), str_(ss.str_) {
        ss.offset_   = 0;
        ss.capacity_ = 0;
        ss.str_      = nullptr;
    }

    StringStream(const StringStream &ss) {
        if (ss.offset_ != 0) {
            str_      = HAllocator::Allocate<char>(ss.offset_ + 1U);
            capacity_ = ss.offset_;
            insert(ss.str_, ss.offset_);
        }
    }

    StringStream &operator=(StringStream &&ss) noexcept {
        if (this != &ss) {
            offset_      = ss.offset_;
            capacity_    = ss.capacity_;
            ss.offset_   = 0;
            ss.capacity_ = 0;

            HAllocator::Deallocate(str_);
            str_    = ss.str_;
            ss.str_ = nullptr;
        }

        return *this;
    }

    StringStream &operator=(const StringStream &ss) {
        if (this != &ss) {
            offset_   = 0;
            capacity_ = 0;
            HAllocator::Deallocate(str_);
            str_ = nullptr;

            Insert(ss.str_, ss.offset_);
        }

        return *this;
    }

    void operator+=(char one_char) {
        if (capacity_ == offset_) {
            ULong n_size = capacity_;

            if (n_size == 0) {
                n_size = QENTEM_STRINGSTREAM_INITIALSIZE_;
            }

            expand(n_size << 2U);
        }

        str_[offset_] = one_char;
        ++offset_;
    }

    inline void operator+=(const String &src) {
        insert(src.Storage(), src.Length());
    }

    inline void operator+=(const char *str) {
        insert(str, String::Count(str));
    }

    inline void Insert(const char *str, ULong length) {
        insert(str, length);
    }

    void Clear() noexcept {
        offset_   = 0;
        capacity_ = 0;
        HAllocator::Deallocate(str_);
        str_ = nullptr;
    }

    inline void StepBack(ULong length) noexcept {
        if (length <= offset_) {
            offset_ -= length;
        }
    }

    inline void SoftReset() noexcept {
        offset_ = 0;
    }

    // To write directly to the buffer, set the needed length.
    char *Buffer(ULong len) noexcept {
        const ULong current_offset = offset_;
        offset_ += len;

        if (capacity_ < offset_) {
            expand((ULong{2} << Q_CLZL(offset_)));
        }

        return (str_ + current_offset);
    }

    inline const char *Storage() const noexcept {
        if (offset_ != 0) {
            str_[offset_] = '\0';
            return str_;
        }

        return nullptr;
    }

    inline ULong Length() const noexcept {
        return offset_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    char *Eject() noexcept {
        str_[offset_] = '\0';

        offset_   = 0;
        capacity_ = 0;
        char *str = str_;
        str_      = nullptr;

        return str;
    }

    String GetString() {
        if (offset_ != 0) {
            const ULong len = offset_;
            char *      tmp = str_;
            tmp[offset_]    = '\0';
            str_            = nullptr;
            offset_         = 0;
            capacity_       = 0;
            return String(tmp, len);
        }

        return String("");
    }

    //////////// Private ////////////

  private:
    void insert(const char *str, const ULong len) {
        if (len != 0) {
            const ULong current_offset = offset_;
            offset_ += len;

            if (capacity_ < offset_) {
                expand((ULong{2} << Q_CLZL(offset_)));
            }

            Memory::Copy((str_ + current_offset), str, len);
        }
    }

    void expand(ULong capacity) {
        char *old_str = str_;
        str_          = HAllocator::Allocate<char>(capacity + 1U);

        if (capacity_ != 0) {
            Memory::Copy(str_, old_str, capacity_);
            HAllocator::Deallocate(old_str);
        }

        capacity_ = capacity;
    }

    ULong offset_{0};
    ULong capacity_{0};
    char *str_{nullptr};
};

} // namespace Qentem

#endif
