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

#include "Array.hpp"
#include "HArray.hpp"
#include "JSONUtils.hpp"

#ifndef QENTEM_VALUE_H_
#define QENTEM_VALUE_H_

namespace Qentem {

enum class ValueType {
    Undefined = 0,
    Object,
    Array,
    String,
    Number, // TODO: Add INT
    True,
    False,
    Null
};

template <typename Char_T_>
class Value {
    using JSONotation_T_ = JSON::JSONotation<Char_T_>;
    using VHArray        = HArray<Value, Char_T_>;
    using VArray         = Array<Value>;
    using VString        = String<Char_T_>;

  public:
    Value() = default;

    Value(Value &&val) noexcept : value_(val.value_), type_(val.type_) {
        val.type_ = ValueType::Undefined; // Prevents deletion.
    }

    Value(const Value &val) : type_(val.type_) {
        copyValue(val);
    }

    explicit Value(ValueType type) noexcept : type_(type) {
        switch (type) {
            case ValueType::Object: {
                value_.object_ = HAllocator::AllocateInit<VHArray>();
                break;
            }

            case ValueType::Array: {
                value_.array_ = HAllocator::AllocateInit<VArray>();
                break;
            }

            case ValueType::String: {
                value_.string_ = HAllocator::AllocateInit<VString>();
                break;
            }

            case ValueType::Number: {
                value_.number_ = 0;
                break;
            }

            default: {
            }
        }
    }

    explicit Value(VHArray *obj) noexcept
        : value_(obj), type_(ValueType::Object) {
    }

    explicit Value(VArray *arr) noexcept
        : value_(arr), type_(ValueType::Array) {
    }

    explicit Value(VString *str) noexcept
        : value_(str), type_(ValueType::String) {
    }

    explicit Value(double num) noexcept
        : value_(num), type_(ValueType::Number) {
    }

    ~Value() {
        if (type_ != ValueType::Undefined) {
            Reset();
        }
    }

    Value &operator[](ULong index) {
        switch (type_) {
            case ValueType::Array: {
                if (value_.array_->Size() > index) {
                    return *(value_.array_->First() + index);
                }

                if (value_.array_->Size() == index) {
                    *(value_.array_) += Value();
                    return *(value_.array_->First() + index);
                }

                break;
            }

            case ValueType::Object: {
                Value *val = value_.object_->GetValue(index);

                if (val != nullptr) {
                    return (*val);
                }

                break;
            }

            case ValueType::Undefined: {
                if (index == 0) {
                    type_         = ValueType::Array;
                    value_.array_ = HAllocator::AllocateInit<VArray>();
                    value_.array_->ResizeAndInitialize(1);
                    return *(value_.array_->First());
                }

                break;
            }

            default: {
            }
        }

        throw 1;
    }

    Value &operator[](VString &&key) {
        switch (type_) {
            case ValueType::Object: {
                return (*(value_.object_))[static_cast<VString &&>(key)];
            }

            case ValueType::Undefined: {
                type_          = ValueType::Object;
                value_.object_ = HAllocator::AllocateInit<VHArray>();
                return (*(value_.object_))[static_cast<VString &&>(key)];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator[](const VString &key) {
        switch (type_) {
            case ValueType::Object: {
                return (*(value_.object_))[key];
            }

            case ValueType::Undefined: {
                type_          = ValueType::Object;
                value_.object_ = HAllocator::AllocateInit<VHArray>();
                return (*(value_.object_))[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator=(Value &&val) noexcept {
        if (this != &val) {
            if (type_ != ValueType::Undefined) {
                Reset();
            }

            type_  = val.type_;
            value_ = val.value_;

            val.type_ = ValueType::Undefined; // Prevent deletion.
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            if (type_ == val.type_) {
                switch (type_) {
                    case ValueType::Object: {
                        *(value_.object_) = *(val.value_.object_);
                        break;
                    }

                    case ValueType::Array: {
                        *(value_.array_) = *(val.value_.array_);
                        break;
                    }

                    case ValueType::String: {
                        *(value_.string_) = *(val.value_.string_);
                        break;
                    }

                    case ValueType::Number: {
                        value_.number_ = val.value_.number_;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                if (type_ != ValueType::Undefined) {
                    Reset();
                }

                if (val.type_ != ValueType::Undefined) {
                    type_ = val.type_;
                    copyValue(val);
                }
            }
        }

        return *this;
    }

    Value &operator=(VHArray &&obj) {
        switch (type_) {
            case ValueType::Object: {
                *(value_.object_) = static_cast<VHArray &&>(obj);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_          = ValueType::Object;
                value_.object_ = HAllocator::AllocateInit<VHArray>(
                    static_cast<VHArray &&>(obj));
                break;
            }

            default: {
                Reset();
                type_          = ValueType::Object;
                value_.object_ = HAllocator::AllocateInit<VHArray>(
                    static_cast<VHArray &&>(obj));
            }
        }

        return *this;
    }

    Value &operator=(const VHArray &obj) {
        *this = static_cast<VHArray &&>(VHArray(obj));
        return *this;
    }

    Value &operator=(VArray &&arr) {
        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) = static_cast<VArray &&>(arr);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_         = ValueType::Array;
                value_.array_ = HAllocator::AllocateInit<VArray>(
                    static_cast<VArray &&>(arr));
                break;
            }

            default: {
                Reset();
                type_         = ValueType::Array;
                value_.array_ = HAllocator::AllocateInit<VArray>(
                    static_cast<VArray &&>(arr));
            }
        }

        return *this;
    }

    Value &operator=(const VArray &arr) {
        *this = static_cast<VArray &&>(VArray(arr));
        return *this;
    }

    Value &operator=(VString &&str) {
        switch (type_) {
            case ValueType::String: {
                *(value_.string_) = static_cast<VString &&>(str);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_          = ValueType::String;
                value_.string_ = HAllocator::AllocateInit<VString>(
                    static_cast<VString &&>(str));
                break;
            }

            default: {
                Reset();
                type_          = ValueType::String;
                value_.string_ = HAllocator::AllocateInit<VString>(
                    static_cast<VString &&>(str));
            }
        }

        return *this;
    }

    Value &operator=(const VString &str) {
        *this = static_cast<VString &&>(VString(str));
        return *this;
    }

    Value &operator=(const Char_T_ *str) {
        if (str != nullptr) {
            *this = static_cast<VString &&>(VString(str));
        } else {
            switch (type_) {
                case ValueType::Null: {
                    break;
                }

                case ValueType::Undefined:
                case ValueType::True:
                case ValueType::False: {
                    type_ = ValueType::Null;
                    break;
                }

                default: {
                    Reset();
                    type_ = ValueType::Null;
                }
            }
        }

        return *this;
    }

    Value &operator=(double num) noexcept {
        switch (type_) {
            case ValueType::Number: {
                break;
            }

            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::Number;
                break;
            }

            default: {
                Reset();
                type_ = ValueType::Number;
            }
        }

        value_.number_ = num;
        return *this;
    }

    inline Value &operator=(ULong num) noexcept {
        *this = static_cast<double>(num);
        return *this;
    }

    inline Value &operator=(int num) noexcept {
        *this = static_cast<double>(num);
        return *this;
    }

    inline Value &operator=(UInt num) noexcept {
        *this = static_cast<double>(num);
        return *this;
    }

    Value &operator=(bool is_true) noexcept {
        switch (type_) {
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                break;
            }

            default: {
                Reset();
            }
        }

        type_ = (is_true ? ValueType::True : ValueType::False);
        return *this;
    }

    void operator+=(Value &&val) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
        }

        switch (type_) {
            case ValueType::Array: {
                if (val.type_ == ValueType::Array) {
                    const VArray &src_arr = *(val.value_.array_);
                    VArray &      des_arr = *(value_.array_);

                    Value *      src_val = src_arr.First();
                    const Value *src_end = src_arr.End();

                    while (src_val != src_end) {
                        if (src_val->type_ != ValueType::Undefined) {
                            des_arr += static_cast<Value &&>(*src_val);
                        }

                        ++src_val;
                    }
                } else if (val.type_ != ValueType::Undefined) {
                    *(value_.array_) += static_cast<Value &&>(val);
                }

                break;
            }

            case ValueType::Object: {
                if (val.type_ == ValueType::Object) {
                    *(value_.object_) +=
                        static_cast<VHArray &&>(*(val.value_.object_));
                }

                break;
            }

            default: {
            }
        }

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
        }

        switch (type_) {
            case ValueType::Array: {
                if (val.type_ == ValueType::Array) {
                    const VArray &src_arr = *(val.value_.array_);
                    VArray &      des_arr = *(value_.array_);

                    const Value *src_val = src_arr.First();
                    const Value *src_end = src_arr.End();

                    while (src_val != src_end) {
                        if (src_val->type_ != ValueType::Undefined) {
                            des_arr += *src_val;
                        }

                        ++src_val;
                    }
                } else if (val.type_ != ValueType::Undefined) {
                    *(value_.array_) += val;
                }

                break;
            }

            case ValueType::Object: {
                if (val.type_ == ValueType::Object) {
                    *(value_.object_) += *(val.value_.object_);
                }

                break;
            }

            default: {
            }
        }
    }

    void operator+=(VHArray &&obj) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) += Value{HAllocator::AllocateInit<VHArray>(
                    static_cast<VHArray &&>(obj))};
                break;
            }

            case ValueType::Object: {
                *(value_.object_) += static_cast<VHArray &&>(obj);
                break;
            }

            default: {
            }
        }
    }

    void operator+=(const VHArray &obj) {
        *this += VHArray(obj);
    }

    void operator+=(VArray &&arr) {
        if (type_ == ValueType::Array) {
            *(value_.array_) += static_cast<VArray &&>(arr);
        } else if (type_ == ValueType::Undefined) {
            type_ = ValueType::Array;
            value_.array_ =
                HAllocator::AllocateInit<VArray>(static_cast<VArray &&>(arr));
        }
    }

    void operator+=(const VArray &arr) {
        (*this) += VArray(arr);
    }

    void operator+=(VString &&str) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) += Value{HAllocator::AllocateInit<VString>(
                    static_cast<VString &&>(str))};
                break;
            }

            case ValueType::String: {
                *(value_.string_) += static_cast<VString &&>(str);
                break;
            }

            default: {
            }
        }
    }

    void operator+=(const VString &string_) {
        (*this) += VString(string_);
    }

    void operator+=(const Char_T_ *str) {
        if (str != nullptr) {
            *this += VString(str);
        } else {
            if (type_ == ValueType::Array) {
                *(value_.array_) += Value{ValueType::Null};
            } else if (type_ == ValueType::Undefined) {
                type_         = ValueType::Array;
                value_.array_ = HAllocator::AllocateInit<VArray>();
                *(value_.array_) += Value{ValueType::Null};
            }
        }
    }

    void operator+=(double num) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) += Value{num};
                break;
            }

            case ValueType::Number: {
                value_.number_ += num;
                break;
            }

            default: {
            }
        }
    }

    inline void operator+=(ULong num) {
        *this += static_cast<double>(num);
    }

    inline void operator+=(int num) {
        *this += static_cast<double>(num);
    }

    inline void operator+=(UInt num) {
        *this += static_cast<double>(num);
    }

    void operator+=(bool is_true) {
        if (type_ == ValueType::Array) {
            ValueType type = (is_true ? ValueType::True : ValueType::False);
            *(value_.array_) += Value{type};
        } else if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::AllocateInit<VArray>();
            *this += is_true;
        }
    }

    inline bool IsUndefined() const noexcept {
        return (type_ == ValueType::Undefined);
    }

    inline bool IsObject() const noexcept {
        return (type_ == ValueType::Object);
    }

    inline bool IsArray() const noexcept {
        return (type_ == ValueType::Array);
    }

    inline bool IsString() const noexcept {
        return (type_ == ValueType::String);
    }

    inline bool IsNumber() const noexcept {
        return (type_ == ValueType::Number);
    }

    inline bool IsTrue() const noexcept {
        return (type_ == ValueType::True);
    }

    inline bool IsFalse() const noexcept {
        return (type_ == ValueType::False);
    }

    inline bool IsNull() const noexcept {
        return (type_ == ValueType::Null);
    }

    inline ValueType Type() const noexcept {
        return type_;
    }

    ULong Size() const noexcept {
        switch (type_) {
            case ValueType::Object: {
                return value_.object_->Size();
            }

            case ValueType::Array: {
                return value_.array_->Size();
            }

            default: {
            }
        }

        return 0;
    }

    Value *GetValue(ULong index) const noexcept {
        if (type_ == ValueType::Array) {
            if (index < value_.array_->Size()) {
                Value *val = (value_.array_->First() + index);

                if (!(val->IsUndefined())) {
                    return val;
                }
            }
        } else if (type_ == ValueType::Object) {
            Value *val = value_.object_->GetValue(index);

            if ((val != nullptr) && (!(val->IsUndefined()))) {
                return val;
            }
        }

        return nullptr;
    }

    Value *GetValue(const Char_T_ *key, ULong length) const noexcept {
        if (type_ == ValueType::Object) {
            Value *val = (value_.object_->Find(key, length));

            if ((val != nullptr) && !(val->IsUndefined())) {
                return val;
            }
        }

        return nullptr;
    }

    const VString *GetKey(ULong index) const noexcept {
        if (type_ == ValueType::Object) {
            return value_.object_->GetKey(index);
        }

        return nullptr;
    }

    VHArray *GetObject() const noexcept {
        if (type_ == ValueType::Object) {
            return value_.object_;
        }

        return nullptr;
    }

    VArray *GetArray() const noexcept {
        if (type_ == ValueType::Array) {
            return value_.array_;
        }

        return nullptr;
    }

    const VString *GetString() const noexcept {
        if (type_ == ValueType::String) {
            return value_.string_;
        }

        return nullptr;
    }

    const Char_T_ *StringStorage() const noexcept {
        if (type_ == ValueType::String) {
            return value_.string_->First();
        }

        return nullptr;
    }

    ULong Length() const noexcept {
        if (type_ == ValueType::String) {
            return value_.string_->Length();
        }

        return 0;
    }

    template <typename Number_T_>
    bool SetCharAndLength(const Char_T_ *&key,
                          Number_T_ &     length) const noexcept {
        switch (type_) {
            case ValueType::String: {
                key    = value_.string_->First();
                length = static_cast<Number_T_>(value_.string_->Length());
                return true;
            }

            case ValueType::True: {
                key    = JSONotation_T_::GetTrueString();
                length = JSONotation_T_::TrueStringLength;
                return true;
            }

            case ValueType::False: {
                key    = JSONotation_T_::GetFalseString();
                length = JSONotation_T_::FalseStringLength;
                return true;
            }

            case ValueType::Null: {
                key    = JSONotation_T_::GetNullString();
                length = JSONotation_T_::NullStringLength;
                return true;
            }

            default: {
                return false;
            }
        }
    }

    bool SetString(VString &value) const {
        switch (type_) {
            case ValueType::String: {
                value = *(value_.string_);
                return true;
            }

            case ValueType::Number: {
                value = Digit<Char_T_>::NumberToString(
                    value_.number_, 1, 0, QENTEM_DOUBLE_PRECISION_);
                return true;
            }

            case ValueType::True: {
                value = VString(JSONotation_T_::GetTrueString(),
                                JSONotation_T_::TrueStringLength);
                return true;
            }

            case ValueType::False: {
                value = VString(JSONotation_T_::GetFalseString(),
                                JSONotation_T_::FalseStringLength);
                return true;
            }

            case ValueType::Null: {
                value = VString(JSONotation_T_::GetNullString(),
                                JSONotation_T_::NullStringLength);
                return true;
            }

            default: {
            }
        }

        return false;
    }

    bool InsertString(StringStream<Char_T_> &ss) const {
        switch (type_) {
            case ValueType::String: {
                ss += *(value_.string_);
                return true;
            }

            case ValueType::Number: {
                Digit<Char_T_>::NumberToStringStream(ss, value_.number_, 1, 0,
                                                     QENTEM_DOUBLE_PRECISION_);
                return true;
            }

            case ValueType::True: {
                ss.Insert(JSONotation_T_::GetTrueString(),
                          JSONotation_T_::TrueStringLength);
                return true;
            }

            case ValueType::False: {
                ss.Insert(JSONotation_T_::GetFalseString(),
                          JSONotation_T_::FalseStringLength);
                return true;
            }

            case ValueType::Null: {
                ss.Insert(JSONotation_T_::GetNullString(),
                          JSONotation_T_::NullStringLength);
                return true;
            }

            default: {
            }
        }

        return false;
    }

    bool InsertKey(StringStream<Char_T_> &ss, ULong index) const {
        if (type_ == ValueType::Object) {
            const VString *key = value_.object_->GetKey(index);

            if (key != nullptr) {
                ss += *key;
            }

            return true;
        }

        return false;
    }

    double GetNumber() const noexcept {
        if (type_ == ValueType::Number) {
            return value_.number_;
        }

        return 0;
    }

    bool SetNumber(double &value) const noexcept {
        switch (type_) {
            case ValueType::Number: {
                value = value_.number_;
                return true;
            }

            case ValueType::String: {
                return Digit<Char_T_>::StringToNumber(
                    value, value_.string_->First(),
                    static_cast<UInt>(value_.string_->Length()));
            }

            case ValueType::True: {
                value = 1.0;
                return true;
            }

            case ValueType::False:
            case ValueType::Null: {
                value = 0.0;
                return true;
            }

            default: {
            }
        }

        value = 0.0;
        return false;
    }

    bool GetBool(bool &value) const noexcept {
        switch (type_) {
            case ValueType::True: {
                value = true;
                return true;
            }

            case ValueType::False:
            case ValueType::Null: {
                value = false;
                return true;
            }

            case ValueType::Number: {
                value = (value_.number_ > 0);
                return true;
            }

            case ValueType::String: {
                if (value_.string_->IsEqual(JSONotation_T_::GetTrueString(),
                                            JSONotation_T_::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (value_.string_->IsEqual(
                        JSONotation_T_::GetFalseString(),
                        JSONotation_T_::FalseStringLength)) {
                    value = false;
                    return true;
                }

                return false;
            }

            default: {
            }
        }

        return false;
    }

    void Remove(ULong index) const noexcept {
        if (type_ == ValueType::Object) {
            value_.object_->RemoveIndex(index);
        } else if ((type_ == ValueType::Array) &&
                   (index < value_.array_->Size())) {
            (value_.array_->First() + index)->Reset();
        }
    }

    inline void Remove(const VString &key) const noexcept {
        if (type_ == ValueType::Object) {
            value_.object_->Remove(key);
        }
    }

    void Reset() noexcept {
        switch (type_) {
            case ValueType::Object: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.object_);
                HAllocator::Deallocate(value_.object_);
                value_.object_ = nullptr;
                break;
            }

            case ValueType::Array: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.array_);
                HAllocator::Deallocate(value_.array_);
                value_.array_ = nullptr;
                break;
            }

            case ValueType::String: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.string_);
                HAllocator::Deallocate(value_.string_);
                value_.string_ = nullptr;
                break;
            }

            default: {
                type_ = ValueType::Undefined;
            }
        }
    }

    void Compress() {
        if (type_ == ValueType::Object) {
            value_.object_->Compress();
        } else if (type_ == ValueType::Array) {
            const ULong size = value_.array_->Size();

            if (size != value_.array_->Capacity()) {
                if (size == 0) {
                    value_.array_->Reset();
                    return;
                }

                VArray       new_array(size);
                Value *      src_val = value_.array_->First();
                const Value *src_end = value_.array_->End();

                do {
                    if (src_val->type_ != ValueType::Undefined) {
                        new_array += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                } while (src_val != src_end);

                *(value_.array_) = static_cast<VArray &&>(new_array);
            }
        }
    }

    void Stringify(StringStream<Char_T_> &ss) const {
        const ULong size   = Size();
        const bool  is_obj = (type_ == ValueType::Object);

        const HAItem<Value, Char_T_> *ha_item = nullptr;
        const Value *                 item    = nullptr;

        if (is_obj) {
            ha_item = (value_.object_->First() - 1);
            ss += JSONotation_T_::OCurlyChar;
        } else if (type_ == ValueType::Array) {
            item = (value_.array_->First() - 1);
            ss += JSONotation_T_::OSquareChar;
        } else {
            return;
        }

        if (size != 0) {
            ULong id = 0;

            do {
                if (is_obj) {
                    ++ha_item;

                    if ((ha_item == nullptr) ||
                        (ha_item->Key.First() == nullptr) ||
                        (ha_item->Value.type_ == ValueType::Undefined)) {
                        continue; // Deleted item.
                    }

                    ss += JSONotation_T_::QuoteChar;
                    JSON::EscapeJSON(ha_item->Key.First(),
                                     ha_item->Key.Length(), ss);
                    ss += JSONotation_T_::QuoteChar;
                    ss += JSONotation_T_::ColonChar;

                    item = &(ha_item->Value);
                } else {
                    ++item;

                    if (item->type_ == ValueType::Undefined) {
                        continue; // Deleted key/value.
                    }
                }

                switch (item->type_) {
                    case ValueType::Object:
                    case ValueType::Array: {
                        item->Stringify(ss);
                        break;
                    }

                    case ValueType::String: {
                        ss += JSONotation_T_::QuoteChar;
                        JSON::EscapeJSON(item->value_.string_->First(),
                                         item->value_.string_->Length(), ss);
                        ss += JSONotation_T_::QuoteChar;
                        break;
                    }

                    case ValueType::Number: {
                        Digit<Char_T_>::NumberToStringStream(
                            ss, item->value_.number_);
                        break;
                    }

                    case ValueType::False: {
                        ss.Insert(JSONotation_T_::GetFalseString(),
                                  JSONotation_T_::FalseStringLength);
                        break;
                    }

                    case ValueType::True: {
                        ss.Insert(JSONotation_T_::GetTrueString(),
                                  JSONotation_T_::TrueStringLength);
                        break;
                    }

                    case ValueType::Null: {
                        ss.Insert(JSONotation_T_::GetNullString(),
                                  JSONotation_T_::NullStringLength);
                        break;
                    }

                    default: {
                    }
                }

                ss += JSONotation_T_::CommaChar;
            } while (++id != size);

            if (ss.First()[(ss.Length() - 1)] == JSONotation_T_::CommaChar) {
                ss.StepBack(1);
            }
        }

        if (is_obj) {
            ss += JSONotation_T_::CCurlyChar;
        } else {
            ss += JSONotation_T_::CSquareChar;
        }
    }

    inline VString Stringify() const {
        StringStream<Char_T_> ss;
        Stringify(ss);
        return ss.GetString();
    }

  private:
    void copyValue(const Value &val) {
        switch (type_) {
            case ValueType::Object: {
                value_.object_    = HAllocator::AllocateInit<VHArray>();
                *(value_.object_) = *(val.value_.object_);
                break;
            }

            case ValueType::Array: {
                value_.array_    = HAllocator::AllocateInit<VArray>();
                *(value_.array_) = *(val.value_.array_);

                break;
            }

            case ValueType::String: {
                value_.string_    = HAllocator::AllocateInit<VString>();
                *(value_.string_) = *(val.value_.string_);
                break;
            }

            case ValueType::Number: {
                value_.number_ = val.value_.number_;
                break;
            }

            default: {
            }
        }
    }

    union Value_U_ {
        Value_U_() {
            object_ = nullptr;
        }

        explicit Value_U_(VHArray *object) noexcept {
            object_ = object;
        }

        explicit Value_U_(VArray *array) noexcept {
            array_ = array;
        }

        explicit Value_U_(VString *string) noexcept {
            string_ = string;
        }

        explicit Value_U_(double number) noexcept {
            number_ = number;
        }

        VHArray *object_;
        VArray * array_;
        VString *string_;
        double   number_;
    } value_{};

    ValueType type_{ValueType::Undefined};
};

} // namespace Qentem

#endif
