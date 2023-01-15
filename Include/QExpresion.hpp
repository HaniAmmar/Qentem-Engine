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

#include "Tags.hpp"

#ifndef QENTEM_Q_EXPRESION_H_
#define QENTEM_Q_EXPRESION_H_

namespace Qentem {

struct QExpresion {
    // ExpresionType -------------------------------------------
    enum class ExpresionType : unsigned char {
        Empty = 0,
        RealNumber,
        NaturalNumber,
        IntegerNumber,
        NotANumber,
        Variable,
        SubOperation
    };

    // QOperation -------------------------------------------
    enum class QOperation : unsigned char {
        NoOp = 0,
        Or,             // ||
        And,            // &&
        Equal,          // ==
        NotEqual,       // !=
        GreaterOrEqual, // >=
        LessOrEqual,    // <=
        Greater,        // >
        Less,           // <
        Addition,       // +
        Subtraction,    // -
        Multiplication, // *
        Division,       // /
        Remainder,      // %
        Exponent,       // ^
        Error           // X
    };

    // QNumber -------------------------------------------
    struct QNumber {
        union {
            unsigned long long Natural{0}; // Natural number.
            long long          Integer;    // Integer number.
            double             Real;       // Real number.
        };

        SizeT Offset{0}; // String for use in ==
        SizeT Length{0};
    };

    QExpresion(const QExpresion &)            = delete;
    QExpresion &operator=(const QExpresion &) = delete;
    QExpresion &operator=(QExpresion &&)      = delete;

    QExpresion() noexcept : Number{}, Type{ExpresionType::Empty}, Operation{QOperation::NoOp} {};

    QExpresion(ExpresionType type, QOperation operation) noexcept : Number{}, Type{type}, Operation{operation} {}

    QExpresion(Array<QExpresion> &&subExpresions, QOperation operation) noexcept
        : SubExpresions{static_cast<Array<QExpresion> &&>(subExpresions)}, Type{ExpresionType::SubOperation},
          Operation{operation} {}

    ~QExpresion() {
        if (Type == ExpresionType::SubOperation) {
            Memory::Dispose(&SubExpresions);
        }
    }

    QExpresion(QExpresion &&expr) noexcept : Number{expr.Number}, Type{expr.Type}, Operation{expr.Operation} {
        expr.Type = ExpresionType::Empty;
        // expr.Number  = QNumber{};
    }

    void operator+=(const QExpresion &right) noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Natural += right.Number.Natural;
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Natural += static_cast<unsigned long long>(right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Natural) + right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Integer += static_cast<long long>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Integer += right.Number.Integer;
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Integer) + right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Real += static_cast<double>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Real += static_cast<double>(right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real += right.Number.Real;
                        break;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    void operator-=(const QExpresion &right) noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Natural -= right.Number.Natural;
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Natural -= static_cast<unsigned long long>(right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Natural) - right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Integer -= static_cast<long long>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Integer -= right.Number.Integer;
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Integer) - right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Real -= static_cast<double>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Real -= static_cast<double>(right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real -= right.Number.Real;
                        break;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    void operator*=(const QExpresion &right) noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Natural *= right.Number.Natural;
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        if (right.Number.Integer < 0) {
                            Number.Integer = (static_cast<long long>(Number.Natural) * right.Number.Integer);
                            Type           = ExpresionType::IntegerNumber;
                        } else {
                            Number.Natural *= static_cast<unsigned long long>(right.Number.Integer);
                        }

                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Natural) * right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Integer *= static_cast<long long>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Integer *= right.Number.Integer;
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real = (static_cast<double>(Number.Integer) * right.Number.Real);
                        Type        = ExpresionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        Number.Real *= static_cast<double>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        Number.Real *= static_cast<double>(right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        Number.Real *= right.Number.Real;
                        break;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    static void EvenPowerOf(unsigned long long &left, unsigned long long right) noexcept {
        if (right > 1U) {
            if ((right & 1U) != 1U) {
                EvenPowerOf(left, (right / 2U));
                left *= left;
            } else {
                --right;
                unsigned long long num = left;
                EvenPowerOf(left, (right / 2U));
                left *= left;
                left *= num;
            }
        }
    }

    bool operator^=(const QExpresion &right) noexcept {
        unsigned long long num_left       = 0;
        unsigned long long num_right      = 0;
        bool               left_negative  = false;
        bool               right_negative = false;

        switch (Type) {
            case ExpresionType::NaturalNumber: {
                left_negative = false;
                num_left      = Number.Natural;
                break;
            }

            case ExpresionType::IntegerNumber: {
                left_negative = (Number.Integer < 0);

                if (left_negative) {
                    Number.Integer = -Number.Integer;
                }

                num_left = static_cast<unsigned long long>(Number.Integer);
                break;
            }

            case ExpresionType::RealNumber: {
                left_negative = (Number.Real < 0);

                if (left_negative) {
                    Number.Real = -Number.Real;
                }

                if ((Number.Real < 1) && (Number.Real > 0)) {
                    // No power of fraction at the moment.
                    Number.Natural = 0;
                    Type           = ExpresionType::NotANumber;
                    return false;
                }

                num_left = static_cast<unsigned long long>(Number.Real);
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpresionType::NaturalNumber: {
                right_negative = false;
                num_right      = right.Number.Natural;
                break;
            }

            case ExpresionType::IntegerNumber: {
                right_negative = (right.Number.Integer < 0);

                if (right_negative) {
                    num_right = static_cast<unsigned long long>(-right.Number.Integer);
                } else {
                    num_right = static_cast<unsigned long long>(right.Number.Integer);
                }

                break;
            }

            case ExpresionType::RealNumber: {
                double right_real = right.Number.Real;
                right_negative    = (right_real < 0);

                if (right_negative) {
                    right_real = -right_real;
                }

                if ((right_real < 1) && (right_real > 0)) {
                    // No power of fraction at the moment.
                    Number.Natural = 0;
                    Type           = ExpresionType::NotANumber;
                    return false;
                }

                num_right = static_cast<unsigned long long>(right_real);
                break;
            }

            default: {
            }
        }

        if (num_left != 0) {
            if (num_right != 0) {
                const bool right_odd = (num_right & 1U);

                EvenPowerOf(num_left, num_right);

                if (right_negative) {
                    Number.Real = static_cast<double>(num_left);
                    Number.Real = 1 / Number.Real;
                    Type        = ExpresionType::RealNumber;

                    if (left_negative) {
                        Number.Real = -Number.Real;
                    }

                } else if (left_negative && right_odd) {
                    Number.Integer = static_cast<long long>(num_left);
                    Number.Integer = -Number.Integer;
                    Type           = ExpresionType::IntegerNumber;
                } else {
                    Number.Natural = num_left;
                    Type           = ExpresionType::NaturalNumber;
                }

                return true;
            }

            Number.Natural = 1;
            Type           = ExpresionType::NaturalNumber;

        } else {
            Number.Natural = 0;
            Type           = ExpresionType::NaturalNumber;
        }

        return true;
    }

    void operator/=(const QExpresion &right) noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                Number.Real = static_cast<double>(Number.Natural);
                Type        = ExpresionType::RealNumber;
                break;
            }

            case ExpresionType::IntegerNumber: {
                Number.Real = static_cast<double>(Number.Integer);
                Type        = ExpresionType::RealNumber;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpresionType::NaturalNumber: {
                Number.Real /= static_cast<double>(right.Number.Natural);
                break;
            }

            case ExpresionType::IntegerNumber: {
                Number.Real /= static_cast<double>(right.Number.Integer);
                break;
            }

            case ExpresionType::RealNumber: {
                Number.Real /= right.Number.Real;
                break;
            }

            default: {
            }
        }
    }

    long long operator%(const QExpresion &right) const noexcept {
        long long result = 0;

        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        result = static_cast<long long>(Number.Natural % right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        result = (static_cast<long long>(Number.Natural) % right.Number.Integer);

                        break;
                    }

                    case ExpresionType::RealNumber: {
                        result = (static_cast<long long>(Number.Natural) % static_cast<long long>(right.Number.Real));

                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        result = (Number.Integer % static_cast<long long>(right.Number.Natural));
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        result = (Number.Integer % right.Number.Integer);
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        result = (Number.Integer % static_cast<long long>(right.Number.Real));
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpresionType::RealNumber: {
                result = static_cast<long long>(Number.Real);

                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        result %= static_cast<long long>(right.Number.Natural);
                        break;
                    }

                    case ExpresionType::IntegerNumber: {
                        result %= right.Number.Integer;
                        break;
                    }

                    case ExpresionType::RealNumber: {
                        result %= static_cast<long long>(right.Number.Real);
                        break;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return result;
    }

    template <typename Number_T_>
    bool operator>(const Number_T_ number) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                return (Number.Natural > static_cast<unsigned long long>(number));
            }

            case ExpresionType::IntegerNumber: {
                return (Number.Integer > static_cast<long long>(number));
            }

            default: {
                return (Number.Real > static_cast<double>(number));
            }
        }
    }

    bool operator>=(const QExpresion &right) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Natural >= right.Number.Natural);
                    }

                    case ExpresionType::IntegerNumber: {
                        return (static_cast<long long>(Number.Natural) >= right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Natural) >= right.Number.Real);
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Integer >= static_cast<long long>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Integer >= right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Integer) >= right.Number.Real);
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Real >= static_cast<double>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Real >= static_cast<double>(right.Number.Integer));
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return (Number.Real >= right.Number.Real);
    }

    bool operator>(const QExpresion &right) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Natural > right.Number.Natural);
                    }

                    case ExpresionType::IntegerNumber: {
                        return (static_cast<long long>(Number.Natural) > right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Natural) > right.Number.Real);
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Integer > static_cast<long long>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Integer > right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Integer) > right.Number.Real);
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Real > static_cast<double>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Real > static_cast<double>(right.Number.Integer));
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return (Number.Real > right.Number.Real);
    }

    bool operator<=(const QExpresion &right) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Natural <= right.Number.Natural);
                    }

                    case ExpresionType::IntegerNumber: {
                        return (static_cast<long long>(Number.Natural) <= right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Natural) <= right.Number.Real);
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Integer <= static_cast<long long>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Integer <= right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Integer) <= right.Number.Real);
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Real <= static_cast<double>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Real <= static_cast<double>(right.Number.Integer));
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return (Number.Real <= right.Number.Real);
    }

    bool operator<(const QExpresion &right) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Natural < right.Number.Natural);
                    }

                    case ExpresionType::IntegerNumber: {
                        return (static_cast<long long>(Number.Natural) < right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Natural) < right.Number.Real);
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Integer < static_cast<long long>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Integer < right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Integer) < right.Number.Real);
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Real < static_cast<double>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Real < static_cast<double>(right.Number.Integer));
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return (Number.Real < right.Number.Real);
    }

    bool operator==(const QExpresion &right) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Natural == right.Number.Natural);
                    }

                    case ExpresionType::IntegerNumber: {
                        return (static_cast<long long>(Number.Natural) == right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Natural) == right.Number.Real);
            }

            case ExpresionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Integer == static_cast<long long>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Integer == right.Number.Integer);
                    }

                    default: {
                    }
                }

                return (static_cast<double>(Number.Integer) == right.Number.Real);
            }

            case ExpresionType::RealNumber: {
                switch (right.Type) {
                    case ExpresionType::NaturalNumber: {
                        return (Number.Real == static_cast<double>(right.Number.Natural));
                    }

                    case ExpresionType::IntegerNumber: {
                        return (Number.Real == static_cast<double>(right.Number.Integer));
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }

        return (Number.Real == right.Number.Real);
    }

    template <typename Number_T_>
    bool operator!=(const Number_T_ number) const noexcept {
        switch (Type) {
            case ExpresionType::NaturalNumber: {
                return (Number.Natural != static_cast<unsigned long long>(number));
            }

            case ExpresionType::IntegerNumber: {
                return (Number.Integer != static_cast<long long>(number));
            }

            default: {
                return (Number.Real != static_cast<double>(number));
            }
        }
    }

    // struct Bucket_ {
    //     SizeT            padding1{0};
    //     SizeT            padding2{0};
    //     QPointer<SizeT> *padding3{};
    // };

    union {
        // Bucket_           bucket_;
        Array<QExpresion> SubExpresions{};
        QNumber           Number;
        Tags::VariableTag Variable; // {var:...}
    };

    ExpresionType Type;
    QOperation    Operation;
};

template <typename Char_T_>
struct QOperationSymbol_T_ {
  public:
    static constexpr Char_T_ RemainderExp = '%';
    static constexpr Char_T_ MultipleExp  = '*';
    static constexpr Char_T_ DivideExp    = '/';
    static constexpr Char_T_ AddExp       = '+';
    static constexpr Char_T_ SubtractExp  = '-';
    static constexpr Char_T_ EqualExp     = '=';
    static constexpr Char_T_ NotExp       = '!';
    static constexpr Char_T_ LessExp      = '<';
    static constexpr Char_T_ GreaterExp   = '>';
    static constexpr Char_T_ AndExp       = '&';
    static constexpr Char_T_ OrExp        = '|';

    static constexpr Char_T_ ParenthesStart = '(';
    static constexpr Char_T_ ParenthesEnd   = ')';
    static constexpr Char_T_ BracketStart   = '{';
    static constexpr Char_T_ BracketEnd     = '}';
    static constexpr Char_T_ ExponentExp    = '^';
    static constexpr Char_T_ SpaceChar      = ' ';
    static constexpr Char_T_ ColonChar      = ':';
    static constexpr Char_T_ SlashChar      = '/';
};

} // namespace Qentem

#endif
