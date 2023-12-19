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

#include "String.hpp"

#ifndef QENTEM_STRINGSTREAM_H
#define QENTEM_STRINGSTREAM_H

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

    explicit StringStream(SizeT size) {
        if (size != SizeT{0}) {
            allocate(size);
        }
    }

    StringStream(StringStream &&stream) noexcept
        : _storage{stream.Storage()}, _length{stream.Length()}, _capacity{stream.Capacity()} {
        stream.setCapacity(SizeT{0});
        stream.setLength(SizeT{0});
        stream.clearStorage();
    }

    StringStream(const StringStream &stream) {
        if (stream.Length() != SizeT{0}) {
            allocate(stream.Length());
            write(stream.First(), stream.Length());
        }
    }

    StringStream &operator=(StringStream &&stream) noexcept {
        if (this != &stream) {
            Memory::Deallocate(Storage());
            setStorage(stream.Storage());
            setLength(stream.Length());
            setCapacity(stream.Capacity());

            stream.setCapacity(SizeT{0});
            stream.setLength(SizeT{0});
            stream.clearStorage();
        }

        return *this;
    }

    StringStream &operator=(const StringStream &stream) {
        if (this != &stream) {
            setCapacity(SizeT{0});
            setLength(SizeT{0});
            Memory::Deallocate(Storage());
            clearStorage();
            write(stream.First(), stream.Length());
        }

        return *this;
    }

    StringStream &operator=(const Char_T *str) {
        setCapacity(SizeT{0});
        setLength(SizeT{0});
        Memory::Deallocate(Storage());
        clearStorage();
        write(str, StringUtils::Count(str));

        return *this;
    }

    StringStream &operator=(const String<Char_T> &string) {
        setCapacity(SizeT{0});
        setLength(SizeT{0});
        Memory::Deallocate(Storage());
        clearStorage();

        const SizeT len = string.Length();
        write(string.Storage(len), len);

        return *this;
    }

    void operator+=(Char_T one_char) {
        const SizeT len = Length();

        ++_length;

        if (Capacity() == len) {
            expand(_length);
        }

        Storage()[len] = one_char;
    }

    inline void operator+=(const StringStream<Char_T> &stream) {
        write(stream.First(), stream.Length());
    }

    inline void operator+=(const String<Char_T> &string) {
        const SizeT len = string.Length();
        write(string.Storage(len), len);
    }

    inline void operator+=(const Char_T *str) {
        write(str, StringUtils::Count(str));
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const StringStream &stream) {
        const SizeT len = stream.Length();

        if (stream.Capacity() > len) {
            stream.Storage()[len] = Char_T{0};
            out << stream.First();
        } else {
            StringStream n_src = stream;
            n_src += Char_T{0};
            out << n_src.First();
        }

        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const StringStream &stream) {
        out.write(stream.First(), stream.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const String<Char_T> &string) {
        const SizeT len = string.Length();

        out.write(string.Storage(len), len);
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
        const SizeT len = string.Length();

        return ((Length() == len) && StringUtils::IsEqual(First(), string.Storage(len), Length()));
    }

    inline bool operator==(const Char_T *str) const noexcept {
        const SizeT len = StringUtils::Count(str);

        return ((Length() == len) && StringUtils::IsEqual(First(), str, len));
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

    inline bool operator!=(const Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline void Write(const Char_T *str, const SizeT length) {
        write(str, length);
    }

    inline void Clear() noexcept {
        setLength(SizeT{0});
    }

    void Reset() noexcept {
        setCapacity(SizeT{0});
        setLength(SizeT{0});
        Memory::Deallocate(Storage());
        clearStorage();
    }

    inline void StepBack(const SizeT len) noexcept {
        if (len <= Length()) {
            _length -= len;
        }
    }

    inline void Reverse(SizeT index = SizeT{0}) noexcept {
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

            write(&tmp, SizeT{1});
        }
    }

    // Set the needed length to write directly to the buffer,
    inline Char_T *Buffer(const SizeT len) {
        const SizeT current_offset = Length();
        _length += len;

        if (_length > Capacity()) {
            expand(_length);
        }

        return (Storage() + current_offset);
    }

    inline void Expect(SizeT len) {
        len += Length();

        if (len > Capacity()) {
            expand(len);
        }
    }

    Char_T *Detach() noexcept {
        setCapacity(SizeT{0});
        setLength(SizeT{0});
        Char_T *str = Storage();
        clearStorage();

        return str;
    }

    String<Char_T> GetString() {
        const SizeT len = Length();

        if (Capacity() > len) {
            Storage()[len] = Char_T{0};
            return String<Char_T>(Detach(), len);
        }

        String<Char_T> str{First(), len};
        Reset();

        return str;
    }

    inline Char_T *Storage() const noexcept {
        return _storage;
    }

    inline SizeT Length() const noexcept {
        return _length;
    }

    inline SizeT Capacity() const noexcept {
        return _capacity;
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
        return (Length() == SizeT{0});
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
        _storage = new_storage;
    }

    void clearStorage() noexcept {
        setStorage(nullptr);
    }

    void setLength(const SizeT new_length) noexcept {
        _length = new_length;
    }

    void setCapacity(const SizeT new_capacity) noexcept {
        _capacity = new_capacity;
    }

    void allocate(SizeT size) {
        size = Memory::AlignSize(size * SizeT{4});
        setCapacity(size);
        setStorage(Memory::Allocate<Char_T>(size));
    }

    inline void write(const Char_T *str, const SizeT len) {
        constexpr SizeT32 size           = sizeof(Char_T);
        const SizeT       current_offset = Length();
        _length += len;

        if (Capacity() < _length) {
            expand(_length);
        }

        Memory::Copy((Storage() + current_offset), str, (len * size));
    }

    void expand(const SizeT new_capacity) {
        constexpr SizeT32 size    = sizeof(Char_T);
        const SizeT       src_cap = Capacity();
        Char_T           *str     = Storage();
        allocate(new_capacity);

        Memory::Copy(Storage(), str, (src_cap * size));
        Memory::Deallocate(str);
    }

    Char_T *_storage{nullptr};
    SizeT   _length{0};
    SizeT   _capacity{0};
};

} // namespace Qentem

#endif
