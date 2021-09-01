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
#include "Platform.hpp"

#ifndef QENTEM_VALUE_H_
#define QENTEM_VALUE_H_

namespace Qentem {

enum class ValueType : unsigned char {
    Undefined = 0,
    Object,
    Array,
    String,
    UInt64, // unsigned long long
    Int64,  //          long long
    Double, //          double
    True,
    False,
    Null
};

template <typename Char_T_>
class Value {
    using JSONotation_T_ = JSON::JSONotation<Char_T_>;
    using VObject        = HArray<Value, Char_T_>;
    using VArray         = Array<Value>;
    using VString        = String<Char_T_>;

  public:
    Value() noexcept : number_{} {}

    Value(Value &&val) noexcept : number_{val.number_} {
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
        setType(val.Type());
#endif
        val.setTypeToUndefined();
    }

    Value(const Value &val) { copyValue(val); }

    ~Value() {
        if (!IsUndefined()) {
            reset();
        }
    }

    explicit Value(ValueType type) noexcept {
        switch (type) {
            case ValueType::Object: {
                initObject();
                return;
            }

            case ValueType::Array: {
                initArray();
                return;
            }

            case ValueType::String: {
                initString();
                return;
            }

            default: {
                setType(type);
            }
        }
    }

    explicit Value(VObject &&obj) noexcept
        : object_{static_cast<VObject &&>(obj)} {
        setTypeToObject();
    }

    explicit Value(VArray &&arr) noexcept
        : array_{static_cast<VArray &&>(arr)} {
        setTypeToArray();
    }

    explicit Value(VString &&str) noexcept
        : string_{static_cast<VString &&>(str)} {
        setTypeToString();
    }

    explicit Value(const VObject &obj) noexcept : object_{obj} {
        setTypeToObject();
    }

    explicit Value(const VArray &arr) noexcept : array_{arr} {
        setTypeToArray();
    }

    explicit Value(const VString &str) noexcept : string_{str} {
        setTypeToString();
    }

    explicit Value(const Char_T_ *str, SizeT length) noexcept
        : string_{str, length} {
        setTypeToString();
    }

    explicit Value(unsigned long long num) noexcept : number_{num} {
        setTypeToUInt64();
    }

    explicit Value(long long num) noexcept : number_{num} { setTypeToInt64(); }

    explicit Value(double num) noexcept : number_{num} { setTypeToDouble(); }

    template <typename Number_T_>
    explicit Value(Number_T_ num) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        constexpr bool is_float =
            (static_cast<double>(static_cast<Number_T_>(1.5)) == 1.5);

        if (is_unsigned) {
            number_.SetNumber(static_cast<unsigned long long>(num));
            setTypeToUInt64();
        } else if (!is_float) {
            number_.SetNumber(static_cast<long long>(num));
            setTypeToInt64();
        } else {
            number_.SetNumber(static_cast<double>(num));
            setTypeToDouble();
        }
    }

    explicit Value(NullType) noexcept { setTypeToNull(); }
    explicit Value(bool bl) noexcept {
        if (bl) {
            setTypeToTrue();
        } else {
            setTypeToFalse();
        }
    }

    Value &operator=(Value &&val) noexcept {
        constexpr unsigned long long num = 0;

        if (this != &val) {
            if (!IsUndefined()) {
                reset();
            }

            number_ = val.number_;
            setType(val.Type());

            val.number_ = VNumber{num};
            val.setTypeToUndefined();
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            if (Type() == val.Type()) {
                switch (Type()) {
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

                    case ValueType::UInt64:
                    case ValueType::Int64:
                    case ValueType::Double: {
                        number_ = val.number_;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                reset();

                if (!(val.IsUndefined())) {
                    copyValue(val);
                }

                setType(val.Type());
            }
        }

        return *this;
    }

    Value &operator=(VObject &&obj) {
        if (IsObject()) {
            object_ = static_cast<VObject &&>(obj);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(static_cast<VObject &&>(obj));
        setTypeToObject();
        return *this;
    }

    Value &operator=(const VObject &obj) {
        if (IsObject()) {
            object_ = obj;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(obj);
        setTypeToObject();
        return *this;
    }

    Value &operator=(VArray &&arr) {
        if (IsArray()) {
            array_ = static_cast<VArray &&>(arr);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(static_cast<VArray &&>(arr));
        setTypeToArray();
        return *this;
    }

    Value &operator=(const VArray &arr) {
        if (IsArray()) {
            array_ = arr;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(arr);
        setTypeToArray();
        return *this;
    }

    Value &operator=(VString &&str) {
        if (IsString()) {
            string_ = static_cast<VString &&>(str);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(static_cast<VString &&>(str));
        setTypeToString();
        return *this;
    }

    Value &operator=(const VString &str) {
        if (IsString()) {
            string_ = str;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(str);
        setTypeToString();
        return *this;
    }

    Value &operator=(const Char_T_ *str) {
        if (IsString()) {
            string_ = VString{str};
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(VString{str});
        setTypeToString();
        return *this;
    }

    template <typename Number_T_>
    inline Value &operator=(Number_T_ num) noexcept {
        constexpr bool is_unsigned = (static_cast<Number_T_>(-1) > 0);
        constexpr bool is_float =
            (static_cast<double>(static_cast<Number_T_>(1.5)) == 1.5);

        if (!IsNumber() && !IsUndefined()) {
            reset();
        }

        if (is_unsigned) {
            number_.SetNumber(static_cast<unsigned long long>(num));
            setTypeToUInt64();
        } else if (!is_float) {
            number_.SetNumber(static_cast<long long>(num));
            setTypeToInt64();
        } else {
            number_.SetNumber(static_cast<double>(num));
            setTypeToDouble();
        }

        return *this;
    }

    Value &operator=(NullType) noexcept {
        if (!IsUndefined()) {
            reset();
        }

        setTypeToNull();
        return *this;
    }

    Value &operator=(bool is_true) noexcept {
        if (!IsUndefined()) {
            reset();
        }

        if (is_true) {
            setTypeToTrue();
        } else {
            setTypeToFalse();
        }

        return *this;
    }

    void operator+=(Value &&val) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += static_cast<Value &&>(val);

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += val;
    }

    void operator+=(VObject &&obj) {
        if (IsObject()) {
            object_ += static_cast<VObject &&>(obj);
        } else {
            if (!(IsArray())) {
                Reset();
                initArray();
            }

            array_ +=
                static_cast<Value &&>(Value{static_cast<VObject &&>(obj)});
        }
    }

    void operator+=(const VObject &obj) {
        *this += static_cast<VObject &&>(VObject(obj));
    }

    void operator+=(VArray &&arr) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        if (arr.Size() != 0) {
            array_ += static_cast<VArray &&>(arr);
        } else {
            array_ += static_cast<Value &&>(Value{static_cast<VArray &&>(arr)});
        }
    }

    void operator+=(const VArray &arr) {
        (*this) += static_cast<VArray &&>(VArray(arr));
    }

    void operator+=(VString &&str) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += Value{static_cast<VString &&>(str)};
    }

    void operator+=(const VString &str) {
        *this += static_cast<VString &&>(VString(str));
    }

    void operator+=(const Char_T_ *str) {
        *this += static_cast<VString &&>(VString(str));
    }

    template <typename Number_T_>
    void operator+=(Number_T_ num) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += static_cast<Value &&>(Value{num});
    }

    void operator+=(NullType) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += static_cast<Value &&>(Value{nullptr});
    }

    void operator+=(bool is_true) {
        if (!(IsArray())) {
            Reset();
            initArray();
        }

        array_ += static_cast<Value &&>(Value{is_true});
    }

    Value &operator[](const Char_T_ *key) {
        if (!(IsObject())) {
            Reset();
            initObject();
        }

        return object_[key];
    }

    Value &operator[](VString &&key) {
        if (!(IsObject())) {
            Reset();
            initObject();
        }

        return object_[static_cast<VString &&>(key)];
    }

    Value &operator[](const VString &key) {
        if (!(IsObject())) {
            Reset();
            initObject();
        }

        return object_[key];
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

            Reset();
            initArray();
        }

        if (array_.Size() == index) {
            if (array_.Capacity() == array_.Size()) {
                array_.Resize((index + 1) * 2);
            }

            array_ += Value();
        } else {
            array_.ResizeAndInitialize(index + 1);
        }

        return (array_.Storage()[index]);
    }

    template <typename Type_T_>
    Value &operator[](Type_T_ index) {
        return (*this)[static_cast<SizeT>(index)];
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

                case ValueType::UInt64: {
                    return (number_.GetUInt64() < val.number_.GetUInt64());
                }

                case ValueType::Int64: {
                    return (number_.GetInt64() < val.number_.GetInt64());
                }

                case ValueType::Double: {
                    return (number_.GetDouble() < val.number_.GetDouble());
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

                case ValueType::UInt64: {
                    return (number_.GetUInt64() > val.number_.GetUInt64());
                }

                case ValueType::Int64: {
                    return (number_.GetInt64() > val.number_.GetInt64());
                }

                case ValueType::Double: {
                    return (number_.GetDouble() > val.number_.GetDouble());
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

    inline bool IsNumber() const noexcept {
        return ((Type() == ValueType::UInt64) || (Type() == ValueType::Int64) ||
                (Type() == ValueType::Double));
    }

    inline bool IsUInt64() const noexcept {
        return (Type() == ValueType::UInt64);
    }

    inline bool IsInt64() const noexcept {
        return (Type() == ValueType::Int64);
    }

    inline bool IsDouble() const noexcept {
        return (Type() == ValueType::Double);
    }

    inline bool IsTrue() const noexcept { return (Type() == ValueType::True); }

    inline bool IsFalse() const noexcept {
        return (Type() == ValueType::False);
    }

    inline bool IsNull() const noexcept { return (Type() == ValueType::Null); }

    SizeT Size() const noexcept {
        if (IsObject()) {
            return object_.Size();
        }

        if (IsArray()) {
            return array_.Size();
        }

        return 0;
    }

    void Merge(Value &&val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            for (Value *src_val = val.array_.Storage(),
                       *end     = (src_val + val.array_.Size());
                 src_val < end; src_val++) {
                if (!(src_val->IsUndefined())) {
                    array_ += static_cast<Value &&>(*src_val);
                }
            }
        } else if (IsObject() && val.IsObject()) {
            object_ += static_cast<VObject &&>(val.object_);
        }

        val.Reset();
    }
    void Merge(Value &val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            for (Value *src_val = val.array_.Storage(),
                       *end     = (src_val + val.array_.Size());
                 src_val < end; src_val++) {
                if (!(src_val->IsUndefined())) {
                    array_ += *src_val;
                }
            }
        } else if (IsObject() && val.IsObject()) {
            object_ += val.object_;
        }
    }

    Value *GetValue(SizeT index) const noexcept {
        const ValueType type = Type();

        if ((type == ValueType::Array) && (index < array_.Size())) {
            Value *val = (array_.Storage() + index);

            if (!(val->IsUndefined())) {
                return val;
            }

            return nullptr;
        }

        if (type == ValueType::Object) {
            Value *val = object_.GetValue(index);

            if ((val != nullptr) && (!(val->IsUndefined()))) {
                return val;
            }
        }

        return nullptr;
    }

    Value *GetValue(const Char_T_ *key, SizeT length) const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            Value *val = object_.Find(key, length);

            if ((val != nullptr) && !(val->IsUndefined())) {
                return val;
            }

            return nullptr;
        }

        SizeT index;

        if ((type == ValueType::Array) &&
            (Digit<Char_T_>::StringToNumber(index, key, length)) &&
            (index < array_.Size())) {
            Value *val = (array_.Storage() + index);

            if (!(val->IsUndefined())) {
                return val;
            }
        }

        return nullptr;
    }

    const VString *GetKey(SizeT index) const noexcept {
        if (IsObject()) {
            return object_.GetKey(index);
        }

        return nullptr;
    }

    const VObject *GetObject() const noexcept {
        if (IsObject()) {
            return &object_;
        }

        return nullptr;
    }

    const VArray *GetArray() const noexcept {
        if (IsArray()) {
            return &array_;
        }

        return nullptr;
    }

    const VString *GetString() const noexcept {
        if (IsString()) {
            return &string_;
        }

        return nullptr;
    }

    const Char_T_ *StringStorage() const noexcept {
        if (IsString()) {
            return string_.First();
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        if (IsString()) {
            return string_.Length();
        }

        return 0;
    }

    // To get a pointer to a key and its length.
    template <typename Number_T_>
    bool SetKeyCharAndLength(SizeT index, const Char_T_ *&key,
                             Number_T_ &length) const noexcept {
        const VString *val = GetKey(index);

        if (val != nullptr) {
            key    = val->First();
            length = static_cast<Number_T_>(val->Length());
            return true;
        }

        return false;
    }

    // To get a pointer to the string and its length.
    template <typename Number_T_>
    bool SetCharAndLength(const Char_T_ *&key,
                          Number_T_ &     length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                key    = string_.First();
                length = static_cast<Number_T_>(string_.Length());
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
        switch (Type()) {
            case ValueType::String: {
                value = string_;
                return true;
            }

            case ValueType::UInt64: {
                value = Digit<Char_T_>::NumberToString(number_.GetUInt64(), 1);
                return true;
            }

            case ValueType::Int64: {
                value = Digit<Char_T_>::NumberToString(number_.GetInt64(), 1);
                return true;
            }

            case ValueType::Double: {
                value = Digit<Char_T_>::NumberToString(
                    number_.GetDouble(), 1, 0, QENTEM_DOUBLE_PRECISION);
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
                return false;
            }
        }
    }

    bool InsertStringInto(StringStream<Char_T_> &ss) const {
        switch (Type()) {
            case ValueType::String: {
                ss += string_;
                return true;
            }

            case ValueType::UInt64: {
                Digit<Char_T_>::NumberToStringStream(ss, number_.GetUInt64(),
                                                     1);
                return true;
            }

            case ValueType::Int64: {
                Digit<Char_T_>::NumberToStringStream(ss, number_.GetInt64(), 1);
                return true;
            }

            case ValueType::Double: {
                Digit<Char_T_>::NumberToStringStream(
                    ss, number_.GetDouble(), 1, 0, QENTEM_DOUBLE_PRECISION);
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
                return false;
            }
        }
    }

    bool InsertIndexOfKeyInto(StringStream<Char_T_> &ss, SizeT index) const {
        if (IsObject()) {
            const VString *key = object_.GetKey(index);

            if (key != nullptr) {
                ss += *key;
            }

            return true;
        }

        return false;
    }

    unsigned long long GetUInt64() const noexcept {
        unsigned long long num = 0;
        SetNumber(num);
        return num;
    }

    long long GetInt64() const noexcept {
        long long num = 0;
        SetNumber(num);
        return num;
    }

    double GetDouble() const noexcept {
        double num = 0;
        SetNumber(num);
        return num;
    }

    double GetNumber() const noexcept { return GetDouble(); }

    template <typename Number_T_>
    bool SetNumber(Number_T_ &value) const noexcept {
        switch (Type()) {
            case ValueType::UInt64: {
                value = static_cast<Number_T_>(number_.GetUInt64());
                return true;
            }

            case ValueType::Int64: {
                value = static_cast<Number_T_>(number_.GetInt64());
                return true;
            }

            case ValueType::Double: {
                value = static_cast<Number_T_>(number_.GetDouble());
                return true;
            }

            case ValueType::String: {
                double num;

                if (Digit<Char_T_>::StringToNumber(num, string_.First(),
                                                   string_.Length())) {
                    value = static_cast<Number_T_>(num);
                    return true;
                }

                return false;
            }

            case ValueType::True: {
                value = 1;
                return true;
            }

            case ValueType::False:
            case ValueType::Null: {
                value = 0;
                return true;
            }

            default: {
                return false;
            }
        }
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

            case ValueType::UInt64: {
                value = (number_.GetUInt64() > 0);
                return true;
            }

            case ValueType::Int64: {
                value = (number_.GetInt64() > 0);
                return true;
            }

            case ValueType::Double: {
                value = (number_.GetDouble() > 0);
                return true;
            }

            case ValueType::String: {
                if (string_.IsEqual(JSONotation_T_::GetTrueString(),
                                    JSONotation_T_::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (string_.IsEqual(JSONotation_T_::GetFalseString(),
                                    JSONotation_T_::FalseStringLength)) {
                    value = false;
                    return true;
                }

                return false;
            }

            default: {
                return false;
            }
        }
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        if (IsObject()) {
            object_.Remove(key);
        }
    }

    inline void Remove(const VString &key) const noexcept {
        if (IsObject()) {
            object_.Remove(key);
        }
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (IsObject()) {
            object_.RemoveIndex(index);
        } else if (IsArray() && (index < array_.Size())) {
            (array_.Storage() + index)->Reset();
        }
    }

    template <typename Number_T_>
    inline void RemoveIndex(Number_T_ index) const noexcept {
        RemoveIndex(static_cast<SizeT>(index));
    }

    void Reset() noexcept {
        reset();
        setTypeToUndefined();
    }

    void Compress() {
        if (IsArray()) {
            Value *      src_val = array_.Storage();
            const Value *src_end = array_.End();
            SizeT        size    = 0;

            while (src_val != src_end) {
                if (!(src_val->IsUndefined())) {
                    ++size;
                }

                ++src_val;
            }

            if (size != array_.Capacity()) {
                if (size == 0) {
                    array_.Reset();
                    return;
                }

                VArray new_array(size);
                src_val = array_.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                } while (src_val != src_end);

                array_ = static_cast<VArray &&>(new_array);
            }
        } else if (IsObject()) {
            object_.Compress();
        }
    }

    static void StringifyObject(const VObject &obj, StringStream<Char_T_> &ss) {
        using V_item_ = HAItem<Value, Char_T_>;

        ss += JSONotation_T_::SCurlyChar;

        for (const V_item_ *h_item = obj.First(), *end = (h_item + obj.Size());
             h_item != end; h_item++) {
            if ((h_item != nullptr) && !(h_item->Value.IsUndefined())) {
                ss += JSONotation_T_::QuoteChar;
                JSON::EscapeJSON(h_item->Key.First(), h_item->Key.Length(), ss);
                ss += JSONotation_T_::QuoteChar;
                ss += JSONotation_T_::ColonChar;

                StringifyValue(h_item->Value, ss);
                ss += JSONotation_T_::CommaChar;
            }
        }

        if (*(ss.Last()) == JSONotation_T_::CommaChar) {
            ss.StepBack(1);
        }

        ss += JSONotation_T_::ECurlyChar;
    }

    static void StringifyArray(const VArray &arr, StringStream<Char_T_> &ss) {
        ss += JSONotation_T_::SSquareChar;

        for (const Value *item = arr.First(), *end = (item + arr.Size());
             item != end; item++) {
            if (!(item->IsUndefined())) {
                StringifyValue(*item, ss);
                ss += JSONotation_T_::CommaChar;
            }
        }

        if (*(ss.Last()) == JSONotation_T_::CommaChar) {
            ss.StepBack(1);
        }

        ss += JSONotation_T_::ESquareChar;
    }

    static void StringifyValue(const Value &val, StringStream<Char_T_> &ss) {
        switch (val.Type()) {
            case ValueType::Object: {
                StringifyObject(val.object_, ss);
                break;
            }

            case ValueType::Array: {
                StringifyArray(val.array_, ss);
                break;
            }

            case ValueType::String: {
                ss += JSONotation_T_::QuoteChar;
                JSON::EscapeJSON(val.string_.First(), val.string_.Length(), ss);
                ss += JSONotation_T_::QuoteChar;
                break;
            }

            case ValueType::UInt64: {
                Digit<Char_T_>::NumberToStringStream(
                    ss, val.number_.GetUInt64(), 1);
                break;
            }

            case ValueType::Int64: {
                Digit<Char_T_>::NumberToStringStream(ss, val.number_.GetInt64(),
                                                     1);
                break;
            }

            case ValueType::Double: {
                Digit<Char_T_>::NumberToStringStream(
                    ss, val.number_.GetDouble(), 1);
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
    }

    inline VString Stringify() const {
        StringStream<Char_T_> ss;

        const ValueType type = Type();

        if (type == ValueType::Object) {
            StringifyObject(this->object_, ss);
        } else if (type == ValueType::Array) {
            StringifyArray(this->array_, ss);
        }

        return ss.GetString();
    }

    inline ValueType Type() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        return static_cast<ValueType>(type_.Value.GetHighTag());
#else
        return type_;
#endif
    }

    bool GroupBy(Value &groupedValue, const Char_T_ *key,
                 const SizeT length) const noexcept {
        using V_item_ = HAItem<Value, Char_T_>;
        VObject new_sub_obj;
        VString grouped_key;
        SizeT   grouped_key_index;

        if (IsArray()) {
            groupedValue.Reset();
            groupedValue.initObject();

            const Value *_item = array_.First();

            if ((_item == nullptr) || !(_item->IsObject()) ||
                !(_item->object_.GetKeyIndex(grouped_key_index, key, length))) {
                return false;
            }

            for (const Value *end = array_.End(); _item != end; _item++) {
                if ((_item == nullptr) || !(_item->IsObject())) {
                    return false;
                }

                SizeT count = 0;
                for (const V_item_ *obj_item = _item->object_.First(),
                                   *obj_end  = _item->object_.End();
                     obj_item != obj_end; obj_item++) {
                    if ((obj_item == nullptr) ||
                        obj_item->Value.IsUndefined()) {
                        return false;
                    }

                    if (count != grouped_key_index) {
                        new_sub_obj[obj_item->Key] = obj_item->Value;
                    } else if (!obj_item->Value.SetString(grouped_key)) {
                        return false;
                    }

                    ++count;
                }

                groupedValue.object_[static_cast<VString &&>(grouped_key)] +=
                    static_cast<VObject &&>(new_sub_obj);
            }

            return true;
        }

        return false;
    }

    bool GroupBy(Value &groupedValue, const Char_T_ *key) const noexcept {
        return GroupBy(groupedValue, key, StringUtils::Count(key));
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            object_.Sort(ascend);
        } else if (type == ValueType::Array) {
            array_.Sort(ascend);
        }
    }

  private:
    struct VType_; // For tagging pointers

    inline void setType(ValueType new_type) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        type_.Value.SetHighTag(static_cast<unsigned char>(new_type));
#else
        type_ = new_type;
#endif
    }

    inline void setTypeToUndefined() noexcept { setType(ValueType::Undefined); }
    inline void setTypeToObject() noexcept { setType(ValueType::Object); }
    inline void setTypeToArray() noexcept { setType(ValueType::Array); }
    inline void setTypeToString() noexcept { setType(ValueType::String); }
    inline void setTypeToUInt64() noexcept { setType(ValueType::UInt64); }
    inline void setTypeToInt64() noexcept { setType(ValueType::Int64); }
    inline void setTypeToDouble() noexcept { setType(ValueType::Double); }
    inline void setTypeToTrue() noexcept { setType(ValueType::True); }
    inline void setTypeToFalse() noexcept { setType(ValueType::False); }
    inline void setTypeToNull() noexcept { setType(ValueType::Null); }

    inline void initObject() {
        Memory::Construct(&object_, VObject());
        setTypeToObject();
    }

    inline void initArray() {
        Memory::Construct(&array_, VArray());
        setTypeToArray();
    }

    inline void initString() {
        Memory::Construct(&string_, VString());
        setTypeToString();
    }

    inline void initValue(VObject &&obj) {
        Memory::Construct(&object_, static_cast<VObject &&>(obj));
        setTypeToObject();
    }

    inline void initValue(const VObject &obj) {
        Memory::Construct(&object_, obj);
        setTypeToObject();
    }

    inline void initValue(VArray &&arr) {
        Memory::Construct(&array_, static_cast<VArray &&>(arr));
        setTypeToArray();
    }

    inline void initValue(const VArray &arr) {
        Memory::Construct(&array_, arr);
        setTypeToArray();
    }

    inline void initValue(VString &&str) {
        Memory::Construct(&string_, static_cast<VString &&>(str));
        setTypeToString();
    }

    inline void initValue(const VString &str) {
        Memory::Construct(&string_, str);
        setTypeToString();
    }

    void reset() {
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
            }
        }
    }

    void copyValue(const Value &val) {
        switch (val.Type()) {
            case ValueType::Object: {
                initValue(val.object_);
                break;
            }

            case ValueType::Array: {
                initValue(val.array_);
                break;
            }

            case ValueType::String: {
                initValue(val.string_);
                break;
            }

            default: {
                number_ = val.number_;
#if !defined(QENTEM_POINTER_TAGGING) || QENTEM_POINTER_TAGGING != 1
                setType(val.Type());
#endif
            }
        }
    }

    struct VNumber {
      public:
        VNumber() = default;

        template <typename Number_T_>
        explicit VNumber(const Number_T_ &num) noexcept : number_{num} {}

        inline void SetNumber(double num) noexcept { number_.d = num; }

        inline void SetNumber(unsigned long long num) noexcept {
            number_.ull = num;
        }

        inline void SetNumber(long long num) noexcept { number_.sll = num; }

        inline unsigned long long GetUInt64() const noexcept {
            return number_.ull;
        }

        inline long long GetInt64() const noexcept { return number_.sll; }
        inline double    GetDouble() const noexcept { return number_.d; }

      private:
        union Number_T_ {
            Number_T_() noexcept : padding_{0, 0} {}
            explicit Number_T_(unsigned long long num) noexcept : ull{num} {}
            explicit Number_T_(long long num) noexcept : sll{num} {}
            explicit Number_T_(double num) noexcept : d{num} {}

            unsigned long long ull;
            long long          sll;
            double             d;
            SizeT              padding_[2]; // Just in case SizeT is set to long
        };

#ifndef QENTEM_BIG_ENDIAN
        Number_T_   number_;
        const void *padding_{nullptr};
#else
        const void *padding_{nullptr};
        Number_T_   number_;
#endif
    };

    union {
        VObject object_;
        VArray  array_;
        VString string_;
        VNumber number_{double(0)};
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        VType_ type_;
    };

    struct VType_ {
#ifndef QENTEM_BIG_ENDIAN
      private:
        SizeT padding_[2]{0, 0};

      public:
        QPointer<void> Value{};
#else
      public:
        QPointer<void> Value{};

      private:
        SizeT padding_[2]{0, 0};
#endif
    };
#else
    };

    ValueType type_{ValueType::Undefined};
#endif
};

} // namespace Qentem

#endif
