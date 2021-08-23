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

#include "Platform.hpp"
#include "String.hpp"

#ifndef QENTEM_STRINGSTREAM_H_
#define QENTEM_STRINGSTREAM_H_

namespace Qentem {

/*
 * String Stream class without the null terminator.
 */
template <typename Char_T_>
class StringStream {
    static constexpr SizeT initial_size = 8;

  public:
    StringStream() = default;

    ~StringStream() { deallocate(Storage()); }

    explicit StringStream(SizeT size) {
        if (size != 0) {
            setCapacity(algineSize(size));
            allocate();
        }
    }

    StringStream(StringStream &&src) noexcept
        : storage_(src.Storage()), length_(src.Length()),
          capacity_(src.Capacity()) {
        src.setLength(0);
        src.setCapacity(0);
        src.clearStorage();
    }

    StringStream(const StringStream &src) {
        if (src.Length() != 0) {
            setCapacity(algineSize(src.Length()));
            allocate();
            insert(src.First(), src.Length());
        }
    }

    StringStream &operator=(StringStream &&src) noexcept {
        if (this != &src) {
            deallocate(Storage());
            setStorage(src.Storage());
            setLength(src.Length());
            setCapacity(src.Capacity());

            src.setLength(0);
            src.setCapacity(0);
            src.clearStorage();
        }

        return *this;
    }

    StringStream &operator=(const StringStream &src) {
        if (this != &src) {
            setLength(0);
            setCapacity(0);
            deallocate(Storage());
            clearStorage();
            Insert(src.First(), src.Length());
        }

        return *this;
    }

    StringStream &operator=(const Char_T_ *str) {
        setLength(0);
        setCapacity(0);
        deallocate(Storage());
        clearStorage();
        insert(str, StringUtils::Count(str));

        return *this;
    }

    StringStream &operator=(const String<Char_T_> &src) {
        setLength(0);
        setCapacity(0);
        deallocate(Storage());
        clearStorage();
        insert(src.First(), src.Length());

        return *this;
    }

    void operator+=(Char_T_ one_char) {
        if (Capacity() == Length()) {
            SizeT n_size = Capacity();

            if (n_size == 0) {
                n_size = initial_size;
            }

            expand(n_size << 2U);
        }

        Storage()[Length()] = one_char;
        ++length_;
    }

    inline void operator+=(const StringStream<Char_T_> &src) {
        insert(src.First(), src.Length());
    }

    inline void operator+=(const String<Char_T_> &src) {
        insert(src.First(), src.Length());
    }

    inline void operator+=(const Char_T_ *str) {
        insert(str, StringUtils::Count(str));
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const StringStream &src) {
        const Char_T_ *str = src.First();
        const Char_T_ *end = (str + src.Length());

        while (str != end) {
            out << *str;
            ++str;
        }

        return out;
    }

    friend StringStream &operator<<(StringStream &      out,
                                    const StringStream &src) {
        out.insert(src.First(), src.Length());
        return out;
    }

    friend StringStream &operator<<(StringStream &         out,
                                    const String<Char_T_> &src) {
        out.insert(src.First(), src.Length());
        return out;
    }

    friend StringStream &operator<<(StringStream &out, const Char_T_ ch) {
        out += ch;
        return out;
    }

    friend StringStream &operator<<(StringStream &out, const Char_T_ *str) {
        out.insert(str, StringUtils::Count(str));
        return out;
    }

    inline bool operator==(const StringStream &ss) const noexcept {
        if (Length() != ss.Length()) {
            return false;
        }

        return StringUtils::IsEqual(First(), ss.First(), Length());
    }

    inline bool operator==(const String<Char_T_> &string) const noexcept {
        if (Length() != string.Length()) {
            return false;
        }

        return StringUtils::IsEqual(First(), string.First(), Length());
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT len = StringUtils::Count(str);

        if (Length() != len) {
            return false;
        }

        return StringUtils::IsEqual(First(), str, len);
    }

    inline bool operator!=(const StringStream &ss) const noexcept {
        return (!(*this == ss));
    }

    inline bool operator!=(const String<Char_T_> &string) const noexcept {
        return (!(*this == string));
    }

    inline bool operator!=(const Char_T_ *str) const noexcept {
        return (!(*this == str));
    }

    inline bool IsEqual(const Char_T_ *str, SizeT length) const noexcept {
        if (Length() != length) {
            return false;
        }

        return StringUtils::IsEqual(First(), str, length);
    }

    inline void Insert(const Char_T_ *str, SizeT length) {
        insert(str, length);
    }

    inline void Clear() noexcept { setLength(0); }

    void Reset() noexcept {
        setLength(0);
        setCapacity(0);
        deallocate(Storage());
        clearStorage();
    }

    inline void StepBack(SizeT length) noexcept {
        if (length <= Length()) {
            length_ -= length;
        }
    }

    // Set the needed length to write directly to the buffer,
    Char_T_ *Buffer(SizeT len) noexcept {
        const SizeT current_offset = Length();
        length_ += len;

        if (Length() > Capacity()) {
            expand(algineSize(Length()));
        }

        return (Storage() + current_offset);
    }

    inline void Expect(SizeT len) {
        const SizeT new_len = (Length() + len);

        if (new_len > Capacity()) {
            expand(algineSize(new_len));
        }
    }

    Char_T_ *Eject() noexcept {
        setLength(0);
        setCapacity(0);
        Char_T_ *str = Storage();
        clearStorage();

        return str;
    }

    String<Char_T_> GetString() {
        if (Capacity() > Length()) {
            const SizeT len = Length();
            Storage()[len]  = 0;
            return String<Char_T_>(Eject(), len);
        }

        String<Char_T_> str{First(), Length()};
        Reset();

        return str;
    }

    inline Char_T_ *      Storage() const noexcept { return storage_; }
    inline SizeT          Length() const noexcept { return length_; }
    inline SizeT          Capacity() const noexcept { return capacity_; }
    inline const Char_T_ *First() const noexcept { return Storage(); }
    inline bool           IsEmpty() const noexcept { return (Length() == 0); }
    inline bool           IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Char_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Length() - 1));
        }

        return nullptr;
    }

    //////////// Private ////////////

  private:
    void setStorage(Char_T_ *new_storage) noexcept { storage_ = new_storage; }
    void allocate() { setStorage(Memory::Allocate<Char_T_>(Capacity())); }
    void deallocate(Char_T_ *old_storage) { Memory::Deallocate(old_storage); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setLength(SizeT new_length) noexcept { length_ = new_length; }
    void setCapacity(SizeT new_capacity) noexcept { capacity_ = new_capacity; }

    SizeT algineSize(SizeT n_size) noexcept {
        return (SizeT{2} << Platform::CLZ(n_size | 4U));
    }

    void insert(const Char_T_ *str, const SizeT len) {
        if (len != 0) {
            const SizeT current_offset = Length();
            length_ += len;

            if (Capacity() < Length()) {
                expand(algineSize(Length()));
            }

            Memory::Copy((Storage() + current_offset), str,
                         (len * sizeof(Char_T_)));
        }
    }

    void expand(SizeT new_capacity) {
        SizeT src_cap = Capacity();
        setCapacity(new_capacity);
        Char_T_ *src = Storage();
        allocate();

        Memory::Copy(Storage(), src, (src_cap * sizeof(Char_T_)));
        deallocate(src);
    }

    Char_T_ *storage_{nullptr};
    SizeT    length_{0};
    SizeT    capacity_{0};
};

} // namespace Qentem

#endif
