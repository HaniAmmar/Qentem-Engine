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

#ifndef QENTEM_STRING_VIEW_H
#define QENTEM_STRING_VIEW_H

#include "QPointer.hpp"
#include "StringUtils.hpp"

namespace Qentem {

template <typename, bool>
struct StringViewData;

template <typename Char_T>
struct StringViewData<Char_T, false> {
    // Little-Endian
    StringViewData() noexcept  = default;
    ~StringViewData() noexcept = default;

    StringViewData(StringViewData &&src) noexcept
        : Length{src.Length}, Padding{src.Padding}, Storage{Memory::Move(src.Storage)} {
        src.Length = 0;
    }

    StringViewData(const StringViewData &)            = delete;
    StringViewData &operator=(StringViewData &&)      = delete;
    StringViewData &operator=(const StringViewData &) = delete;

    SizeT                  Length{0};
    SizeT                  Padding{0};
    QPointer<const Char_T> Storage{};
};

template <typename Char_T>
struct StringViewData<Char_T, true> {
    // Big-Endian
    StringViewData() noexcept  = default;
    ~StringViewData() noexcept = default;

    StringViewData(StringViewData &&src) noexcept
        : Storage{Memory::Move(src.Storage)}, Padding{src.Padding}, Length{src.Length} {
        src.Length = SizeT{0};
    }

    StringViewData(const StringViewData &)            = delete;
    StringViewData &operator=(StringViewData &&)      = delete;
    StringViewData &operator=(const StringViewData &) = delete;

    QPointer<const Char_T> Storage{};
    SizeT                  Padding{0};
    SizeT                  Length{0};
};

template <typename Char_T>
struct StringView {
    using CharType = Char_T;

    StringView() noexcept                 = default;
    StringView(StringView &&src) noexcept = default;
    ~StringView() noexcept                = default;

    StringView(const StringView &src) {
        data_.Length = src.data_.Length;
    }

    StringView(const Char_T *str, SizeT len) {
        setLength(len);
        setStorage(str);
    }

    StringView(const Char_T *str) {
        setLength(StringUtils::Count(str));
        setStorage(str);
    }

    StringView &operator=(StringView &&src) noexcept {
        if (this != &src) {
            setLength(src.Length());
            data_.Storage.MovePointerOnly(src.data_.Storage);
            src.clearLength();
        }

        return *this;
    }

    StringView &operator=(const StringView &src) {
        if (this != &src) {
            setLength(src.Length());
            setStorage(src.First());
        }

        return *this;
    }

    StringView &operator=(const Char_T *str) {
        setLength(StringUtils::Count(str));
        setStorage(str);

        return *this;
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const StringView &src) {
        const Char_T *str = src.First();
        SizeT         index{0};

        while (index < src.Length()) {
            out << str[index];
            ++index;
        }

        return out;
    }

    inline bool operator==(const StringView &string) const noexcept {
        return (((Length() == string.Length())) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    inline bool operator==(const Char_T *str) const noexcept {
        return ((Length() == StringUtils::Count(str)) && StringUtils::IsEqual(First(), str, Length()));
    }

    inline bool operator!=(const StringView &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const StringView &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator<(const Char_T *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator<=(const StringView &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator<=(const Char_T *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool operator>(const StringView &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator>(const Char_T *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator>=(const StringView &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator>=(const Char_T *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T *str, SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    void Reset() noexcept {
        clearStorage();
        clearLength();
    }

    inline SizeT Length() const noexcept {
        return data_.Length;
    }

    inline const Char_T *First() const noexcept {
        return data_.Storage.GetPointer();
    }

    inline const Char_T *Last() const noexcept {
        if (Length() != SizeT{0}) {
            return (First() + (Length() - SizeT{1}));
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

    inline SizeT8 GetHighByte() const noexcept {
        return data_.Storage.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        data_.Storage.SetHighByte(byte);
    }

    inline SizeT8 GetLowByte() const noexcept {
        return data_.Storage.GetLowByte();
    }

    inline void SetLowByte(SizeT8 byte) noexcept {
        data_.Storage.SetLowByte(byte);
    }

    // For STL
    inline const Char_T *begin() const noexcept {
        return First();
    }

    inline const Char_T *end() const noexcept {
        return End();
    }

    //////////// Private ////////////

  private:
    void clearLength() noexcept {
        data_.Length = SizeT{0};
    }

    void setLength(SizeT length) noexcept {
        data_.Length = length;
    }

    void setStorage(const Char_T *ptr) noexcept {
        data_.Storage.SetPointer(ptr);
    }

    void clearStorage() noexcept {
        data_.Storage.Reset();
    }

    StringViewData<Char_T, Config::IsBigEndian> data_;
};

} // namespace Qentem

#endif
