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

#ifndef _QENTEM_STRINGSTREAM_H
#define _QENTEM_STRINGSTREAM_H

namespace Qentem {

/*
 * String Stream class without the null terminator.
 */
template <typename _Char_T>
class StringStream {
  public:
    using CharType = _Char_T;

    StringStream() = default;

    ~StringStream() {
        Memory::Deallocate(Storage());
    }

    explicit StringStream(SizeT size) {
        if (size != SizeT{0}) {
            allocate(size);
        }
    }

    StringStream(StringStream &&src) noexcept
        : _storage{src.Storage()}, _length{src.Length()}, _capacity{src.Capacity()} {
        src.setLength(SizeT{0});
        src.setCapacity(SizeT{0});
        src.clearStorage();
    }

    StringStream(const StringStream &src) {
        if (src.Length() != SizeT{0}) {
            allocate(src.Length());
            write(src.First(), src.Length());
        }
    }

    StringStream &operator=(StringStream &&src) noexcept {
        if (this != &src) {
            Memory::Deallocate(Storage());
            setStorage(src.Storage());
            setLength(src.Length());
            setCapacity(src.Capacity());

            src.setLength(SizeT{0});
            src.setCapacity(SizeT{0});
            src.clearStorage();
        }

        return *this;
    }

    StringStream &operator=(const StringStream &src) {
        if (this != &src) {
            setLength(SizeT{0});
            setCapacity(SizeT{0});
            Memory::Deallocate(Storage());
            clearStorage();
            write(src.First(), src.Length());
        }

        return *this;
    }

    StringStream &operator=(const _Char_T *str) {
        setLength(SizeT{0});
        setCapacity(SizeT{0});
        Memory::Deallocate(Storage());
        clearStorage();
        write(str, StringUtils::Count(str));

        return *this;
    }

    StringStream &operator=(const String<_Char_T> &src) {
        setLength(SizeT{0});
        setCapacity(SizeT{0});
        Memory::Deallocate(Storage());
        clearStorage();
        write(src.First(), src.Length());

        return *this;
    }

    void operator+=(_Char_T one_char) {
        const SizeT len = Length();
        ++_length;

        if (Capacity() == len) {
            expand(_length);
        }

        Storage()[len] = one_char;
    }

    inline void operator+=(const StringStream<_Char_T> &src) {
        write(src.First(), src.Length());
    }

    inline void operator+=(const String<_Char_T> &src) {
        write(src.First(), src.Length());
    }

    inline void operator+=(const _Char_T *str) {
        write(str, StringUtils::Count(str));
    }

    template <typename _Stream_T>
    friend _Stream_T &operator<<(_Stream_T &out, const StringStream &src) {
        const SizeT len = src.Length();

        if (src.Capacity() > len) {
            src.Storage()[len] = _Char_T{0};
            out << src.First();
        } else {
            StringStream n_src = src;
            n_src += _Char_T{0};
            out << n_src.First();
        }

        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const StringStream &src) {
        out.write(src.First(), src.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const String<_Char_T> &src) {
        out.write(src.First(), src.Length());
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, _Char_T ch) {
        out += ch;
        return out;
    }

    inline friend StringStream &operator<<(StringStream &out, const _Char_T *str) {
        out.write(str, StringUtils::Count(str));
        return out;
    }

    inline bool operator==(const StringStream &stream) const noexcept {
        return ((Length() == stream.Length()) && StringUtils::IsEqual(First(), stream.First(), Length()));
    }

    inline bool operator==(const String<_Char_T> &string) const noexcept {
        return ((Length() == string.Length()) && StringUtils::IsEqual(First(), string.First(), Length()));
    }

    inline bool operator==(const _Char_T *str) const noexcept {
        const SizeT len = StringUtils::Count(str);
        return ((Length() == len) && StringUtils::IsEqual(First(), str, len));
    }

    inline bool IsEqual(const _Char_T *str, const SizeT length) const noexcept {
        return ((Length() == length) && StringUtils::IsEqual(First(), str, length));
    }

    inline bool operator!=(const StringStream &stream) const noexcept {
        return (!(*this == stream));
    }

    inline bool operator!=(const String<_Char_T> &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const _Char_T *str) const noexcept {
        return (!(*this == str));
    }

    inline void Write(const _Char_T *str, const SizeT length) {
        write(str, length);
    }

    inline void Clear() noexcept {
        setLength(SizeT{0});
    }

    void Reset() noexcept {
        setLength(SizeT{0});
        setCapacity(SizeT{0});
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
            const _Char_T tmp = Storage()[index];

            --end;
            Storage()[index] = Storage()[end];
            Storage()[end]   = tmp;
            ++index;
        }
    }

    inline void InsertAt(_Char_T ch, SizeT index) {
        if (index < Length()) {
            _Char_T       *first  = (Storage() + index);
            _Char_T       *second = first;
            const _Char_T *end    = End();

            _Char_T tmp = *first;
            *first      = ch;

            while (++second < end) {
                ch      = *second;
                *second = tmp;
                tmp     = ch;
            }

            write(&tmp, SizeT{1});
        }
    }

    // Set the needed length to write directly to the buffer,
    inline _Char_T *Buffer(const SizeT len) {
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

    _Char_T *Detach() noexcept {
        setLength(SizeT{0});
        setCapacity(SizeT{0});
        _Char_T *str = Storage();
        clearStorage();

        return str;
    }

    String<_Char_T> GetString() {
        const SizeT len = Length();

        if (Capacity() > len) {
            Storage()[len] = _Char_T{0};
            return String<_Char_T>(Detach(), len);
        }

        String<_Char_T> str{First(), len};
        Reset();

        return str;
    }

    inline _Char_T *Storage() const noexcept {
        return _storage;
    }

    inline SizeT Length() const noexcept {
        return _length;
    }

    inline SizeT Capacity() const noexcept {
        return _capacity;
    }

    inline const _Char_T *First() const noexcept {
        return Storage();
    }

    inline _Char_T *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Length() - SizeT{1}));
        }

        return nullptr;
    }

    inline const _Char_T *End() const noexcept {
        return (First() + Length());
    }

    inline bool IsEmpty() const noexcept {
        return (Length() == SizeT{0});
    }

    inline bool IsNotEmpty() const noexcept {
        return !(IsEmpty());
    }

    // For STL
    inline const _Char_T *begin() const noexcept {
        return First();
    }

    inline const _Char_T *end() const noexcept {
        return End();
    }

    inline _Char_T *begin() noexcept {
        return Storage();
    }

    inline _Char_T *end() noexcept {
        return (Storage() + Length());
    }
    //////////// Private ////////////

  private:
    void setStorage(_Char_T *new_storage) noexcept {
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
        setStorage(Memory::Allocate<_Char_T>(size));
    }

    inline void write(const _Char_T *str, const SizeT len) {
        constexpr SizeT32 size           = sizeof(_Char_T);
        const SizeT       current_offset = Length();
        _length += len;

        if (Capacity() < _length) {
            expand(_length);
        }

        Memory::Copy<size>((Storage() + current_offset), str, (len * size));
    }

    void expand(const SizeT new_capacity) {
        constexpr SizeT32 size    = sizeof(_Char_T);
        const SizeT       src_cap = Capacity();
        _Char_T          *src     = Storage();
        allocate(new_capacity);

        Memory::Copy<size>(Storage(), src, (src_cap * size));
        Memory::Deallocate(src);
    }

    _Char_T *_storage{nullptr};
    SizeT    _length{0};
    SizeT    _capacity{0};
};

} // namespace Qentem

#endif
