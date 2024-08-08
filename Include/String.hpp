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
#include "QPointer.hpp"
#include "StringUtils.hpp"

namespace Qentem {
/*
 * String container with null terminator and a taggable pointer.
 */

template <typename, bool>
struct StringData;

template <typename Char_T>
struct StringData<Char_T, false> {
    // Little-Endian
    StringData() noexcept  = default;
    ~StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Length{src.Length}, Padding{src.Padding}, Storage{Memory::Move(src.Storage)} {
        src.Length = 0;
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline Char_T *GetShortStorage() noexcept {
        return Memory::ChangePointer<Char_T>(&Length);
    }

    inline const Char_T *GetShortStorage() const noexcept {
        return Memory::ChangePointer<const Char_T>(&Length);
    }

    SizeT            Length{0};
    SizeT            Padding{0};
    QPointer<Char_T> Storage{};
};

template <typename Char_T>
struct StringData<Char_T, true> {
    // Big-Endian
    StringData() noexcept  = default;
    ~StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Storage{Memory::Move(src.Storage)}, Padding{src.Padding}, Length{src.Length} {
        src.Length = SizeT{0};
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline Char_T *GetShortStorage() noexcept {
        // Two tags at the start
        return Memory::ChangePointer<Char_T>(Memory::ChangePointer<char>(&Storage) + 2U);
        // return reinterpret_cast<Char_T *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    inline const Char_T *GetShortStorage() const noexcept {
        // Two tags at the start
        return Memory::ChangePointer<const Char_T>(Memory::ChangePointer<const char>(&Storage) + 2U);
        // return reinterpret_cast<Char_T *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    QPointer<Char_T> Storage{};
    SizeT            Padding{0};
    SizeT            Length{0};
};

template <typename Char_T>
struct String {
    using CharType = Char_T;

    // Two numbers, and one pointer -2 for tagging bytes (see QPointer).
    static constexpr SizeT ShortStringMax = ((sizeof(StringData<Char_T, false>) - 2U) / sizeof(Char_T));

    String() noexcept             = default;
    String(String &&src) noexcept = default;

    String(const String &src) {
        copyString(src.Storage(), src.Length());
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
            setLength(src.Length());
            data_.Storage.MovePointerOnly(src.data_.Storage);
            src.clearLength();
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            deallocate();
            copyString(src.Storage(), src.Length());
        }

        return *this;
    }

    String &operator=(const Char_T *str) {
        deallocate();
        copyString(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(String &&src) {
        Write(src.Storage(), src.Length());
        src.Reset();

        return *this;
    }

    String &operator+=(const String &src) {
        Write(src.Storage(), src.Length());
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
        return merge(Storage(), Length(), str, StringUtils::Count(str));
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const String &src) {
        out << src.First();

        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        return (((Length() == string.Length())) && StringUtils::IsEqual(Storage(), string.Storage(), Length()));
    }

    inline bool operator==(const Char_T *str) const noexcept {
        SizeT offset{0};

        if (str != nullptr) {
            while ((*str != Char_T{0}) && (*str == Storage()[offset])) {
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
        return StringUtils::IsLess(Storage(), string.Storage(), Length(), string.Length(), false);
    }

    inline bool operator<(const Char_T *str) const noexcept {
        return StringUtils::IsLess(Storage(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        return StringUtils::IsLess(Storage(), string.Storage(), Length(), string.Length(), true);
    }

    inline bool operator<=(const Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsLess(Storage(len), str, len, StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        return StringUtils::IsGreater(Storage(), string.Storage(), Length(), string.Length(), false);
    }

    inline bool operator>(const Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsGreater(Storage(len), str, len, StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        return StringUtils::IsGreater(Storage(), string.Storage(), Length(), string.Length(), true);
    }

    inline bool operator>=(const Char_T *str) const noexcept {
        return StringUtils::IsGreater(Storage(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T *str, SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(Storage(), str, length));
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
        return data_.Length;
    }

    inline Char_T *Storage() noexcept {
        return data_.Storage.GetPointer();
    }

    inline const Char_T *Storage() const noexcept {
        return data_.Storage.GetPointer();
    }

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.Storage(), src1.Length(), src2.Storage(), src2.Length());
    }

    void Write(const Char_T *str, const SizeT len) {
        if ((str != nullptr) && (len != SizeT{0})) {
            constexpr SizeT32 size    = sizeof(Char_T);
            const SizeT       src_len = Length();
            SizeT             new_len = SizeT((src_len + len) + SizeT{1});
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
        const Char_T *str    = src.Storage();

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
            return (Storage(length) + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T *End() const noexcept {
        const SizeT length = Length();
        return (Storage(length) + length);
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    inline SizeT8 GetHighByte() const noexcept {
        return data_.Storage.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        data_.Storage.SetHighByte(byte);
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
        data_.Length = SizeT{0};
    }

    void setLength(SizeT new_length) noexcept {
        data_.Length = new_length;
    }

    void setStorage(Char_T *ptr) noexcept {
        data_.Storage.SetPointer(ptr);
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
        data_.Storage.Reset();
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

    static constexpr SizeT8 not_short_value_ = 255;

    StringData<Char_T, Config::IsBigEndian> data_;
};

} // namespace Qentem

#endif
