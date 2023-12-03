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

#ifndef QENTEM_STRING_H_
#define QENTEM_STRING_H_

namespace Qentem {

/*
 * String container with null terminator and a taggable pointer.
 */

template <typename Type_T_, bool>
struct StringData;

template <typename Char_T_>
struct StringData<Char_T_, false> {
    // Little-Endian
    StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Padding{src.Padding}, Length{src.Length}, Storage{Memory::Move(src.Storage)} {
        src.Length = 0;
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline Char_T_ *shortStorage() noexcept {
        return Memory::ChangePointer<Char_T_>(&Padding);
    }

    inline const Char_T_ *shortStorage() const noexcept {
        return Memory::ChangePointer<const Char_T_>(&Padding);
    }

    SizeT             Padding{0};
    SizeT             Length{0};
    QPointer<Char_T_> Storage{};
};

template <typename Char_T_>
struct StringData<Char_T_, true> {
    // Big-Endian
    StringData() noexcept = default;

    StringData(StringData &&src) noexcept
        : Storage{Memory::Move(src.Storage)}, Padding{src.Padding}, Length{src.Length} {
        src.Length = 0;
    }

    StringData(const StringData &)            = delete;
    StringData &operator=(StringData &&)      = delete;
    StringData &operator=(const StringData &) = delete;

    inline Char_T_ *shortStorage() noexcept {
        // Two tags at the start
        return Memory::ChangePointer<Char_T_>(Memory::ChangePointer<char>(&Storage) + 2U);
        // return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    inline const Char_T_ *shortStorage() const noexcept {
        // Two tags at the start
        return Memory::ChangePointer<const Char_T_>(Memory::ChangePointer<const char>(&Storage) + 2U);
        // return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&Storage) +
        // 2));
    }

    QPointer<Char_T_> Storage{};
    SizeT             Padding{0};
    SizeT             Length{0};
};

template <typename Char_T_>
class String {
  public:
    using CharType = Char_T_;

    // Two numbers, and one pointer -2 for tagging bytes (see QPointer).
    static constexpr SizeT ShortStringMax = ((sizeof(StringData<Char_T_, false>) - 2U) / sizeof(Char_T_));

    String() noexcept             = default;
    String(String &&src) noexcept = default;

    String(const String &src) {
        const SizeT length = src.Length();

        copyString(src.getStorage(length), length);
    }

    explicit String(SizeT len) {
        if (len != SizeT{0}) {
            Char_T_ *ns = allocate(len + SizeT{1});
            ns[len]     = Char_T_{0};
            setLength(len);
        }
    }

    String(Char_T_ *str, SizeT len) {
        if constexpr (Config::ShortStringOptimization) {
            if (len < ShortStringMax) {
                copyString(str, len);
                Memory::Deallocate(str);
                return;
            }
        }

        setLength(len);
        setStorage(str);
    }

    String(const Char_T_ *str, SizeT len) {
        copyString(str, len);
    }

    explicit String(const Char_T_ *str) {
        copyString(str, StringUtils::Count(str));
    }

    ~String() {
        deallocate();
    }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate();
            const SizeT len = src.Length();
            setLength(len);

            if constexpr (Config::ShortStringOptimization) {
                if ((len != SizeT{0}) && (len < ShortStringMax)) {
                    // Setting every variable is easier unless SizeT is Set to
                    // a type where there will be padding on the stack,
                    // for that the entire stack has to be copied.
                    Char_T_ *str = Storage();
                    Memory::Copy(str, src.Storage(), (len * sizeof(Char_T_)));
                    str[len] = Char_T_{0};
                }
            }

            data_.Storage.MovePointerOnly(src.data_.Storage);
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

    String &operator=(const Char_T_ *str) {
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

    String &operator+=(const Char_T_ *str) {
        Write(str, StringUtils::Count(str));
        return *this;
    }

    String &operator+=(const Char_T_ ch) {
        Char_T_ arr[1];
        arr[0U] = ch;
        Write(&(arr[0U]), SizeT{1});

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

    String operator+(const Char_T_ *str) const {
        const SizeT len = Length();
        return merge(getStorage(len), len, str, StringUtils::Count(str));
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const String &src) {
        out << src.First();
        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return (((len == len2)) && StringUtils::IsEqual(getStorage(len), string.getStorage(len2), len));
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = StringUtils::Count(str);

        return ((len == len2) && StringUtils::IsEqual(getStorage(len), str, len));
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsLess(getStorage(len), string.getStorage(len2), len, len2, false);
    }

    inline bool operator<(const Char_T_ *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsLess(getStorage(len), str, len, StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsLess(getStorage(len), string.getStorage(len2), len, len2, true);
    }

    inline bool operator<=(const Char_T_ *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsLess(getStorage(len), str, len, StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsGreater(getStorage(len), string.getStorage(len2), len, len2, false);
    }

    inline bool operator>(const Char_T_ *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsGreater(getStorage(len), str, len, StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        const SizeT len  = Length();
        const SizeT len2 = string.Length();

        return StringUtils::IsGreater(getStorage(len), string.getStorage(len2), len, len2, true);
    }

    inline bool operator>=(const Char_T_ *str) const noexcept {
        const SizeT len = Length();

        return StringUtils::IsGreater(getStorage(len), str, len, StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T_ *str, SizeT length) const noexcept {
        const SizeT len = Length();

        return ((len == length) && StringUtils::IsEqual(getStorage(len), str, length));
    }

    void Reset() noexcept {
        deallocate();
        clearLength();
        clearStorage();
    }

    Char_T_ *Detach() {
        Char_T_ *str;

        if constexpr (Config::ShortStringOptimization) {
            const SizeT len = Length();
            Char_T_    *src = getStorage(len);

            if (len < ShortStringMax) {
                const SizeT len2 = (len + SizeT{1});
                str              = Memory::Allocate<Char_T_>(len2);
                Memory::Copy(str, src, (len2 * sizeof(Char_T_)));
                str[len] = Char_T_{0};
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
        if constexpr (Config::ShortStringOptimization) {
            const unsigned char len = data_.Storage.GetLowByte();
            return ((len == not_short_value_) ? data_.Length : len);
        } else {
            return data_.Length;
        }
    }

    inline Char_T_ *Storage() noexcept {
        if constexpr (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
                return shortStorage();
            }
        }

        return data_.Storage.GetPointer();
    }

    inline const Char_T_ *Storage() const noexcept {
        if constexpr (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
                return shortStorage();
            }
        }

        return data_.Storage.GetPointer();
    }

    static String Merge(const String &src1, const String &src2) {
        const SizeT len1 = src1.Length();
        const SizeT len2 = src2.Length();

        return merge(src1.getStorage(len1), len1, src2.getStorage(len2), len2);
    }

    void Write(const Char_T_ *str, const SizeT len) {
        if ((str != nullptr) && (len != SizeT{0})) {
            const SizeT src_len = Length();
            SizeT       new_len = (src_len + len) + SizeT{1};
            Char_T_    *src     = getStorage(src_len);
            Char_T_    *ns;

            if constexpr (Config::ShortStringOptimization) {
                if (new_len > ShortStringMax) {
                    ns = Memory::Allocate<Char_T_>(new_len);

                    if (src != nullptr) {
                        Memory::Copy(ns, src, (src_len * sizeof(Char_T_)));
                        if (src_len >= ShortStringMax) {
                            Memory::Deallocate(src);
                        }
                    }

                    setStorage(ns);

                } else {
                    ns = Memory::ChangePointer<Char_T_>(&(data_.Padding));
                }
            } else {
                ns = allocate(new_len);

                if (src != nullptr) {
                    Memory::Copy(ns, src, (src_len * sizeof(Char_T_)));
                    Memory::Deallocate(src);
                }
            }

            Memory::Copy((ns + src_len), str, (len * sizeof(Char_T_)));
            --new_len;
            ns[new_len] = Char_T_{0};
            setLength(new_len);
        }
    }

    static String Trim(const String &src) {
        SizeT          length = src.Length();
        SizeT          offset = SizeT{0};
        const Char_T_ *str    = src.getStorage(length);

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    inline void StepBack(const SizeT len) noexcept {
        if (len <= Length()) {
            data_.Length -= len;
        }
    }

    inline void Reverse(SizeT index = SizeT{0}) noexcept {
        SizeT    end = Length();
        Char_T_ *str = getStorage(end);

        while (index < end) {
            const Char_T_ tmp = str[index];

            --end;
            str[index] = str[end];
            str[end]   = tmp;
            ++index;
        }
    }

    void InsertAt(Char_T_ ch, SizeT index) {
        const SizeT length = Length();

        if (index < length) {
            Char_T_       *str    = getStorage(length);
            Char_T_       *first  = (str + index);
            const Char_T_ *end    = (str + length);
            Char_T_       *second = first;

            Char_T_ tmp = *first;
            *first      = ch;

            while (++second < end) {
                ch      = *second;
                *second = tmp;
                tmp     = ch;
            }

            Write(&tmp, SizeT{1});
        }
    }

    inline const Char_T_ *First() const noexcept {
        return Storage();
    }

    inline Char_T_ *Last() noexcept {
        const SizeT length = Length();

        if (length != 0) {
            return (getStorage(length) + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T_ *Last() const noexcept {
        const SizeT length = Length();

        if (length != 0) {
            return (getStorage(length) + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T_ *End() const noexcept {
        const SizeT length = Length();
        return (getStorage(length) + length);
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const Char_T_ *begin() const noexcept {
        return First();
    }

    inline const Char_T_ *end() const noexcept {
        return End();
    }

    inline Char_T_ *begin() noexcept {
        return Storage();
    }

    inline Char_T_ *end() noexcept {
        const SizeT length = Length();
        return (getStorage(length) + length);
    }

    //////////// Private ////////////

  private:
    inline const Char_T_ *getStorage(SizeT length) const noexcept {
        if constexpr (Config::ShortStringOptimization) {
            if ((length != SizeT{0}) && (length < ShortStringMax)) {
                return shortStorage();
            }
        }

        return data_.Storage.GetPointer();
    }

    inline Char_T_ *getStorage(SizeT length) noexcept {
        if constexpr (Config::ShortStringOptimization) {
            if ((length != SizeT{0}) && (length < ShortStringMax)) {
                return shortStorage();
            }
        }

        return data_.Storage.GetPointer();
    }

    void clearLength() noexcept {
        if constexpr (Config::ShortStringOptimization) {
            data_.Storage.SetLowByte((unsigned char)(0));
        }

        data_.Length = SizeT{0};
    }

    void setLength(SizeT new_length) noexcept {
        if constexpr (Config::ShortStringOptimization) {
            if (new_length < ShortStringMax) {
                data_.Storage.SetLowByte((unsigned char)(new_length));
            } else {
                data_.Storage.SetLowByte(not_short_value_);
                data_.Length = new_length;
            }
        } else {
            data_.Length = new_length;
        }
    }

    void setStorage(Char_T_ *ptr) noexcept {
        data_.Storage.SetPointer(ptr);
    }

    Char_T_ *allocate(SizeT new_size) {
        if constexpr (Config::ShortStringOptimization) {
            if (new_size <= ShortStringMax) {
                return shortStorage();
            }
        }

        Char_T_ *ns = Memory::Allocate<Char_T_>(new_size);
        setStorage(ns);
        return ns;
    }

    void deallocate() noexcept {
        if constexpr (Config::ShortStringOptimization) {
            const SizeT length = Length();

            if (length >= ShortStringMax) {
                Memory::Deallocate(getStorage(length));
            }
        } else {
            Memory::Deallocate(Storage());
        }
    }

    void clearStorage() noexcept {
        data_.Storage.Reset();
    }

    inline Char_T_ *shortStorage() noexcept {
        return data_.shortStorage();
    }

    inline const Char_T_ *shortStorage() const noexcept {
        return data_.shortStorage();
    }

    static String merge(const Char_T_ *str1, const SizeT len1, const Char_T_ *str2, const SizeT len2) {
        String   ns  = String{SizeT(len1 + len2)};
        Char_T_ *des = ns.Storage();

        if (len1 != SizeT{0}) {
            Memory::Copy(des, str1, (len1 * sizeof(Char_T_)));
        }

        if (len2 != SizeT{0}) {
            Memory::Copy((des + len1), str2, (len2 * sizeof(Char_T_)));
        }

        return ns;
    }

    void copyString(const Char_T_ *str, const SizeT len) {
        Char_T_ *ns = allocate(len + SizeT{1});
        Memory::Copy(ns, str, (len * sizeof(Char_T_)));
        ns[len] = Char_T_{0};
        setLength(len);
    }

    static constexpr unsigned char           not_short_value_ = 255;
    StringData<Char_T_, Config::IsBigEndian> data_;
};

} // namespace Qentem

#endif
