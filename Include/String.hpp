/**
 * @file String.hpp
 * @brief Defines the String class for string management in Qentem Engine.
 *
 * This header provides a lightweight, high-performance string class optimized for
 * fast, and cross-platform text manipulation. String.hpp is a foundational
 * component of the Qentem Engine library, designed for dynamic template rendering,
 * JSON processing, and general-purpose string handling.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
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

    String(String &&src) noexcept : storage_{src.storage_}, length_{src.length_} {
        src.clearLength();
        src.clearStorage();
    }

    String(const String &src) {
        copyString(src.First(), src.Length());
    }

    explicit String(SizeT length) {
        if (length != 0) {
            Char_T *ns = allocate(length + SizeT{1});
            ns[length] = Char_T{0};
            setLength(length);
        }
    }

    String(Char_T *str, SizeT length) {
        setLength(length);
        setStorage(str);
    }

    String(const Char_T *str, SizeT length) {
        copyString(str, length);
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

            src.clearStorage();
            src.clearLength();
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

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    void Write(const Char_T *str, const SizeT length) {
        if ((str != nullptr) && (length != 0)) {
            const SizeT src_len = Length();
            SizeT       new_len = (src_len + length + SizeT{1});
            Char_T     *src     = Storage();
            Char_T     *ns      = allocate(new_len);

            Memory::CopyTo((ns + src_len), str, length);

            --new_len;
            ns[new_len] = Char_T{0};
            setLength(new_len);

            if (src != nullptr) {
                Memory::CopyTo(ns, src, src_len);
                Memory::Deallocate(src);
            }
        }
    }

    static String Trim(const String &src) {
        SizeT         length = src.Length();
        SizeT         offset = 0;
        const Char_T *str    = src.First();

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    inline void StepBack(const SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
            Storage()[length_] = Char_T{0};
        }
    }

    inline void Reverse(SizeT index = 0) noexcept {
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

    inline void InsertAt(Char_T ch, SizeT index) {
        if (index < Length()) {
            const SizeT new_length = (Length() + SizeT{1});

            Char_T *new_storage = Memory::Allocate<Char_T>(new_length);

            // 1. Copy prefix [0, index)
            if (index != 0) {
                Memory::CopyTo(new_storage, Storage(), index);
            }

            // 2. Insert new char at 'index'
            new_storage[index] = ch;

            // 3. Copy suffix [index, length)
            if (index < Length()) {
                Memory::CopyTo(new_storage + index + 1, (Storage() + index), (Length() - index));
            }

            // Clean up old storage and set new storage/capacity
            Memory::Deallocate(Storage());
            setStorage(new_storage);
            setLength(new_length);
        }
    }

    inline SizeT Length() const noexcept {
        return length_;
    }

    inline void SetLength(SizeT length) {
        if (Length() < length) {
            expand(length);
        }

        if (Length() != 0) {
            Storage()[length] = Char_T{0};
            setLength(length); // Update internal length
        }
    }

    inline Char_T *Storage() noexcept {
        return storage_;
    }

    inline const Char_T *Storage() const noexcept {
        return storage_;
    }

    inline const Char_T *First() const noexcept {
        return Storage();
    }

    inline Char_T *Last() noexcept {
        if (Length() != 0) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T *Last() const noexcept {
        const SizeT length = Length();

        if (length != 0) {
            return (First() + (length - SizeT{1}));
        }

        return nullptr;
    }

    inline const Char_T *End() const noexcept {
        return (First() + Length());
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == 0);
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
        length_ = 0;
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
        String  ns  = String{SizeT(len1 + len2)};
        Char_T *des = ns.Storage();

        if (len1 != 0) {
            Memory::CopyTo(des, str1, len1);
        }

        if (len2 != 0) {
            Memory::CopyTo((des + len1), str2, len2);
        }

        return ns;
    }

    void expand(const SizeT new_length) {
        Char_T *old_storage = Storage();
        Char_T *new_storage = allocate(new_length + SizeT{1});

        if (Length() != 0) {
            Memory::CopyTo(new_storage, old_storage, Length());
        }

        deallocate();            // Free the old storage
        setStorage(new_storage); // Swap in new pointer
    }

    void copyString(const Char_T *str, const SizeT length) {
        Char_T *ns = allocate(length + SizeT{1});

        Memory::CopyTo(ns, str, length);

        ns[length] = Char_T{0};

        setLength(length);
    }

    Char_T *storage_{nullptr};
    SizeT   length_{0};
};

} // namespace Qentem

#endif
