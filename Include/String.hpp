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

#include "StringView.hpp"
#include "Reserver.hpp"

namespace Qentem {
/*
 * String container with null terminator.
 */

template <typename Char_T>
struct String {
    using CharType = Char_T;

    QENTEM_INLINE String() = default;

    QENTEM_INLINE ~String() {
        Reserver::Release(Storage(), (Capacity() + SizeT{1}));
    }

    explicit String(SizeT capacity) {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    String(String &&src) noexcept : storage_{src.Storage()}, capacity_{src.Capacity()}, length_{src.Length()} {
        src.clearStorage();
        src.setLength(0);
        src.setCapacity(0);
    }

    String(const String &src) {
        if (src.Length() != 0) {
            write(src.First(), src.Length());
        }
    }

    String(const Char_T *str, SizeT length) {
        write(str, length);
    }

    String(const Char_T *str) {
        write(str, StringUtils::Count(str));
    }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            Reserver::Release(Storage(), (Capacity() + SizeT{1}));

            setCapacity(src.Capacity());
            setStorage(src.Storage());
            setLength(src.Length());

            src.clearStorage();
            src.setCapacity(0);
            src.setLength(0);
        }

        return *this;
    }

    String &operator=(const Char_T *str) {
        Clear();
        write(str, StringUtils::Count(str));

        return *this;
    }

    String &operator=(const String &src) {
        Clear();
        write(src.First(), src.Length());
        return *this;
    }

    String &operator=(const StringView<Char_T> &src) {
        Clear();
        write(src.First(), src.Length());

        return *this;
    }

    QENTEM_INLINE void operator+=(Char_T ch) {
        Write(ch);
    }

    QENTEM_INLINE void operator+=(const String &r) {
        write(r.First(), r.Length());
    }

    String &operator+=(String &&src) {
        write(src.First(), src.Length());
        src.Reset();

        return *this;
    }

    QENTEM_INLINE void operator+=(const StringView<char> &string_view) {
        write(string_view.First(), string_view.Length());
    }

    QENTEM_INLINE void operator+=(const Char_T *str) {
        write(str, StringUtils::Count(str));
    }

    String operator+(String &&src) const {
        String ns{Merge(*this, src)};
        src.Reset();

        return ns;
    }

    QENTEM_INLINE String operator+(const String &src) const {
        return Merge(*this, src);
    }

    QENTEM_INLINE String operator+(const Char_T *str) const {
        return merge(First(), Length(), str, StringUtils::Count(str));
    }

    template <typename Stream_T>
    QENTEM_INLINE friend Stream_T &operator<<(Stream_T &out, const String &string) {
        out << string.First();
        return out;
    }

    QENTEM_INLINE friend String &operator<<(String &out, const String &string) {
        out.write(string.First(), string.Length());
        return out;
    }

    QENTEM_INLINE friend String &operator<<(String &out, const StringView<Char_T> &string) {
        out.write(string.First(), string.Length());
        return out;
    }

    QENTEM_INLINE friend String &operator<<(String &out, Char_T ch) {
        out.Write(ch);
        return out;
    }

    QENTEM_INLINE friend String &operator<<(String &out, const Char_T *str) {
        out.write(str, StringUtils::Count(str));
        return out;
    }

    QENTEM_INLINE bool operator==(const String &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    QENTEM_INLINE bool operator==(const StringView<Char_T> &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    QENTEM_INLINE bool operator==(const Char_T *str) const noexcept {
        const SizeT length = StringUtils::Count(str);

        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    QENTEM_INLINE bool IsEqual(const Char_T *str, const SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    QENTEM_INLINE bool operator!=(const String &string) const noexcept {
        return (!(*this == string));
    }

    QENTEM_INLINE bool operator!=(const StringView<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    QENTEM_INLINE bool operator!=(const Char_T *str) const noexcept {
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

    void Write(Char_T ch) {
        const SizeT new_length = (Length() + SizeT{1});

        if (Capacity() == Length()) {
            expand(new_length);
        }

        Storage()[Length()] = ch;
        setLength(new_length);
    }

    QENTEM_INLINE void Write(const Char_T *str, const SizeT length) {
        write(str, length);
    }

    QENTEM_INLINE void WriteAt(const SizeT index, const Char_T *str, const SizeT length) {
        if ((index + length) <= Length()) {
            MemoryUtils::CopyTo((Storage() + index), str, length);
        }
    }

    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    QENTEM_INLINE void Expect(SizeT length) {
        length += Length();

        if (Capacity() < length) {
            expand(length);
        }
    }

    QENTEM_INLINE void Reserve(const SizeT size) {
        Reset();

        if (size != 0) {
            reserve(size);
        }
    }

    QENTEM_INLINE void SetLength(SizeT length) {
        if (Capacity() < length) {
            expand(length);
        }

        setLength(length);
    }

    QENTEM_INLINE void Clear() noexcept {
        setLength(0);
    }

    void Reset() noexcept {
        Reserver::Release(Storage(), (Capacity() + SizeT{1}));

        clearStorage();
        setLength(0);
        setCapacity(0);
    }

    QENTEM_INLINE StringView<Char_T> GetStringView() {
        return StringView<Char_T>{First(), Length()};
    }

    // Without NULL terminator
    QENTEM_INLINE StringView<Char_T> View() const noexcept {
        return StringView<Char_T>{First(), Length()};
    }

    Char_T *Detach() noexcept {
        Char_T *str = Storage();
        clearStorage();

        setLength(0);
        setCapacity(0);

        return str;
    }

    void Adopt(Char_T *str, SizeT length, SizeT capacity_with_out_null) {
        // Clean up any existing storage
        Reset();

        // Take ownership of the new buffer
        setStorage(str);
        setLength(length);
        setCapacity(capacity_with_out_null);
    }

    QENTEM_INLINE void StepBack(const SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
        }
    }

    QENTEM_INLINE void Reverse(SizeT start = 0) noexcept {
        StringUtils::Reverse(Storage(), start, Length());
    }

    void InsertAt(Char_T ch, SizeT index) {
        if (index < Length()) {
            const SizeT new_length = (Length() + SizeT{1});

            if (new_length <= Capacity()) {
                // Enough capacity, shift tail right by one, insert in-place.
                Char_T *data = Storage();

                // Shift right: move everything [index, length) -> [index+1, new_length)
                SizeT i = Length();

                while (i > index) {
                    data[i] = data[i - SizeT{1}];
                    --i;
                }

                data[index] = ch;
                setLength(new_length);
            } else {
                // Not enough capacity: reserve a bigger buffer and copy in 3 steps.
                SizeT   new_capacity = MemoryUtils::AlignToPow2(new_length);
                Char_T *new_storage  = Reserver::Reserve<Char_T>(new_length);

                // 1. Copy prefix [0, index)
                if (index != 0) {
                    MemoryUtils::CopyTo(new_storage, Storage(), index);
                }

                // 2. Insert new char at 'index'
                new_storage[index] = ch;

                // 3. Copy suffix [index, length)
                if (index < Length()) {
                    MemoryUtils::CopyTo(new_storage + index + 1, (Storage() + index), (Length() - index));
                }

                // Clean up old storage and set new storage/capacity
                Reserver::Release(Storage(), (Capacity() + SizeT{1}));
                setStorage(new_storage);
                setCapacity(new_capacity);
                setLength(new_length);
            }
        }
    }

    void ShiftRight(SizeT shift) {
        if ((shift != 0) && IsNotEmpty()) {
            const SizeT old_length = Length();
            const SizeT new_length = (old_length + shift);

            if (new_length <= Capacity()) {
                // Enough capacity, shift in place (backwards to avoid overlap).
                Char_T *data = Storage();
                SizeT   i    = old_length;

                while (i != 0) {
                    data[i + (shift - SizeT{1})] = data[i - SizeT{1}];

                    --i;
                }

                setLength(new_length);
            } else {
                // Not enough capacity: reserve, then copy with the shift.
                SizeT   new_capacity = MemoryUtils::AlignToPow2(new_length);
                Char_T *new_storage  = Reserver::Reserve<Char_T>(new_capacity);

                // 2. Copy old data to the right position in new storage
                if (old_length != 0) {
                    MemoryUtils::CopyTo(new_storage + shift, Storage(), old_length);
                }

                // Clean up, set new pointers
                Reserver::Release(Storage(), (Capacity() + SizeT{1}));
                setStorage(new_storage);
                setCapacity(new_capacity);
                setLength(new_length);
            }
        }
    }

    // Set the needed length to write directly to a returned buffer,
    Char_T *Buffer(SizeT length) {
        const SizeT new_length = (Length() + length);

        if (Capacity() < new_length) {
            expand(new_length);
        }

        Char_T *str = (Storage() + Length());

        setLength(new_length);

        return str;
    }

    static String Trim(const String &src) {
        SizeT         length = src.Length();
        SizeT         offset = 0;
        const Char_T *str    = src.First();

        StringUtils::Trim(str, offset, length);
        return String((str + offset), length);
    }

    QENTEM_INLINE Char_T *Storage() noexcept {
        return storage_;
    }

    QENTEM_INLINE const Char_T *Storage() const noexcept {
        return storage_;
    }

    QENTEM_INLINE SizeT Length() const noexcept {
        return length_;
    }

    QENTEM_INLINE SizeT Capacity() const noexcept {
        return capacity_;
    }

    QENTEM_INLINE const Char_T *First() const noexcept {
        return storage_;
    }

    QENTEM_INLINE Char_T *Last() noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Char_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    QENTEM_INLINE const Char_T *End() const noexcept {
        return (First() + Length());
    }

    QENTEM_INLINE bool IsEmpty() const noexcept {
        return (Length() == 0);
    }

    QENTEM_INLINE bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    QENTEM_INLINE const Char_T *begin() const noexcept {
        return First();
    }

    QENTEM_INLINE const Char_T *end() const noexcept {
        return End();
    }

    QENTEM_INLINE Char_T *begin() noexcept {
        return Storage();
    }

    QENTEM_INLINE Char_T *end() noexcept {
        return (Storage() + Length());
    }

    //////////// Private ////////////
  private:
    QENTEM_INLINE void setStorage(Char_T *new_storage) noexcept {
        storage_ = new_storage;
    }

    QENTEM_INLINE void clearStorage() noexcept {
        setStorage(nullptr);
    }

    QENTEM_INLINE void setLength(const SizeT new_length) noexcept {
        length_ = new_length;
    }

    QENTEM_INLINE void setCapacity(const SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    void write(const Char_T *str, const SizeT length) {
        if (length != 0) {
            const SizeT new_length = (Length() + length);

            if (Capacity() < new_length) {
                expand(new_length);
            }

            MemoryUtils::CopyTo((Storage() + Length()), str, length);

            Storage()[new_length] = 0;
            setLength(new_length);
        }
    }

    static String merge(const Char_T *str1, const SizeT len1, const Char_T *str2, const SizeT len2) {
        const SizeT length = SizeT(len1 + len2);
        String      ns{length};
        ns.setLength(length);

        Char_T *des = ns.Storage();

        if (len1 != 0) {
            MemoryUtils::CopyTo(des, str1, len1);
        }

        if (len2 != 0) {
            MemoryUtils::CopyTo((des + len1), str2, len2);
        }

        des[length] = 0;

        return ns;
    }

    void expand(const SizeT new_capacity) {
        Char_T *old_storage  = Storage();
        SizeT   old_capacity = Capacity();

        reserve(new_capacity);

        MemoryUtils::CopyTo(Storage(), old_storage, Length());
        Reserver::Release(old_storage, (old_capacity + SizeT{1}));
    }

    void reserve(SizeT capacity) {
        setStorage(Reserver::Reserve<Char_T>(capacity + SizeT{1})); // 1 for the null terminator
        setCapacity(capacity);
    }

    Char_T *storage_{nullptr};
    SizeT   capacity_{0};
    SizeT   length_{0};
};

} // namespace Qentem

#endif
