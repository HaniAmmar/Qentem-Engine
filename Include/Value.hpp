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

#ifndef QENTEM_VALUE_H
#define QENTEM_VALUE_H

#include "StringStream.hpp"
#include "QNumber.hpp"
#include "Array.hpp"
#include "HArray.hpp"
#include "JSONUtils.hpp"

namespace Qentem {

enum struct ValueType : SizeT8 {
    Undefined = 0,
    Object,
    Array,
    String,
    UIntLong, // unsigned long long
    IntLong,  //          long long
    Double,   //          double
    True,
    False,
    Null
};
///////////////////////////////////////////////
template <typename Char_T>
struct Value {
    using JSONotation = JSONUtils::JSONotation_T<Char_T>;
    using VItem       = HAItem_T<String<Char_T>, Value>;

    using ObjectT = HArray<String<Char_T>, Value>;
    using ArrayT  = Array<Value>;
    using StringT = String<Char_T>;

    Value() noexcept : object_{} {
    }

    ~Value() {
        switch (Type()) {
            case ValueType::Object: {
                Memory::Dispose(&object_);
                break;
            }

            case ValueType::Array: {
                Memory::Dispose(&array_);
                break;
            }

            case ValueType::String: {
                Memory::Dispose(&string_);
                break;
            }

            default: {
            }
        }
    }

    Value(Value &&val) noexcept {
        switch (val.Type()) {
            case ValueType::Object: {
                object_ = Memory::Move(val.object_);
                break;
            }

            case ValueType::Array: {
                array_ = Memory::Move(val.array_);
                break;
            }

            case ValueType::String: {
                string_ = Memory::Move(val.string_);
                break;
            }

            default: {
                number_ = val.number_;
            }
        }

        setType(val.Type());
        val.setTypeToUndefined();
    }

    Value(const Value &val) {
        copyValue(val);
    }

    inline explicit Value(ValueType type) noexcept {
        setType(type);
    }

    inline explicit Value(ValueType type, SizeT size) noexcept {
        switch (type) {
            case ValueType::Array: {
                array_.Reserve(size);
                break;
            }

            case ValueType::Object: {
                object_.Reserve(size);
                break;
            }

            default: {
            }
        }

        setType(type);
    }

    inline explicit Value(ObjectT &&obj) noexcept : object_{Memory::Move(obj)} {
        setTypeToObject();
    }

    inline explicit Value(ArrayT &&arr) noexcept : array_{Memory::Move(arr)} {
        setTypeToArray();
    }

    inline explicit Value(StringT &&str) noexcept : string_{Memory::Move(str)} {
        setTypeToString();
    }

    inline explicit Value(const ObjectT &obj) noexcept : object_{obj} {
        setTypeToObject();
    }

    inline explicit Value(const ArrayT &arr) noexcept : array_{arr} {
        setTypeToArray();
    }

    inline explicit Value(const StringT &str) noexcept : string_{str} {
        setTypeToString();
    }

    inline explicit Value(const Char_T *str, SizeT length) : string_{str, length} {
        setTypeToString();
    }

    inline explicit Value(SizeT64 num) noexcept : number_{num} {
        setTypeToUInt64();
    }

    inline explicit Value(SizeT64I num) noexcept : number_{num} {
        setTypeToInt64();
    }

    inline explicit Value(double num) noexcept : number_{num} {
        setTypeToDouble();
    }

    template <typename Number_T>
    explicit Value(Number_T num) noexcept : number_{num} {
        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            setTypeToUInt64();
        } else {
            setTypeToInt64();
        }
    }

    explicit Value(NullType) noexcept {
        setTypeToNull();
    }

    explicit Value(bool bl) noexcept {
        if (bl) {
            setTypeToTrue();
        } else {
            setTypeToFalse();
        }
    }

    Value &operator=(Value &&val) noexcept {
        if (this != &val) {
            const ValueType type = val.Type();

            val.setTypeToUndefined();

            reset();
            setType(type);

            switch (type) {
                case ValueType::Object: {
                    object_ = Memory::Move(val.object_);
                    break;
                }

                case ValueType::Array: {
                    array_ = Memory::Move(val.array_);
                    break;
                }

                case ValueType::String: {
                    string_ = Memory::Move(val.string_);
                    break;
                }

                default: {
                    number_ = val.number_;
                }
            }
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            reset();
            copyValue(val);
        }

        return *this;
    }

    Value &operator=(ValueType type) noexcept {
        setType(type);
        return *this;
    }

    Value &operator=(ObjectT &&obj) noexcept {
        reset();
        object_ = Memory::Move(obj);
        setTypeToObject();
        return *this;
    }

    Value &operator=(const ObjectT &obj) {
        reset();
        object_ = obj;
        setTypeToObject();
        return *this;
    }

    Value &operator=(ArrayT &&arr) noexcept {
        reset();
        array_ = Memory::Move(arr);
        setTypeToArray();
        return *this;
    }

    Value &operator=(const ArrayT &arr) {
        reset();
        array_ = arr;
        setTypeToArray();
        return *this;
    }

    Value &operator=(StringT &&str) noexcept {
        reset();
        string_ = Memory::Move(str);
        setTypeToString();
        return *this;
    }

    Value &operator=(const StringT &str) {
        reset();
        string_ = str;
        setTypeToString();
        return *this;
    }

    Value &operator=(const Char_T *str) {
        reset();
        string_ = StringT{str};
        setTypeToString();
        return *this;
    }

    Value &operator=(SizeT64 num) noexcept {
        reset();
        number_ = num;
        setTypeToUInt64();
        return *this;
    }

    Value &operator=(SizeT64I num) noexcept {
        reset();
        number_ = num;
        setTypeToInt64();
        return *this;
    }

    Value &operator=(double num) noexcept {
        reset();
        number_ = num;
        setTypeToDouble();
        return *this;
    }

    template <typename Number_T>
    Value &operator=(Number_T num) noexcept {
        reset();
        number_ = num;

        if QENTEM_CONST_EXPRESSION (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if QENTEM_CONST_EXPRESSION (IsUnsigned<Number_T>()) {
            setTypeToUInt64();
        } else {
            setTypeToInt64();
        }

        return *this;
    }

    Value &operator=(NullType) noexcept {
        reset();
        setTypeToNull();
        return *this;
    }

    Value &operator=(bool is_true) noexcept {
        reset();

        if (is_true) {
            setTypeToTrue();
        } else {
            setTypeToFalse();
        }

        return *this;
    }

    inline void operator+=(Value &&val) {
        if (IsObject() && val.IsObject()) {
            object_ += Memory::Move(val.object_);
            val.setTypeToUndefined();
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            array_ += Memory::Move(val);
        }
    }

    inline void operator+=(const Value &val) {
        if (IsObject() && val.IsObject()) {
            object_ += val.object_;
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            array_ += val;
        }
    }

    inline void operator+=(ObjectT &&obj) {
        if (IsObject()) {
            object_ += Memory::Move(obj);
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            array_ += Value{Memory::Move(obj)};
        }
    }

    inline void operator+=(const ObjectT &obj) {
        *this += ObjectT(obj);
    }

    inline void operator+=(ArrayT &&arr) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        if (arr.Size() != SizeT{0}) {
            array_ += Memory::Move(arr);
        } else {
            array_ += Value{Memory::Move(arr)};
        }
    }

    inline void operator+=(const ArrayT &arr) {
        (*this) += ArrayT(arr);
    }

    inline void operator+=(StringT &&str) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{Memory::Move(str)};
    }

    inline void operator+=(const StringT &str) {
        *this += StringT(str);
    }

    inline void operator+=(const Char_T *str) {
        *this += StringT(str);
    }

    template <typename Number_T>
    inline void operator+=(Number_T num) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{num};
    }

    inline void operator+=(NullType) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{nullptr};
    }

    inline void operator+=(bool is_true) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{is_true};
    }

    template <typename Stream_T>
    friend Stream_T &operator<<(Stream_T &out, const Value &value) {
        out << value.Stringify();
        return out;
    }

    inline Value &operator[](const Char_T *key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_[key]);
    }

    inline Value &operator[](const StringView<Char_T> &key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(key.First(), key.Length()));
    }

    inline Value &operator[](StringT &&key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_[Memory::Move(key)]);
    }

    inline Value &operator[](const StringT &key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_[key]);
    }

    Value &operator[](SizeT index) {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            if (array_.Size() > index) {
                return (array_.Storage()[index]);
            }
        } else {
            if (type == ValueType::Object) {
                Value *val = object_.GetValue(index);

                if (val != nullptr) {
                    return *val;
                }
            }

            reset();
            setTypeToArray();
        }

        if (array_.Size() == index) {
            array_ += Value{};
        } else {
            array_.ResizeAndInitialize(index + SizeT{1});
        }

        return (array_.Storage()[index]);
    }

    template <typename Type_T>
    Value &operator[](Type_T index) {
        return (*this)[SizeT(index)];
    }

    // Will insert the key if it does not exist.
    inline Value &Get(const Char_T *key, SizeT length) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(key, length));
    }

    inline Value &Get(const StringView<Char_T> &key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(key.First(), key.Length()));
    }

    inline bool operator<(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (object_.Size() < val.object_.Size());
                }

                case ValueType::Array: {
                    return (array_.Size() < val.array_.Size());
                }

                case ValueType::String: {
                    return (string_ < val.string_);
                }

                case ValueType::UIntLong: {
                    return (number_.Natural < val.number_.Natural);
                }

                case ValueType::IntLong: {
                    return (number_.Integer < val.number_.Integer);
                }

                case ValueType::Double: {
                    return (number_.Real < val.number_.Real);
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return false;
                }
            }
        }

        return (type < val.Type());
    }

    inline bool operator>(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (object_.Size() > val.object_.Size());
                }

                case ValueType::Array: {
                    return (array_.Size() > val.array_.Size());
                }

                case ValueType::String: {
                    return (string_ > val.string_);
                }

                case ValueType::UIntLong: {
                    return (number_.Natural > val.number_.Natural);
                }

                case ValueType::IntLong: {
                    return (number_.Integer > val.number_.Integer);
                }

                case ValueType::Double: {
                    return (number_.Real > val.number_.Real);
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return false;
                }
            }
        }

        return (type > val.Type());
    }

    inline bool operator<=(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (object_.Size() <= val.object_.Size());
                }

                case ValueType::Array: {
                    return (array_.Size() <= val.array_.Size());
                }

                case ValueType::String: {
                    return (string_ <= val.string_);
                }

                case ValueType::UIntLong: {
                    return (number_.Natural <= val.number_.Natural);
                }

                case ValueType::IntLong: {
                    return (number_.Integer <= val.number_.Integer);
                }

                case ValueType::Double: {
                    return (number_.Real <= val.number_.Real);
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        }

        return (type < val.Type());
    }

    inline bool operator>=(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (object_.Size() >= val.object_.Size());
                }

                case ValueType::Array: {
                    return (array_.Size() >= val.array_.Size());
                }

                case ValueType::String: {
                    return (string_ >= val.string_);
                }

                case ValueType::UIntLong: {
                    return (number_.Natural >= val.number_.Natural);
                }

                case ValueType::IntLong: {
                    return (number_.Integer >= val.number_.Integer);
                }

                case ValueType::Double: {
                    return (number_.Real >= val.number_.Real);
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        }

        return (type > val.Type());
    }

    inline bool operator==(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (object_.Size() == val.object_.Size());
                }

                case ValueType::Array: {
                    return (array_.Size() == val.array_.Size());
                }

                case ValueType::String: {
                    return (string_ == val.string_);
                }

                case ValueType::UIntLong: {
                    return (number_.Natural == val.number_.Natural);
                }

                case ValueType::IntLong: {
                    return (number_.Integer == val.number_.Integer);
                }

                case ValueType::Double: {
                    return (number_.Real == val.number_.Real);
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        }

        return (type > val.Type());
    }

    inline bool IsNumber() const noexcept {
        switch (Type()) {
            case ValueType::UIntLong:
            case ValueType::IntLong:
            case ValueType::Double: {
                return true;
            }

            default: {
                return false;
            }
        }
    }

    inline QNumberType GetNumberType() const noexcept {
        switch (Type()) {
            case ValueType::UIntLong: {
                // unsigned number.
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                // signed number.
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                // float number.
                return QNumberType::Real;
            }

            default: {
                // NAN: Not A Number
                return QNumberType::NotANumber;
            }
        }
    }

    inline bool IsUndefined() const noexcept {
        return (Type() == ValueType::Undefined);
    }

    inline bool IsObject() const noexcept {
        return (Type() == ValueType::Object);
    }

    inline bool IsArray() const noexcept {
        return (Type() == ValueType::Array);
    }

    inline bool IsString() const noexcept {
        return (Type() == ValueType::String);
    }

    inline bool IsUInt64() const noexcept {
        return (Type() == ValueType::UIntLong);
    }

    inline bool IsInt64() const noexcept {
        return (Type() == ValueType::IntLong);
    }

    inline bool IsDouble() const noexcept {
        return (Type() == ValueType::Double);
    }

    inline bool IsTrue() const noexcept {
        return (Type() == ValueType::True);
    }

    inline bool IsFalse() const noexcept {
        return (Type() == ValueType::False);
    }

    inline bool IsNull() const noexcept {
        return (Type() == ValueType::Null);
    }

    SizeT Size() const noexcept {
        if (IsObject()) {
            return (object_.Size());
        }

        if (IsArray()) {
            return (array_.Size());
        }

        return SizeT{0};
    }

    void Merge(Value &&val) {
        if (IsUndefined()) {
            setTypeToArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val.array_.Storage();
            const Value *end     = val.array_.End();

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    array_ += Memory::Move(*src_val);
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            object_ += Memory::Move(val.object_);
        }

        val.Reset();
    }

    void Merge(const Value &val) {
        if (IsUndefined()) {
            setTypeToArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val.array_.Storage();
            const Value *end     = val.array_.End();

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    array_ += *src_val;
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            object_ += val.object_;
        }
    }

    Value *GetValue(SizeT index) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = object_.GetValue(index);

                if ((val != nullptr) && (!(val->IsUndefined()))) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                if (index < array_.Size()) {
                    Value *val = (array_.Storage() + index);

                    if (!(val->IsUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

            default:
                return nullptr;
        }
    }

    Value *GetValue(const Char_T *key, SizeT length) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = object_.GetValue(key, length);

                if ((val != nullptr) && !(val->IsUndefined())) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                SizeT index;
                Digit::FastStringToNumber(index, key, length);

                if (index < array_.Size()) {
                    Value *val = (array_.Storage() + index);

                    if (!(val->IsUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

            default:
                return nullptr;
        }
    }

    Value *GetValue(const StringView<Char_T> &key) const noexcept {
        return GetValue(key.First(), key.Length());
    }

    Value *Storage() {
        switch (Type()) {
            case ValueType::Object: {
                VItem *item = object_.Storage();

                if (item != nullptr) {
                    return &(item->Value);
                }
            }

            case ValueType::Array: {
                return array_.Storage();
            }

            default:
                return nullptr;
        }
    }

    const Value *First() const {
        switch (Type()) {
            case ValueType::Object: {
                const VItem *item = object_.First();

                if (item != nullptr) {
                    return &(item->Value);
                }

                return nullptr;
            }

            case ValueType::Array: {
                return array_.First();
            }

            default:
                return nullptr;
        }
    }

    Value *Last() const {
        switch (Type()) {
            case ValueType::Object: {
                VItem *item = object_.Last();

                if (item != nullptr) {
                    return &(item->Value);
                }

                return nullptr;
            }

            case ValueType::Array: {
                return array_.Last();
            }

            default:
                return nullptr;
        }
    }

    const Value *End() const {
        switch (Type()) {
            case ValueType::Object: {
                VItem *item = object_.End();

                if (item != nullptr) {
                    return &(item->Value);
                }
            }

            case ValueType::Array: {
                return array_.End();
            }

            default:
                return nullptr;
        }
    }

    const StringT *GetKey(SizeT index) const noexcept {
        if (IsObject()) {
            return (object_.GetKey(index));
        }

        return nullptr;
    }

    const ObjectT *GetObject() const noexcept {
        if (IsObject()) {
            return &(object_);
        }

        return nullptr;
    }

    ObjectT *GetObject() noexcept {
        if (IsObject()) {
            return &(object_);
        }

        return nullptr;
    }

    const ArrayT *GetArray() const noexcept {
        if (IsArray()) {
            return &(array_);
        }

        return nullptr;
    }

    ArrayT *GetArray() noexcept {
        if (IsArray()) {
            return &(array_);
        }

        return nullptr;
    }

    const StringT *GetString() const noexcept {
        if (IsString()) {
            return &(string_);
        }

        return nullptr;
    }

    StringT *GetString() noexcept {
        if (IsString()) {
            return &(string_);
        }

        return nullptr;
    }

    const Char_T *StringStorage() const noexcept {
        if (IsString()) {
            return (string_.First());
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        if (IsString()) {
            return (string_.Length());
        }

        return SizeT{0};
    }

    // To get a pointer to a key and its length.
    template <typename Number_T>
    bool SetKeyCharAndLength(SizeT index, const Char_T *&key, Number_T &length) const noexcept {
        const StringT *val = GetKey(index);

        if (val != nullptr) {
            key    = val->First();
            length = Number_T(val->Length());
            return true;
        }

        return false;
    }

    template <typename Number_T>
    void SetValueKeyLength(SizeT index, const Value *&value, const Char_T *&key, Number_T &length) const noexcept {
        if (IsObject()) {
            const VItem *item = object_.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.IsUndefined())) {
                value  = &(item->Value);
                key    = item->Key.First();
                length = Number_T(item->Key.Length());
            }
        }
    }

    void SetValueAndKey(SizeT index, const Value *&value, StringView<Char_T> &key) const noexcept {
        if (IsObject()) {
            const VItem *item = object_.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.IsUndefined())) {
                value = &(item->Value);
                key   = StringView<Char_T>{item->Key.First(), item->Key.Length()};
            }
        }
    }

    // To get a pointer to a string value and its length.
    template <typename Number_T>
    bool SetCharAndLength(const Char_T *&key, Number_T &length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                key    = string_.First();
                length = Number_T{string_.Length()};
                return true;
            }

            case ValueType::True: {
                key    = JSONotation::TrueString;
                length = JSONotation::TrueStringLength;
                return true;
            }

            case ValueType::False: {
                key    = JSONotation::FalseString;
                length = JSONotation::FalseStringLength;
                return true;
            }

            case ValueType::Null: {
                key    = JSONotation::NullString;
                length = JSONotation::NullStringLength;
                return true;
            }

            default: {
                return false;
            }
        }
    }

    template <typename StringStream_T>
    using CopyValueToStringFunction_T = void(StringStream_T, const Char_T *, SizeT);

    template <typename StringStream_T, typename StringFunction_T = CopyValueToStringFunction_T<StringStream_T>>
    bool CopyValueTo(StringStream_T             &stream,
                     const Digit::RealFormatInfo format          = Digit::RealFormatInfo{Config::DoublePrecision},
                     StringFunction_T           *string_function = nullptr) const {
        switch (Type()) {
            case ValueType::String: {
                if (string_function != nullptr) {
                    string_function(stream, string_.First(), string_.Length());
                } else {
                    stream.Write(string_.First(), string_.Length());
                }

                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, number_.Natural);
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, number_.Integer);
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, number_.Real, format);
                break;
            }

            case ValueType::True: {
                stream.Write(JSONotation::TrueString, JSONotation::TrueStringLength);
                break;
            }

            case ValueType::False: {
                stream.Write(JSONotation::FalseString, JSONotation::FalseStringLength);
                break;
            }

            case ValueType::Null: {
                stream.Write(JSONotation::NullString, JSONotation::NullStringLength);
                break;
            }

            default: {
                return false;
            }
        }

        return true;
    }

    template <typename StringStream_T>
    bool CopyKeyByIndexTo(StringStream_T &stream, SizeT index) const {
        if (IsObject()) {
            const StringT *key = object_.GetKey(index);

            if (key != nullptr) {
                stream += *key;
            }

            return true;
        }

        return false;
    }

    SizeT64 GetUInt64() const noexcept {
        QNumber64 number;

        switch (SetNumber(number)) {
            case QNumberType::Natural:
            case QNumberType::Integer: {
                return number.Natural;
            }

            case QNumberType::Real: {
                return QNumber64{SizeT64I(number.Real)}.Natural;
            }

            default: {
                return SizeT64{0};
            }
        };
    }

    SizeT64I GetInt64() const noexcept {
        QNumber64 number;

        switch (SetNumber(number)) {
            case QNumberType::Natural:
            case QNumberType::Integer: {
                return number.Integer;
            }

            case QNumberType::Real: {
                return SizeT64I(number.Real);
            }

            default: {
                return SizeT64I{0};
            }
        };
    }

    double GetDouble() const noexcept {
        QNumber64 number;

        switch (SetNumber(number)) {
            case QNumberType::Natural: {
                return double(number.Natural);
            }

            case QNumberType::Integer: {
                return double(number.Integer);
            }

            case QNumberType::Real: {
                return number.Real;
            }

            default: {
                return 0.0;
            }
        };
    }

    double GetNumber() const noexcept {
        return GetDouble();
    }

    QNumberType SetNumber(QNumber64 &number) const noexcept {
        switch (Type()) {
            case ValueType::UIntLong: {
                number.Natural = (number_.Natural);
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                number.Integer = (number_.Integer);
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                number.Real = (number_.Real);
                return QNumberType::Real;
            }

            case ValueType::True: {
                number.Natural = SizeT64{1};
                return QNumberType::Natural;
            }

            case ValueType::False:
            case ValueType::Null: {
                number.Natural = SizeT64{0};
                return QNumberType::Natural;
            }

            case ValueType::String: {
                SizeT             offset = 0;
                const QNumberType n_type = Digit::StringToNumber(number, string_.First(), offset, string_.Length());

                if (offset == string_.Length()) {
                    return n_type;
                }
            }

            default: {
            }
        }

        return QNumberType::NotANumber;
    }

    bool GetBool(bool &value) const noexcept {
        switch (Type()) {
            case ValueType::True: {
                value = true;
                return true;
            }

            case ValueType::False:
            case ValueType::Null: {
                value = false;
                return true;
            }

            case ValueType::UIntLong: {
                value = (number_.Natural > 0);
                return true;
            }

            case ValueType::IntLong: {
                value = (number_.Integer > 0);
                return true;
            }

            case ValueType::Double: {
                value = (number_.Real > 0);
                return true;
            }

            case ValueType::String: {
                if (string_.IsEqual(JSONotation::TrueString, JSONotation::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (string_.IsEqual(JSONotation::FalseString, JSONotation::FalseStringLength)) {
                    value = false;
                    return true;
                }
            }

            default: {
            }
        }

        return false;
    }

    inline void Remove(const Char_T *key, SizeT length) const noexcept {
        if (IsObject()) {
            object_.Remove(key, length);
        }
    }

    inline void Remove(const StringT &key) const noexcept {
        Remove(key.First(), string_.Length());
    }

    inline void Remove(const Char_T *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (IsObject()) {
            object_.RemoveIndex(index);
        } else if (IsArray() && (index < array_.Size())) {
            (array_.Storage() + index)->Reset();
        }
    }

    template <typename Number_T>
    inline void RemoveIndex(Number_T index) const noexcept {
        RemoveIndex(SizeT(index));
    }

    void Reset() noexcept {
        reset();
        setTypeToUndefined();
    }

    void Compress() {
        if (IsArray()) {
            Value       *src_val = array_.Storage();
            const Value *src_end = array_.End();
            SizeT        size    = 0;

            while (src_val < src_end) {
                if (!(src_val->IsUndefined())) {
                    ++size;
                }

                ++src_val;
            }

            if (size != array_.Capacity()) {
                if (size == SizeT{0}) {
                    array_.Reset();
                    return;
                }

                ArrayT new_array{size};
                src_val = array_.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += Memory::Move(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                array_ = Memory::Move(new_array);
            }

            src_val = array_.Storage();
            src_end = array_.End();

            while (src_val < src_end) {
                if (src_val->IsArray() || src_val->IsObject()) {
                    src_val->Compress();
                }

                ++src_val;
            }
        } else if (IsObject()) {
            object_.Compress();

            VItem       *src_val = object_.Storage();
            const VItem *src_end = (src_val + object_.Size());

            while (src_val < src_end) {
                if (src_val->Value.IsArray() || src_val->Value.IsObject()) {
                    src_val->Value.Compress();
                }

                ++src_val;
            }
        }
    }

    inline ValueType Type() const noexcept {
        return type_;
    }

    bool GroupBy(Value &groupedValue, const Char_T *key, const SizeT length) const {
        StringStream<Char_T> stream;
        ObjectT              new_sub_obj;
        const Char_T        *str     = nullptr;
        SizeT                str_len = 0;
        SizeT                grouped_key_index;

        if (IsArray()) {
            const Value *item_ = array_.First();
            groupedValue.reset();
            groupedValue.setTypeToObject();

            if ((item_ != nullptr) && item_->IsObject() && item_->object_.GetKeyIndex(grouped_key_index, key, length)) {
                const Value *end = array_.End();

                while (item_ != end) {
                    if ((item_ != nullptr) && item_->IsObject()) {
                        SizeT count = 0;

                        const VItem *obj_item = item_->object_.First();
                        const VItem *obj_end  = item_->object_.End();

                        while (obj_item != obj_end) {
                            if ((obj_item != nullptr) && !(obj_item->Value.IsUndefined())) {
                                if (count != grouped_key_index) {
                                    new_sub_obj[obj_item->Key] = obj_item->Value;
                                } else if (!(obj_item->Value.SetCharAndLength(str, str_len))) {
                                    stream.Clear();

                                    if (obj_item->Value.CopyValueTo(stream)) {
                                        str     = stream.First();
                                        str_len = stream.Length();
                                    } else {
                                        return false;
                                    }
                                }

                                ++count;
                                ++obj_item;
                                continue;
                            }

                            return false;
                        }

                        groupedValue.object_.Get(str, str_len) += Memory::Move(new_sub_obj);

                        ++item_;
                        continue;
                    }

                    return false;
                }

                return true;
            }
        }

        return false;
    }

    bool GroupBy(Value &groupedValue, const Char_T *key) const {
        return GroupBy(groupedValue, key, StringUtils::Count(key));
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            object_.Sort(ascend);
        } else if (type == ValueType::Array) {
            array_.Sort(ascend);
        }
    }

    template <typename Stream_T>
    inline Stream_T &Stringify(Stream_T &stream, SizeT32 precision = Config::DoublePrecision) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            stringifyObject(object_, stream, precision);
        } else if (type == ValueType::Array) {
            stringifyArray(array_, stream, precision);
        }

        return stream;
    }

    inline StringT Stringify(SizeT32 precision = Config::DoublePrecision) const {
        StringStream<Char_T> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename Stream_T>
    static void stringifyObject(const ObjectT &obj, Stream_T &stream, SizeT32 precision) {
        stream += JSONotation::SCurlyChar;

        const VItem *h_item = obj.First();
        const VItem *end    = (h_item + obj.Size());

        while (h_item != end) {
            if ((h_item != nullptr) && !(h_item->Value.IsUndefined())) {
                stream += JSONotation::QuoteChar;
                JSONUtils::EscapeJSON(h_item->Key.First(), h_item->Key.Length(), stream);
                stream += JSONotation::QuoteChar;
                stream += JSONotation::ColonChar;

                stringifyValue(h_item->Value, stream, precision);
                stream += JSONotation::CommaChar;
            }

            ++h_item;
        }

        Char_T *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ECurlyChar;
        } else {
            stream += JSONotation::ECurlyChar;
        }
    }

    template <typename Stream_T>
    static void stringifyArray(const ArrayT &arr, Stream_T &stream, SizeT32 precision) {
        stream += JSONotation::SSquareChar;

        const Value *item = arr.First();
        const Value *end  = arr.End();

        while (item != end) {
            if (!(item->IsUndefined())) {
                stringifyValue(*item, stream, precision);
                stream += JSONotation::CommaChar;
            }

            ++item;
        }

        Char_T *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ESquareChar;
        } else {
            stream += JSONotation::ESquareChar;
        }
    }

    template <typename Stream_T>
    static void stringifyValue(const Value &val, Stream_T &stream, SizeT32 precision) {
        switch (val.Type()) {
            case ValueType::Object: {
                stringifyObject(val.object_, stream, precision);
                break;
            }

            case ValueType::Array: {
                stringifyArray(val.array_, stream, precision);
                break;
            }

            case ValueType::String: {
                stream += JSONotation::QuoteChar;
                JSONUtils::EscapeJSON(val.string_.First(), val.string_.Length(), stream);
                stream += JSONotation::QuoteChar;
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, val.number_.Natural);
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, val.number_.Integer);
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, val.number_.Real, precision);
                break;
            }

            case ValueType::False: {
                stream.Write(JSONotation::FalseString, JSONotation::FalseStringLength);
                break;
            }

            case ValueType::True: {
                stream.Write(JSONotation::TrueString, JSONotation::TrueStringLength);
                break;
            }

            case ValueType::Null: {
                stream.Write(JSONotation::NullString, JSONotation::NullStringLength);
                break;
            }

            default: {
            }
        }
    }

    inline void setType(ValueType type) noexcept {
        type_ = type;
    }

    inline void setTypeToUndefined() noexcept {
        setType(ValueType::Undefined);
    }

    inline void setTypeToObject() noexcept {
        setType(ValueType::Object);
    }

    inline void setTypeToArray() noexcept {
        setType(ValueType::Array);
    }

    inline void setTypeToString() noexcept {
        setType(ValueType::String);
    }

    inline void setTypeToUInt64() noexcept {
        setType(ValueType::UIntLong);
    }

    inline void setTypeToInt64() noexcept {
        setType(ValueType::IntLong);
    }

    inline void setTypeToDouble() noexcept {
        setType(ValueType::Double);
    }

    inline void setTypeToTrue() noexcept {
        setType(ValueType::True);
    }

    inline void setTypeToFalse() noexcept {
        setType(ValueType::False);
    }

    inline void setTypeToNull() noexcept {
        setType(ValueType::Null);
    }

    void reset() noexcept {
        switch (Type()) {
            case ValueType::Object: {
                object_.Reset();
                break;
            }

            case ValueType::Array: {
                array_.Reset();
                break;
            }

            case ValueType::String: {
                string_.Reset();
                break;
            }

            default: {
                number_.Natural = SizeT64{0};
            }
        }
    }

    void copyValue(const Value &val) {
        switch (val.Type()) {
            case ValueType::Object: {
                object_ = val.object_;
                break;
            }

            case ValueType::Array: {
                array_ = val.array_;
                break;
            }

            case ValueType::String: {
                string_ = val.string_;
                break;
            }

            default: {
                number_ = val.number_;
            }
        }

        setType(val.Type());
    }

    union {
        ObjectT   object_{};
        ArrayT    array_;
        StringT   string_;
        QNumber64 number_;
    };

    ValueType type_{ValueType::Undefined};
};

} // namespace Qentem

#endif
