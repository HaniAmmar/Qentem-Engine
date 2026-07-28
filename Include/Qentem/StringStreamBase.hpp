/**
 * @file StringStreamBase.hpp
 * @brief Memory-provider-based character stream for Qentem Engine.
 *
 * StringStreamBase implements a dynamically expandable character stream for
 * sequential text construction and output generation. Characters are stored
 * in a contiguous buffer managed through a configurable memory provider,
 * allowing allocation, release, growth, and shrinking policies to be
 * customized independently of the stream logic.
 *
 * The stream supports efficient single-character and block writes, explicit
 * null termination, direct buffer access, ownership transfer, and automatic
 * capacity growth. Storage management is delegated entirely to the selected
 * memory provider, enabling the same implementation to be reused with
 * allocator-backed, page-backed, or custom memory strategies.
 *
 * This class serves as the foundation for StringStream, LiteStream, and
 * other stream types built on the same write-oriented interface.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_STRING_STREAM_BASE_H
#define QENTEM_STRING_STREAM_BASE_H

#include "Qentem/StringView.hpp"
#include "Qentem/MemoryUtils.hpp"

namespace Qentem {
/**
 * @brief Generic character stream with configurable storage backend.
 *
 * The stream does not maintain a trailing null terminator automatically.
 * Call InsertNull() when a null-terminated buffer is required.
 */
template <typename Char_T, typename MemoryProvider_T>
struct StringStreamBase {
    using CharType = Char_T;

    static constexpr SizeT ExpandShift{2};

    QENTEM_INLINE StringStreamBase() noexcept = default;

    explicit StringStreamBase(SizeT capacity) {
        if (capacity != 0) {
            reserve(capacity);
        }
    }

    StringStreamBase(StringStreamBase &&src) noexcept
        : storage_{src.Storage()}, capacity_{src.Capacity()}, length_{src.Length()} {
        src.clearStorage();
        src.setCapacity(0);
        src.setLength(0);
    }

    StringStreamBase(const StringStreamBase &src) {
        if (src.Length() != 0) {
            reserve(src.Length());
            Write(src.First(), src.Length());
        }
    }

    StringStreamBase(const Char_T *str, SizeT length) {
        reserve(length);
        Write(str, length);
    }

    StringStreamBase(const Char_T *str) {
        const SizeT length = StringUtils::Count(str);
        reserve(length);
        Write(str, length);
    }

    QENTEM_INLINE ~StringStreamBase() {
        release(Storage(), Capacity());
    }

    StringStreamBase &operator=(StringStreamBase &&src) noexcept {
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

    StringStreamBase &operator=(const StringStreamBase &src) {
        if (this != &src) {
            Clear();
            Write(src.First(), src.Length());
        }

        return *this;
    }

    StringStreamBase &operator=(const StringView<Char_T> &string) {
        Clear();
        Write(string.First(), string.Length());

        return *this;
    }

    StringStreamBase &operator=(const Char_T *str) {
        Clear();
        Write(str, StringUtils::Count(str));

        return *this;
    }

    QENTEM_INLINE void operator+=(const StringStreamBase &stream) {
        Write(stream.First(), stream.Length());
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
    friend Stream_T &operator<<(Stream_T &out, const StringStreamBase &stream) {
        const Char_T *str = stream.First();
        SizeT         index{0};

        while (index < stream.Length()) {
            out << str[index];
            ++index;
        }

        return out;
    }

    QENTEM_INLINE StringStreamBase &operator<<(const StringStreamBase &stream) {
        Write(stream.First(), stream.Length());
        return *this;
    }

    QENTEM_INLINE StringStreamBase &operator<<(const StringView<Char_T> &string) {
        Write(string.First(), string.Length());
        return *this;
    }

    QENTEM_INLINE StringStreamBase &operator<<(Char_T ch) {
        Write(ch);
        return *this;
    }

    QENTEM_INLINE StringStreamBase &operator<<(const Char_T *str) {
        Write(str, StringUtils::Count(str));
        return *this;
    }

    QENTEM_INLINE bool operator==(const StringStreamBase &stream) const noexcept {
        return ((Length() == stream.Length()) && StringUtils::IsEqual(First(), stream.First(), Length()));
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

    QENTEM_INLINE bool operator!=(const StringStreamBase &stream) const noexcept {
        return (!(*this == stream));
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
            expand(static_cast<SizeT>(new_length << ExpandShift));
        }

        Storage()[Length()] = ch;
        setLength(new_length);
    }

    void Write(const Char_T *str, const SizeT length) {
        if (length != 0) {
            const SizeT new_length = (Length() + length);

            if (Capacity() < new_length) {
                expand(static_cast<SizeT>(new_length << ExpandShift));
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

    QENTEM_INLINE void Reserve(SizeT capacity) {
        if (capacity != 0) {
            setLength(0);

            if (capacity > Capacity()) {
                expand(capacity);
            } else if (capacity < Capacity() && shrink(Storage(), Capacity(), capacity)) {
                setCapacity(capacity);
            }
        } else {
            Reset();
        }
    }

    QENTEM_INLINE void Expect(SizeT length) {
        length += Length();

        if (Capacity() < length) {
            expand(length);
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
    QENTEM_INLINE StringView<Char_T> GetStringView() const noexcept {
        return StringView<Char_T>{First(), Length()};
    }

    // Returns view without null-terminator.
    QENTEM_INLINE StringView<Char_T> View() const noexcept {
        return GetStringView();
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
        return (Length() != 0);
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

  protected:
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

    QENTEM_NOINLINE void expand(SizeT new_capacity) {
        constexpr SizeT32 long_m1 = (sizeof(SystemLong) - 1U);

        if (tryInplaceExpand(new_capacity)) {
            return;
        }

        Char_T       *old_storage    = Storage();
        SizeT         old_capacity   = Capacity();
        const SizeT32 aligned_length = (((Length() * sizeof(Char_T)) + long_m1) & ~long_m1) / sizeof(SystemLong);

        reserve(new_capacity);

        SystemLong *dst = reinterpret_cast<SystemLong *>(Storage());
        SystemLong *src = reinterpret_cast<SystemLong *>(old_storage);

        SizeT32 offset = 0;
        while (offset < aligned_length) {
            dst[offset] = src[offset];
            ++offset;
        }

        // MemoryUtils::CopyTo(Storage(), old_storage, Length());

        release(old_storage, old_capacity);
    }

    bool tryInplaceExpand(SizeT new_capacity) {
        if (tryExpand(Storage(), Capacity(), new_capacity)) {
            setCapacity(new_capacity);
            return true;
        }

        return false;
    }

    void reserve(SizeT capacity) {
        setStorage(reserveOnly(capacity));
        setCapacity((capacity));
    }

    QENTEM_INLINE static Char_T *reserveOnly(SizeT &capacity) {
        return MemoryProvider_T::template Reserve<Char_T>(capacity);
    }

    QENTEM_INLINE static void release(Char_T *storage, SizeT capacity) {
        MemoryProvider_T::Release(storage, capacity);
    }

    QENTEM_INLINE static bool shrink(Char_T *storage, SizeT from_size, SizeT &to_size) noexcept {
        return MemoryProvider_T::template Shrink<Char_T>(storage, from_size, to_size);
    }

    QENTEM_INLINE static bool tryExpand(Char_T *storage, SizeT from_size, SizeT &to_size) noexcept {
        return MemoryProvider_T::TryExpand(storage, from_size, to_size);
    }

    Char_T *storage_{nullptr};
    SizeT   capacity_{0};
    SizeT   length_{0};
};

} // namespace Qentem

#endif
