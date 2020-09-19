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

#include "Platform.hpp"
#include "String.hpp"

#ifndef QENTEM_STRINGSTREAM_H_
#define QENTEM_STRINGSTREAM_H_

#ifndef QENTEM_STRINGSTREAM_INITIALSIZE_
#define QENTEM_STRINGSTREAM_INITIALSIZE_ 8U
#endif

namespace Qentem {

/*
 * String Stream class without the null terminator.
 */
template <typename Char_T_>
class StringStream {
  public:
    StringStream() = default;

    ~StringStream() {
        HAllocator::Deallocate(storage_);
    }

    explicit StringStream(ULong size) : capacity_(size) {
        if (size != 0) {
            capacity_ = (ULong{1} << Platform::CLZL(capacity_));

            if (capacity_ < size) {
                capacity_ <<= 1U;
            }

            storage_ = HAllocator::Allocate<Char_T_>(capacity_);
        }
    }

    StringStream(StringStream &&ss) noexcept
        : length_(ss.length_), capacity_(ss.capacity_), storage_(ss.storage_) {
        ss.length_   = 0;
        ss.capacity_ = 0;
        ss.storage_  = nullptr;
    }

    StringStream(const StringStream &ss) {
        if (ss.length_ != 0) {
            capacity_ = ss.length_;

            capacity_ = (ULong{1} << Platform::CLZL(capacity_));

            if (capacity_ < ss.length_) {
                capacity_ <<= 1U;
            }

            storage_ = HAllocator::Allocate<Char_T_>(capacity_);

            insert(ss.storage_, ss.length_);
        }
    }

    StringStream &operator=(StringStream &&ss) noexcept {
        if (this != &ss) {
            length_      = ss.length_;
            capacity_    = ss.capacity_;
            ss.length_   = 0;
            ss.capacity_ = 0;

            HAllocator::Deallocate(storage_);
            storage_    = ss.storage_;
            ss.storage_ = nullptr;
        }

        return *this;
    }

    StringStream &operator=(const StringStream &ss) {
        if (this != &ss) {
            length_   = 0;
            capacity_ = 0;
            HAllocator::Deallocate(storage_);
            storage_ = nullptr;

            Insert(ss.storage_, ss.length_);
        }

        return *this;
    }

    void operator+=(Char_T_ one_char) {
        if (capacity_ == length_) {
            ULong n_size = capacity_;

            if (n_size == 0) {
                n_size = QENTEM_STRINGSTREAM_INITIALSIZE_;
            }

            expand(n_size << 1U);
        }

        storage_[length_] = one_char;
        ++length_;
    }

    inline void operator+=(const String<Char_T_> &src) {
        insert(src.Storage(), src.Length());
    }

    inline void operator+=(const Char_T_ *str) {
        insert(str, StringUtils::Count(str));
    }

    inline bool operator==(const StringStream &ss) const noexcept {
        if (length_ != ss.length_) {
            return false;
        }

        return Memory::IsEqual(storage_, ss.storage_, length_);
    }

    inline bool operator==(const String<Char_T_> &string) const noexcept {
        if (length_ != string.Length()) {
            return false;
        }

        return Memory::IsEqual(storage_, string.Storage(), length_);
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const ULong len = StringUtils::Count(str);

        if (length_ != len) {
            return false;
        }

        return Memory::IsEqual(storage_, str, len);
    }

    inline bool operator!=(const StringStream &ss) const noexcept {
        return (!(*this == ss));
    }

    inline bool operator!=(const String<Char_T_> &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool IsEqual(const Char_T_ *str, ULong length) const noexcept {
        if (length_ != length) {
            return false;
        }

        return Memory::IsEqual(storage_, str, length);
    }

    inline void Insert(const Char_T_ *str, ULong length) {
        insert(str, length);
    }

    void Reset() noexcept {
        length_   = 0;
        capacity_ = 0;
        HAllocator::Deallocate(storage_);
        storage_ = nullptr;
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
    Char_T_ *Buffer(ULong len) noexcept {
        const ULong current_offset = length_;
        length_ += len;

        if (capacity_ < length_) {
            expand((ULong{2} << Platform::CLZL(length_)));
        }

        return (storage_ + current_offset);
    }

    inline const Char_T_ *Storage() const noexcept {
        return storage_;
    }

    inline ULong Length() const noexcept {
        return length_;
    }

    inline ULong Capacity() const noexcept {
        return capacity_;
    }

    Char_T_ *Eject() noexcept {
        length_      = 0;
        capacity_    = 0;
        Char_T_ *str = storage_;
        storage_     = nullptr;

        return str;
    }

    String<Char_T_> GetString() {
        if (capacity_ > length_) {
            storage_[length_] = 0;
            const ULong len   = length_;
            return String<Char_T_>(Eject(), len);
        }

        String<Char_T_> str{Storage(), length_};
        Reset();

        return str;
    }

    //////////// Private ////////////

  private:
    void insert(const Char_T_ *str, const ULong len) {
        if (len != 0) {
            const ULong current_offset = length_;
            length_ += len;

            if (capacity_ < length_) {
                expand((ULong{2} << Platform::CLZL(length_)));
            }

            Memory::Copy<Char_T_>((storage_ + current_offset), str,
                                  (len * sizeof(Char_T_)));
        }
    }

    void expand(ULong capacity) {
        Char_T_ *old_str = storage_;
        storage_         = HAllocator::Allocate<Char_T_>(capacity);

        if (capacity_ != 0) {
            const ULong c_size = (capacity_ * sizeof(Char_T_));

            if (c_size > 7) {
                // DWORD
                Memory::Copy<long long>(storage_, old_str, c_size);
            } else {
                Memory::Copy<Char_T_>(storage_, old_str, c_size);
            }

            HAllocator::Deallocate(old_str);
        }

        capacity_ = capacity;
    }

    ULong    length_{0};
    ULong    capacity_{0};
    Char_T_ *storage_{nullptr};
};

} // namespace Qentem

#endif
