/*
 * Copyright (c) 2023 Hani Ammar
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
#include "StringStream.hpp"

#include "Array.hpp"
#include "HArray.hpp"
#include "JSONUtils.hpp"

#ifndef QENTEM_VALUE_H_
#define QENTEM_VALUE_H_

namespace Qentem {

enum class ValueType : unsigned char {
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

struct VNumber {
    VNumber()                      = default;
    VNumber &operator=(VNumber &&) = default;
    VNumber(const VNumber &)       = default;
    ~VNumber()                     = default;

    VNumber(VNumber &&v_num) noexcept : number_{v_num.number_}, p_number_{v_num.p_number_} {
        v_num.number_.ull = 0;
        v_num.p_number_   = 0;
    }

    VNumber &operator=(const VNumber &v_num) noexcept {
        if (this != &v_num) {
            number_   = v_num.number_;
            p_number_ = v_num.p_number_;
        }

        return *this;
    }

    template <typename Number_T_>
    explicit VNumber(const Number_T_ &num) noexcept : number_{num} {
    }

    inline void SetNumber(double num) noexcept {
        number_.ddl = num;
    }

    inline void SetNumber(unsigned long long num) noexcept {
        number_.ull = num;
    }

    inline void SetNumber(long long num) noexcept {
        number_.sll = num;
    }

    inline unsigned long long GetUInt64() const noexcept {
        return number_.ull;
    }

    inline long long GetInt64() const noexcept {
        return number_.sll;
    }

    inline double GetDouble() const noexcept {
        return number_.ddl;
    }

    inline void ClearAll() noexcept {
        if constexpr (Config::PointerTagging) {
            number_.ull = 0;
            p_number_   = 0;
        }
    }

  private:
    union Number_T {
        explicit Number_T(unsigned long long num) noexcept : ull{num} {
        }

        explicit Number_T(long long num) noexcept : sll{num} {
        }

        explicit Number_T(double num) noexcept : ddl{num} {
        }

        unsigned long long ull;
        long long          sll;
        double             ddl;
        SizeT              padding_[2]{0}; // Just in case SizeT is set to long
    };

#ifndef QENTEM_BIG_ENDIAN
    Number_T           number_{0ULL};
    unsigned long long p_number_{0ULL};
#else
    unsigned long long p_number_{0ULL};
    Number_T           number_{0ULL};
#endif
};

template <typename Char_T_>
class Value {
    using JSONotation = JSONotation_T_<Char_T_>;
    using VObject     = HArray<Value, Char_T_>;
    using VArray      = Array<Value>;
    using VString     = String<Char_T_>;

  public:
    Value() noexcept : number_{} {
    }

    Value(Value &&val) noexcept : number_{Memory::Move(val.number_)} {
        if (!Config::PointerTagging) {
            setType(val.Type());
            val.setTypeToUndefined();
        }
    }

    Value(const Value &val) {
        copyValue(val);
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

    explicit Value(VObject &&obj) noexcept : object_{Memory::Move(obj)} {
        setTypeToObject();
    }

    explicit Value(VArray &&arr) noexcept : array_{Memory::Move(arr)} {
        setTypeToArray();
    }

    explicit Value(VString &&str) noexcept : string_{Memory::Move(str)} {
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

    explicit Value(const Char_T_ *str, SizeT length) : string_{str, length} {
        setTypeToString();
    }

    explicit Value(unsigned long long num) noexcept : number_{num} {
        setTypeToUInt64();
    }

    explicit Value(long long num) noexcept : number_{num} {
        setTypeToInt64();
    }

    explicit Value(double num) noexcept : number_{num} {
        setTypeToDouble();
    }

    template <typename Number_T_>
    explicit Value(Number_T_ num) noexcept {
        if constexpr (IsFloat<Number_T_>()) {
            number_.SetNumber(double(num));
            setTypeToDouble();
        } else if constexpr (IsUnsigned<Number_T_>()) {
            number_.SetNumber((unsigned long long)(num));
            setTypeToUInt64();
        } else {
            number_.SetNumber((long long)(num));
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
            const VNumber   tmp    = val.number_;
            const ValueType t_type = val.Type();

            if constexpr (Config::PointerTagging) {
                val.number_.ClearAll();
            } else {
                val.setTypeToUndefined();
            }

            if (!IsUndefined()) {
                reset();
            }

            if constexpr (!Config::PointerTagging) {
                setType(t_type);
            }

            number_ = tmp;
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            const ValueType type = Type();

            if (type == val.Type()) {
                switch (type) {
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

                    case ValueType::UIntLong:
                    case ValueType::IntLong:
                    case ValueType::Double: {
                        number_ = val.number_;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                reset();

                if constexpr (Config::PointerTagging) {
                    number_.ClearAll();
                } else {
                    setTypeToUndefined();
                }

                if (!(val.IsUndefined())) {
                    copyValue(val);
                }
            }
        }

        return *this;
    }

    Value &operator=(VObject &&obj) noexcept {
        if (IsObject()) {
            object_ = Memory::Move(obj);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(obj));
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
        return *this;
    }

    Value &operator=(VArray &&arr) noexcept {
        if (IsArray()) {
            array_ = Memory::Move(arr);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(arr));
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
        return *this;
    }

    Value &operator=(VString &&str) noexcept {
        if (IsString()) {
            string_ = Memory::Move(str);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(str));
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
        return *this;
    }

    template <typename Number_T_>
    inline Value &operator=(Number_T_ num) noexcept {
        if (!IsNumber()) {
            reset();
        }

        if constexpr (IsFloat<Number_T_>()) {
            number_.SetNumber(double(num));
            setTypeToDouble();
        } else if constexpr (IsUnsigned<Number_T_>()) {
            number_.SetNumber((unsigned long long)(num));
            setTypeToUInt64();
        } else {
            number_.SetNumber((long long)(num));
            setTypeToInt64();
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
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += Memory::Move(val);

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += val;
    }

    void operator+=(VObject &&obj) {
        if (IsObject()) {
            object_ += Memory::Move(obj);
        } else {
            if (!IsArray()) {
                Reset();
                initArray();
            }

            array_ += Value{Memory::Move(obj)};
        }
    }

    void operator+=(const VObject &obj) {
        *this += VObject(obj);
    }

    void operator+=(VArray &&arr) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        if (arr.Size() != 0) {
            array_ += Memory::Move(arr);
        } else {
            array_ += Value{Memory::Move(arr)};
        }
    }

    void operator+=(const VArray &arr) {
        (*this) += VArray(arr);
    }

    void operator+=(VString &&str) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += Value{Memory::Move(str)};
    }

    void operator+=(const VString &str) {
        *this += VString(str);
    }

    void operator+=(const Char_T_ *str) {
        *this += VString(str);
    }

    template <typename Number_T_>
    void operator+=(Number_T_ num) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += Value{num};
    }

    void operator+=(NullType) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += Value{nullptr};
    }

    void operator+=(bool is_true) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        array_ += Value{is_true};
    }

    Value &operator[](const Char_T_ *key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return object_[key];
    }

    Value &operator[](VString &&key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return object_[Memory::Move(key)];
    }

    Value &operator[](const VString &key) {
        if (!IsObject()) {
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
        return (*this)[SizeT(index)];
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
                    return (number_.GetUInt64() < val.number_.GetUInt64());
                }

                case ValueType::IntLong: {
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

                case ValueType::UIntLong: {
                    return (number_.GetUInt64() > val.number_.GetUInt64());
                }

                case ValueType::IntLong: {
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
                    return (number_.GetUInt64() <= val.number_.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (number_.GetInt64() <= val.number_.GetInt64());
                }

                case ValueType::Double: {
                    return (number_.GetDouble() <= val.number_.GetDouble());
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
                    return (number_.GetUInt64() >= val.number_.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (number_.GetInt64() >= val.number_.GetInt64());
                }

                case ValueType::Double: {
                    return (number_.GetDouble() >= val.number_.GetDouble());
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
                    return (number_.GetUInt64() == val.number_.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (number_.GetInt64() == val.number_.GetInt64());
                }

                case ValueType::Double: {
                    return (number_.GetDouble() == val.number_.GetDouble());
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

    inline unsigned int GetNumberType() const noexcept {
        switch (Type()) {
            case ValueType::UIntLong: {
                // Natural number.
                return 1U;
            }

            case ValueType::IntLong: {
                // Integer number.
                return 2U;
            }

            case ValueType::Double: {
                // Real number.
                return 3U;
            }

            default: {
                // NAN: Not A Number
                return 0;
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
            Value       *src_val = val.array_.Storage();
            const Value *end     = (src_val + val.array_.Size());

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
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val.array_.Storage();
            const Value *end     = (src_val + val.array_.Size());

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

    Value *GetValue(const Char_T_ *key, SizeT length) const noexcept {
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
    bool SetKeyCharAndLength(SizeT index, const Char_T_ *&key, Number_T_ &length) const noexcept {
        const VString *val = GetKey(index);

        if (val != nullptr) {
            key    = val->First();
            length = Number_T_(val->Length());
            return true;
        }

        return false;
    }

    template <typename Number_T_>
    void SetValueKeyLength(SizeT index, const Value *&value, const Char_T_ *&key, Number_T_ &length) const noexcept {
        if (IsObject()) {
            const HAItem_T_<Value, Char_T_> *item = object_.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.IsUndefined())) {
                value  = &(item->Value);
                key    = item->Key.First();
                length = Number_T_(item->Key.Length());
            }
        }
    }

    // To get a pointer to a string value and its length.
    template <typename Number_T_>
    bool SetCharAndLength(const Char_T_ *&key, Number_T_ &length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                key    = string_.First();
                length = Number_T_{string_.Length()};
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

    bool SetString(VString &value, unsigned int precision = Config::FloatDoublePrecision) const {
        switch (Type()) {
            case ValueType::String: {
                value = string_;
                break;
            }

            case ValueType::UIntLong: {
                value.Reset();
                Digit::NumberToString(value, number_.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                value.Reset();
                Digit::NumberToString(value, number_.GetInt64());
                break;
            }

            case ValueType::Double: {
                value.Reset();
                Digit::NumberToString(value, number_.GetDouble(), precision);
                break;
            }

            case ValueType::True: {
                value = VString(JSONotation::TrueString, JSONotation::TrueStringLength);
                break;
            }

            case ValueType::False: {
                value = VString(JSONotation::FalseString, JSONotation::FalseStringLength);
                break;
            }

            case ValueType::Null: {
                value = VString(JSONotation::NullString, JSONotation::NullStringLength);
                break;
            }

            default: {
                return false;
            }
        }

        return true;
    }

    template <typename StringStream_T_>
    bool CopyStringValueTo(StringStream_T_ &stream, unsigned int precision = Config::FloatDoublePrecision) const {
        switch (Type()) {
            case ValueType::String: {
                stream.Write(string_.First(), string_.Length());
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, number_.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, number_.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, number_.GetDouble(), precision);
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

    template <typename StringStream_T_>
    bool CopyKeyByIndexTo(StringStream_T_ &stream, SizeT index) const {
        if (IsObject()) {
            const VString *key = object_.GetKey(index);

            if (key != nullptr) {
                stream += *key;
            }

            return true;
        }

        return false;
    }

    unsigned long long GetUInt64() const noexcept {
        QNumber number;

        switch (SetNumber(number)) {
            case QNumberType::Natural:
            case QNumberType::Integer: {
                return number.Natural;
            }

            case QNumberType::Real: {
                return QNumber{(long long)(number.Real)}.Natural;
            }

            default: {
                // TODO: try to convert string to number
                return 0;
            }
        };
    }

    long long GetInt64() const noexcept {
        QNumber number;

        switch (SetNumber(number)) {
            case QNumberType::Natural:
            case QNumberType::Integer: {
                return number.Integer;
            }

            case QNumberType::Real: {
                return (long long)(number.Real);
            }

            default: {
                // TODO: try to convert string to number
                return 0;
            }
        };
    }

    double GetDouble() const noexcept {
        QNumber number;

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
                // TODO: try to convert string to number
                return 0.0;
            }
        };
    }

    double GetNumber() const noexcept {
        return GetDouble();
    }

    QNumberType SetNumber(QNumber &number) const noexcept {
        switch (Type()) {
            case ValueType::UIntLong: {
                number.Natural = number_.GetUInt64();
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                number.Integer = number_.GetInt64();
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                number.Real = number_.GetDouble();
                return QNumberType::Real;
            }

            case ValueType::True: {
                number.Natural = 1;
                return QNumberType::Natural;
            }

            case ValueType::False:
            case ValueType::Null: {
                number.Natural = 0;
                return QNumberType::Natural;
            }

            case ValueType::String: {
                SizeT             offset = 0;
                const SizeT       length = string_.Length();
                const QNumberType n_type = Digit::StringToNumber(number, string_.First(), offset, string_.Length());

                if (offset == length) {
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
                value = (number_.GetUInt64() > 0);
                return true;
            }

            case ValueType::IntLong: {
                value = (number_.GetInt64() > 0);
                return true;
            }

            case ValueType::Double: {
                value = (number_.GetDouble() > 0);
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

    inline void Remove(const Char_T_ *key, SizeT length) const noexcept {
        if (IsObject()) {
            object_.Remove(key, length);
        }
    }

    inline void Remove(const VString &key) const noexcept {
        Remove(key.First(), key.Length());
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
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
        RemoveIndex(SizeT(index));
    }

    void Reset() noexcept {
        reset();

        if constexpr (Config::PointerTagging) {
            number_.ClearAll();
        } else {
            setTypeToUndefined();
        }
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
                if (size == 0) {
                    array_.Reset();

                    if constexpr (Config::PointerTagging) {
                        setTypeToArray();
                    }

                    return;
                }

                VArray new_array(size);
                src_val = array_.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += Memory::Move(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                array_ = Memory::Move(new_array);
            }
        } else if (IsObject()) {
            object_.Compress();
        }
    }

    inline ValueType Type() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        return ValueType(type_.Storage_.GetHighByte());
#else
        return type_;
#endif
    }

    bool GroupBy(Value &groupedValue, const Char_T_ *key, const SizeT length) const {
        using V_item_ = HAItem_T_<Value, Char_T_>;
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
                for (const V_item_ *obj_item = _item->object_.First(), *obj_end = _item->object_.End();
                     obj_item != obj_end; obj_item++) {
                    if ((obj_item == nullptr) || obj_item->Value.IsUndefined()) {
                        return false;
                    }

                    if (count != grouped_key_index) {
                        new_sub_obj[obj_item->Key] = obj_item->Value;
                    } else if (!obj_item->Value.SetString(grouped_key)) {
                        return false;
                    }

                    ++count;
                }

                groupedValue.object_[Memory::Move(grouped_key)] += Memory::Move(new_sub_obj);
            }

            return true;
        }

        return false;
    }

    bool GroupBy(Value &groupedValue, const Char_T_ *key) const {
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

    template <typename Stream_T_>
    inline Stream_T_ &Stringify(Stream_T_ &stream, unsigned int precision = Config::FloatDoublePrecision) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            stringifyObject(object_, stream, precision);
        } else if (type == ValueType::Array) {
            stringifyArray(array_, stream, precision);
        }

        return stream;
    }

    inline VString Stringify(unsigned int precision = Config::FloatDoublePrecision) const {
        StringStream<Char_T_> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename Stream_T_>
    static void stringifyObject(const VObject &obj, Stream_T_ &stream, unsigned int precision) {
        using V_item_ = HAItem_T_<Value, Char_T_>;

        stream += JSONotation::SCurlyChar;

        for (const V_item_ *h_item = obj.First(), *end = (h_item + obj.Size()); h_item != end; h_item++) {
            if ((h_item != nullptr) && !(h_item->Value.IsUndefined())) {
                stream += JSONotation::QuoteChar;
                EscapeJSON(h_item->Key.First(), h_item->Key.Length(), stream);
                stream += JSONotation::QuoteChar;
                stream += JSONotation::ColonChar;

                stringifyValue(h_item->Value, stream, precision);
                stream += JSONotation::CommaChar;
            }
        }

        Char_T_ *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ECurlyChar;
        } else {
            stream += JSONotation::ECurlyChar;
        }
    }

    template <typename Stream_T_>
    static void stringifyArray(const VArray &arr, Stream_T_ &stream, unsigned int precision) {
        stream += JSONotation::SSquareChar;

        for (const Value *item = arr.First(), *end = (item + arr.Size()); item != end; item++) {
            if (!(item->IsUndefined())) {
                stringifyValue(*item, stream, precision);
                stream += JSONotation::CommaChar;
            }
        }

        Char_T_ *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ESquareChar;
        } else {
            stream += JSONotation::ESquareChar;
        }
    }

    template <typename Stream_T_>
    static void stringifyValue(const Value &val, Stream_T_ &stream, unsigned int precision) {
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
                EscapeJSON(val.string_.First(), val.string_.Length(), stream);
                stream += JSONotation::QuoteChar;
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, val.number_.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, val.number_.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, val.number_.GetDouble(), precision);
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

    inline void setType(ValueType new_type) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        type_.Storage_.SetHighByte((unsigned char)(new_type));
#else
        type_ = new_type;
#endif
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

    inline void initObject() noexcept {
        Memory::Initialize(&object_);
        setTypeToObject();
    }

    inline void initArray() noexcept {
        Memory::Initialize(&array_);
        setTypeToArray();
    }

    inline void initString() noexcept {
        Memory::Initialize(&string_);
        setTypeToString();
    }

    inline void initValue(VObject &&obj) noexcept {
        Memory::Initialize(&object_, Memory::Move(obj));
        setTypeToObject();
    }

    inline void initValue(const VObject &obj) {
        Memory::Initialize(&object_, obj);
        setTypeToObject();
    }

    inline void initValue(VArray &&arr) noexcept {
        Memory::Initialize(&array_, Memory::Move(arr));
        setTypeToArray();
    }

    inline void initValue(const VArray &arr) {
        Memory::Initialize(&array_, arr);
        setTypeToArray();
    }

    inline void initValue(VString &&str) noexcept {
        Memory::Initialize(&string_, Memory::Move(str));
        setTypeToString();
    }

    inline void initValue(const VString &str) {
        Memory::Initialize(&string_, str);
        setTypeToString();
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
                setType(val.Type());
            }
        }
    }

    struct VType {
#ifndef QENTEM_BIG_ENDIAN
      private:
        SizeT index_capacity_[2]{0};

      public:
        QPointer<void> Storage_{};
#else
      public:
        QPointer<void> Storage_{};

      private:
        SizeT index_capacity_[2]{0};
#endif
    };

    union {
        VObject object_;
        VArray  array_;
        VString string_;
        VNumber number_;
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        VType type_;
#endif
    };

#if !defined(QENTEM_POINTER_TAGGING) || (QENTEM_POINTER_TAGGING != 1)
    ValueType type_{ValueType::Undefined};
#endif
};

} // namespace Qentem

#endif
