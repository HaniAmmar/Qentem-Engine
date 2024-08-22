/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_STRING_H
#define QENTEM_STRING_H

#include "Memory.hpp"
#include "StringUtils.hpp"

namespace Qentem {
/*
 * String container with null terminator.
 */

template <typename Char_T>
struct String {
    using CharType = Char_T;

    String() noexcept = default;

    String(String &&src) noexcept : length_{src.length_}, storage_{src.storage_} {
        src.clearLength();
        src.clearStorage();
    }

    String(const String &src) {
        copyString(src.First(), src.Length());
    }

    explicit String(SizeT len) {
        if (len != SizeT{0}) {
            Char_T *ns = allocate(len + SizeT{1});
            ns[len]    = Char_T{0};
            setLength(len);
        }
    }

    String(Char_T *str, SizeT len) {
        setLength(len);
        setStorage(str);
    }

    String(const Char_T *str, SizeT len) {
        copyString(str, len);
    }

    String(const Char_T *str) {
        copyString(str, StringUtils::Count(str));
    }

    ~String() {
        deallocate();
    }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate();
            setStorage(src.Storage());
            setLength(src.Length());
            src.clearLength();
            src.clearStorage();
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            deallocate();
            copyString(src.First(), src.Length());
        }

        return *this;
    }

    String &operator=(const Char_T *str) {
        deallocate();
        copyString(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(String &&src) {
        Write(src.First(), src.Length());
        src.Reset();

        return *this;
    }

    String &operator+=(const String &src) {
        Write(src.First(), src.Length());
        return *this;
    }

    String &operator+=(const Char_T *str) {
        Write(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(const Char_T ch) {
        Write(&ch, SizeT{1});

        return *this;
    }

    String operator+(String &&src) const {
        String ns{Merge(*this, src)};
        src.Reset();

        return ns;
    }

    inline String operator+(const String &src) const {
        return Merge(*this, src);
    }

    String operator+(const Char_T *str) const {
        return merge(First(), Length(), str, StringUtils::Count(str));
    }

    friend String &operator<<(String &out, const Char_T *str) {
        out.Write(str, StringUtils::Count(str));

        return out;
    }

    friend String &operator<<(String &out, const String &src) {
        out += src;

        return out;
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const String &src) {
        out << src.First();

        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        return (((Length() == string.Length())) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    inline bool operator==(const Char_T *str) const noexcept {
        SizeT offset{0};

        if (str != nullptr) {
            while ((*str != Char_T{0}) && (*str == First()[offset])) {
                ++str;
                ++offset;
            }
        }

        return ((*str == Char_T{0}) && (Length() == offset));
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator<(const Char_T *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator<=(const Char_T *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator>(const Char_T *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator>=(const Char_T *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T *str, SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    void Reset() noexcept {
        deallocate();
        clearStorage();
        clearLength();
    }

    Char_T *Detach() {
        Char_T *str = Storage();

        clearStorage();
        clearLength();

        return str;
    }

    inline SizeT Length() const noexcept {
        return length_;
    }

    inline Char_T *Storage() const noexcept {
        return storage_;
    }

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    void Write(const Char_T *str, const SizeT len) {
        if ((str != nullptr) && (len != SizeT{0})) {
            constexpr SizeT32 size    = sizeof(Char_T);
            const SizeT       src_len = Length();
            SizeT             new_len = (src_len + len + SizeT{1});
            Char_T           *src     = Storage();
            Char_T           *ns      = allocate(new_len);

            if (src != nullptr) {
                Memory::Copy(ns, src, (src_len * size));
                Memory::Deallocate(src);
            }

            Memory::Copy((ns + src_len), str, (len * size));
            --new_len;
            ns[new_len] = Char_T{0};
            setLength(new_len);
        }
    }

    static String Trim(const String &src) {
        SizeT         length = src.Length();
        SizeT         offset = SizeT{0};
        const Char_T *str    = src.First();

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    inline void StepBack(const SizeT len) noexcept {
        if (len <= Length()) {
            Char_T     *str     = Storage();
            const SizeT new_len = (Length() - len);

            setLength(new_len);
            str[new_len] = Char_T{0};
        }
    }

    inline void Reverse(SizeT index = SizeT{0}) noexcept {
        SizeT   end = Length();
        Char_T *str = Storage();

        while (index < end) {
            const Char_T tmp = str[index];

            --end;
            str[index] = str[end];
            str[end]   = tmp;
            ++index;
        }
    }

    void InsertAt(Char_T ch, SizeT index) {
        if (index < Length()) {
            Char_T       *str    = Storage();
            Char_T       *first  = (str + index);
            const Char_T *end    = (str + Length());
            Char_T       *second = first;

            Char_T tmp = *first;
            *first     = ch;

            while (++second < end) {
                ch      = *second;
                *second = tmp;
                tmp     = ch;
            }

            Write(&tmp, SizeT{1});
        }
    }

    inline const Char_T *First() const noexcept {
        return Storage();
    }

    inline Char_T *Last() noexcept {
        if (Length() != SizeT{0}) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T *Last() const noexcept {
        const SizeT length = Length();

        if (length != SizeT{0}) {
            return (First() + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T *End() const noexcept {
        return (First() + Length());
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Char_T *begin() const noexcept {
        return First();
    }

    inline const Char_T *end() const noexcept {
        return End();
    }

    inline Char_T *begin() noexcept {
        return Storage();
    }

    inline Char_T *end() noexcept {
        return (Storage() + Length());
    }

    //////////// Private ////////////

  private:
    void clearLength() noexcept {
        length_ = SizeT{0};
    }

    void setLength(SizeT new_length) noexcept {
        length_ = new_length;
    }

    void setStorage(Char_T *ptr) noexcept {
        storage_ = ptr;
    }

    Char_T *allocate(SizeT new_size) {
        Char_T *ns = Memory::Allocate<Char_T>(new_size);
        setStorage(ns);
        return ns;
    }

    void deallocate() noexcept {
        Memory::Deallocate(Storage());
    }

    void clearStorage() noexcept {
        storage_ = nullptr;
    }

    static String merge(const Char_T *str1, const SizeT len1, const Char_T *str2, const SizeT len2) {
        constexpr SizeT32 size = sizeof(Char_T);
        String            ns   = String{SizeT(len1 + len2)};
        Char_T           *des  = ns.Storage();

        if (len1 != SizeT{0}) {
            Memory::Copy(des, str1, (len1 * size));
        }

        if (len2 != SizeT{0}) {
            Memory::Copy((des + len1), str2, (len2 * size));
        }

        return ns;
    }

    void copyString(const Char_T *str, const SizeT len) {
        constexpr SizeT32 size = sizeof(Char_T);
        Char_T           *ns   = allocate(len + SizeT{1});
        Memory::Copy(ns, str, (len * size));
        ns[len] = Char_T{0};
        setLength(len);
    }

    SizeT   length_{0};
    Char_T *storage_{nullptr};
};

} // namespace Qentem

#endif
