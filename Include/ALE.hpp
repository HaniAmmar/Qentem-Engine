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

#include "Digit.hpp"
#include "Engine.hpp"

#ifndef QENTEM_ALE_H_
#define QENTEM_ALE_H_

namespace Qentem {

/*
 * Arithmetic and logic evaluator
 *
 * Time complexity:  O(n)
 * Space complexity: O(1)
 */

template <typename>
class ALEBasicHelper;

template <typename>
class ALEExpressions;

class ALE {
  public:
    ALE()                       = delete;
    ALE(ALE &&)                 = delete;
    ALE(const ALE &)            = delete;
    ALE &operator=(ALE &&)      = delete;
    ALE &operator=(const ALE &) = delete;
    ~ALE()                      = delete;

    union Number {
        double Number;

        struct {
            SizeT Offset{0};
            SizeT Length{0};
        } Content{};
    };

    enum class Expression {
        None = 0U,
        Or,             // ||
        And,            // &&
        Equal,          // ==
        NotEqual,       // !=
        BiggerOrEqual,  // >=
        LessOrEqual,    // <=
        Bigger,         // >
        Less,           // <
        Subtraction,    // -
        Addition,       // +
        Division,       // /
        Multiplication, // *
        Remainder,      // %
        Exponent,       // ^
        Error           // X
    };

    template <typename Char_T_, typename Helper_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length, const Helper_T_ *callback) noexcept {
        Number     num;
        Expression current_exp = Expression::None;
        SizeT      offset      = 0;

        const bool result = parse(current_exp, num, content, offset, length, callback);
        number            = num.Number;

        if (!result) {
            number = 0;
        }

        return result;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content, const Helper_T_ *callback) noexcept {
        return Evaluate(number, content, StringUtils::Count(content), callback);
    }

    template <typename Char_T_, typename Helper_T_>
    static double Evaluate(const Char_T_ *content, SizeT length, const Helper_T_ *callback) noexcept {
        double number;

        Evaluate(number, content, length, callback);

        return number;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static double Evaluate(const Char_T_ *content, const Helper_T_ *callback) noexcept {
        return Evaluate(content, StringUtils::Count(content), callback);
    }

    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content, SizeT length) noexcept {
        ALEBasicHelper<Char_T_> helper;
        return Evaluate(number, content, length, &helper);
    }

    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content) noexcept {
        return Evaluate(number, content, StringUtils::Count(content));
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content, SizeT length) noexcept {
        double number;

        Evaluate(number, content, length);

        return number;
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content) noexcept {
        return Evaluate(content, StringUtils::Count(content));
    }

  private:
    template <typename Char_T_, typename Helper_T_>
    static bool parse(Expression &current_exp, Number &left, const Char_T_ *content, SizeT &offset,
                      const SizeT end_offset, const Helper_T_ *callback) noexcept {
        Number     right;
        SizeT      previous_offset = offset;
        Expression op_w;
        Expression exp             = nextExpression(op_w, content, offset, end_offset);
        bool       left_evaluated  = false;
        bool       right_evaluated = false;

        if (getNumber(left, content, previous_offset, (offset - previous_offset), callback, exp)) {
            advance(exp, offset);
            previous_offset = offset;

            while (offset < end_offset) {
                Expression       next_exp_w;
                const Expression next_exp = nextExpression(next_exp_w, content, offset, end_offset);

                if (next_exp_w > op_w) {
                    Expression tmp_exp = exp;
                    right_evaluated    = (op_w == Expression::Equal);

                    if (parse(tmp_exp, right, content, previous_offset, end_offset, callback) &&
                        process(content, left, right, left_evaluated, right_evaluated, exp, callback)) {
                        exp            = tmp_exp;
                        offset         = previous_offset;
                        left_evaluated = true;
                        continue;
                    }
                } else if (getNumber(right, content, previous_offset, (offset - previous_offset), callback, exp) &&
                           process(content, left, right, left_evaluated, right_evaluated, exp, callback)) {
                    advance(next_exp, offset);

                    if (next_exp_w < current_exp) {
                        current_exp = next_exp;
                        return true;
                    }

                    exp             = next_exp;
                    op_w            = next_exp_w;
                    previous_offset = offset;
                    left_evaluated  = true;
                    continue;
                }

                return false;
            }

            return (exp == Expression::None);
        }

        return false;
    }

    static void advance(Expression exp, SizeT &offset) {
        ++offset;

        if ((exp >= Expression::Or) && (exp <= Expression::LessOrEqual)) {
            ++offset;
        }
    }

    template <typename Char_T_>
    static Expression nextExpression(Expression &weight, const Char_T_ *content, SizeT &offset,
                                     const SizeT end_offset) noexcept {
        using ALEExpressions_T_ = ALEExpressions<Char_T_>;

        while (offset < end_offset) {
            switch (content[offset]) {
                case ALEExpressions_T_::OrOp: { // ||
                    if (content[(offset + 1)] == ALEExpressions_T_::OrOp) {
                        weight = Expression::And;
                        return Expression::Or;
                    }

                    weight = Expression::None;
                    return Expression::Error;
                }

                case ALEExpressions_T_::AndOp: { // &&
                    if (content[(offset + 1)] == ALEExpressions_T_::AndOp) {
                        weight = Expression::And;
                        return Expression::And;
                    }

                    weight = Expression::None;
                    return Expression::Error;
                }

                case ALEExpressions_T_::BiggerOp: { // > or >=
                    weight = Expression::BiggerOrEqual;

                    if (content[(offset + 1)] == ALEExpressions_T_::EqualOp) {
                        return Expression::BiggerOrEqual;
                    }

                    return Expression::Bigger;
                }

                case ALEExpressions_T_::LessOp: { // < or <=
                    weight = Expression::LessOrEqual;

                    if (content[(offset + 1)] == ALEExpressions_T_::EqualOp) {
                        return Expression::LessOrEqual;
                    }

                    return Expression::Less;
                }

                case ALEExpressions_T_::NotOp: { // !=
                    if (content[(offset + 1)] == ALEExpressions_T_::EqualOp) {
                        weight = Expression::Equal;
                        return Expression::NotEqual;
                    }

                    weight = Expression::None;
                    return Expression::Error;
                }

                case ALEExpressions_T_::EqualOp: { // ==
                    if (content[(offset + 1)] == ALEExpressions_T_::EqualOp) {
                        weight = Expression::Equal;
                        return Expression::Equal;
                    }

                    weight = Expression::None;
                    return Expression::Error;
                }

                case ALEExpressions_T_::SubtractOp: {
                    if (isExpression(content, offset)) {
                        weight = Expression::Addition;
                        return Expression::Subtraction;
                    }

                    break;
                }

                case ALEExpressions_T_::AddOp: {
                    if (isExpression(content, offset)) {
                        weight = Expression::Addition;
                        return Expression::Addition;
                    }

                    break;
                }

                case ALEExpressions_T_::DivideOp: {
                    weight = Expression::Multiplication;
                    return Expression::Division;
                }

                case ALEExpressions_T_::MultipleOp: {
                    weight = Expression::Multiplication;
                    return Expression::Multiplication;
                }

                case ALEExpressions_T_::RemainderOp: {
                    weight = Expression::Remainder;
                    return Expression::Remainder;
                }

                case ALEExpressions_T_::ExponentOp: {
                    weight = Expression::Exponent;
                    return Expression::Exponent;
                }

                case ALEExpressions_T_::ParenthesStart: {
                    // (...) are evaluated to numbers.

                    ++offset;
                    offset = Engine::SkipInnerPatterns<Char_T_>(ALEExpressions_T_::ParenthesStart,
                                                                ALEExpressions_T_::ParenthesEnd, content, offset,
                                                                end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    offset = end_offset;
                    weight = Expression::None;
                    return Expression::Error;
                }

                case ALEExpressions_T_::BracketStart: {
                    // {...} are evaluated by callback to a number or
                    // string.

                    ++offset;
                    offset = Engine::FindOne<Char_T_>(ALEExpressions_T_::BracketEnd, content, offset, end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    offset = end_offset;
                    weight = Expression::None;
                    return Expression::Error;
                }
            }

            ++offset;
        }

        weight = Expression::None;
        return Expression::None;
    }

    template <typename Char_T_>
    static bool isExpression(const Char_T_ *content, SizeT offset) noexcept {
        using ALEExpressions_T_ = ALEExpressions<Char_T_>;

        while (offset != 0) {
            --offset;

            switch (content[offset]) {
                case ALEExpressions_T_::SpaceChar: {
                    break;
                }

                case ALEExpressions_T_::ParenthesEnd:
                case ALEExpressions_T_::BracketEnd: {
                    // (...) and {} are numbers.
                    return true;
                }

                default: {
                    // A number
                    return ((content[offset] < ALEExpressions_T_::ColonChar) &&
                            (content[offset] > ALEExpressions_T_::SlashChar));
                }
            }
        }

        return false;
    }

    template <typename Char_T_, typename Helper_T_>
    static bool getNumber(Number &val, const Char_T_ *content, SizeT offset, SizeT length, const Helper_T_ *callback,
                          Expression exp) noexcept {
        using ALEExpressions_T_ = ALEExpressions<Char_T_>;

        if (exp == Expression::Error) {
            return false;
        }

        StringUtils::Trim(content, offset, length);

        if ((exp == Expression::Equal) || (exp == Expression::NotEqual)) {
            val.Content.Offset = static_cast<unsigned int>(offset);
            val.Content.Length = static_cast<unsigned int>(length);
            return true;
        }

        switch (content[offset]) {
            case ALEExpressions_T_::ParenthesStart: {
                length += offset;
                ++offset;
                --length;

                Expression current_exp = Expression::None;
                return parse(current_exp, val, content, offset, length, callback);
            }

            case ALEExpressions_T_::BracketStart: {
                return (callback->ALESetNumber(val.Number, (content + offset), length));
            }

            default: {
                return (Digit<Char_T_>::StringToNumber(val.Number, (content + offset), length));
            }
        }
    }

    template <typename Char_T_, typename Helper_T_>
    static bool process(const Char_T_ *content, Number &left, Number right, bool left_evaluated, bool right_evaluated,
                        Expression exp, const Helper_T_ *callback) noexcept {
        switch (exp) {
            case Expression::Exponent: { // ^
                if (right.Number != 0.0) {
                    // NOTE: Needs more work to evaluate fractions
                    if (left.Number != 0.0) {
                        const bool neg = (right.Number < 0);

                        if (neg) {
                            right.Number *= -1;
                        }

                        if (right.Number < 1) {
                            return false;
                        }

                        unsigned int times = static_cast<unsigned int>(right.Number);
                        const double num   = left.Number;

                        while (--times != 0) {
                            left.Number *= num;
                        }

                        if (neg) {
                            left.Number = (1 / left.Number);
                        }
                    }

                    break;
                }

                left.Number = 1;
                break;
            }

            case Expression::Remainder: { // %
                left.Number = static_cast<double>(static_cast<unsigned long long>(left.Number) %
                                                  static_cast<unsigned long long>(right.Number));
                break;
            }

            case Expression::Multiplication: { // *
                left.Number *= right.Number;
                break;
            }

            case Expression::Division: { // /
                if (right.Number != 0.0) {
                    left.Number /= right.Number;
                    break;
                }

                return false;
            }

            case Expression::Addition: { // +
                left.Number += right.Number;
                break;
            }

            case Expression::Subtraction: { // -
                left.Number -= right.Number;
                break;
            }

            case Expression::Less: { // <
                left.Number = (left.Number < right.Number) ? 1 : 0;
                break;
            }

            case Expression::LessOrEqual: { // <=
                left.Number = (left.Number <= right.Number) ? 1 : 0;
                break;
            }

            case Expression::Bigger: { // >
                left.Number = (left.Number > right.Number) ? 1 : 0;
                break;
            }

            case Expression::BiggerOrEqual: { // >=
                left.Number = (left.Number >= right.Number) ? 1 : 0;
                break;
            }

            case Expression::And: { // &&
                left.Number = ((left.Number > 0) && (right.Number > 0)) ? 1 : 0;
                break;
            }

            case Expression::Or: { // ||
                left.Number = ((left.Number > 0) || (right.Number > 0)) ? 1 : 0;
                break;
            }

            case Expression::Equal:      // ==
            case Expression::NotEqual: { // !=
                bool is_equal;

                if (callback->ALEIsEqual(is_equal, content, left, right, left_evaluated, right_evaluated)) {
                    if (exp == Expression::Equal) {
                        left.Number = (is_equal ? 1 : 0);
                    } else {
                        left.Number = (is_equal ? 0 : 1);
                    }

                    break;
                }

                return false;
            }

            default: {
            }
        }

        return true;
    }
};

template <typename Char_T_>
class ALEBasicHelper {
  public:
    static bool ALESetNumber(double &number, const Char_T_ *content, SizeT length) noexcept {
        return false;

        (void)number;
        (void)content;
        (void)length;
    }

    static bool ALEIsEqual(bool &result, const Char_T_ *content, ALE::Number left, ALE::Number right,
                           bool left_evaluated, bool right_evaluated) noexcept {
        using ALEExpressions_T_ = ALEExpressions<Char_T_>;

        if (!left_evaluated) {
            const Char_T_ *left_content = (content + left.Content.Offset);

            if (*left_content != ALEExpressions_T_::ParenthesStart) {
                if (!(Digit<Char_T_>::StringToNumber(left.Number, left_content, left.Content.Length))) {
                    return false;
                }
            } else if (!(ALE::Evaluate(left.Number, (++left_content), (left.Content.Length - 2)))) {
                return false;
            }
        }

        if (!right_evaluated) {
            const Char_T_ *right_content = (content + right.Content.Offset);

            if (*right_content != ALEExpressions_T_::ParenthesStart) {
                if (!(Digit<Char_T_>::StringToNumber(right.Number, right_content, right.Content.Length))) {
                    return false;
                }
            } else if (!(ALE::Evaluate(right.Number, (++right_content), (right.Content.Length - 2)))) {
                return false;
            }
        }

        result = (left.Number == right.Number);
        return true;
    }
};

template <typename Char_T_>
class ALEExpressions {
  public:
    static constexpr Char_T_ RemainderOp = '%';
    static constexpr Char_T_ MultipleOp  = '*';
    static constexpr Char_T_ DivideOp    = '/';
    static constexpr Char_T_ AddOp       = '+';
    static constexpr Char_T_ SubtractOp  = '-';
    static constexpr Char_T_ EqualOp     = '=';
    static constexpr Char_T_ NotOp       = '!';
    static constexpr Char_T_ LessOp      = '<';
    static constexpr Char_T_ BiggerOp    = '>';
    static constexpr Char_T_ AndOp       = '&';
    static constexpr Char_T_ OrOp        = '|';

    static constexpr Char_T_ ParenthesStart = '(';
    static constexpr Char_T_ ParenthesEnd   = ')';
    static constexpr Char_T_ BracketStart   = '{';
    static constexpr Char_T_ BracketEnd     = '}';
    static constexpr Char_T_ ExponentOp     = '^';
    static constexpr Char_T_ SpaceChar      = ' ';
    static constexpr Char_T_ ColonChar      = ':';
    static constexpr Char_T_ SlashChar      = '/';
};

} // namespace Qentem

#endif
