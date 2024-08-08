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

#ifndef QENTEM_Q_EXPRESSION_H
#define QENTEM_Q_EXPRESSION_H

#include "QNumber.hpp"
#include "VariableTag.hpp"
#include "Array.hpp"

namespace Qentem {

struct QExpression {
    // ExpressionType -------------------------------------------
    enum struct ExpressionType : SizeT8 {
        Empty = 0U,
        RealNumber,
        NaturalNumber,
        IntegerNumber,
        NotANumber,
        Variable,
        SubOperation
    };

    // QOperation -------------------------------------------
    enum struct QOperation : SizeT8 {
        NoOp = 0U,
        Or,             // ||
        And,            // &&
        Equal,          // ==
        NotEqual,       // !=
        GreaterOrEqual, // >=
        LessOrEqual,    // <=
        Greater,        // >
        Less,           // <
        BitwiseOr,      // |
        BitwiseAnd,     // &
        Addition,       // +
        Subtraction,    // -
        Multiplication, // *
        Division,       // /
        Remainder,      // %
        Exponent,       // ^
        Error           // X
    };

    // QExpression -------------------------------------------
    struct ExpressionValue {
        QNumber64 Number{0};
        SizeT     Offset{0}; // String for use in ==
        SizeT     Length{0};
    };

    QExpression(const QExpression &)            = delete;
    QExpression &operator=(const QExpression &) = delete;
    QExpression &operator=(QExpression &&)      = delete;

    QExpression() noexcept : Value{}, Operation{QOperation::NoOp}, Type{ExpressionType::Empty} {};

    QExpression(ExpressionType type, QOperation operation) noexcept : Value{}, Operation{operation}, Type{type} {
    }

    QExpression(Array<QExpression> &&subExpressions, QOperation operation) noexcept
        : SubExpressions{Memory::Move(subExpressions)}, Operation{operation}, Type{ExpressionType::SubOperation} {
    }

    ~QExpression() {
        if (Type == ExpressionType::SubOperation) {
            Memory::Dispose(&SubExpressions);
        }
    }

    QExpression(QExpression &&expr) noexcept : Value{expr.Value}, Operation{expr.Operation}, Type{expr.Type} {
        expr.Type = ExpressionType::Empty;
        // expr.Value  = QExpression{};
    }

    void operator+=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Natural += right.Value.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer += right.Value.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real = (double(Value.Number.Natural) + right.Value.Number.Real);
                        Type              = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer += right.Value.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real = (double(Value.Number.Integer) + right.Value.Number.Real);
                        Type              = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Real += double(right.Value.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Real += double(right.Value.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real += right.Value.Number.Real;
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

    void operator-=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        if (Value.Number.Natural < right.Value.Number.Natural) {
                            Type = ExpressionType::IntegerNumber;
                        }

                        Value.Number.Natural -= right.Value.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer -= right.Value.Number.Integer;
                        Type = ExpressionType::IntegerNumber;

                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real = (double(Value.Number.Natural) - right.Value.Number.Real);
                        Type              = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer -= right.Value.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real = (double(Value.Number.Integer) - right.Value.Number.Real);
                        Type              = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Real -= double(right.Value.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Real -= double(right.Value.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real -= right.Value.Number.Real;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    void operator*=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Natural *= right.Value.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer *= right.Value.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real = (double(Value.Number.Natural) * right.Value.Number.Real);
                        Type              = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    Value.Number.Real = (double(Value.Number.Integer) * right.Value.Number.Real);
                    Type              = ExpressionType::RealNumber;
                } else {
                    Value.Number.Integer *= right.Value.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Real *= double(right.Value.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Real *= double(right.Value.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Real *= right.Value.Number.Real;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    static void PowerOf(SizeT64 &left, SizeT64 right) noexcept {
        if (right > 1U) {
            if ((right & SizeT64{1}) == SizeT64{0}) {
                PowerOf(left, (right / 2U));
                left *= left;
            } else {
                --right;
                const SizeT64 num = left;
                PowerOf(left, (right / SizeT64{2}));
                left *= left;
                left *= num;
            }
        }
    }

    bool operator^=(const QExpression &right) noexcept {
        SizeT64 num_right      = 0;
        bool    left_negative  = false;
        bool    right_negative = false;

        switch (Type) {
            case ExpressionType::NaturalNumber: {
                left_negative = false;
                break;
            }

            case ExpressionType::IntegerNumber: {
                left_negative = (Value.Number.Integer < 0);

                if (left_negative) {
                    Value.Number.Integer = -Value.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                left_negative = (Value.Number.Real < 0.0);

                if (left_negative) {
                    Value.Number.Real = -Value.Number.Real;
                }

                if ((Value.Number.Real < 1.0) && (Value.Number.Real > 0.0)) {
                    // No power of fraction at the moment.
                    Value.Number.Natural = SizeT64{0};
                    Type                 = ExpressionType::NotANumber;
                    return false;
                }

                Value.Number.Natural = QNumber64{SizeT64I(Value.Number.Real)}.Natural;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                right_negative = false;
                num_right      = right.Value.Number.Natural;
                break;
            }

            case ExpressionType::IntegerNumber: {
                right_negative = (right.Value.Number.Integer < 0);

                if (right_negative) {
                    num_right = QNumber64{-right.Value.Number.Integer}.Natural;
                } else {
                    num_right = right.Value.Number.Natural;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                double right_real = right.Value.Number.Real;
                right_negative    = (right_real < 0.0);

                if (right_negative) {
                    right_real = -right_real;
                }

                if ((right_real < 1.0) && (right_real > 0.0)) {
                    // No power of fraction at the moment.
                    Value.Number.Natural = SizeT64{0};
                    Type                 = ExpressionType::NotANumber;
                    return false;
                }

                num_right = QNumber64{SizeT64I(right_real)}.Natural;
            }

            default: {
            }
        }

        if (Value.Number.Natural != SizeT64{0}) {
            if (num_right != SizeT64{0}) {
                const bool right_odd = ((num_right & SizeT64{1}) == SizeT64{1});

                PowerOf(Value.Number.Natural, num_right);

                if (right_negative) {
                    Value.Number.Real = double(Value.Number.Natural);
                    Value.Number.Real = (1.0 / Value.Number.Real);
                    Type              = ExpressionType::RealNumber;

                    if (left_negative) {
                        Value.Number.Real = -Value.Number.Real;
                    }

                } else if (left_negative && right_odd) {
                    Value.Number.Integer = -Value.Number.Integer;
                    Type                 = ExpressionType::IntegerNumber;
                } else {
                    Type = ExpressionType::NaturalNumber;
                }

                return true;
            }

            Value.Number.Natural = SizeT64{1};
            Type                 = ExpressionType::NaturalNumber;

        } else {
            Value.Number.Natural = SizeT64{0};
            Type                 = ExpressionType::NaturalNumber;
        }

        return true;
    }

    void operator/=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                Value.Number.Real = double(Value.Number.Natural);
                Type              = ExpressionType::RealNumber;
                break;
            }

            case ExpressionType::IntegerNumber: {
                Value.Number.Real = double(Value.Number.Integer);
                Type              = ExpressionType::RealNumber;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                Value.Number.Real /= double(right.Value.Number.Natural);
                break;
            }

            case ExpressionType::IntegerNumber: {
                Value.Number.Real /= double(right.Value.Number.Integer);
                break;
            }

            case ExpressionType::RealNumber: {
                Value.Number.Real /= right.Value.Number.Real;
                break;
            }

            default: {
            }
        }
    }

    SizeT64I operator%(const QExpression &right) const noexcept {
        SizeT64I result = 0;

        switch (Type) {
            case ExpressionType::NaturalNumber:
            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    result = (Value.Number.Integer % SizeT64I(right.Value.Number.Real));
                } else {
                    result = (Value.Number.Integer % right.Value.Number.Integer);
                }

                break;
            }

            case ExpressionType::RealNumber: {
                result = SizeT64I(Value.Number.Real);
                if (right.Type == ExpressionType::RealNumber) {
                    result %= SizeT64I(right.Value.Number.Real);
                } else {
                    result %= right.Value.Number.Integer;
                }
            }

            default: {
            }
        }

        return result;
    }

    void operator&=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Natural &= right.Value.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer &= right.Value.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Integer &= SizeT64I(right.Value.Number.Real);
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    Value.Number.Integer &= SizeT64I(right.Value.Number.Real);
                    Type = ExpressionType::IntegerNumber;
                } else {
                    Value.Number.Integer &= right.Value.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer = SizeT64I(Value.Number.Real) & right.Value.Number.Integer;
                        Type                 = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Integer = SizeT64I(Value.Number.Real) & SizeT64I(right.Value.Number.Real);
                        Type                 = ExpressionType::IntegerNumber;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    void operator|=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Value.Number.Natural |= right.Value.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer |= right.Value.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Integer |= SizeT64I(right.Value.Number.Real);
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    Value.Number.Integer |= SizeT64I(right.Value.Number.Real);
                    Type = ExpressionType::IntegerNumber;
                } else {
                    Value.Number.Integer |= right.Value.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        Value.Number.Integer = SizeT64I(Value.Number.Real) | right.Value.Number.Integer;
                        Type                 = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Value.Number.Integer = SizeT64I(Value.Number.Real) | SizeT64I(right.Value.Number.Real);
                        Type                 = ExpressionType::IntegerNumber;
                    }

                    default: {
                    }
                }
            }

            default: {
            }
        }
    }

    template <typename Number_T>
    bool operator>(const Number_T number) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                return (Value.Number.Natural > SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (Value.Number.Integer > SizeT64I(number));
            }

            default: {
                return (Value.Number.Real > double(number));
            }
        }
    }

    bool operator>=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Natural) >= right.Value.Number.Real);
                }

                return (Value.Number.Integer >= right.Value.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Integer) >= right.Value.Number.Real);
                }

                return (Value.Number.Integer >= right.Value.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Value.Number.Real >= double(right.Value.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Value.Number.Real >= right.Value.Number.Real);
    }

    bool operator>(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Natural) > right.Value.Number.Real);
                }

                return (Value.Number.Integer > right.Value.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Integer) > right.Value.Number.Real);
                }

                return (Value.Number.Integer > right.Value.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Value.Number.Real > double(right.Value.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Value.Number.Real > right.Value.Number.Real);
    }

    bool operator<=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Natural) <= right.Value.Number.Real);
                }

                return (Value.Number.Integer <= right.Value.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Integer) <= right.Value.Number.Real);
                }

                return (Value.Number.Integer <= right.Value.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Value.Number.Real <= double(right.Value.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Value.Number.Real <= right.Value.Number.Real);
    }

    bool operator<(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Natural) < right.Value.Number.Real);
                }

                return (Value.Number.Integer < right.Value.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Integer) < right.Value.Number.Real);
                }

                return (Value.Number.Integer < right.Value.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Value.Number.Real < double(right.Value.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Value.Number.Real < right.Value.Number.Real);
    }

    bool operator==(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Natural) == right.Value.Number.Real);
                }

                return (Value.Number.Integer == right.Value.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Value.Number.Integer) == right.Value.Number.Real);
                }

                return (Value.Number.Integer == right.Value.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Value.Number.Real == double(right.Value.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Value.Number.Real == right.Value.Number.Real);
    }

    template <typename Number_T>
    bool operator!=(const Number_T number) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                return (Value.Number.Natural != SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (Value.Number.Integer != SizeT64I(number));
            }

            default: {
                return (Value.Number.Real != double(number));
            }
        }
    }

    union {
        Array<QExpression> SubExpressions{};
        ExpressionValue    Value;
        Tags::VariableTag  Variable; // {var:...}
    };

    QOperation     Operation;
    ExpressionType Type;
};

template <typename Char_T>
struct QOperationSymbol_T {
  public:
    static constexpr Char_T RemainderExp = '%';
    static constexpr Char_T MultipleExp  = '*';
    static constexpr Char_T DivideExp    = '/';
    static constexpr Char_T AddExp       = '+';
    static constexpr Char_T SubtractExp  = '-';
    static constexpr Char_T EqualExp     = '=';
    static constexpr Char_T NotExp       = '!';
    static constexpr Char_T LessExp      = '<';
    static constexpr Char_T GreaterExp   = '>';
    static constexpr Char_T AndExp       = '&';
    static constexpr Char_T OrExp        = '|';

    static constexpr Char_T ParenthesesStart = '(';
    static constexpr Char_T ParenthesesEnd   = ')';
    static constexpr Char_T BracketStart     = '{';
    static constexpr Char_T BracketEnd       = '}';
    static constexpr Char_T ExponentExp      = '^';
    static constexpr Char_T SpaceChar        = ' ';
};

} // namespace Qentem

#endif
