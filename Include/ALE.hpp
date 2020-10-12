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

#include "ALEOperations.hpp"
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
struct ALEHelper;

class ALE {
  public:
    ALE()            = delete;
    ALE(ALE &&)      = delete;
    ALE(const ALE &) = delete;
    ALE &operator=(ALE &&) = delete;
    ALE &operator=(const ALE &) = delete;
    ~ALE()                      = delete;

    template <typename Char_T_, typename Helper_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length,
                         const Helper_T_ *callback) noexcept {
        Value_T_  num;
        Operation current_op = Operation::None;
        SizeT     offset     = 0;

        if (parse(current_op, num, content, offset, length, callback)) {
            number = num.number;
            return true;
        }

        return false;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content,
                                const Helper_T_ *callback) noexcept {
        return Evaluate(number, content, StringUtils::Count(content), callback);
    }

    template <typename Char_T_, typename Helper_T_>
    static double Evaluate(const Char_T_ *content, SizeT length,
                           const Helper_T_ *callback) noexcept {
        double number;

        if (Evaluate(number, content, length, callback)) {
            return number;
        }

        return 0;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static double Evaluate(const Char_T_ *  content,
                                  const Helper_T_ *callback) noexcept {
        return Evaluate(content, StringUtils::Count(content), callback);
    }

    //////////////////////////// For testing //////////////////////////////////

    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content,
                                SizeT length) noexcept {
        static ALEHelper<Char_T_> helper;
        return Evaluate(number, content, length, &helper);
    }

    template <typename Char_T_>
    inline static bool Evaluate(double &       number,
                                const Char_T_ *content) noexcept {
        return Evaluate(number, content, StringUtils::Count(content));
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content,
                                  SizeT          length) noexcept {
        double number;

        if (Evaluate(number, content, length)) {
            return number;
        }

        return 0;
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content) noexcept {
        return Evaluate(content, StringUtils::Count(content));
    }

    enum class Operation {
        None = 0,
        Or,
        And,
        BiggerOrEqual,
        Bigger,
        LessOrEqual,
        Less,

        NotEqual,
        Equal,
        NumberNotEqual,
        NumberEqual,
        HelperIsNotEqual,
        HelperIsEqual,

        Subtraction,
        Addition,
        Division,
        Multiplication,
        Remainder,
        Exponent,
        Error
    };

  private:
    union Value_T_ {
        double       number;
        unsigned int str[2]{0, 0}; // 0 for the offset and 1 for the length.
    };

    template <typename Char_T_, typename Helper_T_>
    static bool parse(Operation &current_op, Value_T_ &left,
                      const Char_T_ *content, SizeT &offset, SizeT end_offset,
                      const Helper_T_ *callback) noexcept {
        SizeT     previous_offset = offset;
        Operation op_w;
        Operation op = nextOperation(op_w, content, offset, end_offset);

        if (getNumber(left, content, previous_offset,
                      (offset - previous_offset), callback, op)) {
            advance(op, offset);
            previous_offset = offset;

            while (offset < end_offset) {
                Value_T_        right;
                Operation       next_op_w;
                const Operation next_op =
                    nextOperation(next_op_w, content, offset, end_offset);

                if (next_op_w > op_w) {
                    Operation tmp_op = op;

                    if (parse(tmp_op, right, content, previous_offset,
                              end_offset, callback) &&
                        process(content, left, right, op, callback)) {
                        op     = tmp_op;
                        offset = previous_offset;
                        continue;
                    }
                } else if (getNumber(right, content, previous_offset,
                                     (offset - previous_offset), callback,
                                     op) &&
                           process(content, left, right, op, callback)) {
                    advance(next_op, offset);

                    if (next_op_w < current_op) {
                        current_op = next_op;
                        return true;
                    }

                    op              = next_op;
                    op_w            = next_op_w;
                    previous_offset = offset;
                    continue;
                }

                return false;
            }

            return (op == Operation::None);
        }

        return false;
    }

    static void advance(Operation op, SizeT &offset) {
        switch (op) {
            case Operation::Equal:
            case Operation::NotEqual:
            case Operation::NumberEqual:
            case Operation::NumberNotEqual:
            case Operation::HelperIsEqual:
            case Operation::HelperIsNotEqual:
            case Operation::LessOrEqual:
            case Operation::BiggerOrEqual:
            case Operation::And:
            case Operation::Or: {
                offset += 2;
                break;
            }

            default: {
                ++offset;
            }
        }
    }

    template <typename Char_T_>
    static Operation nextOperation(Operation &weight, const Char_T_ *content,
                                   SizeT &offset, SizeT end_offset) noexcept {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        while (offset < end_offset) {
            switch (content[offset]) {
                case ALEOperations_T_::OrOp: { // ||
                    if (content[(offset + 1)] == ALEOperations_T_::OrOp) {
                        weight = Operation::And;
                        return Operation::Or;
                    }

                    weight = Operation::None;
                    return Operation::Error;
                }

                case ALEOperations_T_::AndOp: { // &&
                    if (content[(offset + 1)] == ALEOperations_T_::AndOp) {
                        weight = Operation::And;
                        return Operation::And;
                    }

                    weight = Operation::None;
                    return Operation::Error;
                }

                case ALEOperations_T_::BiggerOp: { // > or >=
                    weight = Operation::BiggerOrEqual;

                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        return Operation::BiggerOrEqual;
                    }

                    return Operation::Bigger;
                }

                case ALEOperations_T_::LessOp: { // < or <=
                    weight = Operation::LessOrEqual;

                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        return Operation::LessOrEqual;
                    }

                    return Operation::Less;
                }

                case ALEOperations_T_::NotOp: { // !=
                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        weight = Operation::Equal;
                        return Operation::NotEqual;
                    }

                    weight = Operation::None;
                    return Operation::Error;
                }

                case ALEOperations_T_::EqualOp: { // ==
                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        weight = Operation::Equal;
                        return Operation::Equal;
                    }

                    weight = Operation::None;
                    return Operation::Error;
                }

                case ALEOperations_T_::SubtractOp: {
                    if (isOperation(content, offset)) {
                        weight = Operation::Addition;
                        return Operation::Subtraction;
                    }

                    break;
                }

                case ALEOperations_T_::AddOp: {
                    if (isOperation(content, offset)) {
                        weight = Operation::Addition;
                        return Operation::Addition;
                    }

                    break;
                }

                case ALEOperations_T_::DivideOp: {
                    weight = Operation::Multiplication;
                    return Operation::Division;
                }

                case ALEOperations_T_::MultipleOp: {
                    weight = Operation::Multiplication;
                    return Operation::Multiplication;
                }

                case ALEOperations_T_::RemainderOp: {
                    weight = Operation::Remainder;
                    return Operation::Remainder;
                }

                case ALEOperations_T_::ExponentOp: {
                    weight = Operation::Exponent;
                    return Operation::Exponent;
                }

                case ALEOperations_T_::ParenthesStart: {
                    // (...) are evaluated to numbers.

                    ++offset;
                    offset = Engine::SkipInnerPatterns(
                        ALEOperations_T_::ParenthesStart,
                        ALEOperations_T_::ParenthesEnd, content, offset,
                        end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    offset = end_offset;
                    weight = Operation::None;
                    return Operation::Error;
                }

                case ALEOperations_T_::BracketStart: {
                    // {...} are evaluated to numbers or strings.

                    ++offset;
                    offset = Engine::FindOne(ALEOperations_T_::BracketEnd,
                                             content, offset, end_offset);

                    if (offset != 0) {
                        continue;
                    }

                    offset = end_offset;
                    weight = Operation::None;
                    return Operation::Error;
                }
            }

            ++offset;
        }

        weight = Operation::None;
        return Operation::None;
    }

    template <typename Char_T_>
    static bool isOperation(const Char_T_ *content, SizeT offset) noexcept {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        while (offset != 0) {
            --offset;

            switch (content[offset]) {
                case ALEOperations_T_::SpaceChar: {
                    break;
                }

                case ALEOperations_T_::ParenthesEnd:
                case ALEOperations_T_::BracketEnd: {
                    // (...) and {} are numbers.
                    return true;
                }

                default: {
                    // A number
                    return ((content[offset] < ALEOperations_T_::ColonChar) &&
                            (content[offset] > ALEOperations_T_::SlashChar));
                }
            }
        }

        return false;
    }

    template <typename Char_T_, typename Helper_T_>
    static bool getNumber(Value_T_ &val, const Char_T_ *content, SizeT offset,
                          SizeT length, const Helper_T_ *callback,
                          Operation &op) noexcept {
        if (op != Operation::Error) {
            StringUtils::SoftTrim(content, offset, length);

            switch (content[offset]) {
                case ALEOperations<Char_T_>::ParenthesStart: {
                    length += offset;
                    ++offset;
                    --length;

                    Operation current_op = Operation::None;
                    return parse(current_op, val, content, offset, length,
                                 callback);
                }

                case ALEOperations<Char_T_>::BracketStart: {
                    val.str[0] = offset;
                    val.str[1] = length;

                    switch (op) {
                        case Operation::HelperIsEqual:
                        case Operation::HelperIsNotEqual: {
                            return true;
                        }

                        case Operation::Equal: {
                            op = Operation::HelperIsEqual;
                            return true;
                        }

                        case Operation::NotEqual: {
                            op = Operation::HelperIsNotEqual;
                            return true;
                        }

                        default: {
                            return (callback->ALESetNumber(
                                val.number, (content + offset), length));
                        }
                    }
                }

                default: {
                    switch (op) {
                        case Operation::HelperIsEqual:
                        case Operation::HelperIsNotEqual: {
                            val.str[0] = offset;
                            val.str[1] = length;
                            return true;
                        }

                        case Operation::Equal:
                        case Operation::NotEqual: {
                            bool bool_val = Digit<Char_T_>::StringToNumber(
                                val.number, (content + offset), length);

                            if (!bool_val) {
                                val.str[0] = offset;
                                val.str[1] = length;

                                if (op == Operation::Equal) {
                                    op = Operation::HelperIsEqual;
                                } else {
                                    op = Operation::HelperIsNotEqual;
                                }
                            } else if (op == Operation::Equal) {
                                op = Operation::NumberEqual;
                            } else {
                                op = Operation::NumberNotEqual;
                            }

                            return true;
                        }

                        default: {
                            return (Digit<Char_T_>::StringToNumber(
                                val.number, (content + offset), length));
                        }
                    }
                }
            }
        }

        return false;
    }

    template <typename Char_T_, typename Helper_T_>
    static bool process(const Char_T_ *content, Value_T_ &left, Value_T_ right,
                        Operation op, const Helper_T_ *callback) noexcept {
        switch (op) {
            case Operation::Exponent: { // ^
                if (right.number != 0.0) {
                    // NOTE: Needs more work.
                    if (left.number != 0.0) {
                        const bool neg = (right.number < 0);

                        if (neg) {
                            right.number *= -1;
                        }

                        if (right.number < 1) {
                            return false;
                        }

                        unsigned int times =
                            static_cast<unsigned int>(right.number);
                        const double num = left.number;

                        while (--times != 0) {
                            left.number *= num;
                        }

                        if (neg) {
                            left.number = (1 / left.number);
                        }
                    }

                    break;
                }

                left.number = 1;
                break;
            }

            case Operation::Remainder: { // %
                left.number =
                    static_cast<double>(static_cast<ULong>(left.number) %
                                        static_cast<ULong>(right.number));
                break;
            }

            case Operation::Multiplication: { // *
                left.number *= right.number;
                break;
            }

            case Operation::Division: { // /
                if (right.number != 0.0) {
                    left.number /= right.number;
                    break;
                }

                return false;
            }

            case Operation::Addition: { // +
                left.number += right.number;
                break;
            }

            case Operation::Subtraction: { // -
                left.number -= right.number;
                break;
            }

            case Operation::Equal:
            case Operation::NumberEqual: { // ==
                left.number = (left.number == right.number) ? 1 : 0;
                break;
            }

            case Operation::NotEqual:
            case Operation::NumberNotEqual: { // !=
                left.number = (left.number != right.number) ? 1 : 0;
                break;
            }

            case Operation::Less: { // <
                left.number = (left.number < right.number) ? 1 : 0;
                break;
            }

            case Operation::LessOrEqual: { // <=
                left.number = (left.number <= right.number) ? 1 : 0;
                break;
            }

            case Operation::Bigger: { // >
                left.number = (left.number > right.number) ? 1 : 0;
                break;
            }

            case Operation::BiggerOrEqual: { // >=
                left.number = (left.number >= right.number) ? 1 : 0;
                break;
            }

            case Operation::And: { // &&
                left.number = ((left.number > 0) && (right.number > 0)) ? 1 : 0;
                break;
            }

            case Operation::Or: { // ||
                left.number = ((left.number > 0) || (right.number > 0)) ? 1 : 0;
                break;
            }

            case Operation::HelperIsEqual: { // ==
                bool is_equal;

                if (callback->ALEIsEqual(is_equal, (content + left.str[0]),
                                         left.str[1], (content + right.str[0]),
                                         right.str[1])) {
                    left.number = (is_equal ? 1 : 0);
                    break;
                }

                return false;
            }

            case Operation::HelperIsNotEqual: { // !=
                bool is_equal;

                if (callback->ALEIsEqual(is_equal, (content + left.str[0]),
                                         left.str[1], (content + right.str[0]),
                                         right.str[1])) {
                    left.number = (is_equal ? 0 : 1);
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
struct ALEHelper {
    static bool ALESetNumber(double &number, const Char_T_ *content,
                             SizeT length) noexcept {
        return false;

        (void)number;
        (void)content;
        (void)length;
    }

    static bool ALEIsEqual(bool &result, const Char_T_ *left, SizeT left_length,
                           const Char_T_ *right, SizeT right_length) noexcept {
        return false;
        (void)result;
        (void)left;
        (void)left_length;
        (void)right;
        (void)right_length;
    }
};

} // namespace Qentem

#endif
