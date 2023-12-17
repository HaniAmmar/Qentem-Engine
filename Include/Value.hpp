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

#ifndef _QENTEM_VALUE_H
#define _QENTEM_VALUE_H

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
template <typename _Number_T, bool>
struct VNumberData;

template <typename _Number_T>
struct VNumberData<_Number_T, false> {
    // Little-Endian
    VNumberData() = default;

    VNumberData(_Number_T number, SizeT64 ptr_number) noexcept : Number{number}, PtrNumber{ptr_number} {
    }

    template <typename _Type>
    explicit VNumberData(_Type number) noexcept : Number{number} {
    }

    _Number_T Number{0ULL};
    SizeT64   PtrNumber{0ULL};
};

template <typename _Number_T>
struct VNumberData<_Number_T, true> {
    // Big-Endian
    VNumberData() = default;

    VNumberData(_Number_T number, SizeT64 ptr_number) noexcept : PtrNumber{ptr_number}, Number{number} {
    }

    template <typename _Type>
    explicit VNumberData(_Type number) noexcept : Number{number} {
    }

    SizeT64   PtrNumber{0ULL};
    _Number_T Number{0ULL};
};

struct VNumberT {
    VNumberT()                       = default;
    VNumberT &operator=(VNumberT &&) = default;
    VNumberT(const VNumberT &)       = default;
    ~VNumberT()                      = default;

    VNumberT(VNumberT &&v_num) noexcept : _data{v_num._data.Number, v_num._data.PtrNumber} {
        v_num._data.Number.ull = 0ULL;
        v_num._data.PtrNumber  = 0ULL;
    }

    VNumberT &operator=(const VNumberT &v_num) noexcept {
        if (this != &v_num) {
            _data.Number    = v_num._data.Number;
            _data.PtrNumber = v_num._data.PtrNumber;
        }

        return *this;
    }

    template <typename _Number_T>
    explicit VNumberT(const _Number_T &num) noexcept : _data{num} {
    }

    inline void SetNumber(double num) noexcept {
        _data.Number.ddl = num;
    }

    inline void SetNumber(SizeT64 num) noexcept {
        _data.Number.ull = num;
    }

    inline void SetNumber(SizeT64I num) noexcept {
        _data.Number.sll = num;
    }

    inline SizeT64 GetUInt64() const noexcept {
        return _data.Number.ull;
    }

    inline SizeT64I GetInt64() const noexcept {
        return _data.Number.sll;
    }

    inline double GetDouble() const noexcept {
        return _data.Number.ddl;
    }

    inline void ClearAll() noexcept {
        if (Config::PointerTagging) {
            _data.Number.ull = 0ULL;
            _data.PtrNumber  = 0ULL;
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
        SizeT    _padding[2]{0}; // Just in case SizeT is set to long
    };

    VNumberData<Number_T, Config::IsBigEndian> _data{};
};
///////////////////////////////////////////////
template <typename _Char_T, typename VObjectT, typename VArrayT, typename VStringT, bool>
struct ValueData;

template <typename _Char_T, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<_Char_T, VObjectT, VArrayT, VStringT, true> {
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

    explicit ValueData(const _Char_T *str, SizeT length) : VString{str, length} {
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
        VObject.SetHighByte(SizeT8(new_type));
    }

    union {
        VObjectT VObject;
        VArrayT  VArray;
        VStringT VString;
        VNumberT VNumber;
    };
};

template <typename _Char_T, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<_Char_T, VObjectT, VArrayT, VStringT, false> {
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

    explicit ValueData(const _Char_T *str, SizeT length) : VString{str, length} {
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

template <typename _Char_T>
struct Value {
    using JSONotation = _JSONotation_T<_Char_T>;
    using VObjectT    = HArray<Value, _Char_T>;
    using VArrayT     = Array<Value>;
    using VStringT    = String<_Char_T>;

    Value() noexcept  = default;
    ~Value() noexcept = default;

    Value(Value &&val) noexcept : _data{Memory::Move(val._data)} {
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

    explicit Value(VObjectT &&obj) noexcept : _data{Memory::Move(obj)} {
        setTypeToObject();
    }

    explicit Value(VArrayT &&arr) noexcept : _data{Memory::Move(arr)} {
        setTypeToArray();
    }

    explicit Value(VStringT &&str) noexcept : _data{Memory::Move(str)} {
        setTypeToString();
    }

    explicit Value(const VObjectT &obj) noexcept : _data{obj} {
        setTypeToObject();
    }

    explicit Value(const VArrayT &arr) noexcept : _data{arr} {
        setTypeToArray();
    }

    explicit Value(const VStringT &str) noexcept : _data{str} {
        setTypeToString();
    }

    explicit Value(const _Char_T *str, SizeT length) : _data{str, length} {
        setTypeToString();
    }

    explicit Value(SizeT64 num) noexcept : _data{num} {
        setTypeToUInt64();
    }

    explicit Value(SizeT64I num) noexcept : _data{num} {
        setTypeToInt64();
    }

    explicit Value(double num) noexcept : _data{num} {
        setTypeToDouble();
    }

    template <typename _Number_T>
    explicit Value(_Number_T num) noexcept {
        if (IsFloat<_Number_T>()) {
            _data.VNumber.SetNumber(double(num));
            setTypeToDouble();
        } else if (IsUnsigned<_Number_T>()) {
            _data.VNumber.SetNumber(SizeT64(num));
            setTypeToUInt64();
        } else {
            _data.VNumber.SetNumber(SizeT64I(num));
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
            const VNumberT  tmp    = val._data.VNumber;
            const ValueType t_type = val.Type();

            if (Config::PointerTagging) {
                val._data.VNumber.ClearAll();
            } else {
                val.setTypeToUndefined();
            }

            if (!IsUndefined()) {
                reset();
            }

            if (!Config::PointerTagging) {
                setType(t_type);
            } else {
                (void)t_type;
            }

            _data.VNumber = tmp;
        }

        return *this;
    }

    Value &operator=(const Value &val) {
        if (this != &val) {
            const ValueType type = Type();

            if (type == val.Type()) {
                switch (type) {
                    case ValueType::Object: {
                        _data.VObject = val._data.VObject;
                        break;
                    }

                    case ValueType::Array: {
                        _data.VArray = val._data.VArray;
                        break;
                    }

                    case ValueType::String: {
                        _data.VString = val._data.VString;
                        break;
                    }

                    case ValueType::UIntLong:
                    case ValueType::IntLong:
                    case ValueType::Double: {
                        _data.VNumber = val._data.VNumber;
                        break;
                    }

                    default: {
                    }
                }
            } else {
                reset();

                if (Config::PointerTagging) {
                    _data.VNumber.ClearAll();
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
            _data.VObject = Memory::Move(obj);
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
            _data.VObject = obj;
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
            _data.VArray = Memory::Move(arr);
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
            _data.VArray = arr;
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
            _data.VString = Memory::Move(str);
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
            _data.VString = str;
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(str);
        return *this;
    }

    Value &operator=(const _Char_T *str) {
        if (IsString()) {
            _data.VString = VStringT{str};
            return *this;
        }

        if (!IsUndefined()) {
            reset();
        }

        initValue(VStringT{str});
        return *this;
    }

    template <typename _Number_T>
    inline Value &operator=(_Number_T num) noexcept {
        if (!IsNumber()) {
            reset();
        }

        if (IsFloat<_Number_T>()) {
            _data.VNumber.SetNumber(double(num));
            setTypeToDouble();
        } else if (IsUnsigned<_Number_T>()) {
            _data.VNumber.SetNumber(SizeT64(num));
            setTypeToUInt64();
        } else {
            _data.VNumber.SetNumber(SizeT64I(num));
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

        _data.VArray += Memory::Move(val);

        val.Reset();
    }

    void operator+=(const Value &val) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        _data.VArray += val;
    }

    void operator+=(VObjectT &&obj) {
        if (IsObject()) {
            _data.VObject += Memory::Move(obj);
        } else {
            if (!IsArray()) {
                Reset();
                initArray();
            }

            _data.VArray += Value{Memory::Move(obj)};
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
            _data.VArray += Memory::Move(arr);
        } else {
            _data.VArray += Value{Memory::Move(arr)};
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

        _data.VArray += Value{Memory::Move(str)};
    }

    void operator+=(const VStringT &str) {
        *this += VStringT(str);
    }

    void operator+=(const _Char_T *str) {
        *this += VStringT(str);
    }

    template <typename _Number_T>
    void operator+=(_Number_T num) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        _data.VArray += Value{num};
    }

    void operator+=(NullType) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        _data.VArray += Value{nullptr};
    }

    void operator+=(bool is_true) {
        if (!IsArray()) {
            Reset();
            initArray();
        }

        _data.VArray += Value{is_true};
    }

    Value &operator[](const _Char_T *key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (_data.VObject[key]);
    }

    Value &operator[](VStringT &&key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (_data.VObject[Memory::Move(key)]);
    }

    Value &operator[](const VStringT &key) {
        if (!IsObject()) {
            Reset();
            initObject();
        }

        return (_data.VObject[key]);
    }

    Value &operator[](SizeT index) {
        const ValueType type = Type();

        if (type == ValueType::Array) {
            if (_data.VArray.Size() > index) {
                return (_data.VArray.Storage()[index]);
            }
        } else {
            if (type == ValueType::Object) {
                Value *val = _data.VObject.GetValue(index);

                if (val != nullptr) {
                    return *val;
                }
            }

            Reset();
            initArray();
        }

        if (_data.VArray.Size() == index) {
            // if (_data.VArray.Capacity() == _data.VArray.Size()) {
            //     _data.VArray.Resize((index + SizeT{1}) * SizeT{2});
            // }

            _data.VArray += Value{};
        } else {
            _data.VArray.ResizeAndInitialize(index + 1);
        }

        return (_data.VArray.Storage()[index]);
    }

    template <typename _Type_T>
    Value &operator[](_Type_T index) {
        return (*this)[SizeT(index)];
    }

    inline bool operator<(const Value &val) const noexcept {
        const ValueType type = Type();

        if (type == val.Type()) {
            switch (type) {
                case ValueType::Object: {
                    return (_data.VObject.Size() < val._data.VObject.Size());
                }

                case ValueType::Array: {
                    return (_data.VArray.Size() < val._data.VArray.Size());
                }

                case ValueType::String: {
                    return (_data.VString < val._data.VString);
                }

                case ValueType::UIntLong: {
                    return (_data.VNumber.GetUInt64() < val._data.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (_data.VNumber.GetInt64() < val._data.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (_data.VNumber.GetDouble() < val._data.VNumber.GetDouble());
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
                    return (_data.VObject.Size() > val._data.VObject.Size());
                }

                case ValueType::Array: {
                    return (_data.VArray.Size() > val._data.VArray.Size());
                }

                case ValueType::String: {
                    return (_data.VString > val._data.VString);
                }

                case ValueType::UIntLong: {
                    return (_data.VNumber.GetUInt64() > val._data.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (_data.VNumber.GetInt64() > val._data.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (_data.VNumber.GetDouble() > val._data.VNumber.GetDouble());
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
                    return (_data.VObject.Size() <= val._data.VObject.Size());
                }

                case ValueType::Array: {
                    return (_data.VArray.Size() <= val._data.VArray.Size());
                }

                case ValueType::String: {
                    return (_data.VString <= val._data.VString);
                }

                case ValueType::UIntLong: {
                    return (_data.VNumber.GetUInt64() <= val._data.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (_data.VNumber.GetInt64() <= val._data.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (_data.VNumber.GetDouble() <= val._data.VNumber.GetDouble());
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
                    return (_data.VObject.Size() >= val._data.VObject.Size());
                }

                case ValueType::Array: {
                    return (_data.VArray.Size() >= val._data.VArray.Size());
                }

                case ValueType::String: {
                    return (_data.VString >= val._data.VString);
                }

                case ValueType::UIntLong: {
                    return (_data.VNumber.GetUInt64() >= val._data.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (_data.VNumber.GetInt64() >= val._data.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (_data.VNumber.GetDouble() >= val._data.VNumber.GetDouble());
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
                    return (_data.VObject.Size() == val._data.VObject.Size());
                }

                case ValueType::Array: {
                    return (_data.VArray.Size() == val._data.VArray.Size());
                }

                case ValueType::String: {
                    return (_data.VString == val._data.VString);
                }

                case ValueType::UIntLong: {
                    return (_data.VNumber.GetUInt64() == val._data.VNumber.GetUInt64());
                }

                case ValueType::IntLong: {
                    return (_data.VNumber.GetInt64() == val._data.VNumber.GetInt64());
                }

                case ValueType::Double: {
                    return (_data.VNumber.GetDouble() == val._data.VNumber.GetDouble());
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
            return (_data.VObject.Size());
        }

        if (IsArray()) {
            return (_data.VArray.Size());
        }

        return SizeT{0};
    }

    void Merge(Value &&val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val._data.VArray.Storage();
            const Value *end     = (src_val + val._data.VArray.Size());

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    _data.VArray += Memory::Move(*src_val);
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            _data.VObject += Memory::Move(val._data.VObject);
        }

        val.Reset();
    }

    void Merge(const Value &val) {
        if (IsUndefined()) {
            initArray();
        }

        if (IsArray() && val.IsArray()) {
            Value       *src_val = val._data.VArray.Storage();
            const Value *end     = (src_val + val._data.VArray.Size());

            while (src_val < end) {
                if (!(src_val->IsUndefined())) {
                    _data.VArray += *src_val;
                }

                ++src_val;
            }
        } else if (IsObject() && val.IsObject()) {
            _data.VObject += val._data.VObject;
        }
    }

    Value *GetValue(SizeT index) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = _data.VObject.GetValue(index);

                if ((val != nullptr) && (!(val->IsUndefined()))) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                if (index < _data.VArray.Size()) {
                    Value *val = (_data.VArray.Storage() + index);

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

    Value *GetValue(const _Char_T *key, SizeT length) const noexcept {
        switch (Type()) {
            case ValueType::Object: {
                Value *val = _data.VObject.GetValue(key, length);

                if ((val != nullptr) && !(val->IsUndefined())) {
                    return val;
                }

                return nullptr;
            }

            case ValueType::Array: {
                SizeT index;
                Digit::FastStringToNumber(index, key, length);

                if (index < _data.VArray.Size()) {
                    Value *val = (_data.VArray.Storage() + index);

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
            return (_data.VObject.GetKey(index));
        }

        return nullptr;
    }

    const VObjectT *GetObject() const noexcept {
        if (IsObject()) {
            return &(_data.VObject);
        }

        return nullptr;
    }

    const VArrayT *GetArray() const noexcept {
        if (IsArray()) {
            return &(_data.VArray);
        }

        return nullptr;
    }

    const VStringT *GetString() const noexcept {
        if (IsString()) {
            return &(_data.VString);
        }

        return nullptr;
    }

    const _Char_T *StringStorage() const noexcept {
        if (IsString()) {
            return (_data.VString.First());
        }

        return nullptr;
    }

    SizeT Length() const noexcept {
        if (IsString()) {
            return (_data.VString.Length());
        }

        return SizeT{0};
    }

    // To get a pointer to a key and its length.
    template <typename _Number_T>
    bool SetKeyCharAndLength(SizeT index, const _Char_T *&key, _Number_T &length) const noexcept {
        const VStringT *val = GetKey(index);

        if (val != nullptr) {
            const SizeT len = val->Length();
            key             = val->Storage(len);
            length          = _Number_T(len);
            return true;
        }

        return false;
    }

    template <typename _Number_T>
    void SetValueKeyLength(SizeT index, const Value *&value, const _Char_T *&key, _Number_T &length) const noexcept {
        if (IsObject()) {
            const _HAItem_T<Value, _Char_T> *item = _data.VObject.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.IsUndefined())) {
                const SizeT len = item->Key.Length();
                value           = &(item->Value);
                key             = item->Key.Storage(len);
                length          = _Number_T(len);
            }
        }
    }

    // To get a pointer to a string value and its length.
    template <typename _Number_T>
    bool SetCharAndLength(const _Char_T *&key, _Number_T &length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                const SizeT len = _data.VString.Length();
                key             = _data.VString.Storage(len);
                length          = _Number_T{len};
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

    template <typename _StringStream_T>
    using _CopyValueToStringFunction_T = void(_StringStream_T, const _Char_T *, SizeT);

    template <typename _StringStream_T, typename _StringFunction_T = _CopyValueToStringFunction_T<_StringStream_T>>
    bool CopyValueTo(_StringStream_T &stream, SizeT32 precision = Config::DoublePrecision,
                     _StringFunction_T *string_function = nullptr) const {
        switch (Type()) {
            case ValueType::String: {
                const SizeT len = _data.VString.Length();

                if (string_function != nullptr) {
                    string_function(stream, _data.VString.Storage(len), len);
                } else {
                    stream.Write(_data.VString.Storage(len), len);
                }

                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, _data.VNumber.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, _data.VNumber.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, _data.VNumber.GetDouble(), precision);
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

    template <typename _StringStream_T>
    bool CopyKeyByIndexTo(_StringStream_T &stream, SizeT index) const {
        if (IsObject()) {
            const VStringT *key = _data.VObject.GetKey(index);

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
                number.Natural = (_data.VNumber.GetUInt64());
                return QNumberType::Natural;
            }

            case ValueType::IntLong: {
                number.Integer = (_data.VNumber.GetInt64());
                return QNumberType::Integer;
            }

            case ValueType::Double: {
                number.Real = (_data.VNumber.GetDouble());
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
                const SizeT       length = _data.VString.Length();
                const QNumberType n_type = Digit::StringToNumber(number, _data.VString.Storage(length), offset, length);

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
                value = (_data.VNumber.GetUInt64() > 0);
                return true;
            }

            case ValueType::IntLong: {
                value = (_data.VNumber.GetInt64() > 0);
                return true;
            }

            case ValueType::Double: {
                value = (_data.VNumber.GetDouble() > 0);
                return true;
            }

            case ValueType::String: {
                if (_data.VString.IsEqual(JSONotation::TrueString, JSONotation::TrueStringLength)) {
                    value = true;
                    return true;
                }

                if (_data.VString.IsEqual(JSONotation::FalseString, JSONotation::FalseStringLength)) {
                    value = false;
                    return true;
                }
            }

            default: {
            }
        }

        return false;
    }

    inline void Remove(const _Char_T *key, SizeT length) const noexcept {
        if (IsObject()) {
            _data.VObject.Remove(key, length);
        }
    }

    inline void Remove(const VStringT &key) const noexcept {
        const SizeT length = _data.VString.Length();
        Remove(key.Storage(length), length);
    }

    inline void Remove(const _Char_T *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (IsObject()) {
            _data.VObject.RemoveIndex(index);
        } else if (IsArray() && (index < _data.VArray.Size())) {
            (_data.VArray.Storage() + index)->Reset();
        }
    }

    template <typename _Number_T>
    inline void RemoveIndex(_Number_T index) const noexcept {
        RemoveIndex(SizeT(index));
    }

    void Reset() noexcept {
        reset();

        if (Config::PointerTagging) {
            _data.VNumber.ClearAll();
        } else {
            setTypeToUndefined();
        }
    }

    void Compress() {
        if (IsArray()) {
            Value       *src_val = _data.VArray.Storage();
            const Value *src_end = _data.VArray.End();
            SizeT        size    = 0;

            while (src_val < src_end) {
                if (!(src_val->IsUndefined())) {
                    ++size;
                }

                ++src_val;
            }

            if (size != _data.VArray.Capacity()) {
                if (size == SizeT{0}) {
                    _data.VArray.Reset();

                    if (Config::PointerTagging) {
                        setTypeToArray();
                    }

                    return;
                }

                VArrayT new_array(size);
                src_val = _data.VArray.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += Memory::Move(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                _data.VArray = Memory::Move(new_array);
            }
        } else if (IsObject()) {
            _data.VObject.Compress();
        }
    }

    inline ValueType Type() const noexcept {
        return _data.GetType();
    }

    bool GroupBy(Value &groupedValue, const _Char_T *key, const SizeT length) const {
        using _V_item = _HAItem_T<Value, _Char_T>;
        StringStream<_Char_T> stream;
        VObjectT              new_sub_obj;
        const _Char_T        *str     = nullptr;
        SizeT                 str_len = 0;
        SizeT                 grouped_key_index;

        if (IsArray()) {
            groupedValue.Reset();
            groupedValue.initObject();

            const Value *_item = _data.VArray.First();

            if ((_item != nullptr) && _item->IsObject() &&
                _item->_data.VObject.GetKeyIndex(grouped_key_index, key, length)) {
                const Value *end = _data.VArray.End();

                while (_item != end) {
                    if ((_item != nullptr) && _item->IsObject()) {
                        SizeT count = 0;

                        const _V_item *obj_item = _item->_data.VObject.First();
                        const _V_item *obj_end  = _item->_data.VObject.End();

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

                        groupedValue._data.VObject.Get(str, str_len) += Memory::Move(new_sub_obj);

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

    bool GroupBy(Value &groupedValue, const _Char_T *key) const {
        return GroupBy(groupedValue, key, StringUtils::Count(key));
    }

    // Set ascend to (false) for descend (ascend: 1,2,3; descend: 3,2,1 )
    void Sort(bool ascend = true) noexcept {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            _data.VObject.Sort(ascend);
        } else if (type == ValueType::Array) {
            _data.VArray.Sort(ascend);
        }
    }

    template <typename _Stream_T>
    inline _Stream_T &Stringify(_Stream_T &stream, SizeT32 precision = Config::DoublePrecision) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            stringifyObject(_data.VObject, stream, precision);
        } else if (type == ValueType::Array) {
            stringifyArray(_data.VArray, stream, precision);
        }

        return stream;
    }

    inline VStringT Stringify(SizeT32 precision = Config::DoublePrecision) const {
        StringStream<_Char_T> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename _Stream_T>
    static void stringifyObject(const VObjectT &obj, _Stream_T &stream, SizeT32 precision) {
        using _V_item = _HAItem_T<Value, _Char_T>;

        stream += JSONotation::SCurlyChar;

        const _V_item *h_item = obj.First();
        const _V_item *end    = (h_item + obj.Size());

        while (h_item != end) {
            if ((h_item != nullptr) && !(h_item->Value.IsUndefined())) {
                stream += JSONotation::QuoteChar;
                const SizeT length = h_item->Key.Length();
                EscapeJSON(h_item->Key.Storage(length), length, stream);
                stream += JSONotation::QuoteChar;
                stream += JSONotation::ColonChar;

                stringifyValue(h_item->Value, stream, precision);
                stream += JSONotation::CommaChar;
            }

            ++h_item;
        }

        _Char_T *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ECurlyChar;
        } else {
            stream += JSONotation::ECurlyChar;
        }
    }

    template <typename _Stream_T>
    static void stringifyArray(const VArrayT &arr, _Stream_T &stream, SizeT32 precision) {
        stream += JSONotation::SSquareChar;

        const Value *item = arr.First();
        const Value *end  = (item + arr.Size());

        while (item != end) {
            if (!(item->IsUndefined())) {
                stringifyValue(*item, stream, precision);
                stream += JSONotation::CommaChar;
            }

            ++item;
        }

        _Char_T *last = stream.Last();

        if ((last != nullptr) && (*last == JSONotation::CommaChar)) {
            *last = JSONotation::ESquareChar;
        } else {
            stream += JSONotation::ESquareChar;
        }
    }

    template <typename _Stream_T>
    static void stringifyValue(const Value &val, _Stream_T &stream, SizeT32 precision) {
        switch (val.Type()) {
            case ValueType::Object: {
                stringifyObject(val._data.VObject, stream, precision);
                break;
            }

            case ValueType::Array: {
                stringifyArray(val._data.VArray, stream, precision);
                break;
            }

            case ValueType::String: {
                stream += JSONotation::QuoteChar;
                EscapeJSON(val._data.VString.First(), val._data.VString.Length(), stream);
                stream += JSONotation::QuoteChar;
                break;
            }

            case ValueType::UIntLong: {
                Digit::NumberToString(stream, val._data.VNumber.GetUInt64());
                break;
            }

            case ValueType::IntLong: {
                Digit::NumberToString(stream, val._data.VNumber.GetInt64());
                break;
            }

            case ValueType::Double: {
                Digit::NumberToString(stream, val._data.VNumber.GetDouble(), precision);
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
        _data.SetType(new_type);
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
        Memory::Initialize(&(_data.VObject));
        setTypeToObject();
    }

    inline void initArray() noexcept {
        Memory::Initialize(&(_data.VArray));
        setTypeToArray();
    }

    inline void initString() noexcept {
        Memory::Initialize(&(_data.VString));
        setTypeToString();
    }

    inline void initValue(VObjectT &&obj) noexcept {
        Memory::Initialize(&(_data.VObject), Memory::Move(obj));
        setTypeToObject();
    }

    inline void initValue(const VObjectT &obj) {
        Memory::Initialize(&(_data.VObject), obj);
        setTypeToObject();
    }

    inline void initValue(VArrayT &&arr) noexcept {
        Memory::Initialize(&(_data.VArray), Memory::Move(arr));
        setTypeToArray();
    }

    inline void initValue(const VArrayT &arr) {
        Memory::Initialize(&(_data.VArray), arr);
        setTypeToArray();
    }

    inline void initValue(VStringT &&str) noexcept {
        Memory::Initialize(&(_data.VString), Memory::Move(str));
        setTypeToString();
    }

    inline void initValue(const VStringT &str) {
        Memory::Initialize(&(_data.VString), str);
        setTypeToString();
    }

    void reset() noexcept {
        switch (Type()) {
            case ValueType::Object: {
                _data.VObject.Reset();
                break;
            }

            case ValueType::Array: {
                _data.VArray.Reset();
                break;
            }

            case ValueType::String: {
                _data.VString.Reset();
                break;
            }

            default: {
            }
        }
    }

    void copyValue(const Value &val) {
        switch (val.Type()) {
            case ValueType::Object: {
                initValue(val._data.VObject);
                break;
            }

            case ValueType::Array: {
                initValue(val._data.VArray);
                break;
            }

            case ValueType::String: {
                initValue(val._data.VString);
                break;
            }

            default: {
                _data.VNumber = val._data.VNumber;
                setType(val.Type());
            }
        }
    }

    ValueData<_Char_T, VObjectT, VArrayT, VStringT, Config::PointerTagging> _data;
};

} // namespace Qentem

#endif
