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

#ifndef QENTEM_STRINGSTREAM_INITIALSIZE_
#define QENTEM_STRINGSTREAM_INITIALSIZE_ 8U
#endif

namespace Qentem {

/*
 * String Stream class without the null terminator.
 */
template <typename Char_T_>
class StringStream {
  public:
    StringStream() = default;

    ~StringStream() { Memory::Deallocate(storage_); }

    explicit StringStream(SizeT size) : capacity_(size) {
        if (size != 0) {
            capacity_ = (SizeT{1} << Platform::CLZ(capacity_));

            if (capacity_ < size) {
                capacity_ <<= 1U;
            }

            storage_ = Memory::Allocate<Char_T_>(capacity_);
        }
    }

    StringStream(StringStream &&ss) noexcept
        : storage_(ss.storage_), length_(ss.length_), capacity_(ss.capacity_) {
        ss.storage_  = nullptr;
        ss.length_   = 0;
        ss.capacity_ = 0;
    }

    StringStream(const StringStream &ss) {
        if (ss.length_ != 0) {
            capacity_ = ss.length_;
            capacity_ = (SizeT{1} << Platform::CLZ(capacity_));

            if (capacity_ < ss.length_) {
                capacity_ <<= 1U;
            }

            storage_ = Memory::Allocate<Char_T_>(capacity_);
            insert(ss.storage_, ss.length_);
        }
    }

    StringStream &operator=(StringStream &&ss) noexcept {
        if (this != &ss) {
            Memory::Deallocate(storage_);
            storage_  = ss.storage_;
            length_   = ss.length_;
            capacity_ = ss.capacity_;

            ss.storage_  = nullptr;
            ss.length_   = 0;
            ss.capacity_ = 0;
        }

        return *this;
    }

    StringStream &operator=(const StringStream &ss) {
        if (this != &ss) {
            if (storage_ != nullptr) {
                Memory::Deallocate(storage_);
                storage_  = nullptr;
                length_   = 0;
                capacity_ = 0;
            }

            Insert(ss.storage_, ss.length_);
        }

        return *this;
    }

    void operator+=(Char_T_ one_char) {
        if (capacity_ == length_) {
            SizeT n_size = capacity_;

            if (n_size == 0) {
                n_size = QENTEM_STRINGSTREAM_INITIALSIZE_;
            }

            expand(n_size << 1U);
        }

        storage_[length_] = one_char;
        ++length_;
    }

    inline void operator+=(const StringStream<Char_T_> &ss_src) {
        insert(ss_src.First(), ss_src.Length());
    }

    inline void operator+=(const String<Char_T_> &src) {
        insert(src.First(), src.Length());
    }

    inline void operator+=(const Char_T_ *str) {
        insert(str, StringUtils::Count(str));
    }

    inline bool operator==(const StringStream &ss) const noexcept {
        if (length_ != ss.length_) {
            return false;
        }

        return StringUtils::IsEqual(storage_, ss.storage_, length_);
    }

    inline bool operator==(const String<Char_T_> &string) const noexcept {
        if (length_ != string.Length()) {
            return false;
        }

        return StringUtils::IsEqual(storage_, string.First(), length_);
    }

    inline bool operator==(const Char_T_ *str) const noexcept {
        const SizeT len = StringUtils::Count(str);

        if (length_ != len) {
            return false;
        }

        return StringUtils::IsEqual(storage_, str, len);
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
        if (length_ != length) {
            return false;
        }

        return StringUtils::IsEqual(storage_, str, length);
    }

    inline void Insert(const Char_T_ *str, SizeT length) {
        insert(str, length);
    }

    inline void Clear() noexcept { length_ = 0; }

    void Reset() noexcept {
        if (storage_ != nullptr) {
            Memory::Deallocate(storage_);
            storage_  = nullptr;
            length_   = 0;
            capacity_ = 0;
        }
    }

    inline void StepBack(SizeT length) noexcept {
        if (length <= length_) {
            length_ -= length;
        }
    }

    // To write directly to the buffer, set the needed length.
    Char_T_ *Buffer(SizeT len) noexcept {
        const SizeT current_offset = length_;
        length_ += len;

        if (length_ > capacity_) {
            expand((SizeT{2} << Platform::CLZ(length_)));
        }

        return (storage_ + current_offset);
    }

    inline void Expect(SizeT len) {
        const SizeT new_len = (length_ + len);

        if (new_len > capacity_) {
            expand((SizeT{2} << Platform::CLZ(new_len)));
        }
    }

    inline const Char_T_ *Storage() const noexcept { return storage_; }
    inline const Char_T_ *First() const noexcept { return Storage(); }
    inline SizeT          Length() const noexcept { return length_; }
    inline SizeT          Capacity() const noexcept { return capacity_; }
    inline bool           IsEmpty() const noexcept { return (length_ == 0); }
    inline bool           IsNotEmpty() const noexcept { return !(IsEmpty()); }

    inline const Char_T_ *Last() const noexcept {
        if (IsNotEmpty()) {
            return (Storage() + (Length() - 1));
        }

        return nullptr;
    }

    Char_T_ *Eject() noexcept {
        Char_T_ *str = storage_;
        storage_     = nullptr;
        length_      = 0;
        capacity_    = 0;

        return str;
    }

    QENTEM_NOINLINE String<Char_T_> GetString() {
        if (capacity_ > length_) {
            storage_[length_] = 0;
            const SizeT len   = length_;
            return String<Char_T_>(Eject(), len);
        }

        String<Char_T_> str{First(), length_};
        Reset();

        return str;
    }

    //////////// Private ////////////

  private:
    void insert(const Char_T_ *str, const SizeT len) {
        if (len != 0) {
            const SizeT current_offset = length_;
            length_ += len;

            if (capacity_ < length_) {
                expand((SizeT{2} << Platform::CLZ(length_)));
            }

            Memory::Copy((storage_ + current_offset), str,
                         (len * sizeof(Char_T_)));
        }
    }

    void expand(SizeT capacity) {
        Char_T_ *old_str = storage_;
        storage_         = Memory::Allocate<Char_T_>(capacity);

        if (capacity_ != 0) {
            const SizeT c_size = (capacity_ * sizeof(Char_T_));
            Memory::Copy(storage_, old_str, c_size);
            Memory::Deallocate(old_str);
        }

        capacity_ = capacity;
    }

    Char_T_ *storage_{nullptr};
    SizeT    length_{0};
    SizeT    capacity_{0};
};

} // namespace Qentem

#endif
