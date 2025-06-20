/**
 * @file StringView.hpp
 * @brief Provides a lightweight, non-owning view of character sequences for Qentem Engine.
 *
 * This header defines the StringView struct, enabling efficient manipulation and
 * inspection of string data without copying or owning the underlying buffer.
 * StringView is a core utility throughout the Qentem Engine library for high-performance,
 * zero-overhead string operations.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_STRING_VIEW_H
#define QENTEM_STRING_VIEW_H

#include "StringUtils.hpp"

namespace Qentem {

template <typename Char_T>
struct StringView {
    using CharType = Char_T;

    StringView() noexcept  = default;
    ~StringView() noexcept = default;

    QENTEM_CONST_EXPRESSION StringView(StringView &&src) noexcept : storage_{src.First()}, length_{src.Length()} {
        src.clearStorage();
        src.clearLength();
    }

    QENTEM_CONST_EXPRESSION StringView(const StringView &src) noexcept : storage_{src.First()}, length_{src.Length()} {
    }

    QENTEM_CONST_EXPRESSION StringView(const Char_T *str, SizeT length) noexcept : storage_{str}, length_{length} {
    }

    QENTEM_CONST_EXPRESSION StringView(const Char_T *str) noexcept : storage_{str}, length_{StringUtils::Count(str)} {
    }

    QENTEM_CONST_EXPRESSION StringView &operator=(StringView &&src) noexcept {
        if (this != &src) {
            setStorage(src.First());
            setLength(src.Length());

            src.clearStorage();
            src.clearLength();
        }

        return *this;
    }

    QENTEM_CONST_EXPRESSION StringView &operator=(const StringView &src) {
        if (this != &src) {
            setStorage(src.First());
            setLength(src.Length());
        }

        return *this;
    }

    QENTEM_CONST_EXPRESSION StringView &operator=(const Char_T *str) {
        setStorage(str);
        setLength(StringUtils::Count(str));

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

    inline QENTEM_CONST_EXPRESSION SizeT Length() const noexcept {
        return length_;
    }

    QENTEM_CONST_EXPRESSION void RemovePrefix(SizeT count) noexcept {
        if (count > Length()) {
            count = Length();
        }

        setStorage(First() + count);
        setLength(Length() - count);
    }

    QENTEM_CONST_EXPRESSION void RemoveSuffix(SizeT count) noexcept {
        if (count > Length()) {
            count = Length();
        }

        setLength(Length() - count);
    }

    inline const Char_T *First() const noexcept {
        return storage_;
    }

    inline const Char_T *Last() const noexcept {
        if (Length() != 0) {
            return (First() + (Length() - SizeT{1}));
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

    //////////// Private ////////////

  private:
    QENTEM_CONST_EXPRESSION void setLength(SizeT length) noexcept {
        length_ = length;
    }

    QENTEM_CONST_EXPRESSION void setStorage(const Char_T *ptr) noexcept {
        storage_ = ptr;
    }

    void clearStorage() noexcept {
        storage_ = nullptr;
    }

    void clearLength() noexcept {
        length_ = 0;
    }

    const Char_T *storage_{nullptr};
    SizeT         length_{0};
};

} // namespace Qentem

#endif
