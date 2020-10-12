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

#include "Memory.hpp"
#include "StringUtils.hpp"

#ifndef QENTEM_STRING_H_
#define QENTEM_STRING_H_

namespace Qentem {

/*
 * String container with null terminator and a taggable pointer.
 */
template <typename Char_T_>
class String {
  public:
    String() = default;

    explicit String(SizeT len) : length_(len) {
        allocate(len + 1);
        Storage()[Length()] = 0;
    }

    String(Char_T_ *str, SizeT len) noexcept : storage_(str), length_(len) {}

    String(const Char_T_ *str, SizeT len) : length_(len) {
        allocate(len + 1);

        if (len != 0) {
            Memory::Copy(Storage(), str, (Length() * sizeof(Char_T_)));
        }

        Storage()[Length()] = 0;
    }

    explicit String(const Char_T_ *str)
        : String(str, StringUtils::Count(str)) {}

    String(String &&src) noexcept
        : storage_(src.storage_), length_(src.Length()) {
        src.clearStorage();
        src.setLength(0);
    }

    String(const String &src) : length_(src.Length()) {
        if (IsNotEmpty()) {
            allocate(Length() + 1);
            Memory::Copy(Storage(), src.First(), (Length() * sizeof(Char_T_)));
            Storage()[Length()] = 0;
        }
    }

    ~String() { deallocate(Storage()); }

    String &operator=(String &&src) noexcept {
        if (this != &src) {
            deallocate(Storage());

            // Do not use Storage(), or the tag will not be copied.
            setStorage(src.storage_);
            setLength(src.Length());
            src.clearStorage();
            src.setLength(0);
        }

        return *this;
    }

    String &operator=(const String &src) {
        if (this != &src) {
            deallocate(Storage());
            setLength(src.Length());
            allocate(Length() + 1);
            Memory::Copy(Storage(), src.First(), (Length() * sizeof(Char_T_)));
            Storage()[Length()] = 0;
        }

        return *this;
    }

    String &operator=(const Char_T_ *str) {
        SizeT len = StringUtils::Count(str);

        deallocate(Storage());
        allocate(len + 1);
        setLength(len);

        if (len != 0) {
            Memory::Copy(Storage(), str, (len * sizeof(Char_T_)));
        }

        Storage()[len] = 0;

        return *this;
    }

    String &operator+=(String &&src) {
        Insert(src.First(), src.Length());
        deallocate(src.Storage());
        src.clearStorage();
        src.setLength(0);

        return *this;
    }

    String &operator+=(const String &src) {
        Insert(src.First(), src.Length());
        return *this;
    }

    String &operator+=(const Char_T_ *str) {
        Insert(str, StringUtils::Count(str));
        return *this;
    }

    String operator+(String &&src) const {
        String ns{Insert(*this, src)};

        src.deallocate(src.Storage());
        src.clearStorage();
        src.setLength(0);

        return ns;
    }

    inline String operator+(const String &src) const {
        return Insert(*this, src);
    }

    String operator+(const Char_T_ *str) const {
        const SizeT len = StringUtils::Count(str);
        String      ns  = String{Length() + len};

        if (IsNotEmpty()) {
            Memory::Copy(ns.Storage(), First(), (Length() * sizeof(Char_T_)));
        }

        if (len != 0) {
            Memory::Copy((ns.Storage() + Length()), str,
                         (len * sizeof(Char_T_)));
        }

        return ns;
    }

    inline bool operator==(const String &string) const noexcept {
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

    inline bool operator!=(const String &string) const noexcept {
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

    void Reset() noexcept {
        deallocate(Storage());
        clearStorage();
        setLength(0);
    }

    Char_T_ *Eject() noexcept {
        Char_T_ *str = Storage();
        clearStorage();
        setLength(0);

        return str;
    }

#if QENTEM_TAGGED_POINTER_ == 1
    inline Char_T_ *Storage() const noexcept {
        return reinterpret_cast<Char_T_ *>(int_storage_ & 281474976710655ULL);
    }
#else
    inline Char_T_ *Storage() const noexcept { return storage_; }
#endif

    inline SizeT          Length() const noexcept { return length_; }
    inline const Char_T_ *First() const noexcept { return Storage(); }
    inline bool           IsEmpty() const noexcept { return (Length() == 0); }
    inline bool           IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Char_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (First() + (Length() - 1));
        }

        return nullptr;
    }

    static String Insert(const String &src1, const String &src2) {
        const SizeT ns_len = (src1.Length() + src2.Length());
        String      ns     = String{ns_len};

        if (src1.IsNotEmpty()) {
            Memory::Copy(ns.Storage(), src1.First(),
                         (src1.Length() * sizeof(Char_T_)));
        }

        if (src2.IsNotEmpty()) {
            Memory::Copy((ns.Storage() + src1.Length()), src2.First(),
                         (src2.Length() * sizeof(Char_T_)));
        }

        return ns;
    }

    void Insert(const Char_T_ *str, SizeT len) {
        if ((str != nullptr) && (len != 0)) {
            Char_T_ *old_str = Storage();
            allocate(Length() + len + 1);

            if (old_str != nullptr) {
                Memory::Copy(Storage(), old_str, (Length() * sizeof(Char_T_)));
                deallocate(old_str);
            }

            Memory::Copy((Storage() + Length()), str, (len * sizeof(Char_T_)));
            length_ += len;
            Storage()[Length()] = 0;
        }
    }

    static String Trim(const String &str) {
        SizeT length = str.Length();
        SizeT offset = 0;
        StringUtils::SoftTrim(str.First(), offset, length);
        return String((str.First() + offset), length);
    }

    //////////// Private ////////////

  private:
    void setStorage(Char_T_ *new_storage) noexcept {
#if QENTEM_TAGGED_POINTER_ == 1
        int_storage_ &= 18446462598732840960ULL; // Preserve the tag
        int_storage_ |= reinterpret_cast<unsigned long long>(
            new_storage); // Restore the tag
#else
        storage_ = new_storage;
#endif
    }

    void allocate(SizeT new_size) {
        setStorage(Memory::Allocate<Char_T_>(new_size));
    }

    void deallocate(Char_T_ *old_storage) { Memory::Deallocate(old_storage); }
    void clearStorage() noexcept { setStorage(nullptr); }
    void setLength(SizeT new_length) noexcept { length_ = new_length; }

#if QENTEM_TAGGED_POINTER_ == 1
    union {
        unsigned long long int_storage_;
        Char_T_ *          storage_{nullptr};
    };
#else
    Char_T_ *storage_{nullptr};
#endif

    SizeT length_{0};
};

} // namespace Qentem

#endif
