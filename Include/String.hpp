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
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
    static constexpr SizeT short_string_max =
        ((6 + (sizeof(SizeT) * 2)) / sizeof(Char_T_));
#endif

  public:
    String() = default;

    String(String &&src) noexcept : unused(src.unused), length_(src.length_) {
        storage_.Set(static_cast<QPointer<Char_T_> &&>(src.storage_));
        src.clearLength();
    }

    String(const String &src) { copyString(src.First(), src.Length()); }

    explicit String(SizeT len) {
        setLength(len);
        allocate(len + 1)[len] = 0;
    }

    String(Char_T_ *str, SizeT len) noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (len < short_string_max) {
            copyString(str, len);
            Memory::Deallocate(str);
            return;
        }
#endif
        setLength(len);
        setStorage(str);
    }

    String(const Char_T_ *str, SizeT len) { copyString(str, len); }

    explicit String(const Char_T_ *str) {
        copyString(str, StringUtils::Count(str));
    }

    ~String() { deallocate(Storage()); }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate(Storage());
            unused  = src.unused;
            length_ = src.length_;
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
            storage_.SetLowTag(src.storage_.GetLowTag());
#endif
            storage_ = static_cast<QPointer<Char_T_> &&>(src.storage_);

            src.Reset();
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

    inline String operator+(const String &src) const {
        return Merge(*this, src);
    }

    String operator+(const Char_T_ *str) const {
        const SizeT src_len = Length();

        const SizeT len = StringUtils::Count(str);
        String      ns  = String{src_len + len};
        Char_T_ *   des = ns.Storage();

        if (IsNotEmpty()) {
            Memory::Copy(des, First(), (src_len * sizeof(Char_T_)));
        }

        if (len != 0) {
            Memory::Copy((des + src_len), str, (len * sizeof(Char_T_)));
        }

        return ns;
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

    inline bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool operator<(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(),
                                   string.Length(), false);
    }

    inline bool operator<(const Char_T_ *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(),
                                   StringUtils::Count(str), false);
    }

    inline bool operator<=(const String &string) const noexcept {
        return StringUtils::IsLess(First(), string.First(), Length(),
                                   string.Length(), true);
    }

    inline bool operator<=(const Char_T_ *str) const noexcept {
        return StringUtils::IsLess(First(), str, Length(),
                                   StringUtils::Count(str), true);
    }

    inline bool operator>(const String &string) const noexcept {
        return StringUtils::IsBigger(First(), string.First(), Length(),
                                     string.Length(), false);
    }

    inline bool operator>(const Char_T_ *str) const noexcept {
        return StringUtils::IsBigger(First(), str, Length(),
                                     StringUtils::Count(str), false);
    }

    inline bool operator>=(const String &string) const noexcept {
        return StringUtils::IsBigger(First(), string.First(), Length(),
                                     string.Length(), true);
    }

    inline bool operator>=(const Char_T_ *str) const noexcept {
        return StringUtils::IsBigger(First(), str, Length(),
                                     StringUtils::Count(str), true);
    }

    inline bool IsEqual(const Char_T_ *str, SizeT length) const noexcept {
        if (Length() != length) {
            return false;
        }

        return StringUtils::IsEqual(First(), str, length);
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, String src) {
        const Char_T_ *str = src.First();
        const Char_T_ *end = (str + src.Length());

        while (str != end) {
            out << *str;
            ++str;
        }

        return out;
    }

    void Reset() noexcept {
        deallocate(Storage());
        clearLength();
        clearStorage();
    }

    Char_T_ *Eject() noexcept {
        Char_T_ *str;

#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        Char_T_ *   src = Storage();
        const SizeT len = Length();

        if (len < short_string_max) {
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
        const unsigned char len = storage_.GetLowTag();

        return ((len != 0) ? len : length_);
#else
        return length_;
#endif
    }

    inline Char_T_ *Storage() const noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        const SizeT len = Length();
        if ((len != 0) && (len < short_string_max)) {
#ifndef QENTEM_BIG_ENDIAN
            return const_cast<Char_T_ *>(
                reinterpret_cast<const Char_T_ *>(&unused));
#else
            // Two tags at the start
            return reinterpret_cast<Char_T_ *>(const_cast<char *>(
                reinterpret_cast<const char *>(&storage_) + 2));
#endif
        }
#endif
        return storage_.GetPointer();
    }

    inline const Char_T_ *First() const noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        const SizeT len = Length();
        if ((len != 0) && (len < short_string_max)) {
#ifndef QENTEM_BIG_ENDIAN
            return reinterpret_cast<const Char_T_ *>(&unused);
#else
            // Two tags at the start
            return reinterpret_cast<const Char_T_ *>(
                reinterpret_cast<const char *>(&storage_) + 2);
#endif
        }
#endif
        return storage_.GetPointer();
    }

    inline bool IsEmpty() const noexcept { return (Length() == 0); }
    inline bool IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Char_T_ *Last() const noexcept {
        const Char_T_ *src = First();

        if (src != nullptr) {
            return (src + (Length() - 1));
        }

        return src;
    }

    static String Merge(const String &src1, const String &src2) {
        const SizeT ns_len = (src1.Length() + src2.Length());
        String      ns     = String{ns_len};
        Char_T_ *   des    = ns.Storage();

        if (src1.IsNotEmpty()) {
            Memory::Copy(des, src1.First(), (src1.Length() * sizeof(Char_T_)));
        }

        if (src2.IsNotEmpty()) {
            Memory::Copy((des + src1.Length()), src2.First(),
                         (src2.Length() * sizeof(Char_T_)));
        }

        return ns;
    }

    void Insert(const Char_T_ *str, SizeT len) {
        if ((str != nullptr) && (len != 0)) {
            const SizeT src_len = Length();
            const SizeT new_len = (src_len + len);
            Char_T_ *   src     = Storage();
            Char_T_ *   des     = allocate(new_len + 1);

            if (src != nullptr) {
                Memory::Copy(des, src, (src_len * sizeof(Char_T_)));
                deallocate(src);
            }

            Memory::Copy((des + src_len), str, (len * sizeof(Char_T_)));
            setLength(new_len);
            des[new_len] = 0;
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
        storage_.SetLowTag(0);
#endif
        length_ = 0;
    }

    void setLength(SizeT new_length) noexcept {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (new_length < short_string_max) {
            storage_.SetLowTag(static_cast<unsigned char>(new_length));
        } else {
            storage_.SetLowTag(0);
            length_ = new_length;
        }
#else
        length_ = new_length;
#endif
    }

    void     setStorage(Char_T_ *ptr) noexcept { storage_.SetPointer(ptr); }
    Char_T_ *allocate(SizeT new_size) {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (new_size <= short_string_max) {
#ifndef QENTEM_BIG_ENDIAN
            return reinterpret_cast<Char_T_ *>(&unused);
#else
            // Two tags at the start
            return reinterpret_cast<Char_T_ *>(
                (reinterpret_cast<char *>(&storage_) + 2));
#endif
        }
#endif

        Char_T_ *new_storage = Memory::Allocate<Char_T_>(new_size);
        setStorage(new_storage);
        return new_storage;
    }

    void deallocate(Char_T_ *old_storage) {
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
        if (Length() >= short_string_max) {
            Memory::Deallocate(old_storage);
        }
#else
        Memory::Deallocate(old_storage);
#endif
    }

    void clearStorage() noexcept { setStorage(nullptr); }

    void copyString(const Char_T_ *str, SizeT len) {
        setLength(len);
        Char_T_ *des;
        des = allocate(len + 1);
        Memory::Copy(des, str, (len * sizeof(Char_T_)));
        des[len] = 0;
    }

#ifndef QENTEM_BIG_ENDIAN
    SizeT             unused{0};
    SizeT             length_{0};
    QPointer<Char_T_> storage_{};
#else
    QPointer<Char_T_> storage_{};
    SizeT unused{0};
    SizeT length_{0};
#endif
};

} // namespace Qentem

#endif
