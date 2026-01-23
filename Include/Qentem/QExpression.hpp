/**
 * @file QExpression.hpp
 * @brief Implements expression parsing and evaluation for Qentem Engine.
 *
 * This header defines the core logic and data structures for parsing,
 * compiling, and evaluating dynamic expressions within Qentem templates.
 * QExpression.hpp enables advanced template features, such as arithmetic,
 * logical operations, variable interpolation, and runtime expression resolution.
 *
 * This component brings together multiple parsing and execution techniques,
 * supporting complex expressions and flexible syntax for end users.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_Q_EXPRESSION_H
#define QENTEM_Q_EXPRESSION_H

#include "Qentem/QNumber.hpp"
#include "Qentem/VariableTag.hpp"
#include "Qentem/Array.hpp"

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
        SizeT     Offset{0}; // For use in (string == string) op
        SizeT     Length{0};
    };

    QExpression &operator=(const QExpression &) = delete;

    QExpression() noexcept : ExprValue{}, Operation{QOperation::NoOp}, Type{ExpressionType::Empty} {};

    QExpression(ExpressionType type, QOperation operation) noexcept : ExprValue{}, Operation{operation}, Type{type} {
    }

    QExpression(Array<QExpression> &&subExpressions, QOperation operation) noexcept
        : SubExprs{QUtility::Move(subExpressions)}, Operation{operation}, Type{ExpressionType::SubOperation} {
    }

    ~QExpression() {
        if (Type == ExpressionType::SubOperation) {
            MemoryUtils::Destruct(&SubExprs);
        }
    }

    QExpression(QExpression &&src) noexcept : Operation{src.Operation}, Type{src.Type} {
        switch (src.Type) {
            case ExpressionType::Variable: {
                VariableTag = src.VariableTag;
                break;
            }

            case ExpressionType::SubOperation: {
                SubExprs = QUtility::Move(src.SubExprs);
                break;
            }

            default: {
                ExprValue = src.ExprValue;
                break;
            }
        }

        src.Type = ExpressionType::Empty;
    }

    QExpression(const QExpression &src) noexcept : Operation{src.Operation}, Type{src.Type} {
        switch (src.Type) {
            case ExpressionType::Variable: {
                VariableTag = src.VariableTag;
                break;
            }

            case ExpressionType::SubOperation: {
                SubExprs = src.SubExprs;
                break;
            }

            default: {
                ExprValue = src.ExprValue;
                break;
            }
        }
    }

    // QOperation GetOperation() const noexcept {
    //     return Operation;
    // }

    // ExpressionType GetType() const noexcept {
    //     return Type;
    // }

    // const ExpressionValue &GetValue() const noexcept {
    //     return Value;
    // }

    // const Array<QExpression> &GetSubExpressions() const noexcept {
    //     return SubExpressions;
    // }

    // const Tags::VariableTag &GetVariable() const noexcept {
    //     return Variable;
    // }

    QExpression &operator=(QExpression &&src) noexcept {
        if (this != &src) {
            if (Type == ExpressionType::SubOperation) {
                MemoryUtils::Destruct(&SubExprs);
            }

            Operation = src.Operation;
            Type      = src.Type;

            switch (src.Type) {
                case ExpressionType::Variable: {
                    VariableTag = src.VariableTag;
                    break;
                }

                case ExpressionType::SubOperation: {
                    SubExprs = QUtility::Move(src.SubExprs);
                    break;
                }

                default: {
                    ExprValue = src.ExprValue;
                    break;
                }
            }

            src.Type = ExpressionType::Empty;
        }

        return *this;
    }

    void operator+=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        ExprValue.Number.Natural += right.ExprValue.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer += right.ExprValue.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real = (double(ExprValue.Number.Natural) + right.ExprValue.Number.Real);
                        Type                  = ExpressionType::RealNumber;
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
                        ExprValue.Number.Integer += right.ExprValue.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real = (double(ExprValue.Number.Integer) + right.ExprValue.Number.Real);
                        Type                  = ExpressionType::RealNumber;
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
                        ExprValue.Number.Real += double(right.ExprValue.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Real += double(right.ExprValue.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real += right.ExprValue.Number.Real;
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
                        if (ExprValue.Number.Natural < right.ExprValue.Number.Natural) {
                            Type = ExpressionType::IntegerNumber;
                        }

                        ExprValue.Number.Natural -= right.ExprValue.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer -= right.ExprValue.Number.Integer;
                        Type = ExpressionType::IntegerNumber;

                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real = (double(ExprValue.Number.Natural) - right.ExprValue.Number.Real);
                        Type                  = ExpressionType::RealNumber;
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
                        ExprValue.Number.Integer -= right.ExprValue.Number.Integer;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real = (double(ExprValue.Number.Integer) - right.ExprValue.Number.Real);
                        Type                  = ExpressionType::RealNumber;
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
                        ExprValue.Number.Real -= double(right.ExprValue.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Real -= double(right.ExprValue.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real -= right.ExprValue.Number.Real;
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
                        ExprValue.Number.Natural *= right.ExprValue.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer *= right.ExprValue.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real = (double(ExprValue.Number.Natural) * right.ExprValue.Number.Real);
                        Type                  = ExpressionType::RealNumber;
                        break;
                    }

                    default: {
                    }
                }

                break;
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    ExprValue.Number.Real = (double(ExprValue.Number.Integer) * right.ExprValue.Number.Real);
                    Type                  = ExpressionType::RealNumber;
                } else {
                    ExprValue.Number.Integer *= right.ExprValue.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber: {
                        ExprValue.Number.Real *= double(right.ExprValue.Number.Natural);
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Real *= double(right.ExprValue.Number.Integer);
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Real *= right.ExprValue.Number.Real;
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
                left_negative = (ExprValue.Number.Integer < 0);

                if (left_negative) {
                    ExprValue.Number.Integer = -ExprValue.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                left_negative = (ExprValue.Number.Real < 0.0);

                if (left_negative) {
                    ExprValue.Number.Real = -ExprValue.Number.Real;
                }

                if ((ExprValue.Number.Real < 1.0) && (ExprValue.Number.Real > 0.0)) {
                    // No power of fraction at the moment.
                    ExprValue.Number.Natural = SizeT64{0};
                    Type                     = ExpressionType::NotANumber;
                    return false;
                }

                ExprValue.Number.Natural = QNumber64{SizeT64I(ExprValue.Number.Real)}.Natural;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                right_negative = false;
                num_right      = right.ExprValue.Number.Natural;
                break;
            }

            case ExpressionType::IntegerNumber: {
                right_negative = (right.ExprValue.Number.Integer < 0);

                if (right_negative) {
                    num_right = QNumber64{-right.ExprValue.Number.Integer}.Natural;
                } else {
                    num_right = right.ExprValue.Number.Natural;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                double right_real = right.ExprValue.Number.Real;
                right_negative    = (right_real < 0.0);

                if (right_negative) {
                    right_real = -right_real;
                }

                if ((right_real < 1.0) && (right_real > 0.0)) {
                    // No power of fraction at the moment.
                    ExprValue.Number.Natural = SizeT64{0};
                    Type                     = ExpressionType::NotANumber;
                    return false;
                }

                num_right = QNumber64{SizeT64I(right_real)}.Natural;
            }

            default: {
            }
        }

        if (ExprValue.Number.Natural != SizeT64{0}) {
            if (num_right != SizeT64{0}) {
                const bool right_odd = ((num_right & SizeT64{1}) == SizeT64{1});

                PowerOf(ExprValue.Number.Natural, num_right);

                if (right_negative) {
                    ExprValue.Number.Real = double(ExprValue.Number.Natural);
                    ExprValue.Number.Real = (1.0 / ExprValue.Number.Real);
                    Type                  = ExpressionType::RealNumber;

                    if (left_negative) {
                        ExprValue.Number.Real = -ExprValue.Number.Real;
                    }

                } else if (left_negative && right_odd) {
                    ExprValue.Number.Integer = -ExprValue.Number.Integer;
                    Type                     = ExpressionType::IntegerNumber;
                } else {
                    Type = ExpressionType::NaturalNumber;
                }

                return true;
            }

            ExprValue.Number.Natural = SizeT64{1};
            Type                     = ExpressionType::NaturalNumber;

        } else {
            ExprValue.Number.Natural = SizeT64{0};
            Type                     = ExpressionType::NaturalNumber;
        }

        return true;
    }

    void operator/=(const QExpression &right) noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                ExprValue.Number.Real = double(ExprValue.Number.Natural);
                Type                  = ExpressionType::RealNumber;
                break;
            }

            case ExpressionType::IntegerNumber: {
                ExprValue.Number.Real = double(ExprValue.Number.Integer);
                Type                  = ExpressionType::RealNumber;
                break;
            }

            default: {
            }
        }

        switch (right.Type) {
            case ExpressionType::NaturalNumber: {
                ExprValue.Number.Real /= double(right.ExprValue.Number.Natural);
                break;
            }

            case ExpressionType::IntegerNumber: {
                ExprValue.Number.Real /= double(right.ExprValue.Number.Integer);
                break;
            }

            case ExpressionType::RealNumber: {
                ExprValue.Number.Real /= right.ExprValue.Number.Real;
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
                    result = (ExprValue.Number.Integer % SizeT64I(right.ExprValue.Number.Real));
                } else {
                    result = (ExprValue.Number.Integer % right.ExprValue.Number.Integer);
                }

                break;
            }

            case ExpressionType::RealNumber: {
                result = SizeT64I(ExprValue.Number.Real);
                if (right.Type == ExpressionType::RealNumber) {
                    result %= SizeT64I(right.ExprValue.Number.Real);
                } else {
                    result %= right.ExprValue.Number.Integer;
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
                        ExprValue.Number.Natural &= right.ExprValue.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer &= right.ExprValue.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Integer &= SizeT64I(right.ExprValue.Number.Real);
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
                    ExprValue.Number.Integer &= SizeT64I(right.ExprValue.Number.Real);
                    Type = ExpressionType::IntegerNumber;
                } else {
                    ExprValue.Number.Integer &= right.ExprValue.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer = SizeT64I(ExprValue.Number.Real) & right.ExprValue.Number.Integer;
                        Type                     = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Integer =
                            SizeT64I(ExprValue.Number.Real) & SizeT64I(right.ExprValue.Number.Real);
                        Type = ExpressionType::IntegerNumber;
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
                        ExprValue.Number.Natural |= right.ExprValue.Number.Natural;
                        break;
                    }

                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer |= right.ExprValue.Number.Integer;
                        Type = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Integer |= SizeT64I(right.ExprValue.Number.Real);
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
                    ExprValue.Number.Integer |= SizeT64I(right.ExprValue.Number.Real);
                    Type = ExpressionType::IntegerNumber;
                } else {
                    ExprValue.Number.Integer |= right.ExprValue.Number.Integer;
                }

                break;
            }

            case ExpressionType::RealNumber: {
                switch (right.Type) {
                    case ExpressionType::NaturalNumber:
                    case ExpressionType::IntegerNumber: {
                        ExprValue.Number.Integer = SizeT64I(ExprValue.Number.Real) | right.ExprValue.Number.Integer;
                        Type                     = ExpressionType::IntegerNumber;
                        break;
                    }

                    case ExpressionType::RealNumber: {
                        ExprValue.Number.Integer =
                            SizeT64I(ExprValue.Number.Real) | SizeT64I(right.ExprValue.Number.Real);
                        Type = ExpressionType::IntegerNumber;
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
                return (ExprValue.Number.Natural > SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (ExprValue.Number.Integer > SizeT64I(number));
            }

            default: {
                return (ExprValue.Number.Real > double(number));
            }
        }
    }

    bool operator>=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Natural) >= right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer >= right.ExprValue.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Integer) >= right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer >= right.ExprValue.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (ExprValue.Number.Real >= double(right.ExprValue.Number.Integer));
                }
            }

            default: {
            }
        }

        return (ExprValue.Number.Real >= right.ExprValue.Number.Real);
    }

    bool operator>(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Natural) > right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer > right.ExprValue.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Integer) > right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer > right.ExprValue.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (ExprValue.Number.Real > double(right.ExprValue.Number.Integer));
                }
            }

            default: {
            }
        }

        return (ExprValue.Number.Real > right.ExprValue.Number.Real);
    }

    bool operator<=(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Natural) <= right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer <= right.ExprValue.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Integer) <= right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer <= right.ExprValue.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (ExprValue.Number.Real <= double(right.ExprValue.Number.Integer));
                }
            }

            default: {
            }
        }

        return (ExprValue.Number.Real <= right.ExprValue.Number.Real);
    }

    bool operator<(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Natural) < right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer < right.ExprValue.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Integer) < right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer < right.ExprValue.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (ExprValue.Number.Real < double(right.ExprValue.Number.Integer));
                }
            }

            default: {
            }
        }

        return (ExprValue.Number.Real < right.ExprValue.Number.Real);
    }

    bool operator==(const QExpression &right) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Natural) == right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer == right.ExprValue.Number.Integer);
            }

            case ExpressionType::IntegerNumber: {
                if (right.Type == ExpressionType::RealNumber) {
                    return (double(ExprValue.Number.Integer) == right.ExprValue.Number.Real);
                }

                return (ExprValue.Number.Integer == right.ExprValue.Number.Integer);
            }

            case ExpressionType::RealNumber: {
                if (right.Type != ExpressionType::RealNumber) {
                    return (ExprValue.Number.Real == double(right.ExprValue.Number.Integer));
                }
            }

            default: {
            }
        }

        return (ExprValue.Number.Real == right.ExprValue.Number.Real);
    }

    template <typename Number_T>
    bool operator!=(const Number_T number) const noexcept {
        switch (Type) {
            case ExpressionType::NaturalNumber: {
                return (ExprValue.Number.Natural != SizeT64(number));
            }

            case ExpressionType::IntegerNumber: {
                return (ExprValue.Number.Integer != SizeT64I(number));
            }

            default: {
                return (ExprValue.Number.Real != double(number));
            }
        }
    }

    union {
        Array<QExpression> SubExprs{};
        ExpressionValue    ExprValue;
        Tags::VariableTag  VariableTag; // {var:...}
    };

    QOperation     Operation;
    ExpressionType Type;
};

template <typename Char_T>
struct QOperationSymbols_T {
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
