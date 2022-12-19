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

template <typename, typename>
class ALESub;

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
            // int is half the size of double. DONT change the type, or it will break.
            unsigned int Offset{0};
            unsigned int Length{0};
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
        Addition,       // +
        Subtraction,    // -
        Multiplication, // *
        Division,       // /
        Remainder,      // %
        Exponent,       // ^
        Error           // X
    };

    template <typename Char_T_, typename Helper_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length, const Helper_T_ *callback) noexcept {
        const ALESub<Char_T_, Helper_T_> ale_sub = ALESub<Char_T_, Helper_T_>(callback, content);

        Number num;

        if (ale_sub.Parse(num, 0, length)) {
            number = num.Number;
            return true;
        }

        number = 0;
        return false;
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
    struct ALESub {
        ALESub(const Helper_T_ *helper, const Char_T_ *content) noexcept : helper_{helper}, content_{content} {}

        bool Parse(Number &number, SizeT offset, const SizeT length) const noexcept {
            const SizeT num_offset = offset;
            Expression  expr       = getExpression(offset, length);
            return subParse(number, offset, num_offset, length, expr, Expression::None);
        }

      private:
        bool subParse(Number &left, SizeT &offset, SizeT num_offset, const SizeT length, Expression &expr,
                      const Expression previous_expr) const noexcept {
            Number     right;
            Expression next_expr;
            const bool no_equal = ((expr != Expression::Equal) && (expr != Expression::NotEqual));

            if ((expr != Expression::Error) && getNumber(left, num_offset, (offset - num_offset), expr)) {
                while (offset < length) {
                    ++offset;

                    if (expr < Expression::Bigger) {
                        ++offset;
                    }

                    num_offset = offset;
                    next_expr  = getExpression(offset, length);

                    if (expr >= next_expr) {
                        if (!getNumber(right, num_offset, (offset - num_offset), expr) ||
                            !process(left, right, no_equal, false, expr)) {
                            return false;
                        }

                        expr = next_expr;

                        if (previous_expr >= next_expr) {
                            break;
                        }
                    } else {
                        if (!subParse(right, offset, num_offset, length, next_expr, expr) ||
                            !process(left, right, no_equal, true, expr)) {
                            return false;
                        }

                        expr = next_expr;
                    }
                }

                return true;
            }

            return false;
        }

        bool getNumber(Number &val, SizeT offset, SizeT length, const Expression expr) const noexcept {
            using ALEExpressions_T_ = ALEExpressions<Char_T_>;

            StringUtils::Trim(content_, offset, length);

            if ((expr == Expression::Equal) || (expr == Expression::NotEqual)) {
                val.Content.Offset = static_cast<unsigned int>(offset);
                val.Content.Length = static_cast<unsigned int>(length);
                return true;
            }

            switch (content_[offset]) {
                case ALEExpressions_T_::ParenthesStart: {
                    // getExpression check for closed parenthes, so "length" will never go over the actual length.
                    length += offset;
                    ++offset; // passing (
                    --length; // before )

                    return Parse(val, offset, length);
                }

                case ALEExpressions_T_::BracketStart: {
                    return (helper_->ALESetNumber(val.Number, (content_ + offset), length));
                }

                default: {
                    return (Digit<Char_T_>::StringToNumber(val.Number, (content_ + offset), length));
                }
            }
        }

        bool process(Number &left, Number right, bool left_evaluated, bool right_evaluated,
                     Expression expr) const noexcept {
            switch (expr) {
                case Expression::Exponent: { // ^
                    if (right.Number != 0.0) {
                        // TODO: Needs more work to evaluate fractions
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

                    if (helper_->ALEIsEqual(is_equal, content_, left, right, left_evaluated, right_evaluated)) {
                        if (expr == Expression::Equal) {
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

        Expression getExpression(SizeT &offset, const SizeT length) const noexcept {
            using ALEExpressions_T_ = ALEExpressions<Char_T_>;

            while (offset < length) {
                switch (content_[offset]) {
                    case ALEExpressions_T_::OrExp: { // ||
                        if (content_[(offset + 1)] == ALEExpressions_T_::OrExp) {
                            return Expression::Or;
                        }

                        return Expression::Error;
                    }

                    case ALEExpressions_T_::AndExp: { // &&
                        if (content_[(offset + 1)] == ALEExpressions_T_::AndExp) {
                            return Expression::And;
                        }

                        return Expression::Error;
                    }

                    case ALEExpressions_T_::BiggerExp: { // > or >=
                        if (content_[(offset + 1)] == ALEExpressions_T_::EqualExp) {
                            return Expression::BiggerOrEqual;
                        }

                        return Expression::Bigger;
                    }

                    case ALEExpressions_T_::LessExp: { // < or <=
                        if (content_[(offset + 1)] == ALEExpressions_T_::EqualExp) {
                            return Expression::LessOrEqual;
                        }

                        return Expression::Less;
                    }

                    case ALEExpressions_T_::NotExp: { // !=
                        if (content_[(offset + 1)] == ALEExpressions_T_::EqualExp) {
                            return Expression::NotEqual;
                        }

                        return Expression::Error;
                    }

                    case ALEExpressions_T_::EqualExp: { // ==
                        if (content_[(offset + 1)] == ALEExpressions_T_::EqualExp) {
                            return Expression::Equal;
                        }

                        return Expression::Error;
                    }

                    case ALEExpressions_T_::SubtractExp: {
                        if (isExpression(content_, offset)) {
                            return Expression::Subtraction;
                        }

                        break;
                    }

                    case ALEExpressions_T_::AddExp: {
                        if (isExpression(content_, offset)) {
                            return Expression::Addition;
                        }

                        break;
                    }

                    case ALEExpressions_T_::DivideExp: {
                        return Expression::Division;
                    }

                    case ALEExpressions_T_::MultipleExp: {
                        return Expression::Multiplication;
                    }

                    case ALEExpressions_T_::RemainderExp: {
                        return Expression::Remainder;
                    }

                    case ALEExpressions_T_::ExponentExp: {
                        return Expression::Exponent;
                    }

                    case ALEExpressions_T_::ParenthesStart: {
                        // (...) are evaluated to numbers.

                        ++offset;
                        offset = Engine::SkipInnerPatterns<Char_T_>(ALEExpressions_T_::ParenthesStart,
                                                                    ALEExpressions_T_::ParenthesEnd, content_, offset,
                                                                    length);

                        if (offset != 0) {
                            continue;
                        }

                        return Expression::Error;
                    }

                    case ALEExpressions_T_::BracketStart: {
                        // {...} are evaluated by callback to a number or
                        // string.

                        ++offset;
                        offset = Engine::FindOne<Char_T_>(ALEExpressions_T_::BracketEnd, content_, offset, length);

                        if (offset != 0) {
                            continue;
                        }

                        offset = length;
                        return Expression::Error;
                    }

                    default: {
                    }
                }

                ++offset;
            }

            return Expression::None;
        }

        const Helper_T_ *helper_;
        const Char_T_   *content_;
    };

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
                if (!(Digit<Char_T_>::StringToNumber(left.Number, left_content,
                                                     static_cast<SizeT>(left.Content.Length)))) {
                    return false;
                }
            } else if (!(ALE::Evaluate(left.Number, (++left_content), static_cast<SizeT>(left.Content.Length - 2u)))) {
                return false;
            }
        }

        if (!right_evaluated) {
            const Char_T_ *right_content = (content + right.Content.Offset);

            if (*right_content != ALEExpressions_T_::ParenthesStart) {
                if (!(Digit<Char_T_>::StringToNumber(right.Number, right_content,
                                                     static_cast<SizeT>(right.Content.Length)))) {
                    return false;
                }
            } else if (!(ALE::Evaluate(right.Number, (++right_content),
                                       static_cast<SizeT>(right.Content.Length - 2u)))) {
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
    static constexpr Char_T_ RemainderExp = '%';
    static constexpr Char_T_ MultipleExp  = '*';
    static constexpr Char_T_ DivideExp    = '/';
    static constexpr Char_T_ AddExp       = '+';
    static constexpr Char_T_ SubtractExp  = '-';
    static constexpr Char_T_ EqualExp     = '=';
    static constexpr Char_T_ NotExp       = '!';
    static constexpr Char_T_ LessExp      = '<';
    static constexpr Char_T_ BiggerExp    = '>';
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
