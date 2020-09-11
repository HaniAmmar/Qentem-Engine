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

#ifndef QENTEM_STRING_H_
#define QENTEM_STRING_H_

namespace Qentem {

/*
 * String container with null terminator.
 */
class String {
  public:
    String() = default;

    explicit String(ULong len) : length_(len) {
        ++len;
        storage_          = HAllocator::Allocate<char>(len);
        storage_[length_] = '\0';
    }

    String(const char *str) : String(str, Count(str)) {
    }

    String(const String &src) : length_(src.length_) {
        if (length_ != 0) {
            storage_ = HAllocator::Allocate<char>(length_ + 1);
            Memory::Copy(storage_, src.storage_, (length_ + 1));
        }
    }

    String(String &&src) noexcept
        : length_(src.length_), storage_(src.storage_) {
        src.length_  = 0;
        src.storage_ = nullptr;
    }

    explicit String(const char *str, ULong len) : length_(len) {
        if (str != nullptr) {
            ++len;
            storage_ = HAllocator::Allocate<char>(len);
            Memory::Copy(storage_, str, length_);
            storage_[length_] = '\0';
        }
    }

    explicit String(char *str, ULong len) noexcept
        : length_(len), storage_(str) {
    }

    ~String() {
        HAllocator::Deallocate(storage_);
    }

    // inline char &operator[](ULong index) const {
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
            storage_ = HAllocator::Allocate<char>(length_ + 1);
            Memory::Copy(storage_, src.storage_, length_);
            storage_[length_] = '\0';
        }

        return *this;
    }

    String &operator=(const char *str) {
        ULong len = Count(str);

        HAllocator::Deallocate(storage_);
        length_  = len;
        storage_ = HAllocator::Allocate<char>(len + 1);

        if (str != nullptr) {
            Memory::Copy(storage_, str, len);
        }

        storage_[len] = '\0';

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

    String &operator+=(const char *str) {
        Insert(str, Count(str));
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

    String operator+(const char *str) const {
        const ULong len = Count(str);
        String      ns(length_ + len);
        Memory::Copy(ns.storage_, storage_, length_);
        Memory::Copy((ns.storage_ + length_), str, len);
        ns.storage_[ns.length_] = '\0';

        return ns;
    }

    inline bool operator==(const String &string) const noexcept {
        if (length_ != string.length_) {
            return false;
        }

        return Memory::Compare(storage_, string.storage_, length_);
    }

    inline bool operator==(const char *str) const noexcept {
        const ULong len = Count(str);

        if (length_ != len) {
            return false;
        }

        return Memory::Compare(storage_, str, len);
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const char *str) const noexcept {
        return (!(*this == str));
    }

    inline bool Compare(const char *str, ULong length) const noexcept {
        if (length_ != length) {
            return false;
        }

        return Memory::Compare(storage_, str, length);
    }

    void Clear() noexcept {
        HAllocator::Deallocate(storage_);
        storage_ = nullptr;
        length_  = 0;
    }

    char *Eject() noexcept {
        length_   = 0;
        char *str = storage_;
        storage_  = nullptr;

        return str;
    }

    inline const char *Storage() const noexcept {
        return storage_;
    }

    inline ULong Length() const noexcept {
        return length_;
    }

    static String Insert(const String &src1, const String &src2) {
        String ns(src1.length_ + src2.length_);
        Memory::Copy(ns.storage_, src1.storage_, src1.length_);
        Memory::Copy((ns.storage_ + src1.length_), src2.storage_, src2.length_);
        ns.storage_[ns.length_] = '\0';

        return ns;
    }

    void Insert(const char *str, ULong len) {
        if ((str != nullptr) && (len != 0)) {
            char *old_str = storage_;
            storage_      = HAllocator::Allocate<char>(length_ + len + 1);
            Memory::Copy(storage_, old_str, length_);
            Memory::Copy((storage_ + length_), str, len);
            HAllocator::Deallocate(old_str);
            length_ += len;
            storage_[length_] = '\0';
        }
    }

    template <typename Type_>
    static void LeftTrim(const char *str, Type_ &offset,
                         Type_ end_before) noexcept {
        // #ifdef QENTEM_SIMD_ENABLED_
        // constexpr unsigned long long line     = 723401728380766730ULL;
        // constexpr unsigned long long tab      = 651061555542690057ULL;
        // constexpr unsigned long long carriage = 940422246894996749ULL;
        // constexpr unsigned long long space    = 2314885530818453536ULL;

        // Type_ m_offset = offset;

        // while (m_offset < end_before) {
        //     const QMM_VAR_ m_str =
        //         QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(str +
        //         m_offset));
        //     QMM_Number_T bits =
        //         QMM_COMPARE_8_MASK_(m_str, QMM_SETONE_64_(line));
        //     bits |= QMM_COMPARE_8_MASK_(m_str, QMM_SETONE_64_(tab));
        //     bits |= QMM_COMPARE_8_MASK_(m_str, QMM_SETONE_64_(carriage));
        //     bits |= QMM_COMPARE_8_MASK_(m_str, QMM_SETONE_64_(space));
        //     bits ^= QMM_MAX_NUMBER_; // Remove any whitespace

        //     if (bits != 0) {
        //         offset = static_cast<Type_>(Q_CTZL(bits) + m_offset);

        //         if (offset > end_before) {
        //             offset = end_before;
        //         }

        //         return;
        //     }

        //     m_offset += QMM_SIZE_;
        // }
        // #else
        while ((offset < end_before) &&
               ((str[offset] == ' ') || (str[offset] == '\n') ||
                (str[offset] == '\t') || (str[offset] == '\r'))) {
            ++offset;
        }
        // #endif
    }

    template <typename Type_>
    static void SoftTrim(const char *str, Type_ &offset,
                         Type_ &length) noexcept {
        if (length != 0) {
            Type_ end = (length + offset);
            LeftTrim(str, offset, end);

            while ((--end > offset) &&
                   ((str[end] == ' ') || (str[end] == '\n') ||
                    (str[end] == '\t') || (str[end] == '\r'))) {
            }

            ++end;
            length = (end - offset);
        }
    }

    static String Trim(const String &str) {
        ULong length = str.length_;
        ULong offset = 0;

        SoftTrim(str.storage_, offset, length);

        const char *n_str = (str.storage_ + offset);
        return String(n_str, length);
    }

    template <typename NumberType = ULong>
    static NumberType Count(const char *str) noexcept {
        NumberType len = 0;

        if (str != nullptr) {
#ifdef QENTEM_SIMD_ENABLED_
            const QMM_VAR_ m_zero = QMM_SETZERO_();
            NumberType     offset = 0;

            do {
                QMM_Number_T bits = QMM_COMPARE_8_MASK_(
                    QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(str + offset)),
                    m_zero);

                if (bits != 0) {
                    return static_cast<NumberType>(Q_CTZL(bits) + offset);
                }

                offset += QMM_SIZE_;
            } while (true);
#else
            // MOST GET THE LENGTH
            while (str[len] != '\0') {
                ++len;
            };
#endif
        }

        return len;
    }

    /*
     * Unicode character converter
     *
     * char *str = new char[5];
     * ULong len;
     * len = ToUTF8(0xC3D, str);
     * len = ToUTF8(0x00A1, str);
     * len = ToUTF8(0x08A7, str);
     * len = ToUTF8(0x10A7B, str);
     * str[len] = '\0';
     */
    static UInt ToUTF8(UInt unicode, char *str) noexcept {
        if (unicode < 0x80U) {
            str[0] = char(unicode);
            return 1;
        }

        UInt length;

        if (unicode < 0x800U) {
            str[0] = char(0xC0U | (unicode >> 6U));
            length = 2;
        } else if (unicode < 0x10000U) {
            str[0] = char(0xE0U | (unicode >> 12U));
            str[1] = char(0x80U | ((unicode >> 6U) & 0x3FU));
            ++str;
            length = 3;
        } else {
            str[0] = char(0xF0U | (unicode >> 18U));
            str[1] = char(0x80U | ((unicode >> 12U) & 0x3FU));
            str[2] = char(0x80U | ((unicode >> 6U) & 0x3FU));
            str += 2;
            length = 4;
        }

        *(++str) = char(0x80U | (unicode & 0x3FU));

        return length;
    }

    //////////// Private ////////////

  private:
    ULong length_{0};
    char *storage_{nullptr};
};

} // namespace Qentem

#endif
