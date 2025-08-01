/**
 * @file String.hpp
 * @brief High-performance dynamic string class for the Qentem Engine.
 *
 * This header defines `String`, a lightweight and efficient container for managing
 * null-terminated character sequences. It is designed for high-performance scenarios
 * such as template rendering, JSON processing, and general-purpose text manipulation.
 *
 * The implementation emphasizes speed, cross-platform consistency, and minimal
 * overhead. As a foundational component of the Qentem Engine, `String` integrates
 * tightly with the memory system and template architecture while offering a familiar
 * interface for dynamic text operations.
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
 * @brief A dynamic string container with null-termination.
 *
 * Provides an efficient, minimal-overhead abstraction for managing character sequences
 * with built-in memory allocation and resizing. Suitable for use across template engines,
 * parsers, and data serializers where performance and correctness are paramount.
 */
template <typename Char_T>
struct String {
    using CharType = Char_T;

    QENTEM_INLINE String() = default;

    QENTEM_INLINE ~String() {
        release(Storage(), Capacity());
    }

    explicit String(SizeT capacity) {
        if (capacity != 0) {
            setStorage(reserve(capacity));
            setCapacity(capacity);
        }
    }

    String(String &&src) noexcept : storage_{src.Storage()}, capacity_{src.Capacity()}, length_{src.Length()} {
        src.clearStorage();
        src.setCapacity(0);
        src.setLength(0);
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
            release(Storage(), Capacity());

            setStorage(src.Storage());
            setCapacity(src.Capacity());
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

    /**
     * @brief Concatenates two strings into a new one.
     *
     * Allocates a new string large enough to hold the contents of `src1` and `src2`,
     * and copies their characters sequentially into the new buffer.
     *
     * @param src1 First source string.
     * @param src2 Second source string.
     * @return A new string containing the merged result.
     */
    static String Merge(const String &src1, const String &src2) {
        return merge(src1.First(), src1.Length(), src2.First(), src2.Length());
    }

    QENTEM_INLINE void Expect(SizeT length) {
        length += Length();

        if (Capacity() < length) {
            expand(length);
        }
    }

    QENTEM_INLINE void Reserve(const SizeT capacity) {
        Reset();

        if (capacity != 0) {
            setStorage(reserve(capacity));
            setCapacity(capacity);
        }
    }

    QENTEM_INLINE void SetLength(SizeT length) {
        if (Capacity() < length) {
            expand(length);
        }

        Storage()[length] = '\0';
        setLength(length);
    }

    QENTEM_INLINE void Clear() noexcept {
        setLength(0);
    }

    void Reset() noexcept {
        release(Storage(), Capacity());

        clearStorage();
        setCapacity(0);
        setLength(0);
    }

    // Returns view without null-terminator.
    QENTEM_INLINE StringView<Char_T> GetStringView() {
        return StringView<Char_T>{First(), Length()};
    }

    // Returns view without null-terminator.
    QENTEM_INLINE StringView<Char_T> View() const noexcept {
        return StringView<Char_T>{First(), Length()};
    }

    Char_T *Detach() noexcept {
        Char_T *str = Storage();
        clearStorage();
        setCapacity(0);
        setLength(0);

        return str;
    }

    /**
     * @brief Takes ownership of an existing buffer with known length and capacity.
     *
     * The caller must ensure the provided buffer is properly allocated and aligned,
     * and that the `capacity_without_null` reflects the size *excluding* the null terminator.
     *
     * @param str                   Pointer to the externally allocated character buffer.
     * @param length                Number of characters used (excluding null terminator).
     * @param capacity_without_null Total capacity of the buffer (excluding null terminator).
     */
    void Adopt(Char_T *str, SizeT length, SizeT capacity_without_null) {
        // Release any previously owned memory
        release(Storage(), Capacity());

        setStorage(str);
        setCapacity(capacity_without_null);
        setLength(length);
        Storage()[length] = '\0';
    }

    QENTEM_INLINE void StepBack(const SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
            Storage()[Length()] = '\0';
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
                // Insufficient capacity: allocate a larger buffer and copy in three stages.
                SizeT   new_capacity = new_length;
                Char_T *new_storage  = reserve(new_capacity);

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

                new_storage[new_length] = '\0';

                // Clean up old storage and set new storage/capacity
                release(Storage(), Capacity());
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
                // Insufficient capacity: allocate a larger buffer and copy in three stages.
                SizeT   new_capacity = new_length;
                Char_T *new_storage  = reserve(new_capacity);

                // 2. Copy old data to the right position in new storage
                if (old_length != 0) {
                    MemoryUtils::CopyTo(new_storage + shift, Storage(), old_length);
                }

                new_storage[new_length] = '\0';

                // Clean up, set new pointers
                release(Storage(), Capacity());
                setStorage(new_storage);
                setCapacity(new_capacity);
                setLength(new_length);
            }
        }
    }

    /**
     * @brief Reserves space for a direct write of `length` characters.
     *
     * This function expands the string as needed, marks the extended length,
     * and returns a writable buffer at the end of the current string.
     * A null-terminator is automatically placed after the reserved space.
     *
     * @param length Number of characters to write.
     * @return Pointer to the start of the writable region.
     */
    Char_T *Buffer(SizeT length) {
        const SizeT new_length = (Length() + length);

        if (Capacity() < new_length) {
            expand(new_length);
        }

        Char_T *str = (Storage() + Length());

        Storage()[new_length] = '\0';
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

    // Iterator support (STL-compatible)
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

            Storage()[new_length] = '\0';
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

        des[length] = '\0';

        return ns;
    }

    void expand(SizeT new_capacity) {
        Char_T *old_storage  = Storage();
        SizeT   old_capacity = Capacity();

        setStorage(reserve(new_capacity));
        setCapacity(new_capacity);

        MemoryUtils::CopyTo(Storage(), old_storage, Length());

        release(old_storage, old_capacity);
    }

    /**
     * @brief Reserves an aligned memory region for `capacity + 1` characters (including null terminator).
     *
     * This helper computes the necessary byte size with alignment via `RoundUpBytes<Char_T>()`,
     * then performs the actual allocation using `Reserver`. The final capacity (excluding the null terminator)
     * is returned by reference via the `capacity` parameter.
     *
     * @param capacity  [in/out] Desired capacity before null-terminator. Updated with actual usable size.
     * @return Pointer to an aligned buffer of at least `capacity + 1` characters.
     */
    QENTEM_INLINE static Char_T *reserve(SizeT &capacity) {
        // Reserve enough space for (capacity + 1) characters, aligned and including null-terminator.
        const SizeT capacity_in_bytes = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(capacity + SizeT{1}));
        void       *storage_in_bytes  = Reserver::Reserve<char>(capacity_in_bytes);
        capacity                      = (capacity_in_bytes / sizeof(Char_T)) - SizeT{1};
        return static_cast<Char_T *>(storage_in_bytes);
    }

    static void release(Char_T *storage, SizeT capacity) {
        Reserver::Release(storage, (capacity + SizeT{1}));
    }

    Char_T *storage_{nullptr};
    SizeT   capacity_{0};
    SizeT   length_{0};
};

} // namespace Qentem

#endif
