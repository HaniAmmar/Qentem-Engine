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
///////////////////////////////////////////////
template <typename Number_T_, bool>
struct VNumberData;

template <typename Number_T_>
struct VNumberData<Number_T_, false> {
    // Little-Endian
    VNumberData() = default;

    VNumberData(Number_T_ number, SizeT64 ptr_number) noexcept : Number{number}, PtrNumber{ptr_number} {
    }

    template <typename Type_>
    explicit VNumberData(Type_ number) noexcept : Number{number} {
    }

    Number_T_ Number{0ULL};
    SizeT64   PtrNumber{0ULL};
};

template <typename Number_T_>
struct VNumberData<Number_T_, true> {
    // Big-Endian
    VNumberData() = default;

    VNumberData(Number_T_ number, SizeT64 ptr_number) noexcept : PtrNumber{ptr_number}, Number{number} {
    }

    template <typename Type_>
    explicit VNumberData(Type_ number) noexcept : Number{number} {
    }

    SizeT64   PtrNumber{0ULL};
    Number_T_ Number{0ULL};
};

struct VNumberT {
    VNumberT()                       = default;
    VNumberT &operator=(VNumberT &&) = default;
    VNumberT(const VNumberT &)       = default;
    ~VNumberT()                      = default;

    VNumberT(VNumberT &&v_num) noexcept : data_{v_num.data_.Number, v_num.data_.PtrNumber} {
        v_num.data_.Number.ull = 0ULL;
        v_num.data_.PtrNumber  = 0ULL;
    }

    VNumberT &operator=(const VNumberT &v_num) noexcept {
        if (this != &v_num) {
            data_.Number    = v_num.data_.Number;
            data_.PtrNumber = v_num.data_.PtrNumber;
        }

        return *this;
    }

    template <typename Number_T_>
    explicit VNumberT(const Number_T_ &num) noexcept : data_{num} {
    }

    inline void SetNumber(double num) noexcept {
        data_.Number.ddl = num;
    }

    inline void SetNumber(SizeT64 num) noexcept {
        data_.Number.ull = num;
    }

    inline void SetNumber(SizeT64I num) noexcept {
        data_.Number.sll = num;
    }

    inline SizeT64 GetUInt64() const noexcept {
        return data_.Number.ull;
    }

    inline SizeT64I GetInt64() const noexcept {
        return data_.Number.sll;
    }

    inline double GetDouble() const noexcept {
        return data_.Number.ddl;
    }

    inline void ClearAll() noexcept {
        if QENTEM_CONSTEXPR (Config::PointerTagging) {
            data_.Number.ull = 0ULL;
            data_.PtrNumber  = 0ULL;
        }
    }

  private:
    union Number_T {
        explicit Number_T(SizeT64 num) noexcept : ull{num} {
        }

        explicit Number_T(SizeT64I num) noexcept : sll{num} {
        }

        explicit Number_T(double num) noexcept : ddl{num} {
        }

        SizeT64  ull;
        SizeT64I sll;
        double   ddl;
        SizeT    padding_[2]{0}; // Just in case SizeT is set to long
    };

    VNumberData<Number_T, Config::IsBigEndian> data_{};
};
///////////////////////////////////////////////
template <typename Char_T_, typename VObjectT, typename VArrayT, typename VStringT, bool>
struct ValueData;

template <typename Char_T_, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<Char_T_, VObjectT, VArrayT, VStringT, true> {
    ValueData() noexcept : VNumber{} {
    }

    ValueData(const ValueData &)            = delete;
    ValueData &operator=(ValueData &&)      = delete;
    ValueData &operator=(const ValueData &) = delete;

    ValueData(ValueData &&val) noexcept : VNumber{Memory::Move(val.VNumber)} {
    }

    ~ValueData() {
        switch (GetType()) {
            case ValueType::Object: {
                Memory::Dispose(&VObject);
                break;
            }

            case ValueType::Array: {
                Memory::Dispose(&VArray);
                break;
            }

            case ValueType::String: {
                Memory::Dispose(&VString);
                break;
            }

            default: {
            }
        }
    }

    explicit ValueData(VObjectT &&obj) noexcept : VObject{Memory::Move(obj)} {
    }

    explicit ValueData(VArrayT &&arr) noexcept : VArray{Memory::Move(arr)} {
    }

    explicit ValueData(VStringT &&str) noexcept : VString{Memory::Move(str)} {
    }

    explicit ValueData(const VObjectT &obj) noexcept : VObject{obj} {
    }

    explicit ValueData(const VArrayT &arr) noexcept : VArray{arr} {
    }

    explicit ValueData(const VStringT &str) noexcept : VString{str} {
    }

    explicit ValueData(const Char_T_ *str, SizeT length) : VString{str, length} {
    }

    explicit ValueData(SizeT64 num) noexcept : VNumber{num} {
    }

    explicit ValueData(SizeT64I num) noexcept : VNumber{num} {
    }

    explicit ValueData(double num) noexcept : VNumber{num} {
    }

    inline ValueType GetType() const noexcept {
        return ValueType(VObject.GetHighByte());
    }

    inline void SetType(ValueType new_type) noexcept {
        VObject.SetHighByte((unsigned char)(new_type));
    }

    union {
        VObjectT VObject;
        VArrayT  VArray;
        VStringT VString;
        VNumberT VNumber;
    };
};

template <typename Char_T_, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<Char_T_, VObjectT, VArrayT, VStringT, false> {
    ValueData() noexcept : VNumber{} {
    }

    ValueData(const ValueData &)            = delete;
    ValueData &operator=(ValueData &&)      = delete;
    ValueData &operator=(const ValueData &) = delete;

    ValueData(ValueData &&val) noexcept : VNumber{Memory::Move(val.VNumber)} {
    }

    ~ValueData() {
        switch (GetType()) {
            case ValueType::Object: {
                Memory::Dispose(&VObject);
                break;
            }

            case ValueType::Array: {
                Memory::Dispose(&VArray);
                break;
            }

            case ValueType::String: {
                Memory::Dispose(&VString);
                break;
            }

            default: {
            }
        }
    }

    explicit ValueData(VObjectT &&obj) noexcept : VObject{Memory::Move(obj)} {
    }

    explicit ValueData(VArrayT &&arr) noexcept : VArray{Memory::Move(arr)} {
    }

    explicit ValueData(VStringT &&str) noexcept : VString{Memory::Move(str)} {
    }

    explicit ValueData(const VObjectT &obj) noexcept : VObject{obj} {
    }

    explicit ValueData(const VArrayT &arr) noexcept : VArray{arr} {
    }

    explicit ValueData(const VStringT &str) noexcept : VString{str} {
    }

    explicit ValueData(const Char_T_ *str, SizeT length) : VString{str, length} {
    }

    explicit ValueData(SizeT64 num) noexcept : VNumber{num} {
    }

    explicit ValueData(SizeT64I num) noexcept : VNumber{num} {
    }

    explicit ValueData(double num) noexcept : VNumber{num} {
    }

    inline ValueType GetType() const noexcept {
        return VType;
    }

    inline void SetType(ValueType new_type) noexcept {
        VType = new_type;
    }

    union {
        VObjectT VObject;
        VArrayT  VArray;
        VStringT VString;
        VNumberT VNumber;
    };

    ValueType VType{ValueType::Undefined};
};

template <typename Char_T_>
class Value {
    using JSONotation = JSONotation_T_<Char_T_>;
    using VObjectT    = HArray<Value, Char_T_>;
    using VArrayT     = Array<Value>;
    using VStringT    = String<Char_T_>;

  public:
    Value() noexcept  = default;
    ~Value() noexcept = default;

    Value(Value &&val) noexcept : data_{Memory::Move(val.data_)} {
        if (!Config::PointerTagging) {
            setType(val.Type());
            val.setTypeToUndefined();
        }
    }

    Value(const Value &val) {
        copyValue(val);
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

    explicit Value(VObjectT &&obj) noexcept : data_{Memory::Move(obj)} {
        setTypeToObject();
    }

    explicit Value(VArrayT &&arr) noexcept : data_{Memory::Move(arr)} {
        setTypeToArray();
    }

    explicit Value(VStringT &&str) noexcept : data_{Memory::Move(str)} {
        setTypeToString();
    }

    explicit Value(const VObjectT &obj) noexcept : data_{obj} {
        setTypeToObject();
    }

    explicit Value(const VArrayT &arr) noexcept : data_{arr} {
        setTypeToArray();
    }

    explicit Value(const VStringT &str) noexcept : data_{str} {
        setTypeToString();
    }

    explicit Value(const Char_T_ *str, SizeT length) : data_{str, length} {
        setTypeToString();
    }

    explicit Value(SizeT64 num) noexcept : data_{num} {
        setTypeToUInt64();
    }

    explicit Value(SizeT64I num) noexcept : data_{num} {
        setTypeToInt64();
    }

    explicit Value(double num) noexcept : data_{num} {
        setTypeToDouble();
    }

    template <typename Number_T_>
    explicit Value(Number_T_ num) noexcept {
        if QENTEM_CONSTEXPR (IsFloat<Number_T_>()) {
            data_.VNumber.SetNumber(double(num));
            setTypeToDouble();
        } else if QENTEM_CONSTEXPR (IsUnsigned<Number_T_>()) {
            data_.VNumber.SetNumber(SizeT64(num));
            setTypeToUInt64();
        } else {
            data_.VNumber.SetNumber(SizeT64I(num));
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
            const VNumberT  tmp    = val.data_.VNumber;
            const ValueType t_type = val.Type();

            if QENTEM_CONSTEXPR (Config::PointerTagging) {
                val.data_.VNumber.ClearAll();
            } else {
                val.setTypeToUndefined();
            }

            if (!IsUndefined()) {
                reset();
            }

            if QENTEM_CONSTEXPR (!Config::PointerTagging) {
                setType(t_type);
            } else {
                (void)t_type;
            }

            data_.VNumber = tmp;
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            const ValueType type = Type();

            if (type == val.Type()) {
                switch (type) {
                    case ValueType::Object: {
                        data_.VObject = val.data_.VObject;
                        break;
                    }

                    case ValueType::Array: {
                        data_.VArray = val.data_.VArray;
                        break;
                    }

                    case ValueType::String: {
                        data_.VString = val.data_.VString;
                        break;
                    }

                    case ValueType::UIntLong:
                    case ValueType::IntLong:
                    case ValueType::Double: {
                        data_.VNumber = val.data_.VNumber;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                reset();

                if QENTEM_CONSTEXPR (Config::PointerTagging) {
                    data_.VNumber.ClearAll();
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

    Value &operator=(VObjectT &&obj) noexcept {
        if (IsObject()) {
            data_.VObject = Memory::Move(obj);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(obj));
        return *this;
    }

    Value &operator=(const VObjectT &obj) {
        if (IsObject()) {
            data_.VObject = obj;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(obj);
        return *this;
    }

    Value &operator=(VArrayT &&arr) noexcept {
        if (IsArray()) {
            data_.VArray = Memory::Move(arr);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(arr));
        return *this;
    }

    Value &operator=(const VArrayT &arr) {
        if (IsArray()) {
            data_.VArray = arr;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(arr);
        return *this;
    }

    Value &operator=(VStringT &&str) noexcept {
        if (IsString()) {
            data_.VString = Memory::Move(str);
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(Memory::Move(str));
        return *this;
    }

    Value &operator=(const VStringT &str) {
        if (IsString()) {
            data_.VString = str;
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
            data_.VString = VStringT{str};
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(VStringT{str});
        return *this;
    }

    template <typename Number_T_>
    inline Value &operator=(Number_T_ num) noexcept {
        if (!IsNumber()) {
            reset();
        }

        if QENTEM_CONSTEXPR (IsFloat<Number_T_>()) {
            data_.VNumber.SetNumber(double(num));
            setTypeToDouble();
        } else if QENTEM_CONSTEXPR (IsUnsigned<Number_T_>()) {
            data_.VNumber.SetNumber(SizeT64(num));
            setTypeToUInt64();
        } else {
            data_.VNumber.SetNumber(SizeT64I(num));
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

        data_.VArray += Memory::Move(val);

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        data_.VArray += val;
    }

    void operator+=(VObjectT &&obj) {
        if (IsObject()) {
            data_.VObject += Memory::Move(obj);
        } else {
            if (!IsArray()) {
                Reset();
                initArray();
            }

            data_.VArray += Value{Memory::Move(obj)};
        }
    }

    void operator+=(const VObjectT &obj) {
        *this += VObjectT(obj);
    }

    void operator+=(VArrayT &&arr) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        if (arr.Size() != SizeT{0}) {
            data_.VArray += Memory::Move(arr);
        } else {
            data_.VArray += Value{Memory::Move(arr)};
        }
    }

    void operator+=(const VArrayT &arr) {
        (*this) += VArrayT(arr);
    }

    void operator+=(VStringT &&str) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        data_.VArray += Value{Memory::Move(str)};
    }

    void operator+=(const VStringT &str) {
        *this += VStringT(str);
    }

    void operator+=(const Char_T_ *str) {
        *this += VStringT(str);
    }

    template <typename Number_T_>
    void operator+=(Number_T_ num) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        data_.VArray += Value{num};
    }

    void operator+=(NullType) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        data_.VArray += Value{nullptr};
    }

    void operator+=(bool is_true) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        data_.VArray += Value{is_true};
    }

    Value &operator[](const Char_T_ *key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (data_.VObject[key]);
    }

    Value &operator[](VStringT &&key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (data_.VObject[Memory::Move(key)]);
    }

    Value &operator[](const VStringT &key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (data_.VObject[key]);
    }

    Value &operator[](SizeT index) {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            if (data_.VArray.Size() > index) {
                return (data_.VArray.Storage()[index]);
            }
        } else {
            if (type == ValueType::Object) {
                Value *val = data_.VObject.GetValue(index);

                if (val != nullptr) {
                    return *val;
                }
            }

            Reset();
            initArray();
        }

        if (data_.VArray.Size() == index) {
            // if (data_.VArray.Capacity() == data_.VArray.Size()) {
            //     data_.VArray.Resize((index + SizeT{1}) * SizeT{2});
            // }

            data_.VArray += Value{};
        } else {
            data_.VArray.ResizeAndInitialize(index + 1);
        }

        return (data_.VArray.Storage()[index]);
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
                    return (data_.VObject.Size() < val.data_.VObject.Size());
                }

                case ValueType::Array: {
                    return (data_.VArray.Size() < val.data_.VArray.Size());
                }

                case ValueType::String: {
                    return (data_.VString < val.data_.VString);
                }

                case ValueType::UIntLong: {
                    return (data_.VNumber.GetUInt64() < val.data_.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (data_.VNumber.GetInt64() < val.data_.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (data_.VNumber.GetDouble() < val.data_.VNumber.GetDouble());
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
                    return (data_.VObject.Size() > val.data_.VObject.Size());
                }

                case ValueType::Array: {
                    return (data_.VArray.Size() > val.data_.VArray.Size());
                }

                case ValueType::String: {
                    return (data_.VString > val.data_.VString);
                }

                case ValueType::UIntLong: {
                    return (data_.VNumber.GetUInt64() > val.data_.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (data_.VNumber.GetInt64() > val.data_.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (data_.VNumber.GetDouble() > val.data_.VNumber.GetDouble());
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
                    return (data_.VObject.Size() <= val.data_.VObject.Size());
                }

                case ValueType::Array: {
                    return (data_.VArray.Size() <= val.data_.VArray.Size());
                }

                case ValueType::String: {
                    return (data_.VString <= val.data_.VString);
                }

                case ValueType::UIntLong: {
                    return (data_.VNumber.GetUInt64() <= val.data_.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (data_.VNumber.GetInt64() <= val.data_.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (data_.VNumber.GetDouble() <= val.data_.VNumber.GetDouble());
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
                    return (data_.VObject.Size() >= val.data_.VObject.Size());
                }

                case ValueType::Array: {
                    return (data_.VArray.Size() >= val.data_.VArray.Size());
                }

                case ValueType::String: {
                    return (data_.VString >= val.data_.VString);
                }

                case ValueType::UIntLong: {
                    return (data_.VNumber.GetUInt64() >= val.data_.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (data_.VNumber.GetInt64() >= val.data_.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (data_.VNumber.GetDouble() >= val.data_.VNumber.GetDouble());
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
                    return (data_.VObject.Size() == val.data_.VObject.Size());
                }

                case ValueType::Array: {
                    return (data_.VArray.Size() == val.data_.VArray.Size());
                }

                case ValueType::String: {
                    return (data_.VString == val.data_.VString);
                }

                case ValueType::UIntLong: {
                    return (data_.VNumber.GetUInt64() == val.data_.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (data_.VNumber.GetInt64() == val.data_.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (data_.VNumber.GetDouble() == val.data_.VNumber.GetDouble());
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
                // Natural number.
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                // Integer number.
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                // Real number.
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
            return (data_.VObject.Size());
        }

        if (IsArray()) {
            return (data_.VArray.Size());
        }

        return SizeT{0};
    }

    void Merge(Value &&val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val.data_.VArray.Storage();
            const Value *end     = (src_val + val.data_.VArray.Size());

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    data_.VArray += Memory::Move(*src_val);
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            data_.VObject += Memory::Move(val.data_.VObject);
        }

        val.Reset();
    }

    void Merge(const Value &val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val.data_.VArray.Storage();
            const Value *end     = (src_val + val.data_.VArray.Size());

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    data_.VArray += *src_val;
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            data_.VObject += val.data_.VObject;
        }
    }

    Value *GetValue(SizeT index) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = data_.VObject.GetValue(index);

                if ((val != nullptr) && (!(val->IsUndefined()))) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                if (index < data_.VArray.Size()) {
                    Value *val = (data_.VArray.Storage() + index);

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
                Value *val = data_.VObject.GetValue(key, length);

                if ((val != nullptr) && !(val->IsUndefined())) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                SizeT index;
                Digit::FastStringToNumber(index, key, length);

                if (index < data_.VArray.Size()) {
                    Value *val = (data_.VArray.Storage() + index);

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

    const VStringT *GetKey(SizeT index) const noexcept {
        if (IsObject()) {
            return (data_.VObject.GetKey(index));
        }

        return nullptr;
    }

    const VObjectT *GetObject() const noexcept {
        if (IsObject()) {
            return &(data_.VObject);
        }

        return nullptr;
    }

    const VArrayT *GetArray() const noexcept {
        if (IsArray()) {
            return &(data_.VArray);
        }

        return nullptr;
    }

    const VStringT *GetString() const noexcept {
        if (IsString()) {
            return &(data_.VString);
        }

        return nullptr;
    }

    const Char_T_ *StringStorage() const noexcept {
        if (IsString()) {
            return (data_.VString.First());
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        if (IsString()) {
            return (data_.VString.Length());
        }

        return SizeT{0};
    }

    // To get a pointer to a key and its length.
    template <typename Number_T_>
    bool SetKeyCharAndLength(SizeT index, const Char_T_ *&key, Number_T_ &length) const noexcept {
        const VStringT *val = GetKey(index);

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
            const HAItem_T_<Value, Char_T_> *item = data_.VObject.GetItem(index);

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
                key    = data_.VString.First();
                length = Number_T_{data_.VString.Length()};
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

    template <typename StringStream_T_>
    bool CopyValueTo(StringStream_T_ &stream, SizeT32 precision = Config::DoublePrecision) const {
        switch (Type()) {
            case ValueType::String: {
                stream.Write(data_.VString.First(), data_.VString.Length());
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, data_.VNumber.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, data_.VNumber.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, data_.VNumber.GetDouble(), precision);
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
            const VStringT *key = data_.VObject.GetKey(index);

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
                number.Natural = (data_.VNumber.GetUInt64());
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                number.Integer = (data_.VNumber.GetInt64());
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                number.Real = (data_.VNumber.GetDouble());
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
                const SizeT       length = data_.VString.Length();
                const QNumberType n_type = Digit::StringToNumber(number, data_.VString.First(), offset, length);

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
                value = (data_.VNumber.GetUInt64() > 0);
                return true;
            }

            case ValueType::IntLong: {
                value = (data_.VNumber.GetInt64() > 0);
                return true;
            }

            case ValueType::Double: {
                value = (data_.VNumber.GetDouble() > 0);
                return true;
            }

            case ValueType::String: {
                if (data_.VString.IsEqual(JSONotation::TrueString, JSONotation::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (data_.VString.IsEqual(JSONotation::FalseString, JSONotation::FalseStringLength)) {
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
            data_.VObject.Remove(key, length);
        }
    }

    inline void Remove(const VStringT &key) const noexcept {
        Remove(key.First(), key.Length());
    }

    inline void Remove(const Char_T_ *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (IsObject()) {
            data_.VObject.RemoveIndex(index);
        } else if (IsArray() && (index < data_.VArray.Size())) {
            (data_.VArray.Storage() + index)->Reset();
        }
    }

    template <typename Number_T_>
    inline void RemoveIndex(Number_T_ index) const noexcept {
        RemoveIndex(SizeT(index));
    }

    void Reset() noexcept {
        reset();

        if QENTEM_CONSTEXPR (Config::PointerTagging) {
            data_.VNumber.ClearAll();
        } else {
            setTypeToUndefined();
        }
    }

    void Compress() {
        if (IsArray()) {
            Value       *src_val = data_.VArray.Storage();
            const Value *src_end = data_.VArray.End();
            SizeT        size    = 0;

            while (src_val < src_end) {
                if (!(src_val->IsUndefined())) {
                    ++size;
                }

                ++src_val;
            }

            if (size != data_.VArray.Capacity()) {
                if (size == SizeT{0}) {
                    data_.VArray.Reset();

                    if QENTEM_CONSTEXPR (Config::PointerTagging) {
                        setTypeToArray();
                    }

                    return;
                }

                VArrayT new_array(size);
                src_val = data_.VArray.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += Memory::Move(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                data_.VArray = Memory::Move(new_array);
            }
        } else if (IsObject()) {
            data_.VObject.Compress();
        }
    }

    inline ValueType Type() const noexcept {
        return data_.GetType();
    }

    bool GroupBy(Value &groupedValue, const Char_T_ *key, const SizeT length) const {
        using V_item_ = HAItem_T_<Value, Char_T_>;
        StringStream<Char_T_> stream;
        VObjectT              new_sub_obj;
        const Char_T_        *str     = nullptr;
        SizeT                 str_len = 0;
        SizeT                 grouped_key_index;

        if (IsArray()) {
            groupedValue.Reset();
            groupedValue.initObject();

            const Value *_item = data_.VArray.First();

            if ((_item != nullptr) && _item->IsObject() &&
                _item->data_.VObject.GetKeyIndex(grouped_key_index, key, length)) {
                const Value *end = data_.VArray.End();

                while (_item != end) {
                    if ((_item != nullptr) && _item->IsObject()) {
                        SizeT count = 0;

                        const V_item_ *obj_item = _item->data_.VObject.First();
                        const V_item_ *obj_end  = _item->data_.VObject.End();

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

                        groupedValue.data_.VObject.Get(str, str_len) += Memory::Move(new_sub_obj);

                        ++_item;
                        continue;
                    }

                    return false;
                }

                return true;
            }
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
            data_.VObject.Sort(ascend);
        } else if (type == ValueType::Array) {
            data_.VArray.Sort(ascend);
        }
    }

    template <typename Stream_T_>
    inline Stream_T_ &Stringify(Stream_T_ &stream, SizeT32 precision = Config::DoublePrecision) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            stringifyObject(data_.VObject, stream, precision);
        } else if (type == ValueType::Array) {
            stringifyArray(data_.VArray, stream, precision);
        }

        return stream;
    }

    inline VStringT Stringify(SizeT32 precision = Config::DoublePrecision) const {
        StringStream<Char_T_> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename Stream_T_>
    static void stringifyObject(const VObjectT &obj, Stream_T_ &stream, SizeT32 precision) {
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
    static void stringifyArray(const VArrayT &arr, Stream_T_ &stream, SizeT32 precision) {
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
    static void stringifyValue(const Value &val, Stream_T_ &stream, SizeT32 precision) {
        switch (val.Type()) {
            case ValueType::Object: {
                stringifyObject(val.data_.VObject, stream, precision);
                break;
            }

            case ValueType::Array: {
                stringifyArray(val.data_.VArray, stream, precision);
                break;
            }

            case ValueType::String: {
                stream += JSONotation::QuoteChar;
                EscapeJSON(val.data_.VString.First(), val.data_.VString.Length(), stream);
                stream += JSONotation::QuoteChar;
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, val.data_.VNumber.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, val.data_.VNumber.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, val.data_.VNumber.GetDouble(), precision);
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
        data_.SetType(new_type);
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
        Memory::Initialize(&(data_.VObject));
        setTypeToObject();
    }

    inline void initArray() noexcept {
        Memory::Initialize(&(data_.VArray));
        setTypeToArray();
    }

    inline void initString() noexcept {
        Memory::Initialize(&(data_.VString));
        setTypeToString();
    }

    inline void initValue(VObjectT &&obj) noexcept {
        Memory::Initialize(&(data_.VObject), Memory::Move(obj));
        setTypeToObject();
    }

    inline void initValue(const VObjectT &obj) {
        Memory::Initialize(&(data_.VObject), obj);
        setTypeToObject();
    }

    inline void initValue(VArrayT &&arr) noexcept {
        Memory::Initialize(&(data_.VArray), Memory::Move(arr));
        setTypeToArray();
    }

    inline void initValue(const VArrayT &arr) {
        Memory::Initialize(&(data_.VArray), arr);
        setTypeToArray();
    }

    inline void initValue(VStringT &&str) noexcept {
        Memory::Initialize(&(data_.VString), Memory::Move(str));
        setTypeToString();
    }

    inline void initValue(const VStringT &str) {
        Memory::Initialize(&(data_.VString), str);
        setTypeToString();
    }

    void reset() noexcept {
        switch (Type()) {
            case ValueType::Object: {
                data_.VObject.Reset();
                break;
            }

            case ValueType::Array: {
                data_.VArray.Reset();
                break;
            }

            case ValueType::String: {
                data_.VString.Reset();
                break;
            }

            default: {
            }
        }
    }

    void copyValue(const Value &val) {
        switch (val.Type()) {
            case ValueType::Object: {
                initValue(val.data_.VObject);
                break;
            }

            case ValueType::Array: {
                initValue(val.data_.VArray);
                break;
            }

            case ValueType::String: {
                initValue(val.data_.VString);
                break;
            }

            default: {
                data_.VNumber = val.data_.VNumber;
                setType(val.Type());
            }
        }
    }

    ValueData<Char_T_, VObjectT, VArrayT, VStringT, Config::PointerTagging> data_;
};

} // namespace Qentem

#endif
