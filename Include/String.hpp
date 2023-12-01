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
 *
 * If the length is less than ((6 + (sizeof(SizeT) * 2))/ sizeof(Char_T_)) the
 * string will be stored on the stack.
 */
template <typename Char_T_>
class String {
  public:
    using CharType = Char_T_;

    // Two numbers, and one pointer -2 for tagging bytes (see QPointer).
    static constexpr SizeT ShortStringMax = ((sizeof(String) - 2U) / sizeof(Char_T_));

    String() = default;

    String(String &&src) noexcept : padding_(src.padding_), length_(src.length_), storage_{Memory::Move(src.storage_)} {
        src.clearLength();
    }

    String(const String &src) {
        copyString(src.First(), src.Length());
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
        deallocate(Storage());
    }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate(Storage());
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

            storage_.MovePointerOnly(src.storage_);
            src.clearLength();
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            deallocate(Storage());
            copyString(src.First(), src.Length());
        }

        return *this;
    }

    String &operator=(const Char_T_ *str) {
        deallocate(Storage());
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
        return merge(First(), Length(), str, StringUtils::Count(str));
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const String &src) {
        out << src.First();
        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        const SizeT len    = Length();
        const SizeT length = string.Length();

        return (((len == length)) && StringUtils::IsEqual(storageLength(len), string.storageLength(length), len));
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT length = Length();
        const SizeT len    = StringUtils::Count(str);

        return ((length == len) && StringUtils::IsEqual(storageLength(length), str, len));
    }

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator<(const Char_T_ *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator<=(const Char_T_ *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), false);
    }

    inline bool operator>(const Char_T_ *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        return StringUtils::IsGreater(First(), string.First(), Length(), string.Length(), true);
    }

    inline bool operator>=(const Char_T_ *str) const noexcept {
        return StringUtils::IsGreater(First(), str, Length(), StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T_ *str, SizeT length) const noexcept {
        const SizeT len = Length();

        return ((len == length) && StringUtils::IsEqual(storageLength(len), str, length));
    }

    void Reset() noexcept {
        deallocate(Storage());
        clearLength();
        clearStorage();
    }

    Char_T_ *Detach() {
        Char_T_ *str;

        if constexpr (Config::ShortStringOptimization) {
            Char_T_    *src = Storage();
            const SizeT len = Length();

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
            const unsigned char len = storage_.GetLowByte();
            return ((len == not_short_value_) ? length_ : len);
        } else {
            return length_;
        }
    }

    inline Char_T_ *Storage() noexcept {
        if constexpr (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
#ifndef QENTEM_BIG_ENDIAN
                return Memory::ChangePointer<Char_T_>(&padding_);
#else
                // Two tags at the start
                return Memory::ChangePointer<Char_T_>(Memory::ChangePointer<char>(&storage_) + 2);
                // return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&storage_) +
                // 2));
#endif
            }
        }

        return storage_.GetPointer();
    }

    inline const Char_T_ *Storage() const noexcept {
        if constexpr (Config::ShortStringOptimization) {
            const SizeT len = Length();
            if ((len != SizeT{0}) && (len < ShortStringMax)) {
#ifndef QENTEM_BIG_ENDIAN
                return Memory::ChangePointer<const Char_T_>(&padding_);
#else
                // Two tags at the start
                return Memory::ChangePointer<const Char_T_>(Memory::ChangePointer<const char>(&storage_) + 2);
                // return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&storage_) +
                // 2));
#endif
            }
        }

        return storage_.GetPointer();
    }

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    void Write(const Char_T_ *str, const SizeT len) {
        if ((str != nullptr) && (len != SizeT{0})) {
            const SizeT src_len = Length();
            SizeT       new_len = (src_len + len) + SizeT{1};
            Char_T_    *src     = Storage();
            Char_T_    *ns;

            if constexpr (Config::ShortStringOptimization) {
                if (new_len > ShortStringMax) {
                    ns = Memory::Allocate<Char_T_>(new_len);

                    if (src != nullptr) {
                        Memory::Copy(ns, src, (src_len * sizeof(Char_T_)));
                        deallocate(src);
                    }

                    setStorage(ns);

                } else {
                    ns = Memory::ChangePointer<Char_T_>(&padding_);
                }
            } else {
                ns = allocate(new_len);

                if (src != nullptr) {
                    Memory::Copy(ns, src, (src_len * sizeof(Char_T_)));
                    deallocate(src);
                }
            }

            Memory::Copy((ns + src_len), str, (len * sizeof(Char_T_)));
            --new_len;
            ns[new_len] = Char_T_{0};
            setLength(new_len);
        }
    }

    static String Trim(const String &src) {
        const Char_T_ *str    = src.First();
        SizeT          length = src.Length();
        SizeT          offset = SizeT{0};

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    inline void StepBack(const SizeT len) noexcept {
        if (len <= Length()) {
            length_ -= len;
        }
    }

    inline void Reverse(SizeT index = SizeT{0}) noexcept {
        SizeT end = Length();

        while (index < end) {
            const Char_T_ tmp = Storage()[index];

            --end;
            Storage()[index] = Storage()[end];
            Storage()[end]   = tmp;
            ++index;
        }
    }

    void InsertAt(Char_T_ ch, SizeT index) {
        if (index < Length()) {
            Char_T_       *first  = (Storage() + index);
            Char_T_       *second = first;
            const Char_T_ *end    = End();

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

    // inline Char_T_ *Last() noexcept {
    //     if (IsNotEmpty()) {
    //         return (Storage() + (Length() - SizeT{1}));
    //     }

    //     return nullptr;
    // }

    inline const Char_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T_ *End() const noexcept {
        return (First() + Length());
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
        return (Storage() + Length());
    }

    //////////// Private ////////////

  private:
    inline const Char_T_ *storageLength(SizeT length) const noexcept {
        if constexpr (Config::ShortStringOptimization) {
            if ((length != SizeT{0}) && (length < ShortStringMax)) {
#ifndef QENTEM_BIG_ENDIAN
                return Memory::ChangePointer<const Char_T_>(&padding_);
#else
                // Two tags at the start
                return Memory::ChangePointer<const Char_T_>(Memory::ChangePointer<const char>(&storage_) + 2);
                // return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&storage_) +
                // 2));
#endif
            }
        }

        return storage_.GetPointer();
    }

    void clearLength() noexcept {
        if constexpr (Config::ShortStringOptimization) {
            storage_.SetLowByte((unsigned char)(0));
        }

        length_ = SizeT{0};
    }

    void setLength(SizeT new_length) noexcept {
        if constexpr (Config::ShortStringOptimization) {
            if (new_length < ShortStringMax) {
                storage_.SetLowByte((unsigned char)(new_length));
            } else {
                storage_.SetLowByte(not_short_value_);
                length_ = new_length;
            }
        } else {
            length_ = new_length;
        }
    }

    void setStorage(Char_T_ *ptr) noexcept {
        storage_.SetPointer(ptr);
    }

    Char_T_ *allocate(SizeT new_size) {
        if constexpr (Config::ShortStringOptimization) {
            if (new_size <= ShortStringMax) {
#ifndef QENTEM_BIG_ENDIAN
                return Memory::ChangePointer<Char_T_>(&padding_);
#else
                // Two tags at the start
                return Memory::ChangePointer<Char_T_>((Memory::ChangePointer<char>(&storage_) + 2));
#endif
            }
        }

        Char_T_ *ns = Memory::Allocate<Char_T_>(new_size);
        setStorage(ns);
        return ns;
    }

    void deallocate(Char_T_ *old_storage) noexcept {
        // TODO: remove Char_T_ *
        if constexpr (Config::ShortStringOptimization) {
            if (Length() >= ShortStringMax) {
                Memory::Deallocate(old_storage);
            }
        } else {
            Memory::Deallocate(old_storage);
        }
    }

    void clearStorage() noexcept {
        storage_.Reset();
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

    static constexpr unsigned char not_short_value_ = 255;

#ifndef QENTEM_BIG_ENDIAN
    SizeT             padding_{0};
    SizeT             length_{0};
    QPointer<Char_T_> storage_{};
#else
    QPointer<Char_T_> storage_{};
    SizeT             padding_{0};
    SizeT             length_{0};
#endif
};

} // namespace Qentem

#endif
