/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_STRINGSTREAM_H
#define QENTEM_STRINGSTREAM_H

#include "String.hpp"
#include "StringView.hpp"

namespace Qentem {
/*
 * String Stream struct without the null terminator.
 */
template <typename Char_T>
struct StringStream {
    using CharType = Char_T;

    StringStream() = default;

    ~StringStream() {
        Memory::Deallocate(Storage());
    }

    inline explicit StringStream(SizeT size) {
        if (size != 0) {
            allocate(size);
        }
    }

    StringStream(StringStream &&stream) noexcept
        : storage_{stream.Storage()}, length_{stream.Length()}, capacity_{stream.Capacity()} {
        stream.clearStorage();
        stream.setLength(0);
        stream.setCapacity(0);
    }

    StringStream(const StringStream &stream) {
        if (stream.Length() != 0) {
            allocate(stream.Length());
            write(stream.First(), stream.Length());
        }
    }

    StringStream &operator=(StringStream &&stream) noexcept {
        if (this != &stream) {
            Memory::Deallocate(Storage());

            setCapacity(stream.Capacity());
            setStorage(stream.Storage());
            setLength(stream.Length());

            stream.clearStorage();
            stream.setCapacity(0);
            stream.setLength(0);
        }

        return *this;
    }

    StringStream &operator=(const StringStream &stream) {
        if (this != &stream) {
            Clear();
            write(stream.First(), stream.Length());
        }

        return *this;
    }

    StringStream &operator=(const Char_T *str) {
        Clear();
        write(str, StringUtils::Count(str));

        return *this;
    }

    StringStream &operator=(const String<Char_T> &string) {
        Clear();
        write(string.First(), string.Length());
        return *this;
    }

    StringStream &operator=(const StringView<Char_T> &string) {
        Clear();
        write(string.First(), string.Length());

        return *this;
    }

    inline void operator+=(const Char_T one_char) {
        const SizeT new_length = (Length() + SizeT{1});

        if (Capacity() == Length()) {
            expand(new_length);
        }

        Storage()[Length()] = one_char;
        setLength(new_length);
    }

    inline void operator+=(const StringStream<Char_T> &stream) {
        write(stream.First(), stream.Length());
    }

    inline void operator+=(const String<Char_T> &string) {
        write(string.First(), string.Length());
    }

    inline void operator+=(const StringView<char> &string_view) {
        write(string_view.First(), string_view.Length());
    }

    inline void operator+=(const Char_T *str) {
        write(str, StringUtils::Count(str));
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

    inline friend StringStream &operator<<(StringStream &out, const StringStream &stream) {
        out.write(stream.First(), stream.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const String<Char_T> &string) {
        out.write(string.First(), string.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const StringView<Char_T> &string) {
        out.write(string.First(), string.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, Char_T ch) {
        out += ch;
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const Char_T *str) {
        out.write(str, StringUtils::Count(str));
        return out;
    }

    inline bool operator==(const StringStream &stream) const noexcept {
        return ((Length() == stream.Length()) && StringUtils::IsEqual(First(), stream.First(), Length()));
    }

    inline bool operator==(const String<Char_T> &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    inline bool operator==(const StringView<Char_T> &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    inline bool operator==(const Char_T *str) const noexcept {
        const SizeT length = StringUtils::Count(str);

        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    inline bool IsEqual(const Char_T *str, const SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    inline bool operator!=(const StringStream &stream) const noexcept {
        return (!(*this == stream));
    }

    inline bool operator!=(const String<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const StringView<Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline void Write(const Char_T *str, const SizeT length) {
        write(str, length);
    }

    inline void Clear() noexcept {
        setLength(0);
    }

    void Reset() noexcept {
        Memory::Deallocate(Storage());

        clearStorage();
        setLength(0);
        setCapacity(0);
    }

    inline void StepBack(const SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
        }
    }

    inline void Reverse(SizeT index = 0) noexcept {
        SizeT end = Length();

        while (index < end) {
            const Char_T tmp = Storage()[index];

            --end;
            Storage()[index] = Storage()[end];
            Storage()[end]   = tmp;
            ++index;
        }
    }

    inline void InsertAt(Char_T ch, SizeT index) {
        if (index < Length()) {
            Char_T       *first  = (Storage() + index);
            Char_T       *second = first;
            const Char_T *end    = End();

            Char_T tmp = *first;
            *first     = ch;

            while (++second < end) {
                ch      = *second;
                *second = tmp;
                tmp     = ch;
            }

            *this += tmp;
        }
    }

    inline void SetLength(SizeT length) {
        if (Capacity() < length) {
            expand(length);
        }

        setLength(length);
    }

    // Set the needed length to write directly to a returned buffer,
    inline Char_T *Buffer(SizeT length) {
        const SizeT new_length = (Length() + length);

        if (Capacity() < new_length) {
            expand(new_length);
        }

        Char_T *str = (Storage() + Length());

        setLength(new_length);

        return str;
    }

    inline void Expect(SizeT length) {
        length += Length();

        if (Capacity() < length) {
            expand(length);
        }
    }

    void Reserve(const SizeT size) {
        Reset();

        if (size != 0) {
            allocate(size);
        }
    }

    Char_T *Detach() noexcept {
        Char_T *str = Storage();
        clearStorage();

        setLength(0);
        setCapacity(0);

        return str;
    }

    String<Char_T> GetString() {
        if (Capacity() > Length()) {
            Storage()[Length()] = Char_T{0};
            return String<Char_T>(Detach(), Length());
        }

        String<Char_T> str{First(), Length()};

        Reset();

        return str;
    }

    StringView<Char_T> GetStringView() {
        InsertNull();
        return StringView<Char_T>{First(), Length()};
    }

    void InsertNull() {
        if (Capacity() == Length()) {
            expand(Length() + SizeT{1});
        }

        Storage()[Length()] = Char_T{0};
    }

    inline Char_T *Storage() const noexcept {
        return storage_;
    }

    inline SizeT Length() const noexcept {
        return length_;
    }

    inline SizeT Capacity() const noexcept {
        return capacity_;
    }

    inline const Char_T *First() const noexcept {
        return Storage();
    }

    inline Char_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Length() - SizeT{1}));
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
    void setStorage(Char_T *new_storage) noexcept {
        storage_ = new_storage;
    }

    void clearStorage() noexcept {
        setStorage(nullptr);
    }

    void setLength(const SizeT new_length) noexcept {
        length_ = new_length;
    }

    void setCapacity(const SizeT new_capacity) noexcept {
        capacity_ = new_capacity;
    }

    inline void write(const Char_T *str, const SizeT length) {
        constexpr SizeT size       = sizeof(Char_T);
        const SizeT     new_length = (Length() + length);

        if (Capacity() < new_length) {
            expand(new_length);
        }

        Memory::Copy((Storage() + Length()), str, (length * size));

        setLength(new_length);
    }

    void expand(const SizeT new_capacity) {
        constexpr SizeT size = sizeof(Char_T);
        Char_T         *str  = Storage();

        allocate(new_capacity * SizeT{4});

        Memory::Copy(Storage(), str, (Length() * size));
        Memory::Deallocate(str);
    }

    void allocate(SizeT size) {
        size = Memory::AlignSize(size);

        setStorage(Memory::Allocate<Char_T>(size));

        setCapacity(size);
    }

    Char_T *storage_{nullptr};
    SizeT   length_{0};
    SizeT   capacity_{0};
};

} // namespace Qentem

#endif
