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
    using VObject        = HArray<Value, Char_T_>;
    using VArray         = Array<Value>;
    using VString        = String<Char_T_>;

  public:
    Value() noexcept : object_(VObject()) {
    }

    Value(Value &&val) noexcept : type_(val.type_) {
        val.type_ = ValueType::Undefined;

        switch (type_) {
            case ValueType::Object: {
                Memory::Construct(&object_,
                                  static_cast<VObject &&>(val.object_));
                break;
            }

            case ValueType::Array: {
                Memory::Construct(&array_, static_cast<VArray &&>(val.array_));
                break;
            }

            case ValueType::String: {
                Memory::Construct(&string_,
                                  static_cast<VString &&>(val.string_));
                break;
            }

            case ValueType::Number: {
                number_ = val.number_;
                break;
            }

            default: {
            }
        }
    }

    Value(const Value &val) : type_(val.type_) {
        copyValue(val);
    }

    explicit Value(ValueType type) noexcept : type_(type) {
        switch (type) {
            case ValueType::Object: {
                Memory::Construct(&object_, VObject());
                break;
            }

            case ValueType::Array: {
                Memory::Construct(&array_, VArray());
                break;
            }

            case ValueType::String: {
                Memory::Construct(&string_, VString());
                break;
            }

            default: {
            }
        }
    }

    explicit Value(VObject &&obj) noexcept
        : object_(static_cast<VObject &&>(obj)), type_(ValueType::Object) {
    }

    explicit Value(VArray &&arr) noexcept
        : array_(static_cast<VArray &&>(arr)), type_(ValueType::Array) {
    }

    explicit Value(VString &&str) noexcept
        : string_(static_cast<VString &&>(str)), type_(ValueType::String) {
    }

    explicit Value(const VObject &obj) noexcept
        : object_(obj), type_(ValueType::Object) {
    }

    explicit Value(const VArray &arr) noexcept
        : array_(arr), type_(ValueType::Array) {
    }

    explicit Value(const VString &str) noexcept
        : string_(str), type_(ValueType::String) {
    }

    explicit Value(double num) noexcept
        : number_(num), type_(ValueType::Number) {
    }

    template <typename Type_T_>
    explicit Value(Type_T_ num) noexcept
        : number_(static_cast<double>(num)), type_(ValueType::Number) {
    }

    explicit Value(NullType) noexcept : type_(ValueType::Null) {
    }

    explicit Value(bool bl) noexcept
        : type_(bl ? ValueType::True : ValueType::False) {
    }

    ~Value() {
        if (type_ == ValueType::Undefined) {
            return;
        }

        reset();
    }

    Value &operator[](const Char_T_ *key) {
        switch (type_) {
            case ValueType::Object: {
                return (object_)[key];
            }

            case ValueType::Undefined: {
                type_ = ValueType::Object;
                Memory::Construct(&object_, VObject());
                return (object_)[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator[](VString &&key) {
        switch (type_) {
            case ValueType::Object: {
                return (object_)[static_cast<VString &&>(key)];
            }

            case ValueType::Undefined: {
                type_ = ValueType::Object;
                Memory::Construct(&object_, VObject());
                return (object_)[static_cast<VString &&>(key)];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator[](const VString &key) {
        switch (type_) {
            case ValueType::Object: {
                return (object_)[key];
            }

            case ValueType::Undefined: {
                type_ = ValueType::Object;
                Memory::Construct(&object_, VObject());
                return (object_)[key];
            }

            default: {
            }
        }

        throw 3;
    }

    Value &operator[](SizeT index) {
        switch (type_) {
            case ValueType::Array: {
                if (array_.Size() > index) {
                    return *(array_.First() + index);
                }

                if (array_.Size() == index) {
                    array_ += Value();
                    return *(array_.First() + index);
                }

                break;
            }

            case ValueType::Object: {
                Value *val = object_.GetValue(index);

                if (val != nullptr) {
                    return (*val);
                }

                break;
            }

            case ValueType::Undefined: {
                if (index == 0) {
                    type_ = ValueType::Array;
                    Memory::Construct(&array_, VArray());
                    array_.ResizeAndInitialize(1);
                    return *(array_.First());
                }

                break;
            }

            default: {
            }
        }

        throw 1;
    }

    template <typename Type_T_>
    Value &operator[](Type_T_ index) {
        return (*this)[static_cast<SizeT>(index)];
    }

    Value &operator=(Value &&val) noexcept {
        if (this != &val) {
            reset();
            type_     = val.type_;
            val.type_ = ValueType::Undefined;

            switch (type_) {
                case ValueType::Object: {
                    Memory::Construct(&object_,
                                      static_cast<VObject &&>(val.object_));
                    break;
                }

                case ValueType::Array: {
                    Memory::Construct(&array_,
                                      static_cast<VArray &&>(val.array_));
                    break;
                }

                case ValueType::String: {
                    Memory::Construct(&string_,
                                      static_cast<VString &&>(val.string_));
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
            if (type_ == val.type_) {
                switch (type_) {
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

                    case ValueType::Number: {
                        number_ = val.number_;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                reset();
                type_ = val.type_;

                if (type_ != ValueType::Undefined) {
                    copyValue(val);
                }
            }
        }

        return *this;
    }

    Value &operator=(VObject &&obj) {
        if (type_ != ValueType::Object) {
            reset();
            type_ = ValueType::Object;
            Memory::Construct(&object_, VObject());
        }

        object_ = static_cast<VObject &&>(obj);
        return *this;
    }

    Value &operator=(const VObject &obj) {
        *this = static_cast<VObject &&>(VObject(obj));
        return *this;
    }

    Value &operator=(VArray &&arr) {
        if (type_ != ValueType::Array) {
            reset();
            type_ = ValueType::Array;
            Memory::Construct(&array_, VArray());
        }

        array_ = static_cast<VArray &&>(arr);
        return *this;
    }

    Value &operator=(const VArray &arr) {
        *this = static_cast<VArray &&>(VArray(arr));
        return *this;
    }

    Value &operator=(VString &&str) {
        if (type_ != ValueType::String) {
            reset();
            type_ = ValueType::String;
            Memory::Construct(&string_, VString());
        }

        string_ = static_cast<VString &&>(str);
        return *this;
    }

    Value &operator=(const VString &str) {
        *this = static_cast<VString &&>(VString(str));
        return *this;
    }

    Value &operator=(const Char_T_ *str) {
        *this = static_cast<VString &&>(VString(str));
        return *this;
    }

    Value &operator=(double num) noexcept {
        if (type_ != ValueType::Number) {
            type_ = ValueType::Number;
            reset();
        }

        number_ = num;
        return *this;
    }

    template <typename Type_T_>
    inline Value &operator=(Type_T_ num) {
        *this = static_cast<double>(num);
        return *this;
    }

    Value &operator=(NullType) {
        reset();
        type_ = ValueType::Null;
        return *this;
    }

    Value &operator=(bool is_true) noexcept {
        reset();
        type_ = (is_true ? ValueType::True : ValueType::False);
        return *this;
    }

    void operator+=(Value &&val) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            if (val.type_ == ValueType::Array) {
                const VArray &src_arr = val.array_;
                VArray &      des_arr = array_;

                Value *      src_val = src_arr.First();
                const Value *src_end = src_arr.End();

                while (src_val != src_end) {
                    if (src_val->type_ != ValueType::Undefined) {
                        des_arr += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                }
            } else if (val.type_ != ValueType::Undefined) {
                array_ += static_cast<Value &&>(val);
            }
        } else if ((type_ == ValueType::Object) && (val.type_ == type_)) {
            object_ += static_cast<VObject &&>(val.object_);
        }

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            if (val.type_ == ValueType::Array) {
                const VArray &src_arr = val.array_;
                VArray &      des_arr = array_;

                const Value *src_val = src_arr.First();
                const Value *src_end = src_arr.End();

                while (src_val != src_end) {
                    if (src_val->type_ != ValueType::Undefined) {
                        des_arr += *src_val;
                    }

                    ++src_val;
                }
            } else if (val.type_ != ValueType::Undefined) {
                array_ += val;
            }
        } else if ((type_ == ValueType::Object) && (val.type_ == type_)) {
            object_ += val.object_;
        }
    }

    void operator+=(VObject &&obj) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            array_ +=
                static_cast<Value &&>(Value{static_cast<VObject &&>(obj)});
        } else if (type_ == ValueType::Object) {
            object_ += static_cast<VObject &&>(obj);
        }
    }

    void operator+=(const VObject &obj) {
        *this += static_cast<VObject &&>(VObject(obj));
    }

    void operator+=(VArray &&arr) {
        if (type_ == ValueType::Array) {
            array_ += static_cast<VArray &&>(arr);
        } else if (type_ == ValueType::Undefined) {
            new (&array_) VArray(static_cast<VArray &&>(arr));
            type_ = ValueType::Array;
        }
    }

    void operator+=(const VArray &arr) {
        (*this) += static_cast<VArray &&>(VArray(arr));
    }

    void operator+=(VString &&str) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            array_ += Value{static_cast<VString &&>(str)};
        }
    }

    void operator+=(const VString &str) {
        *this += static_cast<VString &&>(VString(str));
    }

    void operator+=(const Char_T_ *str) {
        *this += static_cast<VString &&>(VString(str));
    }

    void operator+=(double num) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            array_ += static_cast<Value &&>(Value{num});
        }
    }

    template <typename Type_T_>
    inline void operator+=(Type_T_ num) {
        *this += static_cast<double>(num);
    }

    void operator+=(NullType) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            array_ += static_cast<Value &&>(Value{nullptr});
        }
    }

    void operator+=(bool is_true) {
        if (type_ == ValueType::Undefined) {
            Memory::Construct(&array_, VArray());
            type_ = ValueType::Array;
        }

        if (type_ == ValueType::Array) {
            array_ += static_cast<Value &&>(Value{is_true});
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

    SizeT Size() const noexcept {
        switch (type_) {
            case ValueType::Object: {
                return object_.Size();
            }

            case ValueType::Array: {
                return array_.Size();
            }

            default: {
            }
        }

        return 0;
    }

    Value *GetValue(SizeT index) const noexcept {
        if (type_ == ValueType::Array) {
            if (index < array_.Size()) {
                Value *val = (array_.First() + index);

                if (!(val->IsUndefined())) {
                    return val;
                }
            }
        } else if (type_ == ValueType::Object) {
            Value *val = object_.GetValue(index);

            if ((val != nullptr) && (!(val->IsUndefined()))) {
                return val;
            }
        }

        return nullptr;
    }

    Value *GetValue(const Char_T_ *key, SizeT length) const noexcept {
        if (type_ == ValueType::Object) {
            Value *val = (object_.Find(key, length));

            if ((val != nullptr) && !(val->IsUndefined())) {
                return val;
            }
        }

        return nullptr;
    }

    const VString *GetKey(SizeT index) const noexcept {
        if (type_ == ValueType::Object) {
            return object_.GetKey(index);
        }

        return nullptr;
    }

    const VObject *GetObject() const noexcept {
        if (type_ == ValueType::Object) {
            return &object_;
        }

        return nullptr;
    }

    const VArray *GetArray() const noexcept {
        if (type_ == ValueType::Array) {
            return &array_;
        }

        return nullptr;
    }

    const VString *GetString() const noexcept {
        if (type_ == ValueType::String) {
            return &string_;
        }

        return nullptr;
    }

    const Char_T_ *StringStorage() const noexcept {
        if (type_ == ValueType::String) {
            return string_.First();
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        if (type_ == ValueType::String) {
            return string_.Length();
        }

        return 0;
    }

    template <typename Number_T_>
    bool SetCharAndLength(const Char_T_ *&key,
                          Number_T_ &     length) const noexcept {
        switch (type_) {
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
        switch (type_) {
            case ValueType::String: {
                value = string_;
                return true;
            }

            case ValueType::Number: {
                value = Digit<Char_T_>::NumberToString(
                    number_, 1, 0, QENTEM_DOUBLE_PRECISION_);
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
                ss += string_;
                return true;
            }

            case ValueType::Number: {
                Digit<Char_T_>::NumberToStringStream(ss, number_, 1, 0,
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

    bool InsertKey(StringStream<Char_T_> &ss, SizeT index) const {
        if (type_ == ValueType::Object) {
            const VString *key = object_.GetKey(index);

            if (key != nullptr) {
                ss += *key;
            }

            return true;
        }

        return false;
    }

    double GetNumber() const noexcept {
        if (type_ == ValueType::Number) {
            return number_;
        }

        return 0;
    }

    bool SetNumber(double &value) const noexcept {
        switch (type_) {
            case ValueType::Number: {
                value = number_;
                return true;
            }

            case ValueType::String: {
                return Digit<Char_T_>::StringToNumber(value, string_.First(),
                                                      string_.Length());
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
                value = (number_ > 0);
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
            }
        }

        return false;
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        if (type_ == ValueType::Object) {
            object_.Remove(key);
        }
    }

    inline void Remove(const VString &key) const noexcept {
        if (type_ == ValueType::Object) {
            object_.Remove(key);
        }
    }

    void Remove(SizeT index) const noexcept {
        if (type_ == ValueType::Object) {
            object_.RemoveIndex(index);
        } else if ((type_ == ValueType::Array) && (index < array_.Size())) {
            (array_.First() + index)->Reset();
        }
    }

    template <typename Type_T_>
    inline void Remove(Type_T_ index) const noexcept {
        Remove(static_cast<SizeT>(index));
    }

    void Reset() noexcept {
        reset();
        type_ = ValueType::Undefined;
    }

    void Compress() {
        if (type_ == ValueType::Object) {
            object_.Compress();
        } else if (type_ == ValueType::Array) {
            const SizeT size = array_.Size();

            if (size != array_.Capacity()) {
                if (size == 0) {
                    array_.Reset();
                    return;
                }

                VArray       new_array(size);
                Value *      src_val = array_.First();
                const Value *src_end = array_.End();

                do {
                    if (src_val->type_ != ValueType::Undefined) {
                        new_array += static_cast<Value &&>(*src_val);
                    }

                    ++src_val;
                } while (src_val != src_end);

                array_ = static_cast<VArray &&>(new_array);
            }
        }
    }

    void Stringify(StringStream<Char_T_> &ss) const {
        const SizeT size   = Size();
        const bool  is_obj = (type_ == ValueType::Object);

        const HAItem<Value, Char_T_> *ha_item = nullptr;
        const Value *                 item    = nullptr;

        if (is_obj) {
            ha_item = (object_.First() - 1);
            ss += JSONotation_T_::SCurlyChar;
        } else if (type_ == ValueType::Array) {
            item = (array_.First() - 1);
            ss += JSONotation_T_::SSquareChar;
        } else {
            return;
        }

        if (size != 0) {
            SizeT id = 0;

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
                        JSON::EscapeJSON(item->string_.First(),
                                         item->string_.Length(), ss);
                        ss += JSONotation_T_::QuoteChar;
                        break;
                    }

                    case ValueType::Number: {
                        Digit<Char_T_>::NumberToStringStream(ss, item->number_);
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
            ss += JSONotation_T_::ECurlyChar;
        } else {
            ss += JSONotation_T_::ESquareChar;
        }
    }

    inline VString Stringify() const {
        StringStream<Char_T_> ss;
        Stringify(ss);
        return ss.GetString();
    }

  private:
    QENTEM_NOINLINE void reset() noexcept {
        switch (type_) {
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
        switch (val.type_) {
            case ValueType::Object: {
                new (&object_) VObject(val.object_);
                break;
            }

            case ValueType::Array: {
                new (&array_) VArray(val.array_);
                break;
            }

            case ValueType::String: {
                new (&string_) VString(val.string_);
                break;
            }

            case ValueType::Number: {
                number_ = val.number_;
                break;
            }

            default: {
            }
        }
    }

    union {
        VObject object_;
        VArray  array_;
        VString string_;
        double  number_{0};
    };

    ValueType type_{ValueType::Undefined};
};

} // namespace Qentem

#endif
