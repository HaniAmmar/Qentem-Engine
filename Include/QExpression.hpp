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

#include "VariableTag.hpp"
#include "Array.hpp"

#ifndef QENTEM_Q_EXPRESSION_H_
#define QENTEM_Q_EXPRESSION_H_

namespace Qentem {

struct QExpression {
    // ExpressionType -------------------------------------------
    enum class ExpressionType : SizeT8 {
        Empty = 0,
        RealNumber,
        NaturalNumber,
        IntegerNumber,
        NotANumber,
        Variable,
        SubOperation
    };

    // QOperation -------------------------------------------
    enum class QOperation : SizeT8 {
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

    // QExpression -------------------------------------------
    struct ExpressionValue {
        union {
            SizeT64  Natural{0}; // Natural number.
            SizeT64I Integer;    // Integer number.
            double   Real;       // Real number.
        };

        SizeT Offset{0}; // String for use in ==
        SizeT Length{0};
    };

    QExpression(const QExpression &)            = delete;
    QExpression &operator=(const QExpression &) = delete;
    QExpression &operator=(QExpression &&)      = delete;

    QExpression() noexcept : Number{}, Type{ExpressionType::Empty}, Operation{QOperation::NoOp} {};

    QExpression(ExpressionType type, QOperation operation) noexcept : Number{}, Type{type}, Operation{operation} {
    }

    QExpression(Array<QExpression> &&subExpressions, QOperation operation) noexcept
        : SubExpressions{Memory::Move(subExpressions)}, Type{ExpressionType::SubOperation}, Operation{operation} {
    }

    ~QExpression() {
        if (Type == ExpressionType::SubOperation) {
            Memory::Dispose(&SubExpressions);
        }
    }

    QExpression(QExpression &&expr) noexcept : Number{expr.Number}, Type{expr.Type}, Operation{expr.Operation} {
        expr.Type = ExpressionType::Empty;
        // expr.Number  = QExpression{};
    }

    void operator+=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Number.Natural += right.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Integer += right.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real = (double(Number.Natural) + right.Number.Real);
                        Type        = ExpressionType::RealNumber;
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
                        Number.Integer += right.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real = (double(Number.Integer) + right.Number.Real);
                        Type        = ExpressionType::RealNumber;
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
                        Number.Real += double(right.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Real += double(right.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
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

    void operator-=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        if (Number.Natural < right.Number.Natural) {
                            Type = ExpressionType::IntegerNumber;
                        }

                        Number.Natural -= right.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Integer -= right.Number.Integer;
                        Type = ExpressionType::IntegerNumber;

                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real = (double(Number.Natural) - right.Number.Real);
                        Type        = ExpressionType::RealNumber;
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
                        Number.Integer -= right.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real = (double(Number.Integer) - right.Number.Real);
                        Type        = ExpressionType::RealNumber;
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
                        Number.Real -= double(right.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Real -= double(right.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real -= right.Number.Real;
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
                        Number.Natural *= right.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Integer *= right.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real = (double(Number.Natural) * right.Number.Real);
                        Type        = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    Number.Real = (double(Number.Integer) * right.Number.Real);
                    Type        = ExpressionType::RealNumber;
                } else {
                    Number.Integer *= right.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        Number.Real *= double(right.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        Number.Real *= double(right.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        Number.Real *= right.Number.Real;
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
        SizeT64 num_left       = 0;
        SizeT64 num_right      = 0;
        bool    left_negative  = false;
        bool    right_negative = false;

        switch (Type) {
            case ExpressionType::NaturalNumber: {
                left_negative = false;
                num_left      = Number.Natural;
                break;
            }

            case ExpressionType::IntegerNumber: {
                left_negative = (Number.Integer < 0);

                if (left_negative) {
                    Number.Integer = -Number.Integer;
                }

                num_left = Number.Natural;
                break;
            }

            case ExpressionType::RealNumber: {
                left_negative = (Number.Real < 0.0);

                if (left_negative) {
                    Number.Real = -Number.Real;
                }

                if ((Number.Real < 1.0) && (Number.Real > 0.0)) {
                    // No power of fraction at the moment.
                    Number.Natural = SizeT64{0};
                    Type           = ExpressionType::NotANumber;
                    return false;
                }

                num_left = QNumber64{SizeT64I(Number.Real)}.Natural;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                right_negative = false;
                num_right      = right.Number.Natural;
                break;
            }

            case ExpressionType::IntegerNumber: {
                right_negative = (right.Number.Integer < 0);

                if (right_negative) {
                    num_right = QNumber64{-right.Number.Integer}.Natural;
                } else {
                    num_right = right.Number.Natural;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                double right_real = right.Number.Real;
                right_negative    = (right_real < 0.0);

                if (right_negative) {
                    right_real = -right_real;
                }

                if ((right_real < 1.0) && (right_real > 0.0)) {
                    // No power of fraction at the moment.
                    Number.Natural = SizeT64{0};
                    Type           = ExpressionType::NotANumber;
                    return false;
                }

                num_right = QNumber64{SizeT64I(right_real)}.Natural;
            }

            default: {
            }
        }

        if (num_left != SizeT64{0}) {
            if (num_right != SizeT64{0}) {
                const bool right_odd = ((num_right & SizeT64{1}) == SizeT64{1});

                PowerOf(num_left, num_right);

                if (right_negative) {
                    Number.Real = double(num_left);
                    Number.Real = 1.0 / Number.Real;
                    Type        = ExpressionType::RealNumber;

                    if (left_negative) {
                        Number.Real = -Number.Real;
                    }

                } else if (left_negative && right_odd) {
                    Number.Natural = num_left;
                    Number.Integer = -Number.Integer;
                    Type           = ExpressionType::IntegerNumber;
                } else {
                    Number.Natural = num_left;
                    Type           = ExpressionType::NaturalNumber;
                }

                return true;
            }

            Number.Natural = SizeT64{1};
            Type           = ExpressionType::NaturalNumber;

        } else {
            Number.Natural = SizeT64{0};
            Type           = ExpressionType::NaturalNumber;
        }

        return true;
    }

    void operator/=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                Number.Real = double(Number.Natural);
                Type        = ExpressionType::RealNumber;
                break;
            }

            case ExpressionType::IntegerNumber: {
                Number.Real = double(Number.Integer);
                Type        = ExpressionType::RealNumber;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                Number.Real /= double(right.Number.Natural);
                break;
            }

            case ExpressionType::IntegerNumber: {
                Number.Real /= double(right.Number.Integer);
                break;
            }

            case ExpressionType::RealNumber: {
                Number.Real /= right.Number.Real;
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
                    result = (Number.Integer % SizeT64I(right.Number.Real));
                } else {
                    result = (Number.Integer % right.Number.Integer);
                }

                break;
            }

            case ExpressionType::RealNumber: {
                result = SizeT64I(Number.Real);
                if (right.Type == ExpressionType::RealNumber) {
                    result %= SizeT64I(right.Number.Real);
                } else {
                    result %= right.Number.Integer;
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
            case ExpressionType::NaturalNumber: {
                return (Number.Natural > SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (Number.Integer > SizeT64I(number));
            }

            default: {
                return (Number.Real > double(number));
            }
        }
    }

    bool operator>=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Natural) >= right.Number.Real);
                }

                return (Number.Integer >= right.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Integer) >= right.Number.Real);
                }

                return (Number.Integer >= right.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Number.Real >= double(right.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Number.Real >= right.Number.Real);
    }

    bool operator>(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Natural) > right.Number.Real);
                }

                return (Number.Integer > right.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Integer) > right.Number.Real);
                }

                return (Number.Integer > right.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Number.Real > double(right.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Number.Real > right.Number.Real);
    }

    bool operator<=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Natural) <= right.Number.Real);
                }

                return (Number.Integer <= right.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Integer) <= right.Number.Real);
                }

                return (Number.Integer <= right.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Number.Real <= double(right.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Number.Real <= right.Number.Real);
    }

    bool operator<(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Natural) < right.Number.Real);
                }

                return (Number.Integer < right.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Integer) < right.Number.Real);
                }

                return (Number.Integer < right.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Number.Real < double(right.Number.Integer));
                }
            }

            default: {
            }
        }

        return (Number.Real < right.Number.Real);
    }

    bool operator==(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Natural) == right.Number.Real);
                }

                return (Number.Integer == right.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(Number.Integer) == right.Number.Real);
                }

                return (Number.Integer == right.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (Number.Real == double(right.Number.Integer));
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
            case ExpressionType::NaturalNumber: {
                return (Number.Natural != SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (Number.Integer != SizeT64I(number));
            }

            default: {
                return (Number.Real != double(number));
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
        Array<QExpression> SubExpressions{};
        ExpressionValue    Number;
        Tags::VariableTag  Variable; // {var:...}
    };

    ExpressionType Type;
    QOperation     Operation;
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

    static constexpr Char_T_ ParenthesesStart = '(';
    static constexpr Char_T_ ParenthesesEnd   = ')';
    static constexpr Char_T_ BracketStart     = '{';
    static constexpr Char_T_ BracketEnd       = '}';
    static constexpr Char_T_ ExponentExp      = '^';
    static constexpr Char_T_ SpaceChar        = ' ';
    static constexpr Char_T_ ColonChar        = ':';
    static constexpr Char_T_ SlashChar        = '/';
};

} // namespace Qentem

#endif
