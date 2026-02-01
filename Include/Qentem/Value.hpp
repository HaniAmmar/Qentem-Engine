/**
 * @file Value.hpp
 * @brief Defines the Value struct used for dynamic data representation in Qentem Engine.
 *
 * Value is the core data structure for storing and manipulating various types of values
 * throughout the Qentem Engine library. This structure is primarily based on JSON data models,
 * enabling flexible and efficient handling of JSON-like data.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_VALUE_H
#define QENTEM_VALUE_H

#include "Qentem/StringStream.hpp"
#include "Qentem/QNumber.hpp"
#include "Qentem/Array.hpp"
#include "Qentem/HArray.hpp"
#include "Qentem/JSONUtils.hpp"

namespace Qentem {

enum struct ValueType : SizeT8 {
    Undefined = 0,
    ValuePtr,
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
    using NotationConstants = JSONUtils::NotationConstants_T<Char_T>;
    using VItem             = HAItem_T<String<Char_T>, Value>;

    using StringT     = String<Char_T>;
    using StringViewT = StringView<Char_T>;
    using ArrayT      = Array<Value, QENTEM_VALUE_EXPANSION_MULTIPLIER>;
    using ObjectT     = HArray<StringT, Value, QENTEM_VALUE_EXPANSION_MULTIPLIER>;

    Value() noexcept : array_{} {
    }

    ~Value() {
        switch (Type()) {
            case ValueType::Object: {
                MemoryUtils::Destruct(&object_);
                break;
            }

            case ValueType::Array: {
                MemoryUtils::Destruct(&array_);
                break;
            }

            case ValueType::String: {
                MemoryUtils::Destruct(&string_);
                break;
            }

            default: {
            }
        }
    }

    Value(Value &&val) noexcept : array_{} {
        switch (val.Type()) {
            case ValueType::Object: {
                object_ = QUtility::Move(val.object_);
                break;
            }

            case ValueType::Array: {
                array_ = QUtility::Move(val.array_);
                break;
            }

            case ValueType::String: {
                string_ = QUtility::Move(val.string_);
                break;
            }

            default: {
                number_ = val.number_;
            }
        }

        setType(val.Type());
        val.setTypeToUndefined();
    }

    Value(const Value &val) : array_{} {
        copyValue(val);
    }

    QENTEM_INLINE explicit Value(ValueType type) noexcept : array_{} {
        setType(type);
    }

    QENTEM_INLINE explicit Value(ValueType type, SizeT size) noexcept : array_{} {
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

    QENTEM_INLINE explicit Value(ObjectT &&obj) noexcept : object_{QUtility::Move(obj)} {
        setTypeToObject();
    }

    QENTEM_INLINE explicit Value(ArrayT &&arr) noexcept : array_{QUtility::Move(arr)} {
        setTypeToArray();
    }

    QENTEM_INLINE explicit Value(StringT &&str) noexcept : string_{QUtility::Move(str)} {
        setTypeToString();
    }

    QENTEM_INLINE explicit Value(const ObjectT &obj) noexcept : object_{obj} {
        setTypeToObject();
    }

    QENTEM_INLINE explicit Value(const ArrayT &arr) noexcept : array_{arr} {
        setTypeToArray();
    }

    QENTEM_INLINE explicit Value(const StringT &str) noexcept : string_{str} {
        setTypeToString();
    }

    QENTEM_INLINE explicit Value(const StringViewT &str) noexcept : string_{str.First(), str.Length()} {
        setTypeToString();
    }

    QENTEM_INLINE explicit Value(const Char_T *str) : string_{str} {
        setTypeToString();
    }

    QENTEM_INLINE explicit Value(const Char_T *str, SizeT length) : string_{str, length} {
        setTypeToString();
    }

    QENTEM_INLINE explicit Value(SizeT64 num) noexcept : number_{num} {
        setTypeToUInt64();
    }

    QENTEM_INLINE explicit Value(SizeT64I num) noexcept : number_{num} {
        setTypeToInt64();
    }

    QENTEM_INLINE explicit Value(double num) noexcept : number_{num} {
        setTypeToDouble();
    }

    template <typename Number_T>
    QENTEM_INLINE explicit Value(Number_T num) noexcept : number_{num} {
        if constexpr (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if constexpr (IsUnsigned<Number_T>()) {
            setTypeToUInt64();
        } else {
            setTypeToInt64();
        }
    }

    QENTEM_INLINE explicit Value(NullType) noexcept : array_{} {
        setTypeToNull();
    }

    QENTEM_INLINE explicit Value(bool bl) noexcept : array_{} {
        if (bl) {
            setTypeToTrue();
        } else {
            setTypeToFalse();
        }
    }

    QENTEM_INLINE Value &operator=(ValueType type) noexcept {
        setType(type);
        return *this;
    }

    Value &operator=(Value &&val) noexcept {
        if (this != &val) {
            const ValueType type = val.Type();

            val.setTypeToUndefined();

            reset();
            setType(type);

            switch (type) {
                case ValueType::Object: {
                    object_ = QUtility::Move(val.object_);
                    break;
                }

                case ValueType::Array: {
                    array_ = QUtility::Move(val.array_);
                    break;
                }

                case ValueType::String: {
                    string_ = QUtility::Move(val.string_);
                    break;
                }

                default: {
                    number_ = val.number_;
                }
            }
        }

        return *this;
    }

    QENTEM_INLINE Value &operator=(const Value &val) {
        if (this != &val) {
            reset();
            copyValue(val);
        }

        return *this;
    }

    void SetPointerToValue(const Value *val_ptr) {
        reset();

        if (val_ptr != nullptr) {
            setTypeToPtrValue();
            value_ = val_ptr;
        }
    }

    Value &operator=(ObjectT &&obj) noexcept {
        reset();
        setTypeToObject();

        object_ = QUtility::Move(obj);

        return *this;
    }

    Value &operator=(const ObjectT &obj) {
        reset();
        setTypeToObject();

        object_ = obj;

        return *this;
    }

    Value &operator=(ArrayT &&arr) noexcept {
        reset();
        setTypeToArray();

        array_ = QUtility::Move(arr);

        return *this;
    }

    Value &operator=(const ArrayT &arr) {
        reset();
        setTypeToArray();

        array_ = arr;

        return *this;
    }

    Value &operator=(StringT &&str) noexcept {
        reset();
        setTypeToString();

        string_ = QUtility::Move(str);

        return *this;
    }

    Value &operator=(const StringT &str) {
        reset();
        setTypeToString();

        string_ = str;

        return *this;
    }

    Value &operator=(const StringT *str) {
        if (str != nullptr) {
            reset();
            setTypeToString();

            string_ = *str;
        }

        return *this;
    }

    Value &operator=(StringT *str) {
        if (str != nullptr) {
            reset();
            setTypeToString();

            string_ = *str;
        }

        return *this;
    }

    Value &operator=(const StringViewT &str_v) {
        reset();
        setTypeToString();

        string_ = StringT{str_v.First(), str_v.Length()};

        return *this;
    }

    Value &operator=(const Char_T *str) {
        reset();
        setTypeToString();

        string_ = StringT{str};

        return *this;
    }

    Value &operator=(SizeT64 num) noexcept {
        reset();
        setTypeToUInt64();

        number_ = num;

        return *this;
    }

    Value &operator=(SizeT64I num) noexcept {
        reset();
        setTypeToInt64();

        number_ = num;

        return *this;
    }

    Value &operator=(double num) noexcept {
        reset();
        setTypeToDouble();

        number_ = num;

        return *this;
    }

    template <typename Number_T>
    Value &operator=(Number_T num) noexcept {
        reset();

        if constexpr (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if constexpr (IsUnsigned<Number_T>()) {
            setTypeToUInt64();
        } else {
            setTypeToInt64();
        }

        number_ = num;

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

    void operator+=(Value &&val) {
        if (isObject() && val.isObject()) {
            object_ += QUtility::Move(val.object_);
            val.setTypeToUndefined();
        } else {
            if (!isArray()) {
                reset();
                setTypeToArray();
            }

            array_ += QUtility::Move(val);
        }
    }

    void operator+=(const Value &val) {
        if (isObject() && val.isObject()) {
            object_ += val.object_;
        } else {
            if (!isArray()) {
                reset();
                setTypeToArray();
            }

            array_ += val;
        }
    }

    // Only to value of type array
    void AddPointerToValue(const Value *val_ptr) {
        if (!isArray()) {
            reset();
            setTypeToArray();
        }

        Value val;
        val.SetPointerToValue(val_ptr);

        array_ += QUtility::Move(val);
    }

    void operator+=(ObjectT &&obj) {
        if (isObject()) {
            object_ += QUtility::Move(obj);
        } else {
            if (!isArray()) {
                reset();
                setTypeToArray();
            }

            array_ += Value{QUtility::Move(obj)};
        }
    }

    QENTEM_INLINE void operator+=(const ObjectT &obj) {
        *this += ObjectT(obj);
    }

    void operator+=(ArrayT &&arr) {
        if (!isArray()) {
            reset();
            setTypeToArray();
        }

        if (arr.Size() != 0) {
            array_ += QUtility::Move(arr);
        } else {
            array_ += Value{QUtility::Move(arr)};
        }
    }

    QENTEM_INLINE void operator+=(const ArrayT &arr) {
        (*this) += ArrayT(arr);
    }

    void operator+=(StringT &&str) {
        if (!isArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{QUtility::Move(str)};
    }

    QENTEM_INLINE void operator+=(const StringT &str) {
        *this += StringT(str);
    }

    QENTEM_INLINE void operator+=(const StringViewT &str) {
        *this += StringT(str.First(), str.Length());
    }

    QENTEM_INLINE void operator+=(const Char_T *str) {
        *this += StringT(str);
    }

    template <typename Number_T>
    void operator+=(Number_T num) {
        if (!isArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{num};
    }

    void operator+=(NullType) {
        if (!isArray()) {
            reset();
            setTypeToArray();
        }

        array_ += Value{nullptr};
    }

    void operator+=(bool is_true) {
        if (!isArray()) {
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

    Value &operator[](const Char_T *str) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        return (object_[str]);
    }

    Value &operator[](const StringViewT &key) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(key.First(), key.Length()));
    }

    Value &operator[](StringT &&key) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        return (object_[QUtility::Move(key)]);
    }

    Value &operator[](const StringT &key) {
        if (!isObject()) {
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
                Value *val = object_.GetValueAt(index);

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
            array_.ResizeWithDefaultInit(index + SizeT{1});
        }

        return (array_.Storage()[index]);
    }

    template <typename Type_T>
    Value &operator[](Type_T index) {
        return (*this)[SizeT(index)];
    }

    // Will insert the str if it does not exist.
    Value &Get(const Char_T *str, SizeT length) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(str, length));
    }

    Value &Get(const StringViewT &key) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        return (object_.Get(key.First(), key.Length()));
    }

    void Insert(const StringViewT &key, Value &&val) {
        if (!isObject()) {
            reset();
            setTypeToObject();
        }

        object_.Insert(key.First(), key.Length(), QUtility::Move(val));
    }

    bool operator<(const Value &val) const noexcept {
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

                case ValueType::ValuePtr: {
                    return (*value_ < *(val.value_));
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return false;
                }
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->operator<(val);
        }

        return (type < val.Type());
    }

    bool operator>(const Value &val) const noexcept {
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

                case ValueType::ValuePtr: {
                    return (*value_ > *(val.value_));
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return false;
                }
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->operator>(val);
        }

        return (type > val.Type());
    }

    bool operator<=(const Value &val) const noexcept {
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

                case ValueType::ValuePtr: {
                    return (*value_ <= *(val.value_));
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->operator<=(val);
        }

        return (type < val.Type());
    }

    bool operator>=(const Value &val) const noexcept {
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

                case ValueType::ValuePtr: {
                    return (*value_ >= *(val.value_));
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->operator>=(val);
        }

        return (type > val.Type());
    }

    bool operator==(const Value &val) const noexcept {
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

                case ValueType::ValuePtr: {
                    return (*value_ == *(val.value_));
                }

                case ValueType::True:
                case ValueType::False:
                case ValueType::Null:
                case ValueType::Undefined: {
                    return true;
                }
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->operator==(val);
        }

        return (type > val.Type());
    }

    void Merge(Value &&val) {
        if (isUndefined()) {
            setTypeToArray();
        }

        if (isArray() && val.isArray()) {
            Value       *src_val = val.array_.Storage();
            const Value *end     = val.array_.End();

            while (src_val < end) {
                if (!(src_val->isUndefined())) {
                    array_ += QUtility::Move(*src_val);
                }

                ++src_val;
            }
        } else if (isObject() && val.isObject()) {
            object_ += QUtility::Move(val.object_);
        }

        val.Reset();
    }

    void Merge(const Value &val) {
        if (isUndefined()) {
            setTypeToArray();
        }

        if (isArray() && val.isArray()) {
            const Value *src_val = val.array_.First();
            const Value *end     = val.array_.End();

            while (src_val < end) {
                if (!(src_val->isUndefined())) {
                    array_ += *src_val;
                }

                ++src_val;
            }
        } else if (isObject() && val.isObject()) {
            object_ += val.object_;
        }
    }

    bool IsNumber() const noexcept {
        switch (Type()) {
            case ValueType::UIntLong:
            case ValueType::IntLong:
            case ValueType::Double: {
                return true;
            }

            case ValueType::ValuePtr: {
                return value_->IsNumber();
            }

            default: {
                return false;
            }
        }
    }

    bool IsUndefined() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Undefined) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isUndefined();
        }

        return false;
    }

    bool IsObject() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isObject();
        }

        return false;
    }

    bool IsArray() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isArray();
        }

        return false;
    }

    bool IsString() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::String) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isString();
        }

        return false;
    }

    bool IsUInt64() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::UIntLong) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isUInt64();
        }

        return false;
    }

    bool IsInt64() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::IntLong) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isInt64();
        }

        return false;
    }

    bool IsDouble() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Double) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isDouble();
        }

        return false;
    }

    bool IsTrue() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::True) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isTrue();
        }

        return false;
    }

    bool IsFalse() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::False) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isFalse();
        }

        return false;
    }

    QENTEM_INLINE bool IsBool() const noexcept {
        return (IsTrue() || IsFalse());
    }

    bool IsNull() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Null) {
            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->isNull();
        }

        return false;
    }

    QENTEM_INLINE bool IsPointerToValue() const noexcept {
        return isPtrValue();
    }

    QNumberType GetNumberType() const noexcept {
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

            case ValueType::ValuePtr: {
                return value_->GetNumberType();
            }

            default: {
                // NAN: Not A Number
                return QNumberType::NotANumber;
            }
        }
    }

    SizeT Size() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            return (object_.Size());
        }

        if (type == ValueType::Array) {
            return (array_.Size());
        }

        if (type == ValueType::ValuePtr) {
            return (value_->Size());
        }

        return 0;
    }

    Value *GetValueAt(SizeT index) noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = object_.GetValueAt(index);

                if ((val != nullptr) && (!(val->isUndefined()))) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                if (index < array_.Size()) {
                    Value *val = (array_.Storage() + index);

                    if (!(val->isUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

                // case ValueType::ValuePtr: {
                //     return value_->GetValueAt(index);
                // }

            default:
                return nullptr;
        }
    }

    const Value *GetValueAt(SizeT index) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                const Value *val = object_.GetValueAt(index);

                if ((val != nullptr) && (!(val->isUndefined()))) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                if (index < array_.Size()) {
                    const Value *val = (array_.Storage() + index);

                    if (!(val->isUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

            case ValueType::ValuePtr: {
                return value_->GetValueAt(index);
            }

            default:
                return nullptr;
        }
    }

    Value *GetValue(const Char_T *str, SizeT length) noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = object_.GetValue(str, length);

                if ((val != nullptr) && !(val->isUndefined())) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                SizeT index;
                Digit::FastStringToNumber(index, str, length);

                if (index < array_.Size()) {
                    Value *val = (array_.Storage() + index);

                    if (!(val->isUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

                // case ValueType::ValuePtr: {
                //     return value_->GetValue(str, length);
                // }

            default:
                return nullptr;
        }
    }

    const Value *GetValue(const Char_T *str, SizeT length) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                const Value *val = object_.GetValue(str, length);

                if ((val != nullptr) && !(val->isUndefined())) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                SizeT index;
                Digit::FastStringToNumber(index, str, length);

                if (index < array_.Size()) {
                    const Value *val = (array_.Storage() + index);

                    if (!(val->isUndefined())) {
                        return val;
                    }
                }

                return nullptr;
            }

            case ValueType::ValuePtr: {
                return value_->GetValue(str, length);
            }

            default:
                return nullptr;
        }
    }

    QENTEM_INLINE Value *GetValue(const StringViewT &key) noexcept {
        return GetValue(key.First(), key.Length());
    }

    QENTEM_INLINE const Value *GetValue(const StringViewT &key) const noexcept {
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

            case ValueType::ValuePtr: {
                return value_->Storage();
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

            case ValueType::ValuePtr: {
                return value_->First();
            }

            default:
                return nullptr;
        }
    }

    Value *Last() {
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

                // case ValueType::ValuePtr: {
                //     return value_->Last();
                // }

            default:
                return nullptr;
        }
    }

    const Value *Last() const {
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

            case ValueType::ValuePtr: {
                return value_->Last();
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

            case ValueType::ValuePtr: {
                return value_->End();
            }

            default:
                return nullptr;
        }
    }

    const StringT *GetKeyAt(SizeT index) const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            return (object_.GetKeyAt(index));
        }

        if (type == ValueType::ValuePtr) {
            return (value_->GetKeyAt(index));
        }

        return nullptr;
    }

    const ObjectT *GetObject() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            return &(object_);
        }

        if (type == ValueType::ValuePtr) {
            return (value_->GetObject());
        }

        return nullptr;
    }

    ObjectT *GetObject() noexcept {
        if (isObject()) {
            return &(object_);
        }

        return nullptr;
    }

    const ArrayT *GetArray() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            return &(array_);
        }

        if (type == ValueType::ValuePtr) {
            return value_->GetArray();
        }

        return nullptr;
    }

    ArrayT *GetArray() noexcept {
        if (isArray()) {
            return &(array_);
        }

        return nullptr;
    }

    const StringT *GetString() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::String) {
            return &(string_);
        }

        if (type == ValueType::ValuePtr) {
            return value_->GetString();
        }

        return nullptr;
    }

    StringT *GetString() noexcept {
        if (isString()) {
            return &(string_);
        }

        return nullptr;
    }

    StringViewT GetStringView() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::String) {
            return StringViewT{string_.First(), string_.Length()};
        }

        if (type == ValueType::ValuePtr) {
            return value_->GetStringView();
        }

        return StringViewT{};
    }

    const Char_T *StringStorage() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::String) {
            return (string_.First());
        }

        if (type == ValueType::ValuePtr) {
            return value_->StringStorage();
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        const ValueType type = Type();

        if (type == ValueType::String) {
            return (string_.Length());
        }

        if (type == ValueType::ValuePtr) {
            return (value_->Length());
        }

        return 0;
    }

    // To get a pointer to a key and its length.
    template <typename Number_T>
    bool SetKeyCharAndLengthAt(SizeT index, const Char_T *&str, Number_T &length) const noexcept {
        const StringT *val = GetKeyAt(index);

        if (val != nullptr) {
            str    = val->First();
            length = Number_T(val->Length());
            return true;
        }

        return false;
    }

    template <typename Number_T>
    void SetKeyCharAndLengthAt(SizeT index, const Value *&value, const Char_T *&str, Number_T &length) const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            const VItem *item = object_.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.isUndefined())) {
                value  = &(item->Value);
                str    = item->Key.First();
                length = Number_T(item->Key.Length());
            }
        } else if (type == ValueType::ValuePtr) {
            value_->SetKeyCharAndLengthAt(index, value, str, length);
        }
    }

    void SetValueAndKeyAt(SizeT index, const Value *&value, StringViewT &key) const noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            const VItem *item = object_.GetItemAt(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.isUndefined())) {
                value = &(item->Value);
                key   = StringViewT{item->Key.First(), item->Key.Length()};
            }
        } else if (type == ValueType::ValuePtr) {
            value_->SetValueAndKeyAt(index, value, key);
        }
    }

    // To get a pointer to a string value and its length.
    template <typename Number_T>
    bool SetCharAndLength(const Char_T *&str, Number_T &length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                str    = string_.First();
                length = Number_T{string_.Length()};
                return true;
            }

            case ValueType::True: {
                str    = NotationConstants::TrueString;
                length = NotationConstants::TrueStringLength;
                return true;
            }

            case ValueType::False: {
                str    = NotationConstants::FalseString;
                length = NotationConstants::FalseStringLength;
                return true;
            }

            case ValueType::Null: {
                str    = NotationConstants::NullString;
                length = NotationConstants::NullStringLength;
                return true;
            }

            case ValueType::ValuePtr: {
                return value_->SetCharAndLength(str, length);
            }

            default: {
                return false;
            }
        }
    }

    template <typename StringStream_T>
    using CopyValueToStringFunction_T = void(StringStream_T &, const Char_T *, SizeT);

    template <typename StringStream_T, typename StringFunction_T = CopyValueToStringFunction_T<StringStream_T>>
    bool CopyValueTo(StringStream_T             &stream,
                     const Digit::RealFormatInfo format          = Digit::RealFormatInfo{QentemConfig::DoublePrecision},
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
                stream.Write(NotationConstants::TrueString, NotationConstants::TrueStringLength);
                break;
            }

            case ValueType::False: {
                stream.Write(NotationConstants::FalseString, NotationConstants::FalseStringLength);
                break;
            }

            case ValueType::Null: {
                stream.Write(NotationConstants::NullString, NotationConstants::NullStringLength);
                break;
            }

            case ValueType::ValuePtr: {
                return value_->CopyValueTo(stream, format, string_function);
            }

            default: {
                return false;
            }
        }

        return true;
    }

    template <typename StringStream_T>
    bool CopyKeyAt(StringStream_T &stream, SizeT index) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            const StringT *key = object_.GetKeyAt(index);

            if (key != nullptr) {
                stream.Write(key->First(), key->Length());
            }

            return true;
        }

        if (type == ValueType::ValuePtr) {
            return value_->CopyKeyAt(stream, index);
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

                break;
            }

            case ValueType::ValuePtr: {
                return value_->SetNumber(number);
            }

            default: {
            }
        }

        return QNumberType::NotANumber;
    }

    bool SetBool(bool &value) const noexcept {
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

            case ValueType::ValuePtr: {
                return value_->SetBool(value);
            }

            case ValueType::String: {
                if (string_.IsEqual(NotationConstants::TrueString, NotationConstants::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (string_.IsEqual(NotationConstants::FalseString, NotationConstants::FalseStringLength)) {
                    value = false;
                    return true;
                }
            }

            default: {
            }
        }

        return false;
    }

    QENTEM_INLINE void Remove(const Char_T *str, SizeT length) noexcept {
        if (isObject()) {
            object_.Remove(str, length);
        }
    }

    QENTEM_INLINE void Remove(const StringT &key) noexcept {
        Remove(key.First(), key.Length());
    }

    QENTEM_INLINE void Remove(const Char_T *str) noexcept {
        Remove(str, StringUtils::Count(str));
    }

    void RemoveAt(SizeT index) noexcept {
        if (isObject()) {
            object_.RemoveAt(index);
        } else if (isArray() && (index < array_.Size())) {
            (array_.Storage() + index)->Reset();
        }
    }

    template <typename Number_T>
    QENTEM_INLINE void RemoveAt(Number_T index) noexcept {
        RemoveAt(SizeT(index));
    }

    void Reset() noexcept {
        reset();
        setTypeToUndefined();
    }

    void Compress() {
        if (isArray()) {
            Value       *src_val = array_.Storage();
            const Value *src_end = array_.End();

            SizeT       index    = 0;
            SizeT       new_size = 0;
            const SizeT old_size = Size();

            while (index < old_size) {
                Value *val = (src_val + index);

                if (!(val->isUndefined())) {
                    if (index != new_size) {
                        src_val[new_size] = QUtility::Move(*val);
                    }

                    ++new_size;
                }

                ++index;
            }

            array_.DropFast(old_size - new_size);
            array_.Compress();

            src_val = array_.Storage();
            src_end = array_.End();

            while (src_val < src_end) {
                if (src_val->isArray() || src_val->isObject()) {
                    src_val->Compress();
                }

                ++src_val;
            }
        } else if (isObject()) {
            object_.Compress();

            VItem       *src_val = object_.Storage();
            const VItem *src_end = (src_val + object_.Size());

            while (src_val < src_end) {
                if (src_val->Value.isArray() || src_val->Value.isObject()) {
                    src_val->Value.Compress();
                }

                ++src_val;
            }
        }
    }

    void RemoveExcessStorage() {
        if (isArray()) {
            array_.Compress();

            Value       *src_val = array_.Storage();
            const Value *src_end = array_.End();

            while (src_val < src_end) {
                if (src_val->isArray() || src_val->isObject()) {
                    src_val->RemoveExcessStorage();
                }

                ++src_val;
            }
        } else if (isObject()) {
            object_.RemoveExcessStorage();

            VItem       *src_val = object_.Storage();
            const VItem *src_end = (src_val + object_.Size());

            while (src_val < src_end) {
                if (src_val->Value.isArray() || src_val->Value.isObject()) {
                    src_val->Value.RemoveExcessStorage();
                }

                ++src_val;
            }
        }
    }

    QENTEM_INLINE ValueType Type() const noexcept {
        return type_;
    }

    bool GroupBy(Value &groupedValue, const Char_T *key_str, const SizeT length) const {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            StringStream<Char_T> stream;
            Value                sub_valu;
            const Value         *item_   = array_.First();
            const Char_T        *str     = nullptr;
            SizeT                str_len = 0;
            SizeT                grouped_key_index;

            groupedValue.reset();
            groupedValue.setTypeToObject();

            if ((item_ != nullptr) && item_->isObject() &&
                item_->object_.GetIndex(grouped_key_index, key_str, length)) {
                const Value *end = array_.End();

                while (item_ != end) {
                    if ((item_ != nullptr) && item_->isObject()) {
                        const VItem *obj_item = item_->object_.First();
                        const VItem *obj_end  = item_->object_.End();
                        SizeT        count    = 0;

                        sub_valu.setTypeToObject();

                        while (obj_item != obj_end) {
                            if ((obj_item != nullptr) && !(obj_item->Value.isUndefined())) {
                                if (count != grouped_key_index) {
                                    sub_valu.object_[obj_item->Key] = obj_item->Value;
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

                        Value &tmp = groupedValue.object_.Get(str, str_len);

                        if (!(tmp.isArray())) {
                            tmp.setTypeToArray();
                        }

                        tmp.array_ += QUtility::Move(sub_valu);

                        ++item_;

                        continue;
                    }

                    return false;
                }

                groupedValue.RemoveExcessStorage();

                return true;
            }
        } else if (type == ValueType::ValuePtr) {
            return value_->GroupBy(groupedValue, key_str, length);
        }

        return false;
    }

    QENTEM_INLINE bool GroupBy(Value &groupedValue, const Char_T *str) const {
        return GroupBy(groupedValue, str, StringUtils::Count(str));
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
    Stream_T &Stringify(Stream_T &stream, SizeT32 precision = QentemConfig::DoublePrecision) const {
        const ValueType type = Type();

        switch (type) {
            case ValueType::Object: {
                stringifyObject(object_, stream, precision);
                break;
            }

            case ValueType::Array: {
                stringifyArray(array_, stream, precision);
                break;
            }

            case ValueType::ValuePtr: {
                value_->Stringify(stream, precision);
                break;
            }

            default: {
            }
        }

        return stream;
    }

    QENTEM_INLINE StringT Stringify(SizeT32 precision = QentemConfig::DoublePrecision) const {
        StringStream<Char_T> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename Stream_T>
    static void stringifyObject(const ObjectT &obj, Stream_T &stream, SizeT32 precision) {
        stream.Write(NotationConstants::SCurlyChar);

        const VItem *h_item = obj.First();
        const VItem *end    = (h_item + obj.Size());

        while (h_item != end) {
            if ((h_item != nullptr) && !(h_item->Value.isUndefined())) {
                stream.Write(NotationConstants::QuoteChar);
                JSONUtils::Escape(h_item->Key.First(), h_item->Key.Length(), stream);
                stream.Write(NotationConstants::QuoteChar);
                stream.Write(NotationConstants::ColonChar);

                stringifyValue(h_item->Value, stream, precision);
                stream.Write(NotationConstants::CommaChar);
            }

            ++h_item;
        }

        Char_T *last = stream.Last();

        if ((last != nullptr) && (*last == NotationConstants::CommaChar)) {
            *last = NotationConstants::ECurlyChar;
        } else {
            stream.Write(NotationConstants::ECurlyChar);
        }
    }

    template <typename Stream_T>
    static void stringifyArray(const ArrayT &arr, Stream_T &stream, SizeT32 precision) {
        stream.Write(NotationConstants::SSquareChar);

        const Value *item = arr.First();
        const Value *end  = arr.End();

        while (item != end) {
            if (!(item->isUndefined())) {
                stringifyValue(*item, stream, precision);
                stream.Write(NotationConstants::CommaChar);
            }

            ++item;
        }

        Char_T *last = stream.Last();

        if (*last == NotationConstants::CommaChar) {
            *last = NotationConstants::ESquareChar;
        } else {
            stream.Write(NotationConstants::ESquareChar);
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
                stream.Write(NotationConstants::QuoteChar);
                JSONUtils::Escape(val.string_.First(), val.string_.Length(), stream);
                stream.Write(NotationConstants::QuoteChar);
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
                stream.Write(NotationConstants::FalseString, NotationConstants::FalseStringLength);
                break;
            }

            case ValueType::True: {
                stream.Write(NotationConstants::TrueString, NotationConstants::TrueStringLength);
                break;
            }

            case ValueType::Null: {
                stream.Write(NotationConstants::NullString, NotationConstants::NullStringLength);
                break;
            }

            case ValueType::ValuePtr: {
                stringifyValue(*(val.value_), stream, precision);
                break;
            }

            default: {
            }
        }
    }

    QENTEM_INLINE bool isUndefined() const noexcept {
        return (Type() == ValueType::Undefined);
    }

    QENTEM_INLINE bool isObject() const noexcept {
        return (Type() == ValueType::Object);
    }

    QENTEM_INLINE bool isArray() const noexcept {
        return (Type() == ValueType::Array);
    }

    QENTEM_INLINE bool isString() const noexcept {
        return (Type() == ValueType::String);
    }

    QENTEM_INLINE bool isUInt64() const noexcept {
        return (Type() == ValueType::UIntLong);
    }

    QENTEM_INLINE bool isInt64() const noexcept {
        return (Type() == ValueType::IntLong);
    }

    QENTEM_INLINE bool isDouble() const noexcept {
        return (Type() == ValueType::Double);
    }

    QENTEM_INLINE bool isTrue() const noexcept {
        return (Type() == ValueType::True);
    }

    QENTEM_INLINE bool isFalse() const noexcept {
        return (Type() == ValueType::False);
    }

    QENTEM_INLINE bool isNull() const noexcept {
        return (Type() == ValueType::Null);
    }

    QENTEM_INLINE bool isPtrValue() const noexcept {
        return (Type() == ValueType::ValuePtr);
    }

    QENTEM_INLINE void setType(ValueType type) noexcept {
        type_ = type;
    }

    QENTEM_INLINE void setTypeToUndefined() noexcept {
        setType(ValueType::Undefined);
    }

    QENTEM_INLINE void setTypeToObject() noexcept {
        setType(ValueType::Object);
    }

    QENTEM_INLINE void setTypeToArray() noexcept {
        setType(ValueType::Array);
    }

    QENTEM_INLINE void setTypeToString() noexcept {
        setType(ValueType::String);
    }

    QENTEM_INLINE void setTypeToUInt64() noexcept {
        setType(ValueType::UIntLong);
    }

    QENTEM_INLINE void setTypeToInt64() noexcept {
        setType(ValueType::IntLong);
    }

    QENTEM_INLINE void setTypeToDouble() noexcept {
        setType(ValueType::Double);
    }

    QENTEM_INLINE void setTypeToTrue() noexcept {
        setType(ValueType::True);
    }

    QENTEM_INLINE void setTypeToFalse() noexcept {
        setType(ValueType::False);
    }

    QENTEM_INLINE void setTypeToNull() noexcept {
        setType(ValueType::Null);
    }

    QENTEM_INLINE void setTypeToPtrValue() noexcept {
        setType(ValueType::ValuePtr);
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
        ArrayT       array_;
        ObjectT      object_;
        StringT      string_;
        QNumber64    number_;
        const Value *value_;
    };

    ValueType type_{ValueType::Undefined};
};

} // namespace Qentem

#endif
