/*
 * Copyright (c) 2023 Hani Ammar
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
#include "QPointer.hpp"
#include "StringUtils.hpp"

#ifndef _QENTEM_STRING_H
#define _QENTEM_STRING_H

namespace Qentem {

/*
 * String container with null terminator and a taggable pointer.
 */

template <typename _Type_T, bool>
struct StringData;

template <typename _Char_T>
struct StringData<_Char_T, false> {
    // Little-Endian
    static constexpr SizeT8 _not_short_value = 255;

    StringData() noexcept  = default;
    ~StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Length{src.Length}, Padding{src.Padding}, Storage{Memory::Move(src.Storage)} {
        src.Length = 0;
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline _Char_T *GetShortStorage() noexcept {
        return Memory::ChangePointer<_Char_T>(&Length);
    }

    inline const _Char_T *GetShortStorage() const noexcept {
        return Memory::ChangePointer<const _Char_T>(&Length);
    }

    SizeT             Length{0};
    SizeT             Padding{0};
    QPointer<_Char_T> Storage{};
};

template <typename _Char_T>
struct StringData<_Char_T, true> {
    // Big-Endian
    static constexpr SizeT8 _not_short_value = 255;

    StringData() noexcept  = default;
    ~StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Storage{Memory::Move(src.Storage)}, Padding{src.Padding}, Length{src.Length} {
        src.Length = SizeT{0};
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline _Char_T *GetShortStorage() noexcept {
        // Two tags at the start
        return Memory::ChangePointer<_Char_T>(Memory::ChangePointer<char>(&Storage) + 2U);
        // return reinterpret_cast<_Char_T *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    inline const _Char_T *GetShortStorage() const noexcept {
        // Two tags at the start
        return Memory::ChangePointer<const _Char_T>(Memory::ChangePointer<const char>(&Storage) + 2U);
        // return reinterpret_cast<_Char_T *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    QPointer<_Char_T> Storage{};
    SizeT             Padding{0};
    SizeT             Length{0};
};

template <typename _Char_T>
struct String {
    using CharType = _Char_T;

    // Two numbers, and one pointer -2 for tagging bytes (see QPointer).
    static constexpr SizeT ShortStringMax = ((sizeof(StringData<_Char_T, false>) - 2U) / sizeof(_Char_T));

    String() noexcept             = default;
    String(String &&src) noexcept = default;

    String(const String &src) {
        const SizeT length = src.Length();

        copyString(src.getStorage(length), length);
    }

    explicit String(SizeT len) {
        if (len != SizeT{0}) {
            _Char_T *ns = allocate(len + SizeT{1});
            ns[len]     = _Char_T{0};
            setLength(len);
        }
    }

    String(_Char_T *str, SizeT len) {
        if (Config::ShortStringOptimization) {
            if (len < ShortStringMax) {
                copyString(str, len);
                Memory::Deallocate(str);
                return;
            }
        }

        setLength(len);
        setStorage(str);
    }

    String(const _Char_T *str, SizeT len) {
        copyString(str, len);
    }

    String(const _Char_T *str) {
        copyString(str, StringUtils::Count(str));
    }

    ~String() {
        deallocate();
    }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate();

            if (Config::ShortStringOptimization) {
                _data.Length  = src._data.Length;
                _data.Padding = src._data.Padding;
                setLength(src.Length());
            } else {
                _data.Length = src._data.Length;
            }

            _data.Storage.MovePointerOnly(src._data.Storage);
            src.clearLength();
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            deallocate();
            const SizeT len = src.Length();
            copyString(src.getStorage(len), len);
        }

        return *this;
    }

    String &operator=(const _Char_T *str) {
        deallocate();
        copyString(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(String &&src) {
        const SizeT len = src.Length();

        Write(src.getStorage(len), len);
        src.Reset();

        return *this;
    }

    String &operator+=(const String &src) {
        const SizeT len = src.Length();

        Write(src.getStorage(len), len);
        return *this;
    }

    String &operator+=(const _Char_T *str) {
        Write(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(const _Char_T ch) {
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

    String operator+(const _Char_T *str) const {
        const SizeT len = Length();
        return merge(getStorage(len), len, str, StringUtils::Count(str));
    }

    template <typename _Stream_T>
    friend _Stream_T &operator<<(_Stream_T &out, const String &src) {
        out << src.First();

        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return (((len == len2)) && StringUtils::IsEqual(getStorage(len), string.getStorage(len2), len));
    }

    inline bool operator==(const _Char_T *str) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = StringUtils::Count(str);

        return ((len == len2) && StringUtils::IsEqual(getStorage(len), str, len));
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const _Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsLess(getStorage(len), string.getStorage(len2), len, len2, false);
    }

    inline bool operator<(const _Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsLess(getStorage(len), str, len, StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsLess(getStorage(len), string.getStorage(len2), len, len2, true);
    }

    inline bool operator<=(const _Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsLess(getStorage(len), str, len, StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsGreater(getStorage(len), string.getStorage(len2), len, len2, false);
    }

    inline bool operator>(const _Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsGreater(getStorage(len), str, len, StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsGreater(getStorage(len), string.getStorage(len2), len, len2, true);
    }

    inline bool operator>=(const _Char_T *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsGreater(getStorage(len), str, len, StringUtils::Count(str), true);
    }

    inline bool IsEqual(const _Char_T *str, SizeT length) const noexcept {
        const SizeT len = Length();

        return ((len == length) && StringUtils::IsEqual(getStorage(len), str, length));
    }

    void Reset() noexcept {
        deallocate();
        clearLength();
        clearStorage();
    }

    _Char_T *Detach() {
        _Char_T *str;

        if (Config::ShortStringOptimization) {
            constexpr SizeT32 size = sizeof(_Char_T);
            const SizeT       len  = Length();
            _Char_T          *src  = getStorage(len);

            if (len < ShortStringMax) {
                const SizeT len2 = (len + SizeT{1});
                str              = Memory::Allocate<_Char_T>(len2);
                Memory::Copy<size>(str, src, (len2 * size));
                str[len] = _Char_T{0};
            } else {
                str = src;
            }
        } else {
            str = Storage();
        }

        clearLength();
        clearStorage();

        return str;
    }

    inline SizeT Length() const noexcept {
        if (Config::ShortStringOptimization) {
            const SizeT8 len = _data.Storage.GetLowByte();
            return ((len == _not_short_value) ? _data.Length : len);
        } else {
            return _data.Length;
        }
    }

    inline _Char_T *Storage() noexcept {
        if (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
                return GetShortStorage();
            }
        }

        return _data.Storage.GetPointer();
    }

    inline const _Char_T *Storage() const noexcept {
        if (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
                return GetShortStorage();
            }
        }

        return _data.Storage.GetPointer();
    }

    static String Merge(const String &src1, const String &src2) {
        const SizeT len1 = src1.Length();
        const SizeT len2 = src2.Length();

        return merge(src1.getStorage(len1), len1, src2.getStorage(len2), len2);
    }

    void Write(const _Char_T *str, const SizeT len) {
        if ((str != nullptr) && (len != SizeT{0})) {
            constexpr SizeT32 size    = sizeof(_Char_T);
            const SizeT       src_len = Length();
            SizeT             new_len = SizeT((src_len + len) + SizeT{1});
            _Char_T          *src     = getStorage(src_len);
            _Char_T          *ns;

            if (Config::ShortStringOptimization) {
                if (new_len <= ShortStringMax) {
                    ns = _data.GetShortStorage();

                } else {
                    ns = Memory::Allocate<_Char_T>(new_len);

                    if (src != nullptr) {
                        Memory::Copy<size>(ns, src, (src_len * size));

                        if (src_len >= ShortStringMax) {
                            Memory::Deallocate(src);
                        }
                    }

                    setStorage(ns);
                }
            } else {
                ns = allocate(new_len);

                if (src != nullptr) {
                    Memory::Copy<size>(ns, src, (src_len * size));
                    Memory::Deallocate(src);
                }
            }

            Memory::Copy<size>((ns + src_len), str, (len * size));
            --new_len;
            ns[new_len] = _Char_T{0};
            setLength(new_len);
        }
    }

    static String Trim(const String &src) {
        SizeT          length = src.Length();
        SizeT          offset = SizeT{0};
        const _Char_T *str    = src.getStorage(length);

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    inline void StepBack(const SizeT len) noexcept {
        const SizeT length = Length();

        if (len <= length) {
            _Char_T    *str     = getStorage(length);
            const SizeT new_len = (length - len);

            if (Config::ShortStringOptimization) {
                if (new_len < ShortStringMax) {
                    if (length >= ShortStringMax) {
                        constexpr SizeT32 size = sizeof(_Char_T);

                        _Char_T *str_s = getStorage(new_len);
                        Memory::Copy<size>(str_s, str, (new_len * size));
                        Memory::Deallocate(str);
                        str = str_s;
                    }
                }
            }

            setLength(new_len);
            str[new_len] = _Char_T{0};
        }
    }

    inline void Reverse(SizeT index = SizeT{0}) noexcept {
        SizeT    end = Length();
        _Char_T *str = getStorage(end);

        while (index < end) {
            const _Char_T tmp = str[index];

            --end;
            str[index] = str[end];
            str[end]   = tmp;
            ++index;
        }
    }

    void InsertAt(_Char_T ch, SizeT index) {
        const SizeT length = Length();

        if (index < length) {
            _Char_T       *str    = getStorage(length);
            _Char_T       *first  = (str + index);
            const _Char_T *end    = (str + length);
            _Char_T       *second = first;

            _Char_T tmp = *first;
            *first      = ch;

            while (++second < end) {
                ch      = *second;
                *second = tmp;
                tmp     = ch;
            }

            Write(&tmp, SizeT{1});
        }
    }

    inline const _Char_T *First() const noexcept {
        return Storage();
    }

    inline _Char_T *Last() noexcept {
        const SizeT length = Length();

        if (length != SizeT{0}) {
            return (getStorage(length) + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const _Char_T *Last() const noexcept {
        const SizeT length = Length();

        if (length != SizeT{0}) {
            return (getStorage(length) + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const _Char_T *End() const noexcept {
        const SizeT length = Length();
        return (getStorage(length) + length);
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    inline SizeT8 GetHighByte() const noexcept {
        return _data.Storage.GetHighByte();
    }

    inline void SetHighByte(SizeT8 byte) noexcept {
        _data.Storage.SetHighByte(byte);
    }

    // For STL
    inline const _Char_T *begin() const noexcept {
        return First();
    }

    inline const _Char_T *end() const noexcept {
        return End();
    }

    inline _Char_T *begin() noexcept {
        return Storage();
    }

    inline _Char_T *end() noexcept {
        const SizeT length = Length();
        return (getStorage(length) + length);
    }

    //////////// Private ////////////

  private:
    inline const _Char_T *getStorage(SizeT length) const noexcept {
        if (Config::ShortStringOptimization) {
            if ((length != SizeT{0}) && (length < ShortStringMax)) {
                return GetShortStorage();
            }
        }

        return _data.Storage.GetPointer();
    }

    inline _Char_T *getStorage(SizeT length) noexcept {
        if (Config::ShortStringOptimization) {
            if ((length != SizeT{0}) && (length < ShortStringMax)) {
                return GetShortStorage();
            }
        }

        return _data.Storage.GetPointer();
    }

    void clearLength() noexcept {
        if (Config::ShortStringOptimization) {
            _data.Storage.SetLowByte(SizeT8{0});
        }

        _data.Length = SizeT{0};
    }

    void setLength(SizeT new_length) noexcept {
        if (Config::ShortStringOptimization) {
            if (new_length < ShortStringMax) {
                _data.Storage.SetLowByte(SizeT8(new_length));
            } else {
                _data.Storage.SetLowByte(_not_short_value);
                _data.Length = new_length;
            }
        } else {
            _data.Length = new_length;
        }
    }

    void setStorage(_Char_T *ptr) noexcept {
        _data.Storage.SetPointer(ptr);
    }

    _Char_T *allocate(SizeT new_size) {
        if (Config::ShortStringOptimization) {
            if (new_size <= ShortStringMax) {
                return GetShortStorage();
            }
        }

        _Char_T *ns = Memory::Allocate<_Char_T>(new_size);
        setStorage(ns);
        return ns;
    }

    void deallocate() noexcept {
        if (Config::ShortStringOptimization) {
            const SizeT length = Length();

            if (length >= ShortStringMax) {
                Memory::Deallocate(getStorage(length));
            }
        } else {
            Memory::Deallocate(Storage());
        }
    }

    void clearStorage() noexcept {
        _data.Storage.Reset();
    }

    inline _Char_T *GetShortStorage() noexcept {
        return _data.GetShortStorage();
    }

    inline const _Char_T *GetShortStorage() const noexcept {
        return _data.GetShortStorage();
    }

    static String merge(const _Char_T *str1, const SizeT len1, const _Char_T *str2, const SizeT len2) {
        constexpr SizeT32 size = sizeof(_Char_T);
        String            ns   = String{SizeT(len1 + len2)};
        _Char_T          *des  = ns.Storage();

        if (len1 != SizeT{0}) {
            Memory::Copy<size>(des, str1, (len1 * size));
        }

        if (len2 != SizeT{0}) {
            Memory::Copy<size>((des + len1), str2, (len2 * size));
        }

        return ns;
    }

    void copyString(const _Char_T *str, const SizeT len) {
        constexpr SizeT32 size = sizeof(_Char_T);
        _Char_T          *ns   = allocate(len + SizeT{1});
        Memory::Copy<size>(ns, str, (len * size));
        ns[len] = _Char_T{0};
        setLength(len);
    }

    static constexpr SizeT8                  _not_short_value = 255;
    StringData<_Char_T, Config::IsBigEndian> _data;
};

} // namespace Qentem

#endif
