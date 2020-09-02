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
#include "Digit.hpp"
#include "HArray.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_VALUE_H_
#define QENTEM_VALUE_H_

#define QENTEM_TRUE_LEN_ 4U
#define QENTEM_FALSE_LEN_ 5U
#define QENTEM_NULL_LEN_ 4U

namespace Qentem {
namespace JSON {

static void EscapeString(const char *content, ULong length, StringStream &ss) {
    ULong offset  = 0;
    ULong offset2 = 0;

    while (offset < length) {
        switch (content[offset]) {
            case '\"':
            case '\\':
            case '/':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t': {
                if (offset > offset2) {
                    ss.Add((content + offset2), (offset - offset2));
                }

                offset2 = offset + 1;

                switch (content[offset]) {
                    case '\"': {
                        ss.Add("\\\"", 2);
                        break;
                    }

                    case '\\': {
                        ss.Add("\\\\", 2);
                        break;
                    }

                    case '/': {
                        ss.Add("\\/", 2);
                        break;
                    }

                    case '\b': {
                        ss.Add("\\b", 2);
                        break;
                    }

                    case '\f': {
                        ss.Add("\\f", 2);
                        break;
                    }

                    case '\n': {
                        ss.Add("\\n", 2);
                        break;
                    }

                    case '\r': {
                        ss.Add("\\r", 2);
                        break;
                    }

                    case '\t': {
                        ss.Add("\\t", 2);
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            default: {
            }
        }

        ++offset;
    }

    if (offset > offset2) {
        ss.Add((content + offset2), (offset - offset2));
    }
}
} // namespace JSON
////////////////////////////////////////////////////////////

enum class ValueType {
    Undefined = 0,
    Object,
    Array,
    String,
    Number,
    True,
    False,
    Null
};

class Value {
    union Value_U_;

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
                value_.object_ = HAllocator::AllocateClear<HArray<Value>>(1);
                break;
            }

            case ValueType::Array: {
                value_.array_ = HAllocator::AllocateClear<Array<Value>>(1);
                break;
            }

            case ValueType::String: {
                value_.string_ = HAllocator::AllocateClear<String>(1);
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

    explicit Value(HArray<Value> *obj) noexcept : type_(ValueType::Object) {
        value_.object_ = obj;
    }

    explicit Value(Array<Value> *arr) noexcept : type_(ValueType::Array) {
        value_.array_ = arr;
    }

    explicit Value(String *str) noexcept : type_(ValueType::String) {
        value_.string_ = str;
    }

    explicit Value(double value) noexcept : type_(ValueType::Number) {
        value_.number_ = value;
    }

    ~Value() {
        if (type_ != ValueType::Undefined) {
            Clear();
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
                    value_.array_ = HAllocator::AllocateClear<Array<Value>>(1);
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

    Value &operator[](String &&key) {
        switch (type_) {
            case ValueType::Object: {
                return (*(value_.object_))[static_cast<String &&>(key)];
            }

            case ValueType::Undefined: {
                type_          = ValueType::Object;
                value_.object_ = HAllocator::Allocate(HArray<Value>(1));
                return (*this)[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator[](const String &key) {
        switch (type_) {
            case ValueType::Object: {
                return (*(value_.object_))[key];
            }

            case ValueType::Undefined: {
                type_          = ValueType::Object;
                value_.object_ = HAllocator::Allocate(HArray<Value>(1));
                return (*this)[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator=(Value &&val) noexcept {
        if (this != &val) {
            if (type_ != ValueType::Undefined) {
                Clear();
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
                    Clear();
                }

                if (val.type_ != ValueType::Undefined) {
                    type_ = val.type_;
                    copyValue(val);
                }
            }
        }

        return *this;
    }

    Value &operator=(HArray<Value> &&obj) {
        switch (type_) {
            case ValueType::Object: {
                *(value_.object_) = static_cast<HArray<Value> &&>(obj);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::Object;
                value_.object_ =
                    HAllocator::Allocate(static_cast<HArray<Value> &&>(obj));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::Object;
                value_.object_ =
                    HAllocator::Allocate(static_cast<HArray<Value> &&>(obj));
            }
        }

        return *this;
    }

    Value &operator=(const HArray<Value> &obj) {
        *this = HArray<Value>(obj);
        return *this;
    }

    Value &operator=(Array<Value> &&arr) {
        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) = static_cast<Array<Value> &&>(arr);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::Array;
                value_.array_ =
                    HAllocator::Allocate(static_cast<Array<Value> &&>(arr));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::Array;
                value_.array_ =
                    HAllocator::Allocate(static_cast<Array<Value> &&>(arr));
            }
        }

        return *this;
    }

    Value &operator=(const Array<Value> &arr) {
        *this = Array<Value>(arr);
        return *this;
    }

    Value &operator=(String &&str) {
        switch (type_) {
            case ValueType::String: {
                *(value_.string_) = static_cast<String &&>(str);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::String;
                value_.string_ =
                    HAllocator::Allocate(static_cast<String &&>(str));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::String;
                value_.string_ =
                    HAllocator::Allocate(static_cast<String &&>(str));
            }
        }

        return *this;
    }

    Value &operator=(const String &str) {
        *this = String(str);
        return *this;
    }

    Value &operator=(const char *str) {
        if (str != nullptr) {
            *this = String(str);
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
                    Clear();
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
                Clear();
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
                Clear();
            }
        }

        type_ = (is_true ? ValueType::True : ValueType::False);
        return *this;
    }

    void operator+=(Value &&val) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                if (val.type_ == ValueType::Array) {
                    const Array<Value> &src_arr = *(val.value_.array_);
                    Array<Value> &      des_arr = *(value_.array_);

                    Value *      src_val = src_arr.First();
                    const Value *src_end = (src_arr.Storage() + src_arr.Size());

                    while (src_val < src_end) {
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
                        static_cast<HArray<Value> &&>(*(val.value_.object_));
                }

                break;
            }

            default: {
            }
        }

        val.Clear();
    }

    void operator+=(const Value &val) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                if (val.type_ == ValueType::Array) {
                    const Array<Value> &src_arr = *(val.value_.array_);
                    Array<Value> &      des_arr = *(value_.array_);

                    const Value *src_val = src_arr.Storage();
                    const Value *src_end = (src_arr.Storage() + src_arr.Size());

                    while (src_val < src_end) {
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

    void operator+=(HArray<Value> &&obj) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) += Value(
                    HAllocator::Allocate(static_cast<HArray<Value> &&>(obj)));
                break;
            }

            case ValueType::Object: {
                *(value_.object_) += static_cast<HArray<Value> &&>(obj);
                break;
            }

            default: {
            }
        }
    }

    void operator+=(const HArray<Value> &obj) {
        *this += HArray<Value>(obj);
    }

    void operator+=(Array<Value> &&arr) {
        if (type_ == ValueType::Array) {
            *(value_.array_) += static_cast<Array<Value> &&>(arr);
        } else if (type_ == ValueType::Undefined) {
            type_ = ValueType::Array;
            value_.array_ =
                HAllocator::Allocate(static_cast<Array<Value> &&>(arr));
        }
    }

    void operator+=(const Array<Value> &arr) {
        (*this) += Array<Value>(arr);
    }

    void operator+=(String &&str) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) +=
                    Value(HAllocator::Allocate(static_cast<String &&>(str)));
                break;
            }

            case ValueType::String: {
                *(value_.string_) += static_cast<String &&>(str);
                break;
            }

            default: {
            }
        }
    }

    void operator+=(const String &string_) {
        (*this) += String(string_);
    }

    void operator+=(const char *str) {
        if (str != nullptr) {
            *this += String(str);
        } else {
            if (type_ == ValueType::Array) {
                *(value_.array_) += Value(ValueType::Null);
            } else if (type_ == ValueType::Undefined) {
                type_         = ValueType::Array;
                value_.array_ = HAllocator::Allocate(Array<Value>(1));
                *(value_.array_) += Value(ValueType::Null);
            }
        }
    }

    void operator+=(double num) {
        if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array_) += Value(num);
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

    void operator-=(double num) noexcept {
        if (type_ == ValueType::Number) {
            value_.number_ -= num;
        }
    }

    void operator*=(double num) noexcept {
        if (type_ == ValueType::Number) {
            value_.number_ *= num;
        }
    }

    void operator/=(double num) noexcept {
        if (type_ == ValueType::Number) {
            value_.number_ /= num;
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
            *(value_.array_) +=
                (is_true) ? Value(ValueType::True) : Value(ValueType::False);
        } else if (type_ == ValueType::Undefined) {
            type_         = ValueType::Array;
            value_.array_ = HAllocator::Allocate(Array<Value>(1));
            *this += is_true;
        }
    }

    explicit operator double() const noexcept {
        return GetNumber();
    }

    explicit operator const char *() const noexcept {
        return Char();
    }

    explicit operator bool() const noexcept {
        return IsTrue();
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

    Value *GetValue(const char *key, ULong length) const noexcept {
        if (type_ == ValueType::Object) {
            Value *val = (value_.object_->Find(key, length));

            if ((val != nullptr) && !(val->IsUndefined())) {
                return val;
            }
        }

        return nullptr;
    }

    const String *GetKey(ULong index) const noexcept {
        if (type_ == ValueType::Object) {
            return value_.object_->GetKey(index);
        }

        return nullptr;
    }

    HArray<Value> *GetObject() const noexcept {
        if (type_ == ValueType::Object) {
            return value_.object_;
        }

        return nullptr;
    }

    Array<Value> *GetArray() const noexcept {
        if (type_ == ValueType::Array) {
            return value_.array_;
        }

        return nullptr;
    }

    const String *GetString() const noexcept {
        if (type_ == ValueType::String) {
            return value_.string_;
        }

        return nullptr;
    }

    const char *Char() const noexcept {
        if ((type_ == ValueType::String) && (value_.string_ != nullptr)) {
            return value_.string_->Char();
        }

        return nullptr;
    }

    ULong Length() const noexcept {
        if (type_ == ValueType::String && (value_.string_ != nullptr)) {
            return value_.string_->Length();
        }

        return 0;
    }

    bool SetString(String &value) const {
        switch (type_) {
            case ValueType::String: {
                value = *(value_.string_);
                return true;
            }

            case ValueType::Number: {
                value = Digit::NumberToString(value_.number_, 1, 0,
                                              QENTEM_DOUBLE_PRECISION_);
                return true;
            }

            case ValueType::True: {
                value = String("true", QENTEM_TRUE_LEN_);
                return true;
            }

            case ValueType::False: {
                value = String("false", QENTEM_FALSE_LEN_);
                return true;
            }

            case ValueType::Null: {
                value = String("null", QENTEM_NULL_LEN_);
                return true;
            }

            default: {
            }
        }

        value.Clear();
        return false;
    }

    bool InsertString(StringStream &ss) const {
        switch (type_) {
            case ValueType::String: {
                ss += *(value_.string_);
                return true;
            }

            case ValueType::Number: {
                ss += Digit::NumberToString(value_.number_, 1, 0,
                                            QENTEM_DOUBLE_PRECISION_);
                return true;
            }

            case ValueType::True: {
                ss.Add("true", QENTEM_TRUE_LEN_);
                return true;
            }

            case ValueType::False: {
                ss.Add("false", QENTEM_FALSE_LEN_);
                return true;
            }

            case ValueType::Null: {
                ss.Add("null", QENTEM_NULL_LEN_);
                return true;
            }

            default: {
            }
        }

        return false;
    }

    double GetNumber() const noexcept {
        if (type_ == ValueType::Number) {
            return value_.number_;
        }

        return 0;
    }

    bool GetNumber(double &value) const noexcept {
        switch (type_) {
            case ValueType::Number: {
                value = value_.number_;
                return true;
            }

            case ValueType::String: {
                return Digit::StringToNumber(
                    value, value_.string_->Char(),
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
                if (value_.string_->Compare("true", QENTEM_TRUE_LEN_)) {
                    value = true;
                    return true;
                }

                if (value_.string_->Compare("false", QENTEM_FALSE_LEN_)) {
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

    void Delete(ULong index) noexcept {
        if (type_ == ValueType::Object) {
            value_.object_->DeleteIndex(index);
        } else if ((type_ == ValueType::Array) &&
                   (index < value_.array_->Size())) {
            (value_.array_->First() + index)->Clear();
        }
    }

    inline void Delete(const char *key, ULong length) noexcept {
        if (type_ == ValueType::Object) {
            value_.object_->Delete(key, length);
        }
    }

    void Clear() noexcept {
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
                    value_.array_->Clear();
                    return;
                }

                Array<Value> new_array(size);
                Value *      src_val = value_.array_->First();
                const Value *src_end =
                    (value_.array_->Storage() + value_.array_->Size());

                do {
                    if (src_val->type_ != ValueType::Undefined) {
                        new_array += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                *(value_.array_) = static_cast<Array<Value> &&>(new_array);
            }
        }
    }

    void Stringify(StringStream &ss) const {
        const ULong size   = Size();
        const bool  is_obj = (type_ == ValueType::Object);

        const HAItem<Value> *ha_item = nullptr;
        const Value *        item    = nullptr;

        if (is_obj) {
            ha_item = (value_.object_->Storage() - 1);
            ss += '{';
        } else if (type_ == ValueType::Array) {
            item = (value_.array_->Storage() - 1);
            ss += '[';
        } else {
            return;
        }

        if (size != 0) {
            ULong id = 0;

            do {
                if (is_obj) {
                    ++ha_item;

                    if ((ha_item == nullptr) ||
                        (ha_item->Key.Char() == nullptr) ||
                        (ha_item->Value.type_ == ValueType::Undefined)) {
                        continue; // Deleted item.
                    }

                    ss += '"';
                    JSON::EscapeString(ha_item->Key.Char(),
                                       ha_item->Key.Length(), ss);
                    ss += '"';
                    ss += ':';

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
                        ss += '"';
                        JSON::EscapeString(item->value_.string_->Char(),
                                           item->value_.string_->Length(), ss);
                        ss += '"';
                        break;
                    }

                    case ValueType::Number: {
                        ss += Digit::NumberToString(item->value_.number_);
                        break;
                    }

                    case ValueType::False: {
                        ss.Add("false", QENTEM_FALSE_LEN_);
                        break;
                    }

                    case ValueType::True: {
                        ss.Add("true", QENTEM_TRUE_LEN_);
                        break;
                    }

                    case ValueType::Null: {
                        ss.Add("null", QENTEM_NULL_LEN_);
                        break;
                    }

                    default: {
                    }
                }

                ss += ',';
            } while (++id < size);

            if (ss.Char()[(ss.Length() - 1)] == ',') {
                ss.StepBack(1);
            }
        }

        if (is_obj) {
            ss += '}';
        } else {
            ss += ']';
        }
    }

    inline String Stringify() const {
        StringStream ss;
        Stringify(ss);
        return ss.GetString();
    }

  private:
    union Value_U_ {
        HArray<Value> *object_;
        Array<Value> * array_;
        String *       string_;
        double         number_;
    };

    void copyValue(const Value &val) {
        switch (type_) {
            case ValueType::Object: {
                value_.object_ =
                    HAllocator::Allocate(HArray<Value>(*(val.value_.object_)));
                break;
            }

            case ValueType::Array: {
                value_.array_ =
                    HAllocator::Allocate(Array<Value>(*(val.value_.array_)));
                break;
            }

            case ValueType::String: {
                value_.string_ =
                    HAllocator::Allocate(String(*(val.value_.string_)));
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

    Value_U_  value_{nullptr};
    ValueType type_{ValueType::Undefined};
};

} // namespace Qentem

#endif
