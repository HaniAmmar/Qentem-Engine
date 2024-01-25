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

#include "QNumber.hpp"
#include "String.hpp"
#include "Array.hpp"
#include "HArray.hpp"
#include "JSONUtils.hpp"
#include "StringStream.hpp"

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
template <bool, bool>
struct VNumberData;

template <>
struct VNumberData<false, true> {
    // Little-Endian
    VNumberData()                               = default;
    VNumberData(VNumberData &&)                 = default;
    VNumberData(const VNumberData &)            = default;
    VNumberData &operator=(VNumberData &&)      = default;
    VNumberData &operator=(const VNumberData &) = default;
    ~VNumberData()                              = default;

    template <typename Number_T>
    inline explicit VNumberData(const Number_T num) noexcept : Number{num} {
    }

    inline void Clear() noexcept {
        PtrNumber = SystemIntType{0};

        if (!is_8_) {
            Number.Natural = SizeT64{0};
        } else {
            Padding[0] = SizeT{0};
            Padding[1] = SizeT{0};
        }
    }

    union {
        QNumber64 Number{0};
        SizeT     Padding[2];
    };

    SystemIntType PtrNumber{0};

  private:
    static constexpr bool is_8_ = (sizeof(SizeT) == 8U);
};

template <>
struct VNumberData<true, true> {
    // Big-Endian
    VNumberData() noexcept                          = default;
    VNumberData(VNumberData &&) noexcept            = default;
    VNumberData(const VNumberData &)                = default;
    VNumberData &operator=(VNumberData &&) noexcept = default;
    VNumberData &operator=(const VNumberData &)     = default;
    ~VNumberData()                                  = default;

    template <typename Number_T>
    inline explicit VNumberData(const Number_T num) noexcept : Number{num} {
    }

    inline void Clear() noexcept {
        if (!is_8_) {
            Number.Natural = SizeT64{0};
        } else {
            Padding[0] = SizeT{0};
            Padding[1] = SizeT{0};
        }

        PtrNumber = SystemIntType{0};
    }

    SystemIntType PtrNumber{0};

    union {
        QNumber64 Number{0};
        SizeT     Padding[2];
    };

  private:
    static constexpr bool is_8_ = (sizeof(SizeT) == 8U);
};

template <bool Endian_T>
struct VNumberData<Endian_T, false> {
    // 32-bit only
    VNumberData() noexcept                          = default;
    VNumberData(VNumberData &&) noexcept            = default;
    VNumberData(const VNumberData &)                = default;
    VNumberData &operator=(VNumberData &&) noexcept = default;
    VNumberData &operator=(const VNumberData &)     = default;
    ~VNumberData()                                  = default;

    template <typename Number_T>
    inline explicit VNumberData(const Number_T num) noexcept : Number{num} {
    }

    inline void Clear() noexcept {
        Number.Natural = SizeT64{0};
    }

    union {
        QNumber64 Number{0};
        SizeT     padding_[2];
    };
};

struct VNumberT {
    VNumberT()                            = default;
    VNumberT(const VNumberT &)            = default;
    VNumberT &operator=(VNumberT &&)      = default;
    VNumberT &operator=(const VNumberT &) = default;
    ~VNumberT()                           = default;

    VNumberT(VNumberT &&v_num) noexcept : data_{Memory::Move(v_num.data_)} {
        v_num.Clear();
    }

    template <typename Number_T>
    inline explicit VNumberT(const Number_T num) noexcept : data_{num} {
    }

    template <typename Number_T>
    VNumberT &operator=(Number_T num) noexcept {
        data_.Number = num;
        return *this;
    }

    inline SizeT64 GetUInt64() const noexcept {
        return data_.Number.Natural;
    }

    inline SizeT64I GetInt64() const noexcept {
        return data_.Number.Integer;
    }

    inline double GetDouble() const noexcept {
        return data_.Number.Real;
    }

    inline void Clear() noexcept {
        data_.Clear();
    }

  private:
    VNumberData<Config::IsBigEndian, (Config::Is64bit || (sizeof(SizeT) >= 4U))> data_{};
};
///////////////////////////////////////////////
template <typename Char_T, typename VObjectT, typename VArrayT, typename VStringT, bool>
struct ValueData;

template <typename Char_T, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<Char_T, VObjectT, VArrayT, VStringT, true> {
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

    inline explicit ValueData(VObjectT &&obj) noexcept : VObject{Memory::Move(obj)} {
    }

    inline explicit ValueData(VArrayT &&arr) noexcept : VArray{Memory::Move(arr)} {
    }

    inline explicit ValueData(VStringT &&str) noexcept : VString{Memory::Move(str)} {
    }

    inline explicit ValueData(const VObjectT &obj) noexcept : VObject{obj} {
    }

    inline explicit ValueData(const VArrayT &arr) noexcept : VArray{arr} {
    }

    inline explicit ValueData(const VStringT &str) noexcept : VString{str} {
    }

    inline explicit ValueData(const Char_T *str, SizeT length) : VString{str, length} {
    }

    inline explicit ValueData(SizeT64 num) noexcept : VNumber{num} {
    }

    inline explicit ValueData(SizeT64I num) noexcept : VNumber{num} {
    }

    inline explicit ValueData(double num) noexcept : VNumber{num} {
    }

    template <typename Number_T>
    inline explicit ValueData(Number_T num) noexcept : VNumber{num} {
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

template <typename Char_T, typename VObjectT, typename VArrayT, typename VStringT>
struct ValueData<Char_T, VObjectT, VArrayT, VStringT, false> {
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

    inline explicit ValueData(VObjectT &&obj) noexcept : VObject{Memory::Move(obj)} {
    }

    inline explicit ValueData(VArrayT &&arr) noexcept : VArray{Memory::Move(arr)} {
    }

    inline explicit ValueData(VStringT &&str) noexcept : VString{Memory::Move(str)} {
    }

    inline explicit ValueData(const VObjectT &obj) noexcept : VObject{obj} {
    }

    inline explicit ValueData(const VArrayT &arr) noexcept : VArray{arr} {
    }

    inline explicit ValueData(const VStringT &str) noexcept : VString{str} {
    }

    inline explicit ValueData(const Char_T *str, SizeT length) : VString{str, length} {
    }

    inline explicit ValueData(SizeT64 num) noexcept : VNumber{num} {
    }

    inline explicit ValueData(SizeT64I num) noexcept : VNumber{num} {
    }

    inline explicit ValueData(double num) noexcept : VNumber{num} {
    }

    template <typename Number_T>
    inline explicit ValueData(Number_T num) noexcept : VNumber{num} {
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

template <typename Char_T>
struct Value {
    using JSONotation = JSONUtils::JSONotation_T<Char_T>;
    using VObjectT    = HArray<Value, Char_T>;
    using VArrayT     = Array<Value>;
    using VStringT    = String<Char_T>;

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

    inline explicit Value(ValueType type) noexcept {
        setType(type);
    }

    inline explicit Value(ValueType type, SizeT size) noexcept {
        setType(type);

        switch (type) {
            case ValueType::Array: {
                data_.VArray.Reserve(size);
                break;
            }

            case ValueType::Object: {
                data_.VObject.Reserve(size);
                break;
            }

            default: {
            }
        }
    }

    inline explicit Value(VObjectT &&obj) noexcept : data_{Memory::Move(obj)} {
        setTypeToObject();
    }

    inline explicit Value(VArrayT &&arr) noexcept : data_{Memory::Move(arr)} {
        setTypeToArray();
    }

    inline explicit Value(VStringT &&str) noexcept : data_{Memory::Move(str)} {
        setTypeToString();
    }

    inline explicit Value(const VObjectT &obj) noexcept : data_{obj} {
        setTypeToObject();
    }

    inline explicit Value(const VArrayT &arr) noexcept : data_{arr} {
        setTypeToArray();
    }

    inline explicit Value(const VStringT &str) noexcept : data_{str} {
        setTypeToString();
    }

    inline explicit Value(const Char_T *str, SizeT length) : data_{str, length} {
        setTypeToString();
    }

    inline explicit Value(SizeT64 num) noexcept : data_{num} {
        setTypeToUInt64();
    }

    inline explicit Value(SizeT64I num) noexcept : data_{num} {
        setTypeToInt64();
    }

    inline explicit Value(double num) noexcept : data_{num} {
        setTypeToDouble();
    }

    template <typename Number_T>
    explicit Value(Number_T num) noexcept : data_{num} {
        if (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if (IsUnsigned<Number_T>()) {
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
            // val has to be cleared before setting the current one,
            // just in case the values or have parent-child relationship.

            const VNumberT  val_VNumber = val.data_.VNumber;
            const ValueType val_type    = val.Type();

            if (Config::PointerTagging) {
                val.data_.VNumber.Clear();
            } else {
                val.setTypeToUndefined();
            }

            reset();
            data_.VNumber = val_VNumber;

            if (!Config::PointerTagging) {
                setType(val_type);
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

    Value &operator=(VObjectT &&obj) noexcept {
        reset();
        data_.VObject = Memory::Move(obj);
        setTypeToObject();
        return *this;
    }

    Value &operator=(const VObjectT &obj) {
        reset();
        data_.VObject = obj;
        setTypeToObject();
        return *this;
    }

    Value &operator=(VArrayT &&arr) noexcept {
        reset();
        data_.VArray = Memory::Move(arr);
        setTypeToArray();
        return *this;
    }

    Value &operator=(const VArrayT &arr) {
        reset();
        data_.VArray = arr;
        setTypeToArray();
        return *this;
    }

    Value &operator=(VStringT &&str) noexcept {
        reset();
        data_.VString = Memory::Move(str);
        setTypeToString();
        return *this;
    }

    Value &operator=(const VStringT &str) {
        reset();
        data_.VString = str;
        setTypeToString();
        return *this;
    }

    Value &operator=(const Char_T *str) {
        reset();
        data_.VString = VStringT{str};
        setTypeToString();
        return *this;
    }

    Value &operator=(SizeT64 num) noexcept {
        reset();
        data_.VNumber = num;
        setTypeToUInt64();
        return *this;
    }

    Value &operator=(SizeT64I num) noexcept {
        reset();
        data_.VNumber = num;
        setTypeToInt64();
        return *this;
    }

    Value &operator=(double num) noexcept {
        reset();
        data_.VNumber = num;
        setTypeToDouble();
        return *this;
    }

    template <typename Number_T>
    Value &operator=(Number_T num) noexcept {
        reset();
        data_.VNumber = num;

        if (IsFloat<Number_T>()) {
            setTypeToDouble();
        } else if (IsUnsigned<Number_T>()) {
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
            data_.VObject += Memory::Move(val.data_.VObject);
            val.setTypeToUndefined();
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            data_.VArray += Memory::Move(val);
        }
    }

    inline void operator+=(const Value &val) {
        if (IsObject() && val.IsObject()) {
            data_.VObject += val.data_.VObject;
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            data_.VArray += val;
        }
    }

    inline void operator+=(VObjectT &&obj) {
        if (IsObject()) {
            data_.VObject += Memory::Move(obj);
        } else {
            if (!IsArray()) {
                reset();
                setTypeToArray();
            }

            data_.VArray += Value{Memory::Move(obj)};
        }
    }

    inline void operator+=(const VObjectT &obj) {
        *this += VObjectT(obj);
    }

    inline void operator+=(VArrayT &&arr) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        if (arr.Size() != SizeT{0}) {
            data_.VArray += Memory::Move(arr);
        } else {
            data_.VArray += Value{Memory::Move(arr)};
        }
    }

    inline void operator+=(const VArrayT &arr) {
        (*this) += VArrayT(arr);
    }

    inline void operator+=(VStringT &&str) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        data_.VArray += Value{Memory::Move(str)};
    }

    inline void operator+=(const VStringT &str) {
        *this += VStringT(str);
    }

    inline void operator+=(const Char_T *str) {
        *this += VStringT(str);
    }

    template <typename Number_T>
    inline void operator+=(Number_T num) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        data_.VArray += Value{num};
    }

    inline void operator+=(NullType) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        data_.VArray += Value{nullptr};
    }

    inline void operator+=(bool is_true) {
        if (!IsArray()) {
            reset();
            setTypeToArray();
        }

        data_.VArray += Value{is_true};
    }

    inline Value &operator[](const Char_T *key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (data_.VObject[key]);
    }

    inline Value &operator[](VStringT &&key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
        }

        return (data_.VObject[Memory::Move(key)]);
    }

    inline Value &operator[](const VStringT &key) {
        if (!IsObject()) {
            reset();
            setTypeToObject();
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

            reset();
            setTypeToArray();
        }

        if (data_.VArray.Size() == index) {
            data_.VArray += Value{};
        } else {
            data_.VArray.ResizeAndInitialize(index + SizeT{1});
        }

        return (data_.VArray.Storage()[index]);
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

        return (data_.VObject.Get(key, length));
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
            return (data_.VObject.Size());
        }

        if (IsArray()) {
            return (data_.VArray.Size());
        }

        return SizeT{0};
    }

    void Merge(Value &&val) {
        if (IsUndefined()) {
            setTypeToArray();
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
            setTypeToArray();
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

    Value *GetValue(const Char_T *key, SizeT length) const noexcept {
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

    VObjectT *GetObject() noexcept {
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

    VArrayT *GetArray() noexcept {
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

    VStringT *GetString() noexcept {
        if (IsString()) {
            return &(data_.VString);
        }

        return nullptr;
    }

    const Char_T *StringStorage() const noexcept {
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
    template <typename Number_T>
    bool SetKeyCharAndLength(SizeT index, const Char_T *&key, Number_T &length) const noexcept {
        const VStringT *val = GetKey(index);

        if (val != nullptr) {
            const SizeT len = val->Length();
            key             = val->Storage(len);
            length          = Number_T(len);
            return true;
        }

        return false;
    }

    template <typename Number_T>
    void SetValueKeyLength(SizeT index, const Value *&value, const Char_T *&key, Number_T &length) const noexcept {
        if (IsObject()) {
            const HAItem_T<Value, Char_T> *item = data_.VObject.GetItem(index);

            value = nullptr;

            if ((item != nullptr) && !(item->Value.IsUndefined())) {
                const SizeT len = item->Key.Length();
                value           = &(item->Value);
                key             = item->Key.Storage(len);
                length          = Number_T(len);
            }
        }
    }

    // To get a pointer to a string value and its length.
    template <typename Number_T>
    bool SetCharAndLength(const Char_T *&key, Number_T &length) const noexcept {
        switch (Type()) {
            case ValueType::String: {
                const SizeT len = data_.VString.Length();
                key             = data_.VString.Storage(len);
                length          = Number_T{len};
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
    bool CopyValueTo(StringStream_T &stream, SizeT32 precision = Config::DoublePrecision,
                     StringFunction_T *string_function = nullptr) const {
        switch (Type()) {
            case ValueType::String: {
                const SizeT len = data_.VString.Length();

                if (string_function != nullptr) {
                    string_function(stream, data_.VString.Storage(len), len);
                } else {
                    stream.Write(data_.VString.Storage(len), len);
                }

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

    template <typename StringStream_T>
    bool CopyKeyByIndexTo(StringStream_T &stream, SizeT index) const {
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
                const QNumberType n_type = Digit::StringToNumber(number, data_.VString.Storage(length), offset, length);

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

    inline void Remove(const Char_T *key, SizeT length) const noexcept {
        if (IsObject()) {
            data_.VObject.Remove(key, length);
        }
    }

    inline void Remove(const VStringT &key) const noexcept {
        const SizeT length = data_.VString.Length();
        Remove(key.Storage(length), length);
    }

    inline void Remove(const Char_T *key) const noexcept {
        Remove(key, StringUtils::Count(key));
    }

    void RemoveIndex(SizeT index) const noexcept {
        if (IsObject()) {
            data_.VObject.RemoveIndex(index);
        } else if (IsArray() && (index < data_.VArray.Size())) {
            (data_.VArray.Storage() + index)->Reset();
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
        using V_item = HAItem_T<Value, Char_T>;

        if (IsArray()) {
            Value       *src_val = data_.VArray.Storage();
            const Value *src_end = (src_val + data_.VArray.Size());
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

                    if (Config::PointerTagging) {
                        setTypeToArray();
                    }

                    return;
                }

                VArrayT new_array{size};
                src_val = data_.VArray.Storage();

                do {
                    if (!(src_val->IsUndefined())) {
                        new_array += Memory::Move(*src_val);
                    }

                    ++src_val;
                } while (src_val < src_end);

                data_.VArray = Memory::Move(new_array);
            }

            src_val = data_.VArray.Storage();
            src_end = (src_val + data_.VArray.Size());

            while (src_val < src_end) {
                if (src_val->IsArray() || src_val->IsObject()) {
                    src_val->Compress();
                }

                ++src_val;
            }
        } else if (IsObject()) {
            data_.VObject.Compress();

            V_item       *src_val = data_.VObject.Storage();
            const V_item *src_end = (src_val + data_.VObject.Size());

            while (src_val < src_end) {
                if (src_val->Value.IsArray() || src_val->Value.IsObject()) {
                    src_val->Value.Compress();
                }

                ++src_val;
            }
        }
    }

    inline ValueType Type() const noexcept {
        return data_.GetType();
    }

    bool GroupBy(Value &groupedValue, const Char_T *key, const SizeT length) const {
        using V_item = HAItem_T<Value, Char_T>;
        StringStream<Char_T> stream;
        VObjectT             new_sub_obj;
        const Char_T        *str     = nullptr;
        SizeT                str_len = 0;
        SizeT                grouped_key_index;

        if (IsArray()) {
            const Value *item_ = data_.VArray.First();
            groupedValue.reset();
            groupedValue.setTypeToObject();

            if ((item_ != nullptr) && item_->IsObject() &&
                item_->data_.VObject.GetKeyIndex(grouped_key_index, key, length)) {
                const Value *end = data_.VArray.End();

                while (item_ != end) {
                    if ((item_ != nullptr) && item_->IsObject()) {
                        SizeT count = 0;

                        const V_item *obj_item = item_->data_.VObject.First();
                        const V_item *obj_end  = item_->data_.VObject.End();

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
            data_.VObject.Sort(ascend);
        } else if (type == ValueType::Array) {
            data_.VArray.Sort(ascend);
        }
    }

    template <typename Stream_T>
    inline Stream_T &Stringify(Stream_T &stream, SizeT32 precision = Config::DoublePrecision) const {
        const ValueType type = Type();

        if (type == ValueType::Object) {
            stringifyObject(data_.VObject, stream, precision);
        } else if (type == ValueType::Array) {
            stringifyArray(data_.VArray, stream, precision);
        }

        return stream;
    }

    inline VStringT Stringify(SizeT32 precision = Config::DoublePrecision) const {
        StringStream<Char_T> stream;
        return Stringify(stream, precision).GetString();
    }

  private:
    template <typename Stream_T>
    static void stringifyObject(const VObjectT &obj, Stream_T &stream, SizeT32 precision) {
        using V_item = HAItem_T<Value, Char_T>;

        stream += JSONotation::SCurlyChar;

        const V_item *h_item = obj.First();
        const V_item *end    = (h_item + obj.Size());

        while (h_item != end) {
            if ((h_item != nullptr) && !(h_item->Value.IsUndefined())) {
                stream += JSONotation::QuoteChar;
                const SizeT length = h_item->Key.Length();
                JSONUtils::EscapeJSON(h_item->Key.Storage(length), length, stream);
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
    static void stringifyArray(const VArrayT &arr, Stream_T &stream, SizeT32 precision) {
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
                stringifyObject(val.data_.VObject, stream, precision);
                break;
            }

            case ValueType::Array: {
                stringifyArray(val.data_.VArray, stream, precision);
                break;
            }

            case ValueType::String: {
                stream += JSONotation::QuoteChar;
                JSONUtils::EscapeJSON(val.data_.VString.First(), val.data_.VString.Length(), stream);
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
                data_.VNumber.Clear();
            }
        }
    }

    void copyValue(const Value &val) {
        switch (val.Type()) {
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

            default: {
                data_.VNumber = val.data_.VNumber;
            }
        }

        setType(val.Type());
    }

    ValueData<Char_T, VObjectT, VArrayT, VStringT, Config::PointerTagging> data_;
};

} // namespace Qentem

#endif
