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

    String(Char_T_ *str, SizeT len) noexcept : storage_(str), length_(len) {
    }

    String(const Char_T_ *str, SizeT len) : length_(len) {
        ++len;
        storage_ = Memory::Allocate<Char_T_>(len);

        if (len != 1) {
            Memory::Copy(storage_, str, (length_ * sizeof(Char_T_)));
        }

        storage_[length_] = 0;
    }

    explicit String(const Char_T_ *str) : String(str, StringUtils::Count(str)) {
    }

    String(String &&src) noexcept
        : storage_(src.storage_), length_(src.length_) {
        src.length_  = 0;
        src.storage_ = nullptr;
    }

    String(const String &src) : length_(src.length_) {
        if (length_ != 0) {
            const SizeT len = (length_ + 1);
            storage_        = Memory::Allocate<Char_T_>(len);
            Memory::Copy(storage_, src.storage_, (len * sizeof(Char_T_)));
        }
    }

    ~String() {
        Memory::Deallocate(storage_);
    }

    // inline Char_T_ &operator[](SizeT index) const {
    //     if (index < length_) {
    //         return storage_[index];
    //     }

    //     throw 1; // Index out of range
    // }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            Memory::Deallocate(storage_);

            length_      = src.length_;
            src.length_  = 0;
            storage_     = src.storage_;
            src.storage_ = nullptr;
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            Memory::Deallocate(storage_);
            length_  = src.length_;
            storage_ = Memory::Allocate<Char_T_>(length_ + 1);
            Memory::Copy(storage_, src.storage_, (length_ * sizeof(Char_T_)));
            storage_[length_] = 0;
        }

        return *this;
    }

    String &operator=(const Char_T_ *str) {
        SizeT len = StringUtils::Count(str);

        Memory::Deallocate(storage_);
        length_  = len;
        storage_ = Memory::Allocate<Char_T_>(len + 1);

        if (len != 0) {
            Memory::Copy(storage_, str, (len * sizeof(Char_T_)));
        }

        storage_[len] = 0;

        return *this;
    }

    String &operator+=(String &&src) {
        Insert(src.storage_, src.length_);
        src.length_ = 0;

        Memory::Deallocate(src.storage_);
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
        Memory::Deallocate(src.storage_);
        src.storage_ = nullptr;

        return ns;
    }

    inline String operator+(const String &src) const {
        return Insert(*this, src);
    }

    String operator+(const Char_T_ *str) const {
        const SizeT len         = StringUtils::Count(str);
        const SizeT ns_len      = (length_ + len);
        Char_T_ *   ns_storage_ = Memory::Allocate<Char_T_>(ns_len + 1);

        if (length_ != 0) {
            Memory::Copy(ns_storage_, storage_, (length_ * sizeof(Char_T_)));
        }

        if (len != 0) {
            Memory::Copy((ns_storage_ + length_), str, (len * sizeof(Char_T_)));
        }

        ns_storage_[ns_len] = 0;
        return String(ns_storage_, ns_len);
    }

    inline bool operator==(const String &string) const noexcept {
        if (length_ != string.length_) {
            return false;
        }

        return StringUtils::IsEqual(storage_, string.storage_, length_);
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT len = StringUtils::Count(str);

        if (length_ != len) {
            return false;
        }

        return StringUtils::IsEqual(storage_, str, len);
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool IsEqual(const Char_T_ *str, SizeT length) const noexcept {
        if (length_ != length) {
            return false;
        }

        return StringUtils::IsEqual(storage_, str, length);
    }

    void Reset() noexcept {
        Memory::Deallocate(storage_);
        storage_ = nullptr;
        length_  = 0;
    }

    Char_T_ *Eject() noexcept {
        length_      = 0;
        Char_T_ *str = storage_;
        storage_     = nullptr;

        return str;
    }

    inline Char_T_ *First() const noexcept {
        return storage_;
    }

    inline SizeT Length() const noexcept {
        return length_;
    }

    static String Insert(const String &src1, const String &src2) {
        const SizeT ns_len      = (src1.length_ + src2.length_);
        Char_T_ *   ns_storage_ = Memory::Allocate<Char_T_>(ns_len + 1);

        if (src1.length_ != 0) {
            Memory::Copy(ns_storage_, src1.storage_,
                         (src1.length_ * sizeof(Char_T_)));
        }

        if (src2.length_ != 0) {
            Memory::Copy((ns_storage_ + src1.length_), src2.storage_,
                         (src2.length_ * sizeof(Char_T_)));
        }

        ns_storage_[ns_len] = 0;
        return String(ns_storage_, ns_len);
    }

    void Insert(const Char_T_ *str, SizeT len) {
        if ((str != nullptr) && (len != 0)) {
            Char_T_ *old_str = storage_;
            storage_         = Memory::Allocate<Char_T_>(length_ + len + 1);

            if (old_str != nullptr) {
                Memory::Copy(storage_, old_str, (length_ * sizeof(Char_T_)));
                Memory::Deallocate(old_str);
            }

            Memory::Copy((storage_ + length_), str, (len * sizeof(Char_T_)));
            length_ += len;
            storage_[length_] = 0;
        }
    }

    static String Trim(const String &str) {
        SizeT length = str.length_;
        SizeT offset = 0;

        StringUtils::SoftTrim(str.storage_, offset, length);

        const Char_T_ *n_str = (str.storage_ + offset);
        return String(n_str, length);
    }

  private:
    Char_T_ *storage_{nullptr};
    SizeT    length_{0};
};

} // namespace Qentem

#endif
