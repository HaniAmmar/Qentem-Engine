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
    static constexpr SizeT ShortStringMax = ((6 + (sizeof(void *))) / sizeof(Char_T_));

    String() = default;

    String(String &&src) noexcept
        : padding_(src.padding_), length_(src.length_), storage_{static_cast<QPointer<Char_T_> &&>(src.storage_)} {
        src.clearLength();
    }

    String(const String &src) { copyString(src.First(), src.Length()); }

    explicit String(SizeT len) {
        if (len != 0) {
            Char_T_ *ns = allocate(len + 1);
            ns[len]     = 0;
            setLength(len);
        }
    }

    String(Char_T_ *str, SizeT len) {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (len < ShortStringMax) {
            copyString(str, len);
            Memory::Deallocate(str);
            return;
        }
#endif
        setLength(len);
        setStorage(str);
    }

    String(const Char_T_ *str, SizeT len) { copyString(str, len); }

    String(const Char_T_ *str) { copyString(str, StringUtils::Count(str)); }

    ~String() { deallocate(Storage()); }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate(Storage());
            padding_ = src.padding_;
            length_  = src.length_;
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
            storage_.SetLowByte(src.storage_.GetLowByte());
#endif
            src.clearLength();
            storage_.MovePointerOnly(src.storage_);
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
        Insert(src.First(), src.Length());
        src.Reset();

        return *this;
    }

    String &operator+=(const String &src) {
        Insert(src.First(), src.Length());
        return *this;
    }

    String &operator+=(const Char_T_ *str) {
        Insert(str, StringUtils::Count(str));
        return *this;
    }

    String operator+(String &&src) const {
        String ns{Merge(*this, src)};
        src.deallocate(src.Storage());
        src.clearLength();
        src.clearStorage();

        return ns;
    }

    inline String operator+(const String &src) const { return Merge(*this, src); }
    String        operator+(const Char_T_ *str) const { return merge(First(), Length(), str, StringUtils::Count(str)); }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const String &src) {
        out << src.First();
        return out;
    }

    inline bool operator==(const String &string) const noexcept {
        const SizeT len = Length();

        if (len != string.Length()) {
            return false;
        }

        return StringUtils::IsEqual(First(), string.First(), len);
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT len = StringUtils::Count(str);

        if (Length() != len) {
            return false;
        }

        return StringUtils::IsEqual(First(), str, len);
    }

    inline bool operator!=(const String &string) const noexcept { return (!(*this == string)); }
    inline bool operator!=(const Char_T_ *str) const noexcept { return (!(*this == str)); }

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
        if (Length() != length) {
            return false;
        }

        return StringUtils::IsEqual(First(), str, length);
    }

    void Reset() noexcept {
        deallocate(Storage());
        clearLength();
        clearStorage();
    }

    Char_T_ *Eject() {
        Char_T_ *str;

#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        Char_T_    *src = Storage();
        const SizeT len = Length();

        if (len < ShortStringMax) {
            str = Memory::Allocate<Char_T_>(len + 1);
            Memory::Copy(str, src, ((len + 1) * sizeof(Char_T_)));
        } else {
            str = src;
        }
#else
        str = Storage();
#endif

        clearLength();
        clearStorage();

        return str;
    }

    inline SizeT Length() const noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        const unsigned char len = storage_.GetLowByte();

        return ((len != 0) ? len : length_);
#else
        return length_;
#endif
    }

    inline Char_T_ *Storage() const noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        const SizeT len = Length();
        if ((len != 0) && (len < ShortStringMax)) {
#ifndef QENTEM_BIG_ENDIAN
            return const_cast<Char_T_ *>(reinterpret_cast<const Char_T_ *>(&padding_));
#else
            // Two tags at the start
            return reinterpret_cast<Char_T_ *>(const_cast<char *>(reinterpret_cast<const char *>(&storage_) + 2));
#endif
        }
#endif
        return storage_.GetPointer();
    }

    inline const Char_T_ *First() const noexcept { return Storage(); }
    inline bool           IsEmpty() const noexcept { return (Length() == 0); }
    inline bool           IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Char_T_ *Last() const noexcept {
        const Char_T_ *src = First();

        if (src != nullptr) {
            return (src + (Length() - 1));
        }

        return src;
    }

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    void Insert(const Char_T_ *str, const SizeT len) {
        if ((str != nullptr) && (len != 0)) {
            const SizeT src_len = Length();
            const SizeT new_len = (src_len + len);
            Char_T_    *src     = Storage();

#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
            Char_T_ n_src[ShortStringMax];

            if (src_len < ShortStringMax) {
                for (SizeT i = 0; i < src_len; i++) {
                    n_src[i] = src[i];
                }

                src = &(n_src[0]);
            }
#endif

            Char_T_ *ns = allocate(new_len + 1);

            if (src != nullptr) {
                Memory::Copy(ns, src, (src_len * sizeof(Char_T_)));
                deallocate(src);
            }

            Memory::Copy((ns + src_len), str, (len * sizeof(Char_T_)));
            ns[new_len] = 0;
            setLength(new_len);
        }
    }

    static String Trim(const String &src) {
        const Char_T_ *str    = src.First();
        SizeT          length = src.Length();
        SizeT          offset = 0;

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    //////////// Private ////////////

  private:
    void clearLength() noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        storage_.SetLowByte(0);
#endif
        length_ = 0;
    }

    void setLength(SizeT new_length) noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (new_length < ShortStringMax) {
            storage_.SetLowByte(static_cast<unsigned char>(new_length));
        } else {
            storage_.SetLowByte(0);
            length_ = new_length;
        }
#else
        length_ = new_length;
#endif
    }

    void setStorage(Char_T_ *ptr) noexcept { storage_.SetPointer(ptr); }

    Char_T_ *allocate(SizeT new_size) {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (new_size <= ShortStringMax) {
#ifndef QENTEM_BIG_ENDIAN
            return reinterpret_cast<Char_T_ *>(&padding_);
#else
            // Two tags at the start
            return reinterpret_cast<Char_T_ *>((reinterpret_cast<char *>(&storage_) + 2));
#endif
        }
#endif

        Char_T_ *ns = Memory::Allocate<Char_T_>(new_size);
        setStorage(ns);
        return ns;
    }

    void deallocate(Char_T_ *old_storage) noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (Length() >= ShortStringMax) {
            Memory::Deallocate(old_storage);
        }
#else
        Memory::Deallocate(old_storage);
#endif
    }

    void clearStorage() noexcept { storage_.Reset(); }

    static String merge(const Char_T_ *str1, const SizeT len1, const Char_T_ *str2, const SizeT len2) {
        String   ns  = String{(len1 + len2)};
        Char_T_ *des = ns.Storage();

        if (len1 != 0) {
            Memory::Copy(des, str1, (len1 * sizeof(Char_T_)));
        }

        if (len2 != 0) {
            Memory::Copy((des + len1), str2, (len2 * sizeof(Char_T_)));
        }

        return ns;
    }

    void copyString(const Char_T_ *str, const SizeT len) {
        Char_T_ *ns = allocate(len + 1);
        Memory::Copy(ns, str, (len * sizeof(Char_T_)));
        ns[len] = 0;
        setLength(len);
    }

#ifndef QENTEM_BIG_ENDIAN
    SizeT             padding_{0};
    SizeT             length_{0};
    QPointer<Char_T_> storage_{};
#else
    QPointer<Char_T_> storage_{};
    SizeT padding_{0};
    SizeT length_{0};
#endif
};

} // namespace Qentem

#endif
