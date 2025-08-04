/**
 * @file StringStream.hpp
 * @brief Provides a lightweight, efficient string stream for Qentem Engine.
 *
 * This header defines the StringStream utility, designed for fast, in-memory
 * construction and manipulation of character data. StringStream offers a minimal
 * alternative to traditional string streams, optimized for performance and
 * seamless integration with other Qentem Engine components.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_STRINGSTREAM_H
#define QENTEM_STRINGSTREAM_H

#include "String.hpp"

namespace Qentem {
/*
 * String Stream struct without null-termination.
 */
template <typename Char_T>
struct StringStream {
    using CharType = Char_T;

    static constexpr SizeT ExpandShift{2};

    QENTEM_INLINE StringStream() = default;

    QENTEM_INLINE ~StringStream() {
        release(Storage(), Capacity());
    }

    explicit StringStream(SizeT capacity) {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    StringStream(StringStream &&src) noexcept
        : storage_{src.Storage()}, capacity_{src.Capacity()}, length_{src.Length()} {
        src.clearStorage();
        src.setCapacity(0);
        src.setLength(0);
    }

    StringStream(const StringStream &src) {
        if (src.Length() != 0) {
            reserve(src.Length());
            Write(src.First(), src.Length());
        }
    }

    StringStream(const Char_T *str, SizeT length) {
        reserve(length);
        Write(str, length);
    }

    StringStream(const Char_T *str) {
        const SizeT length = StringUtils::Count(str);
        reserve(length);
        Write(str, length);
    }

    StringStream &operator=(StringStream &&src) noexcept {
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

    StringStream &operator=(const StringStream &src) {
        if (this != &src) {
            Clear();
            Write(src.First(), src.Length());
        }

        return *this;
    }

    StringStream &operator=(const String<Char_T> &string) {
        Clear();
        Write(string.First(), string.Length());
        return *this;
    }

    StringStream &operator=(const StringView<Char_T> &string) {
        Clear();
        Write(string.First(), string.Length());

        return *this;
    }

    StringStream &operator=(const Char_T *str) {
        Clear();
        Write(str, StringUtils::Count(str));

        return *this;
    }

    QENTEM_INLINE void operator+=(const StringStream<Char_T> &stream) {
        Write(stream.First(), stream.Length());
    }

    QENTEM_INLINE void operator+=(const String<Char_T> &string) {
        Write(string.First(), string.Length());
    }

    QENTEM_INLINE void operator+=(const StringView<char> &string_view) {
        Write(string_view.First(), string_view.Length());
    }

    QENTEM_INLINE void operator+=(Char_T ch) {
        Write(ch);
    }

    QENTEM_INLINE void operator+=(const Char_T *str) {
        Write(str, StringUtils::Count(str));
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const StringStream &stream) {
        const Char_T *str = stream.First();
        SizeT         index{0};

        while (index < stream.Length()) {
            out << str[index];
            ++index;
        }

        return out;
    }

    QENTEM_INLINE friend StringStream &operator<<(StringStream &out, const StringStream &stream) {
        out.Write(stream.First(), stream.Length());
        return out;
    }

    QENTEM_INLINE friend StringStream &operator<<(StringStream &out, const String<Char_T> &string) {
        out.Write(string.First(), string.Length());
        return out;
    }

    QENTEM_INLINE friend StringStream &operator<<(StringStream &out, const StringView<Char_T> &string) {
        out.Write(string.First(), string.Length());
        return out;
    }

    QENTEM_INLINE friend StringStream &operator<<(StringStream &out, Char_T ch) {
        out.Write(ch);
        return out;
    }

    QENTEM_INLINE friend StringStream &operator<<(StringStream &out, const Char_T *str) {
        out.Write(str, StringUtils::Count(str));
        return out;
    }

    QENTEM_INLINE bool operator==(const StringStream &stream) const noexcept {
        return ((Length() == stream.Length()) && StringUtils::IsEqual(First(), stream.First(), Length()));
    }

    QENTEM_INLINE bool operator==(const String<Char_T> &string) const noexcept {
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

    QENTEM_INLINE bool operator!=(const StringStream &stream) const noexcept {
        return (!(*this == stream));
    }

    QENTEM_INLINE bool operator!=(const String<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    QENTEM_INLINE bool operator!=(const StringView<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    QENTEM_INLINE bool operator!=(const Char_T *str) const noexcept {
        return (!(*this == str));
    }

    void Write(Char_T ch) {
        const SizeT new_length = (Length() + SizeT{1});

        if (Capacity() == Length()) {
            expand(new_length << ExpandShift);
        }

        Storage()[Length()] = ch;
        setLength(new_length);
    }

    void Write(const Char_T *str, const SizeT length) {
        if (length != 0) {
            const SizeT new_length = (Length() + length);

            if (Capacity() < new_length) {
                expand(new_length << ExpandShift);
            }

            MemoryUtils::CopyTo((Storage() + Length()), str, length);

            setLength(new_length);
        }
    }

    QENTEM_INLINE void WriteAt(const SizeT index, const Char_T *str, const SizeT length) {
        if ((index + length) <= Length()) {
            MemoryUtils::CopyTo((Storage() + index), str, length);
        }
    }

    QENTEM_INLINE void Expect(SizeT length) {
        length += Length();

        if (Capacity() < length) {
            expand(length);
        }
    }

    QENTEM_INLINE void Reserve(SizeT capacity) {
        if (capacity != 0) {
            setLength(0);

            if (capacity > Capacity()) {
                reserve(capacity);
            } else if (capacity < Capacity()) {
                capacity = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(capacity) / sizeof(Char_T));
                Reserver::Shrink(Storage(), Capacity(), capacity);
                setCapacity(capacity);
            }
        } else {
            Reset();
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
        release(Storage(), Capacity());

        clearStorage();
        setCapacity(0);
        setLength(0);
    }

    void InsertNull() {
        if (Capacity() == Length()) {
            expand(Capacity() + SizeT{1});
        }

        Storage()[Length()] = Char_T{0};
    }

    // Returns view without null-terminator.
    QENTEM_INLINE StringView<Char_T> GetStringView() {
        InsertNull();
        return StringView<Char_T>{First(), Length()};
    }

    // Returns view without null-terminator.
    QENTEM_INLINE StringView<Char_T> View() const noexcept {
        return GetStringView();
    }

    String<Char_T> GetString() {
        if (Capacity() > Length()) {
            const SizeT length  = Length(); // Detach() resets Length.
            Storage()[Length()] = Char_T{0};

            Reserver::Shrink(Storage(), Capacity(), (Length() + SizeT{1}));

            String<Char_T> a_string{};
            a_string.Adopt(Detach(), length, length);

            return a_string;
        }

        String<Char_T> str{First(), Length()};

        Reset();

        return str;
    }

    Char_T *Detach() noexcept {
        Char_T *str = Storage();

        clearStorage();
        setCapacity(0);
        setLength(0);

        return str;
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
                Char_T *storage = Storage();

                // Shift right: move everything [index, length) -> [index+1, new_length)
                SizeT offset = Length();

                while (offset > index) {
                    storage[offset] = storage[offset - SizeT{1}];
                    --offset;
                }

                storage[index] = ch;
                setLength(new_length);
            } else {
                Char_T     *old_storage  = Storage();
                const SizeT old_capacity = Capacity();

                // Not enough capacity: reserve a bigger buffer and copy in 3 steps.
                reserve(new_length);

                // 1. Copy prefix [0, index)
                if (index != 0) {
                    MemoryUtils::CopyTo(Storage(), old_storage, index);
                }

                // 2. Insert new char at 'index'
                Storage()[index] = ch;

                // 3. Copy suffix [index, length)
                if (index < Length()) {
                    MemoryUtils::CopyTo((Storage() + index + SizeT{1}), (old_storage + index), (Length() - index));
                }

                // Clean up old storage and set new storage/capacity
                release(old_storage, old_capacity);
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
                Char_T *storage = Storage();
                SizeT   offset  = old_length;

                while (offset != 0) {
                    storage[offset + (shift - SizeT{1})] = storage[offset - SizeT{1}];
                    --offset;
                }

                setLength(new_length);
            } else {
                Char_T     *old_storage  = Storage();
                const SizeT old_capacity = Capacity();

                // Not enough capacity: reserve, then copy with the shift.
                reserve(new_length);

                // 2. Copy old data to the right position in new storage
                if (old_length != 0) {
                    MemoryUtils::CopyTo(Storage() + shift, old_storage, old_length);
                }

                // Clean up, set new pointers
                release(old_storage, old_capacity);
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

    void expand(const SizeT new_capacity) {
        Char_T *old_storage  = Storage();
        SizeT   old_capacity = Capacity();

        reserve(new_capacity);

        MemoryUtils::CopyTo(Storage(), old_storage, Length());
        release(old_storage, old_capacity);
    }

    void reserve(SizeT capacity) {
        capacity = static_cast<SizeT>(Reserver::RoundUpBytes<Char_T>(capacity) / sizeof(Char_T));
        setStorage(Reserver::Reserve<Char_T>(capacity));
        setCapacity((capacity));
    }

    QENTEM_INLINE static void release(Char_T *storage, SizeT capacity) {
        Reserver::Release(storage, capacity);
    }

    Char_T *storage_{nullptr};
    SizeT   capacity_{0};
    SizeT   length_{0};
};

} // namespace Qentem

#endif
