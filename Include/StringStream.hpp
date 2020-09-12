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
 * String Stream class without null terminator.
 *
 * Use StringStream += '\0'; to add one.
 */
class StringStream {
  public:
    StringStream() = default;

    ~StringStream() {
        HAllocator::Deallocate(str_);
    }

    explicit StringStream(ULong size) : capacity_(size) {
        if (size != 0) {
            capacity_ = (ULong{1} << Q_CLZL(capacity_));

            if (capacity_ < size) {
                capacity_ <<= 1;
            }

            str_ = HAllocator::Allocate<char>(capacity_);
        }
    }

    StringStream(StringStream &&ss) noexcept
        : length_(ss.length_), capacity_(ss.capacity_), str_(ss.str_) {
        ss.length_   = 0;
        ss.capacity_ = 0;
        ss.str_      = nullptr;
    }

    StringStream(const StringStream &ss) {
        if (ss.length_ != 0) {
            capacity_ = ss.length_;

            capacity_ = (ULong{1} << Q_CLZL(capacity_));

            if (capacity_ < ss.length_) {
                capacity_ <<= 1;
            }

            str_ = HAllocator::Allocate<char>(capacity_);

            insert(ss.str_, ss.length_);
        }
    }

    StringStream &operator=(StringStream &&ss) noexcept {
        if (this != &ss) {
            length_      = ss.length_;
            capacity_    = ss.capacity_;
            ss.length_   = 0;
            ss.capacity_ = 0;

            HAllocator::Deallocate(str_);
            str_    = ss.str_;
            ss.str_ = nullptr;
        }

        return *this;
    }

    StringStream &operator=(const StringStream &ss) {
        if (this != &ss) {
            length_   = 0;
            capacity_ = 0;
            HAllocator::Deallocate(str_);
            str_ = nullptr;

            Insert(ss.str_, ss.length_);
        }

        return *this;
    }

    void operator+=(char one_char) {
        if (capacity_ == length_) {
            ULong n_size = capacity_;

            if (n_size == 0) {
                n_size = QENTEM_STRINGSTREAM_INITIALSIZE_;
            }

            expand(n_size << 1U);
        }

        str_[length_] = one_char;
        ++length_;
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

    void Reset() noexcept {
        length_   = 0;
        capacity_ = 0;
        HAllocator::Deallocate(str_);
        str_ = nullptr;
    }

    inline void StepBack(ULong length) noexcept {
        if (length <= length_) {
            length_ -= length;
        }
    }

    inline void SoftReset() noexcept {
        length_ = 0;
    }

    // To write directly to the buffer, set the needed length.
    char *Buffer(ULong len) noexcept {
        const ULong current_offset = length_;
        length_ += len;

        if (capacity_ < length_) {
            capacity_ = (ULong{1} << Q_CLZL(length_));

            if (capacity_ < length_) {
                capacity_ <<= 1;
            }

            expand(capacity_);
        }

        return (str_ + current_offset);
    }

    inline const char *Storage() const noexcept {
        return str_;
    }

    inline ULong Length() const noexcept {
        return length_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    char *Eject() noexcept {
        length_   = 0;
        capacity_ = 0;
        char *str = str_;
        str_      = nullptr;

        return str;
    }

    String GetString() {
        const ULong len = length_;
        char *      tmp = str_;
        str_            = nullptr;
        length_         = 0;
        capacity_       = 0;
        return String(tmp, len);
    }

    //////////// Private ////////////

  private:
    void insert(const char *str, const ULong len) {
        if (len != 0) {
            const ULong current_offset = length_;
            length_ += len;

            if (capacity_ < length_) {
                expand((ULong{2} << Q_CLZL(length_)));
            }

            Memory::Copy((str_ + current_offset), str, len);
        }
    }

    void expand(ULong capacity) {
        char *old_str = str_;
        str_          = HAllocator::Allocate<char>(capacity);

        if (capacity_ != 0) {
            Memory::Copy(str_, old_str, capacity_);
            HAllocator::Deallocate(old_str);
        }

        capacity_ = capacity;
    }

    ULong length_{0};
    ULong capacity_{0};
    char *str_{nullptr};
};

} // namespace Qentem

#endif
