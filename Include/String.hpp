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

#include "Memory.hpp"
#include "StringUtils.hpp"

#ifndef QENTEM_STRING_H_
#define QENTEM_STRING_H_

namespace Qentem {

/*
 * String container with null terminator.
 */
template <typename Char_T_>
class String {
  public:
    String() = default;

    String(const Char_T_ *str) : String(str, StringUtils::Count(str)) {
    }

    String(const String &src) : length_(src.length_) {
        if (length_ != 0) {
            const ULong len = (length_ + 1);
            storage_        = HAllocator::Allocate<Char_T_>(len);
            Memory::Copy<Char_T_>(storage_, src.storage_,
                                  (len * sizeof(Char_T_)));
        }
    }

    String(String &&src) noexcept
        : length_(src.length_), storage_(src.storage_) {
        src.length_  = 0;
        src.storage_ = nullptr;
    }

    explicit String(const Char_T_ *str, ULong len) : length_(len) {
        if (str != nullptr) {
            ++len;
            storage_ = HAllocator::Allocate<Char_T_>(len);
            Memory::Copy<Char_T_>(storage_, str, (length_ * sizeof(Char_T_)));
            storage_[length_] = 0;
        }
    }

    explicit String(Char_T_ *str, ULong len) noexcept
        : length_(len), storage_(str) {
    }

    ~String() {
        HAllocator::Deallocate(storage_);
    }

    // inline Char_T_ &operator[](ULong index) const {
    //     if (index < length_) {
    //         return storage_[index];
    //     }

    //     throw 1; // Index out of range
    // }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            HAllocator::Deallocate(storage_);

            length_      = src.length_;
            src.length_  = 0;
            storage_     = src.storage_;
            src.storage_ = nullptr;
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            HAllocator::Deallocate(storage_);
            length_  = src.length_;
            storage_ = HAllocator::Allocate<Char_T_>(length_ + 1);
            Memory::Copy<Char_T_>(storage_, src.storage_,
                                  (length_ * sizeof(Char_T_)));
            storage_[length_] = 0;
        }

        return *this;
    }

    String &operator=(const Char_T_ *str) {
        ULong len = StringUtils::Count(str);

        HAllocator::Deallocate(storage_);
        length_  = len;
        storage_ = HAllocator::Allocate<Char_T_>(len + 1);

        if (str != nullptr) {
            Memory::Copy<Char_T_>(storage_, str, (len * sizeof(Char_T_)));
        }

        storage_[len] = 0;

        return *this;
    }

    String &operator+=(String &&src) {
        Insert(src.storage_, src.length_);
        src.length_ = 0;

        HAllocator::Deallocate(src.storage_);
        src.storage_ = nullptr;

        return *this;
    }

    String &operator+=(const String &src) {
        Insert(src.storage_, src.length_);
        return *this;
    }

    String &operator+=(const Char_T_ *str) {
        Insert(str, StringUtils::Count(str));
        return *this;
    }

    String operator+(String &&src) const {
        String ns(Insert(*this, src));

        src.length_ = 0;
        HAllocator::Deallocate(src.storage_);
        src.storage_ = nullptr;

        return ns;
    }

    inline String operator+(const String &src) const {
        return Insert(*this, src);
    }

    String operator+(const Char_T_ *str) const {
        const ULong len         = StringUtils::Count(str);
        const ULong ns_len      = (length_ + len);
        Char_T_ *   ns_storage_ = HAllocator::Allocate<Char_T_>(len + 1);

        Memory::Copy<Char_T_>(ns_storage_, storage_,
                              (length_ * sizeof(Char_T_)));
        Memory::Copy<Char_T_>((ns_storage_ + length_), str,
                              (len * sizeof(Char_T_)));

        ns_storage_[ns_len] = 0;
        return String(ns_storage_, ns_len);
    }

    inline bool operator==(const String &string) const noexcept {
        if (length_ != string.length_) {
            return false;
        }

        return Memory::IsEqual(storage_, string.storage_, length_);
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const ULong len = StringUtils::Count(str);

        if (length_ != len) {
            return false;
        }

        return Memory::IsEqual(storage_, str, len);
    }

    inline bool operator!=(const String &string) const noexcept {
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

    void Reset() noexcept {
        HAllocator::Deallocate(storage_);
        storage_ = nullptr;
        length_  = 0;
    }

    Char_T_ *Eject() noexcept {
        length_      = 0;
        Char_T_ *str = storage_;
        storage_     = nullptr;

        return str;
    }

    inline const Char_T_ *Storage() const noexcept {
        return storage_;
    }

    inline ULong Length() const noexcept {
        return length_;
    }

    static String Insert(const String &src1, const String &src2) {
        const ULong ns_len      = (src1.length_ + src2.length_);
        Char_T_ *   ns_storage_ = HAllocator::Allocate<Char_T_>(ns_len + 1);

        Memory::Copy<Char_T_>(ns_storage_, src1.storage_,
                              (src1.length_ * sizeof(Char_T_)));
        Memory::Copy<Char_T_>((ns_storage_ + src1.length_), src2.storage_,
                              (src2.length_ * sizeof(Char_T_)));

        ns_storage_[ns_len] = 0;
        return String(ns_storage_, ns_len);
    }

    void Insert(const Char_T_ *str, ULong len) {
        if ((str != nullptr) && (len != 0)) {
            Char_T_ *old_str = storage_;
            storage_         = HAllocator::Allocate<Char_T_>(length_ + len + 1);
            Memory::Copy<Char_T_>(storage_, old_str,
                                  (length_ * sizeof(Char_T_)));
            Memory::Copy<Char_T_>((storage_ + length_), str,
                                  (len * sizeof(Char_T_)));
            HAllocator::Deallocate(old_str);
            length_ += len;
            storage_[length_] = 0;
        }
    }

    static String Trim(const String &str) {
        ULong length = str.length_;
        ULong offset = 0;

        StringUtils::SoftTrim(str.storage_, offset, length);

        const Char_T_ *n_str = (str.storage_ + offset);
        return String(n_str, length);
    }

  private:
    ULong    length_{0};
    Char_T_ *storage_{nullptr};
};

} // namespace Qentem

#endif
