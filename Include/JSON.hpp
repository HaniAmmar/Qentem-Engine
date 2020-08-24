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
#include "Engine.hpp"
#include "HArray.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_JSON_H_
#define QENTEM_JSON_H_

namespace Qentem {
namespace JSON {

static char *UnEscapeString(const char *content, ULong &length) {
    UInt  offset  = 0;
    UInt  offset2 = 0;
    char *str     = HAllocator::Allocate<char>(length + 1);

    while (offset < length) {
        switch (content[offset]) {
            case '\\': {
                ++offset;

                switch (content[offset]) {
                    case '"': {
                        str[offset2] = '"';
                        break;
                    }

                    case '\\': {
                        str[offset2] = '\\';
                        break;
                    }

                    case '/': {
                        str[offset2] = '/';
                        break;
                    }

                    case 'b': {
                        str[offset2] = '\b';
                        break;
                    }

                    case 'f': {
                        str[offset2] = '\f';
                        break;
                    }

                    case 'n': {
                        str[offset2] = '\n';
                        break;
                    }
                    case 'r': {
                        str[offset2] = '\r';
                        break;
                    }

                    case 't': {
                        str[offset2] = '\t';
                        break;
                    }

                    case 'u': {
                        ++offset;

                        if ((length - offset) < 4) {
                            HAllocator::Deallocate(str);
                            return nullptr;
                        }

                        UInt code =
                            Digit::HexStringToNumber((content + offset), 4);
                        offset += 4;

                        if ((code >> 8U) == 0xD8U) {
                            // Surrogate
                            if ((length - offset) < 6) {
                                HAllocator::Deallocate(str);
                                return nullptr;
                            }

                            code = (code ^ 0xD800U) << 10U;
                            offset += 2;
                            code += Digit::HexStringToNumber((content + offset),
                                                             4) &
                                    0x3FFU;
                            code += 0x10000U;

                            offset2 += String::ToUTF8(code, &(str[offset2]));
                            offset += 4;
                        } else {
                            offset2 += String::ToUTF8(code, &(str[offset2]));
                        }

                        continue;
                    }

                    default: {
                        HAllocator::Deallocate(str);
                        return nullptr;
                    }
                }

                break;
            }

            case '\n':
            case '\t': {
                HAllocator::Deallocate(str);
                return nullptr;
            }

            default: {
                str[offset2] = content[offset];
            }
        }

        ++offset;
        ++offset2;
    }

    str[offset2] = '\0';
    length       = offset2;

    return str;
}

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
                }

                break;
            }
        }

        ++offset;
    }

    if (offset > offset2) {
        ss.Add((content + offset2), (offset - offset2));
    }
}

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
    union value_ptr;

  public:
    Value() = default;

    Value(Value &&src) noexcept : type_(src.type_), value_(src.value_) {
        src.type_ = ValueType::Undefined; // Prevents deletion.
    }

    Value(const Value &src) {
        if (src.type_ != ValueType::Undefined) {
            type_ = src.type_;
            copyValue(src);
        }
    }

    explicit Value(ValueType type) noexcept {
        switch (type) {
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = type;
                break;
            }

            default: {
                // The rest needs a value.
            }
        }
    }

    explicit Value(HArray<Value> *obj) noexcept {
        type_         = ValueType::Object;
        value_.object = obj;
    }

    explicit Value(Array<Value> *arr) noexcept {
        type_        = ValueType::Array;
        value_.array = arr;
    }

    explicit Value(String *str) noexcept {
        type_         = ValueType::String;
        value_.string = str;
    }

    explicit Value(double value) noexcept {
        type_         = ValueType::Number;
        value_.number = value;
    }

    ~Value() {
        if (type_ != ValueType::Undefined) {
            Clear();
        }
    }

    Value &operator[](ULong index) {
        switch (type_) {
            case ValueType::Array: {
                if (value_.array->Size() > index) {
                    return *(value_.array->First() + index);
                }

                if (value_.array->Size() == index) {
                    *(value_.array) += Value();
                    return *(value_.array->First() + index);
                }

                break;
            }

            case ValueType::Object: {
                Value *val = value_.object->GetValue(index);

                if (val != nullptr) {
                    return (*val);
                }

                break;
            }

            case ValueType::Undefined: {
                if (index == 0) {
                    type_        = ValueType::Array;
                    value_.array = HAllocator::AllocateClear<Array<Value>>(1);
                    value_.array->ResizeAndInitialize(1);
                    return *(value_.array->First());
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
                return (*(value_.object))[static_cast<String &&>(key)];
            }

            case ValueType::Undefined: {
                type_         = ValueType::Object;
                value_.object = HAllocator::Allocate(HArray<Value>(1));
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
                return (*(value_.object))[key];
            }

            case ValueType::Undefined: {
                type_         = ValueType::Object;
                value_.object = HAllocator::Allocate(HArray<Value>(1));
                return (*this)[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator=(Value &&src) noexcept {
        if (this != &src) {
            if (type_ != ValueType::Undefined) {
                Clear();
            }

            type_  = src.type_;
            value_ = src.value_;

            src.type_ = ValueType::Undefined; // Prevent deletion.
        }

        return *this;
    }

    Value &operator=(const Value &src) {
        if (this != &src) {
            if (type_ == src.type_) {
                switch (type_) {
                    case ValueType::Object: {
                        *(value_.object) = *(src.value_.object);
                        break;
                    }

                    case ValueType::Array: {
                        *(value_.array) = *(src.value_.array);
                        break;
                    }

                    case ValueType::String: {
                        *(value_.string) = *(src.value_.string);
                        break;
                    }

                    case ValueType::Number: {
                        value_.number = src.value_.number;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                if (type_ != ValueType::Undefined) {
                    Clear();
                }

                if (src.type_ != ValueType::Undefined) {
                    type_ = src.type_;
                    copyValue(src);
                }
            }
        }

        return *this;
    }

    Value &operator=(HArray<Value> &&obj) {
        switch (type_) {
            case ValueType::Object: {
                *(value_.object) = static_cast<HArray<Value> &&>(obj);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::Object;
                value_.object =
                    HAllocator::Allocate(static_cast<HArray<Value> &&>(obj));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::Object;
                value_.object =
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
                *(value_.array) = static_cast<Array<Value> &&>(arr);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::Array;
                value_.array =
                    HAllocator::Allocate(static_cast<Array<Value> &&>(arr));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::Array;
                value_.array =
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
                *(value_.string) = static_cast<String &&>(str);
                break;
            }

            case ValueType::Number:
            case ValueType::Undefined:
            case ValueType::True:
            case ValueType::False:
            case ValueType::Null: {
                type_ = ValueType::String;
                value_.string =
                    HAllocator::Allocate(static_cast<String &&>(str));
                break;
            }

            default: {
                Clear();
                type_ = ValueType::String;
                value_.string =
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

    Value &operator=(double num) {
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

        value_.number = num;
        return *this;
    }

    inline Value &operator=(ULong num) {
        *this = static_cast<double>(num);
        return *this;
    }

    inline Value &operator=(int num) {
        *this = static_cast<double>(num);
        return *this;
    }

    inline Value &operator=(UInt num) {
        *this = static_cast<double>(num);
        return *this;
    }

    Value &operator=(bool is_true) {
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

    void operator+=(Value &&src) {
        if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                if (src.type_ == ValueType::Array) {
                    Array<Value> &src_arr = *(src.value_.array);
                    Array<Value> &des_arr = *(value_.array);

                    Value *      src_val = src_arr.First();
                    const Value *src_end = (src_arr.Storage() + src_arr.Size());

                    while (src_val < src_end) {
                        if (src_val->type_ != ValueType::Undefined) {
                            des_arr += static_cast<Value &&>(*src_val);
                        }

                        ++src_val;
                    }
                } else if (src.type_ != ValueType::Undefined) {
                    *(value_.array) += static_cast<Value &&>(src);
                }

                break;
            }

            case ValueType::Object: {
                if (src.type_ == ValueType::Object) {
                    *(value_.object) +=
                        static_cast<HArray<Value> &&>(*(src.value_.object));
                }

                break;
            }

            default: {
            }
        }

        src.Clear();
    }

    void operator+=(const Value &src) {
        if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                if (src.type_ == ValueType::Array) {
                    Array<Value> &src_arr = *(src.value_.array);
                    Array<Value> &des_arr = *(value_.array);

                    const Value *src_val = src_arr.Storage();
                    const Value *src_end = (src_arr.Storage() + src_arr.Size());

                    while (src_val < src_end) {
                        if (src_val->type_ != ValueType::Undefined) {
                            des_arr += *src_val;
                        }

                        ++src_val;
                    }
                } else if (src.type_ != ValueType::Undefined) {
                    *(value_.array) += src;
                }

                break;
            }

            case ValueType::Object: {
                if (src.type_ == ValueType::Object) {
                    *(value_.object) += *(src.value_.object);
                }

                break;
            }

            default: {
            }
        }
    }

    void operator+=(HArray<Value> &&obj) {
        if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array) += Value(
                    HAllocator::Allocate(static_cast<HArray<Value> &&>(obj)));
                break;
            }

            case ValueType::Object: {
                *(value_.object) += static_cast<HArray<Value> &&>(obj);
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
            *(value_.array) += static_cast<Array<Value> &&>(arr);
        } else if (type_ == ValueType::Undefined) {
            type_ = ValueType::Array;
            value_.array =
                HAllocator::Allocate(static_cast<Array<Value> &&>(arr));
        }
    }

    void operator+=(const Array<Value> &arr) {
        (*this) += Array<Value>(arr);
    }

    void operator+=(String &&string) {
        if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array) +=
                    Value(HAllocator::Allocate(static_cast<String &&>(string)));
                break;
            }

            case ValueType::String: {
                *(value_.string) += static_cast<String &&>(string);
                break;
            }

            default: {
            }
        }
    }

    void operator+=(const String &string) {
        (*this) += String(string);
    }

    void operator+=(const char *str) {
        if (str != nullptr) {
            *this += String(str);
        } else {
            if (type_ == ValueType::Array) {
                *(value_.array) += Value(ValueType::Null);
            } else if (type_ == ValueType::Undefined) {
                type_        = ValueType::Array;
                value_.array = HAllocator::Allocate(Array<Value>(1));
                *(value_.array) += Value(ValueType::Null);
            }
        }
    }

    void operator+=(double num) {
        if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
        }

        switch (type_) {
            case ValueType::Array: {
                *(value_.array) += Value(num);
                break;
            }

            case ValueType::Number: {
                value_.number += num;
                break;
            }

            default: {
            }
        }
    }

    void operator-=(double num) {
        if (type_ == ValueType::Number) {
            value_.number -= num;
        }
    }

    void operator*=(double num) {
        if (type_ == ValueType::Number) {
            value_.number *= num;
        }
    }

    void operator/=(double num) {
        if (type_ == ValueType::Number) {
            value_.number /= num;
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
            if (is_true) {
                *(value_.array) += Value(ValueType::True);
            } else {
                *(value_.array) += Value(ValueType::False);
            }
        } else if (type_ == ValueType::Undefined) {
            type_        = ValueType::Array;
            value_.array = HAllocator::Allocate(Array<Value>(1));
            *this += is_true;
        }
    }

    explicit operator double() const {
        return GetNumber();
    }

    explicit operator const char *() const {
        return Char();
    }

    explicit operator bool() const {
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
                return value_.object->Size();
            }

            case ValueType::Array: {
                return value_.array->Size();
            }

            default: {
            }
        }

        return 0;
    }

    Value *GetValue(ULong index) const {
        if (type_ == ValueType::Array) {
            if (index < value_.array->Size()) {
                Value *val = (value_.array->First() + index);

                if (!(val->IsUndefined())) {
                    return val;
                }
            }
        } else if (type_ == ValueType::Object) {
            Value *val = value_.object->GetValue(index);

            if ((val != nullptr) && (!(val->IsUndefined()))) {
                return val;
            }
        }

        return nullptr;
    }

    Value *GetValue(const char *key, ULong length) const {
        if (type_ == ValueType::Object) {
            Value *val = (value_.object->Find(key, length));

            if ((val != nullptr) && !(val->IsUndefined())) {
                return val;
            }
        }

        return nullptr;
    }

    const String *GetKey(ULong index) const {
        if (type_ == ValueType::Object) {
            return value_.object->GetKey(index);
        }

        return nullptr;
    }

    HArray<Value> *GetObject() const noexcept {
        if (type_ == ValueType::Object) {
            return value_.object;
        }

        return nullptr;
    }

    Array<Value> *GetArray() const noexcept {
        if (type_ == ValueType::Array) {
            return value_.array;
        }

        return nullptr;
    }

    const String *GetString() const noexcept {
        if (type_ == ValueType::String) {
            return value_.string;
        }

        return nullptr;
    }

    const char *Char() const noexcept {
        if ((type_ == ValueType::String) && (value_.string != nullptr)) {
            return value_.string->Char();
        }

        return nullptr;
    }

    ULong Length() const noexcept {
        if (type_ == ValueType::String && (value_.string != nullptr)) {
            return value_.string->Length();
        }

        return 0;
    }

    bool SetString(String &value) const {
        switch (type_) {
            case ValueType::String: {
                value = *(value_.string);
                return true;
            }

            case ValueType::Number: {
                value = Digit::NumberToString(value_.number, 1, 0, 14);
                return true;
            }

            case ValueType::True: {
                value = String("true", 4);
                return true;
            }

            case ValueType::False: {
                value = String("false", 5);
                return true;
            }

            case ValueType::Null: {
                value = String("null", 4);
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
                ss += *(value_.string);
                return true;
            }

            case ValueType::Number: {
                ss += Digit::NumberToString(value_.number, 1, 0, 14);
                return true;
            }

            case ValueType::True: {
                ss.Add("true", 4);
                return true;
            }

            case ValueType::False: {
                ss.Add("false", 5);
                return true;
            }

            case ValueType::Null: {
                ss.Add("null", 4);
                return true;
            }

            default: {
            }
        }

        return false;
    }

    double GetNumber() const noexcept {
        if (type_ == ValueType::Number) {
            return value_.number;
        }

        return 0;
    }

    bool GetNumber(double &value) const noexcept {
        switch (type_) {
            case ValueType::Number: {
                value = value_.number;
                return true;
            }

            case ValueType::String: {
                return Digit::StringToNumber(
                    value, value_.string->Char(),
                    static_cast<UInt>(value_.string->Length()));
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
                value = (value_.number > 0);
                return true;
            }

            case ValueType::String: {
                if (value_.string->Compare("true", 4)) {
                    value = true;
                    return true;
                }

                if (value_.string->Compare("false", 5)) {
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

    void Delete(ULong index) {
        if (type_ == ValueType::Object) {
            value_.object->DeleteIndex(index);
        } else if ((type_ == ValueType::Array) &&
                   (index < value_.array->Size())) {
            (value_.array->First() + index)->Clear();
        }
    }

    inline void Delete(const char *key, ULong length) {
        if (type_ == ValueType::Object) {
            value_.object->Delete(key, length);
        }
    }

    void Clear() {
        switch (type_) {
            case ValueType::Object: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.object);
                HAllocator::Deallocate(value_.object);
                value_.object = nullptr;
                break;
            }

            case ValueType::Array: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.array);
                HAllocator::Deallocate(value_.array);
                value_.array = nullptr;
                break;
            }

            case ValueType::String: {
                type_ = ValueType::Undefined;
                HAllocator::Destruct(value_.string);
                HAllocator::Deallocate(value_.string);
                value_.string = nullptr;
                break;
            }

            default: {
                type_ = ValueType::Undefined;
            }
        }
    }

    void Compress() {
        if (type_ == ValueType::Object) {
            value_.object->Compress();
        } else if (type_ == ValueType::Array) {
            const ULong size = value_.array->Size();

            if ((size != 0) && (size != value_.array->Capacity())) {
                Array<Value> new_array(size);
                Value *      src_val = value_.array->First();
                const Value *src_end =
                    (value_.array->Storage() + value_.array->Size());

                do {
                    if (src_val->type_ != ValueType::Undefined) {
                        new_array += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                *(value_.array) = static_cast<Array<Value> &&>(new_array);
            }
        }
    }

    void Stringify(StringStream &ss) const {
        const ULong size   = Size();
        bool        is_obj = (type_ == ValueType::Object);

        const HAItem<Value> *ha_item = nullptr;
        const Value *        item    = nullptr;

        if (is_obj) {
            ha_item = (value_.object->Storage() - 1);
            ss += '{';
        } else if (type_ == ValueType::Array) {
            item = (value_.array->Storage() - 1);
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
                    EscapeString(ha_item->Key.Char(), ha_item->Key.Length(),
                                 ss);
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
                        EscapeString(item->value_.string->Char(),
                                     item->value_.string->Length(), ss);
                        ss += '"';
                        break;
                    }

                    case ValueType::Number: {
                        ss += Digit::NumberToString(item->value_.number);
                        break;
                    }

                    case ValueType::False: {
                        ss.Add("false", 5);
                        break;
                    }

                    case ValueType::True: {
                        ss.Add("true", 4);
                        break;
                    }

                    case ValueType::Null: {
                        ss.Add("null", 4);
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

    friend class JSONParser;

  private:
    union value_ptr {
        HArray<Value> *object;
        Array<Value> * array;
        String *       string;
        double         number;
    };

    void copyValue(const Value &src) {
        switch (type_) {
            case ValueType::Object: {
                value_.object =
                    HAllocator::Allocate(HArray<Value>(*(src.value_.object)));
                break;
            }

            case ValueType::Array: {
                value_.array =
                    HAllocator::Allocate(Array<Value>(*(src.value_.array)));
                break;
            }

            case ValueType::String: {
                value_.string =
                    HAllocator::Allocate(String(*(src.value_.string)));
                break;
            }

            case ValueType::Number: {
                value_.number = src.value_.number;
                break;
            }

            default: {
            }
        }
    }

    ValueType type_{ValueType::Undefined};
    value_ptr value_{nullptr};
};

class JSONParser : Engine {
    enum class Type_;
    struct FindCache_;

  public:
    JSONParser() = default;

    void Parse(Value &value, const char *content, ULong length) {
        ULong offset = 0;

        while (offset != length) {
            switch (content[offset]) {
                case '{':
                case '[': {
                    while (offset != length) {
                        switch (content[--length]) {
                            case ',':
                            case '"': {
                                return;
                            }

                            case '}':
                            case ']': {
                                if (((content[offset] == '{') &&
                                     (content[length] == '}')) ||
                                    ((content[offset] == '[') &&
                                     (content[length] == ']'))) {

                                    if (content[offset] == '{') {
                                        obj_ = HAllocator::AllocateClear<
                                            HArray<Value>>(1);

                                        value = Value(obj_);
                                    } else {
                                        arr_ = HAllocator::AllocateClear<
                                            Array<Value>>(1);
                                        value = Value(arr_);
                                    }

#ifdef QENTEM_SIMD_ENABLED_
                                    FindCache_ fc = FindCache_();
                                    find_cache_   = &fc;
#endif

                                    FindNest(content, ++offset, length, length);

                                    if (!has_error_ && !pass_comma_) {
                                        length -= (offset - 1);
                                        digDeeper_(content, offset, length);
                                    }

                                    if (next_offset_ == 0) {
                                        // Prevent checkPoint2_ form failing on
                                        // an empty objects or arrays
                                        next_offset_ = offset;
                                    }

                                    if (has_error_ ||
                                        (!checkPoint2_(content, length))) {
                                        value.Clear();
                                        return;
                                    }

                                    // value.Compress();
                                }

                                return;
                            }
                        }
                    }
                }
            }

            ++offset;
        }
    }

    //////////// Private ////////////

  private:
    inline bool hasTail() const noexcept final {
        switch (type_) {
            case Type_::Quote:
            case Type_::Curly:
            case Type_::Square: {
                return true;
            }

            default: {
                return false;
            }
        }
    }

    ULong find2(const char *content, ULong offset,
                ULong end_before) noexcept final {
        switch (type_) {
            case Type_::Square: {
                return Find("]", 1, content, offset, end_before);
            }

            case Type_::Curly: {
                return Find("}", 1, content, offset, end_before);
            }

            default: {
#ifdef QENTEM_SIMD_ENABLED_
                // Maybe " is in the cache.
                do {
                    // Remove the current offset.
                    find_cache_->Bits &= (find_cache_->Bits - 1U);

                    if ((find_cache_->Bits == 0) &&
                        (find_cache_->NextOffset < end_before)) {
                        qmm_find_(content, find_cache_->NextOffset, end_before);
                    }

                    ULong index =
                        (Q_CTZL(find_cache_->Bits) + find_cache_->Offset);

                    if ((index >= offset) && (content[index] == '"')) {
                        return (index + 1);
                    }
                } while (find_cache_->Bits != 0);

                return 0;
#else
                return Find("\"", 1, content, offset, end_before);
#endif
            }
        }
    }

#ifdef QENTEM_SIMD_ENABLED_
    void qmm_find_(const char *content, ULong offset,
                   ULong end_before) noexcept {
        do {
            find_cache_->Offset     = offset;
            find_cache_->NextOffset = (find_cache_->Offset + QMM_SIZE_);
            offset                  = find_cache_->NextOffset;

            const QMM_VAR_ m_content =
                QMM_LOAD_(reinterpret_cast<const QMM_VAR_ *>(
                    content + find_cache_->Offset));

            find_cache_->Bits =
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_8_(':'));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_8_('{'));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_8_('['));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_8_('"'));
            find_cache_->Bits |=
                QMM_COMPARE_8_MASK_(m_content, QMM_SETONE_8_(','));
        } while ((find_cache_->Bits == 0) &&
                 (find_cache_->NextOffset < end_before));
    }

    inline ULong find(const char *content, ULong offset,
                      ULong end_before) noexcept final {
        if ((offset > find_cache_->NextOffset) ||
            ((find_cache_->Bits == 0) &&
             (find_cache_->NextOffset < end_before))) {
            qmm_find_(content, offset, end_before);
        }

        ULong index;

        do {
            index = (Q_CTZL(find_cache_->Bits) + find_cache_->Offset);

            if (index >= offset) {
                break;
            }

            // Remove the current offset.
            find_cache_->Bits &= (find_cache_->Bits - 1U);

            if ((find_cache_->Bits == 0) &&
                (find_cache_->NextOffset < end_before)) {
                qmm_find_(content, find_cache_->NextOffset, end_before);
            }
        } while (find_cache_->Bits != 0);

        if ((index < end_before) && (index >= offset)) {
            switch (content[index]) {
                case '"': {
                    type_ = Type_::Quote;
                    break;
                }

                case ',': {
                    type_ = Type_::Comma;
                    break;
                }

                case ':': {
                    type_ = Type_::Colon;
                    break;
                }

                case '{':
                    type_      = Type_::Curly;
                    child_obj_ = HAllocator::AllocateClear<HArray<Value>>(1);
                    break;

                case '[': {
                    type_      = Type_::Square;
                    child_arr_ = HAllocator::AllocateClear<Array<Value>>(1);
                }
            }

            return (index + 1);
        }

        // No match.
        return 0;
    }
#else
    inline ULong find(const char *content, ULong offset,
                      ULong end_before) noexcept final {
        bool found = false;

        while (offset < end_before) {
            switch (content[offset]) {
                case '"': {
                    found = true;
                    type_ = Type_::Quote;
                    break;
                }

                case ',': {
                    found = true;
                    type_ = Type_::Comma;
                    break;
                }

                case ':': {
                    found = true;
                    type_ = Type_::Colon;
                    break;
                }

                case '{':
                    found = true;
                    type_ = Type_::Curly;
                    child_obj_ = HAllocator::AllocateClear<HArray<Value>>(1);
                    break;

                case '[': {
                    found = true;
                    type_ = Type_::Square;
                    child_arr_ = HAllocator::AllocateClear<Array<Value>>(1);
                }
            }

            ++offset;

            if (found) {
                return offset;
            }
        }

        // No match.
        return 0;
    }
#endif

    ULong nest(const char *content, ULong offset, ULong end_before,
               ULong max_end_before) final {
        switch (type_) {
            case Type_::Curly:
            case Type_::Square: {
                JSONParser jp(child_obj_, child_arr_, child_obj_value_,
                              find_cache_, pass_comma_);

                if (next_offset_ >= offset) {
                    // Prevent false error.
                    jp.next_offset_ = next_offset_;
                }

                ULong ret =
                    jp.FindNest(content, offset, end_before, max_end_before);

                pass_comma_ = (ret != 0); // if it has found a value, then the
                                          // next comma needs to be passed.

                if (!(jp.has_error_) && !(pass_comma_) &&
                    ((child_obj_ == nullptr) || jp.has_colon_)) {
                    // if the scan is not done.
                    jp.digDeeper_(content, offset, (end_before - offset));
                }

                if (jp.has_error_) {
                    failed();
                    return max_end_before;
                }

                next_offset_ = jp.next_offset_; // set the current offset.
                child_obj_value_ =
                    jp.obj_value_; // Preserve the child value pointer.

                return ret;
            }

            case Type_::Quote: {
                // TODO: Use Engine::Find

                UInt  times      = 0;
                ULong tmp_offset = (end_before - 1);

                while ((offset < tmp_offset) &&
                       (content[--tmp_offset] == '\\')) {
                    ++times;
                }

                if ((times % 2) != 0) {
                    return end_before;
                }

                break;
            }

            default: {
            }
        }

        return 0;
    }

    // TODO: Most used function
    void Found(const char *content, ULong offset, ULong end_before,
               ULong start_offset, ULong &current_offset) final {
        switch (type_) {
            case Type_::Comma: { // ,
                if (!pass_comma_) {
                    if (!checkPoint_()) {
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    has_colon_ = false;

                    ULong item_offset =
                        (next_offset_ != 0) ? next_offset_ : offset;

                    String::LeftTrim(content, item_offset, start_offset);

                    if (item_offset == start_offset) {
                        // Empty
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    --start_offset;

                    // Right trim.
                    while ((item_offset < --start_offset) &&
                           ((content[start_offset] == ' ') ||
                            (content[start_offset] == '\n') ||
                            (content[start_offset] == '\t') ||
                            (content[start_offset] == '\r'))) {
                    }

                    const UInt len =
                        static_cast<UInt>((start_offset + 1) - item_offset);

                    if (len == 0) {
                        // Empty
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    UInt tmp_offset = 0;

                    switch (content[item_offset]) {
                        case 't': {
                            if (len == 4) {
                                ++item_offset;

                                while (tmp_offset < 3) {
                                    if (content[item_offset] !=
                                        "rue"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 3) {
                                    add_(Value(ValueType::True));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        case 'f': {
                            if (len == 5) {
                                ++item_offset;

                                while (tmp_offset < 4) {
                                    if (content[item_offset] !=
                                        "alse"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 4) {
                                    add_(Value(ValueType::False));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        case 'n': {
                            if (len == 4) {
                                ++item_offset;

                                while (tmp_offset < 3) {
                                    if (content[item_offset] !=
                                        "ull"[tmp_offset]) {
                                        tmp_offset = 0;
                                        break;
                                    }

                                    ++item_offset;
                                    ++tmp_offset;
                                }

                                if (tmp_offset == 3) {
                                    add_(Value(ValueType::Null));
                                    break;
                                }
                            }

                            current_offset = end_before;
                            failed();
                            return;
                        }

                        default: {
                            double num;
                            if (!Digit::StringToNumber(
                                    num, (content + item_offset), len)) {
                                current_offset = end_before;
                                failed();
                                return;
                            }

                            add_(Value(num));
                            break;
                        }
                    }
                } else {
                    // Checking for anything extra.
                    if (!checkPoint2_(content, --start_offset)) {
                        current_offset = end_before;
                        failed();
                        return;
                    }

                    pass_comma_ = false;
                }

                next_offset_ = current_offset;
                break;
            }

            case Type_::Quote: { // ""
                if (!checkPoint3_(content, offset, start_offset)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                next_offset_ = current_offset;

                ULong tmp_length = (current_offset - (start_offset + 1));
                char *str =
                    UnEscapeString((content + start_offset), tmp_length);

                if (str == nullptr) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                if ((obj_ == nullptr) || has_colon_) { // String
                    add_(Value(HAllocator::Allocate(String(str, tmp_length))));

                    has_colon_  = false;
                    pass_comma_ = true;
                } else { // Key
                    obj_value_ = &((*obj_)[String(str, tmp_length)]);
                }

                break;
            }

            case Type_::Curly:    // {}
            case Type_::Square: { // []
                if (!checkPoint_() ||
                    !checkPoint3_(content, offset, start_offset)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                has_colon_   = false;
                pass_comma_  = true;
                next_offset_ = current_offset;

                if (type_ == Type_::Curly) {
                    // child_obj_->Compress();
                    add_(Value(child_obj_));
                    child_obj_ = nullptr;
                } else {
                    // child_arr_->Compress();
                    add_(Value(child_arr_));
                    child_arr_ = nullptr;
                }

                break;
            }

            case Type_::Colon: { // :
                if (has_colon_ || (obj_ == nullptr) ||
                    (obj_value_ == nullptr)) {
                    current_offset = end_before;
                    failed();
                    return;
                }

                has_colon_   = true;
                pass_comma_  = false;
                next_offset_ = start_offset;
                break;
            }

            default: {
            }
        }

        type_ = Type_::None;
    }

    /*
     * Look for anything before ] or } and after comma, as not possable to
     * match if it was a number/false/true/null
     */
    void digDeeper_(const char *content, ULong offset, ULong length) {
        type_ = Type_::Comma;
        --length;

        String::SoftTrim(content, offset, length);

        if (length != 0) {
            ULong fake_comma = ((offset + length) + 1);
            Found(content, offset, fake_comma, fake_comma, fake_comma);
        }
    }

    void add_(Value &&val) noexcept {
        if (obj_ != nullptr) {
            *obj_value_ = static_cast<Value &&>(val);
            obj_value_  = nullptr;
        } else {
            (*arr_) += static_cast<Value &&>(val);
        }
    }

    bool checkPoint_() const noexcept {
        if (obj_ != nullptr) {
            return (has_colon_ && !has_error_);
        }

        return (!has_colon_ && !has_error_);
    }

    bool checkPoint2_(const char *content, ULong offset) noexcept {
        if (next_offset_ < offset) {
            String::LeftTrim(content, next_offset_, offset);

            if (next_offset_ != offset) {
                return false;
            }
        }

        return true;
    }

    // TODO: Improve the second most used function here
    bool checkPoint3_(const char *content, ULong offset,
                      ULong start_offset) const noexcept {
        if (!has_colon_) {
            --start_offset;

            while (start_offset > offset) {
                --start_offset;

                switch (content[start_offset]) {
                    case '\n':
                    case ' ':
                    case '\t':
                        break;

                    case '{':
                    case '[':
                    case ',':
                        return true;

                    default: {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void failed() final {
        if (child_obj_ != nullptr) {
            HAllocator::Destruct(child_obj_);
            HAllocator::Deallocate(child_obj_);
            child_obj_ = nullptr;
        }

        if (child_arr_ != nullptr) {
            HAllocator::Destruct(child_arr_);
            HAllocator::Deallocate(child_arr_);
            child_arr_ = nullptr;
        }

        has_error_ = true;
    }

    JSONParser(HArray<Value> *obj, Array<Value> *arr, Value *obj_value,
               FindCache_ *find_cache, bool pass_comma) noexcept
        : obj_(obj), arr_(arr), obj_value_(obj_value), find_cache_(find_cache),
          pass_comma_(pass_comma) {
    }

    enum class Type_ { None = 0, Curly, Square, Quote, Comma, Colon };

    struct FindCache_ {
        ULong            Offset{0};
        ULong            NextOffset{0};
        QMM_NUMBER_TYPE_ Bits{0};

        ULong            Offset2{0};
        ULong            NextOffset2{0};
        QMM_NUMBER_TYPE_ Bits2{0};
    };

    HArray<Value> *obj_{nullptr};
    Array<Value> * arr_{nullptr};

    HArray<Value> *child_obj_{nullptr};
    Array<Value> * child_arr_{nullptr};
    ULong          next_offset_{0};

    Value *     child_obj_value_{nullptr};
    Value *     obj_value_{nullptr};
    FindCache_ *find_cache_{nullptr};

    Type_ type_{Type_::None};

    bool has_colon_{false};
    bool pass_comma_{false};
    bool has_error_{false};
};

////////////////////////////////////////////////////////////

inline static Value Parse(const char *content, ULong length) {
    Value value;
    JSONParser().Parse(value, content, length);
    return value;
}

inline static Value Parse(const String &content) {
    return Parse(content.Char(), content.Length());
}

inline static Value Parse(const char *content) {
    return Parse(content, String::Count(content));
}

} // namespace JSON
} // namespace Qentem

#endif
