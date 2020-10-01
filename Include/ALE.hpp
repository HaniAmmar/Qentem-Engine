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
#include "Array.hpp"
#include "Digit.hpp"
#include "Engine.hpp"

#ifndef QENTEM_ALE_H_
#define QENTEM_ALE_H_

namespace Qentem {

template <typename Char_T_>
struct ALEHelper {
    static constexpr bool ALESetNumber(double &, const Char_T_ *, SizeT) {
        return false;
    }

    static constexpr bool ALEIsEqual(bool &, const Char_T_ *, SizeT,
                                     const Char_T_ *, SizeT) {
        return false;
    }
};

/*
 * Arithmetic and logic evaluator
 */
template <typename, typename>
class ALE_T_;

class ALE {
  public:
    ALE()            = delete;
    ALE(ALE &&)      = delete;
    ALE(const ALE &) = delete;
    ALE &operator=(ALE &&) = delete;
    ALE &operator=(const ALE &) = delete;
    ~ALE()                      = delete;

    template <typename, typename>
    friend class ALE_T_;
    struct MathBit;

    template <typename Char_T_, typename Helper_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length,
                         const Helper_T_ *callback) {
        Array<MathBit> items;

        if (parse(items, content, 0, length)) {
            if (items.IsNotEmpty()) {
                sortOperations(items, content, 0, length);
                return process(number, content, 0, length, items, callback);
            }

            SizeT offset = 0;
            StringUtils::SoftTrim(content, offset, length);
            return Digit<Char_T_>::StringToNumber(number, (content + offset),
                                                  length);
        }

        return false;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content,
                                const Helper_T_ *callback) {
        return Evaluate(number, content, StringUtils::Count(content), callback);
    }

    template <typename Char_T_, typename Helper_T_>
    static double Evaluate(const Char_T_ *content, SizeT length,
                           const Helper_T_ *callback) {
        double number;

        if (Evaluate(number, content, length, callback)) {
            return number;
        }

        return 0;
    }

    template <typename Char_T_, typename Helper_T_>
    inline static double Evaluate(const Char_T_ *  content,
                                  const Helper_T_ *callback) {
        return Evaluate(content, StringUtils::Count(content), callback);
    }

    //////////////////////////// For testing //////////////////////////////////
    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content,
                                SizeT length) {
        static ALEHelper<Char_T_> helper;
        return Evaluate(number, content, length, &helper);
    }

    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content) {
        return Evaluate(number, content, StringUtils::Count(content));
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content, SizeT length) {
        double number;

        if (Evaluate(number, content, length)) {
            return number;
        }

        return 0;
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content) {
        return Evaluate(content, StringUtils::Count(content));
    }

    enum class Operation {
        Brackets = 0,
        Parentheses,
        Exponent,
        Remainder,
        Multiplication,
        Division,
        Addition,
        Subtraction,
        Equal,
        NotEqual,
        Less,
        LessOrEqual,
        Bigger,
        BiggerOrEqual,
        And,
        Or
    };

    struct MathBit {
        MathBit() = default;

        MathBit(Array<MathBit> sub_items, UInt offset, UInt length,
                Operation op) noexcept
            : SubItems(sub_items), Offset(offset), Length(length), Op(op) {
        }

        Array<MathBit> SubItems{};
        UInt           Offset{0};
        UInt           Length{0};
        Operation      Op{Operation::Parentheses};
    };

  private:
    template <typename Char_T_>
    static bool parse(Array<MathBit> &items, const Char_T_ *content,
                      SizeT offset, SizeT length) {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        while (offset < length) {
            switch (content[offset]) {
                case ALEOperations_T_::StartParenthesOp: {
                    ++offset;

                    SizeT end_offset = Engine::SkipInnerPatterns(
                        ALEOperations_T_::StartParenthesOp,
                        ALEOperations_T_::EndParenthesOp, content, offset,
                        length);

                    if (end_offset != 0) {
                        items += MathBit{{},
                                         offset,
                                         ((end_offset - 1) - offset),
                                         Operation::Parentheses};

                        MathBit &item = items[(items.Size() - 1)];

                        if (parse(item.SubItems, content, offset, end_offset)) {
                            offset = end_offset;
                            continue;
                        }
                    }

                    return false;
                }

                case ALEOperations_T_::StartBracketOp: {
                    SizeT end_offset =
                        Engine::FindOne(ALEOperations_T_::EndBracketOp, content,
                                        (offset + 1), length);

                    if (end_offset != 0) {
                        items += MathBit{{},
                                         offset,
                                         (end_offset - offset),
                                         Operation::Brackets};
                        offset = end_offset;
                        continue;
                    }

                    return false;
                }

                case ALEOperations_T_::ExponentOp: {
                    items += MathBit{{}, offset, 1, Operation::Exponent};
                    break;
                }

                case ALEOperations_T_::RemainderOp: {
                    items += MathBit{{}, offset, 1, Operation::Remainder};
                    break;
                }

                case ALEOperations_T_::MultipleOp: {
                    items += MathBit{{}, offset, 1, Operation::Multiplication};
                    break;
                }

                case ALEOperations_T_::DivideOp: {
                    items += MathBit{{}, offset, 1, Operation::Division};
                    break;
                }

                case ALEOperations_T_::AddOp: {
                    if (isItOP(content, offset)) {
                        items += MathBit{{}, offset, 1, Operation::Addition};
                    }

                    break;
                }

                case ALEOperations_T_::SubtractOp: {
                    if (isItOP(content, offset)) {
                        items += MathBit{{}, offset, 1, Operation::Subtraction};
                    }

                    break;
                }

                case ALEOperations_T_::EqualOp: { // ==
                    const SizeT current_offset = offset;
                    ++offset;

                    if (content[offset] == ALEOperations_T_::EqualOp) {
                        items +=
                            MathBit{{}, current_offset, 2, Operation::Equal};
                        break;
                    }

                    return false;
                }

                case ALEOperations_T_::NotOp: { // !=
                    const SizeT current_offset = offset;
                    ++offset;

                    if (content[offset] == ALEOperations_T_::EqualOp) {
                        items +=
                            MathBit{{}, current_offset, 2, Operation::NotEqual};
                        break;
                    }

                    return false;
                }

                case ALEOperations_T_::LessOp: { // < or <=
                    SizeT current_offset = offset;
                    ++current_offset;

                    if (content[current_offset] == ALEOperations_T_::EqualOp) {
                        items += MathBit{{}, offset, 2, Operation::LessOrEqual};
                        offset = current_offset;
                    } else {
                        items += MathBit{{}, offset, 1, Operation::Less};
                    }

                    break;
                }

                case ALEOperations_T_::BiggerOp: { // > or >=
                    SizeT current_offset = offset;
                    ++current_offset;

                    if (content[current_offset] == ALEOperations_T_::EqualOp) {
                        items +=
                            MathBit{{}, offset, 2, Operation::BiggerOrEqual};
                        offset = current_offset;
                    } else {
                        items += MathBit{{}, offset, 1, Operation::Bigger};
                    }

                    break;
                }

                case ALEOperations_T_::AndOp: { // &&
                    const SizeT current_offset = offset;
                    ++offset;

                    if (content[offset] == ALEOperations_T_::AndOp) {
                        items += MathBit{{}, current_offset, 2, Operation::And};
                        break;
                    }

                    return false;
                }

                case ALEOperations_T_::OrOp: { // ||
                    const SizeT current_offset = offset;
                    ++offset;

                    if (content[offset] == ALEOperations_T_::OrOp) {
                        items += MathBit{{}, current_offset, 2, Operation::Or};
                        break;
                    }

                    return false;
                }
            }

            ++offset;
        }

        return true;
    }

    template <typename Char_T_>
    static bool isItOP(const Char_T_ *content, SizeT offset) noexcept {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        while (offset != 0) {
            --offset;
            const Char_T_ c = content[offset];

            switch (c) {
                case ALEOperations_T_::SpaceChar: {
                    break;
                }

                case ALEOperations_T_::EndParenthesOp:
                case ALEOperations_T_::EndBracketOp: {
                    return true;
                }

                default: {
                    // Number
                    return ((c < ALEOperations_T_::ColonChar) &&
                            (c > ALEOperations_T_::SlashChar));
                }
            }
        }

        return false;
    }

    template <typename Char_T_>
    static void sortOperations(Array<MathBit> &items, const Char_T_ *content,
                               SizeT offset, SizeT length) {
        // Determine the highest operation.
        Operation highest = Operation::Parentheses;

        const MathBit *item = items.First();
        const MathBit *end  = items.End();

        while (item != end) {
            if (item->Op > highest) {
                highest = item->Op;

                if ((highest == Operation::And) || (highest == Operation::Or)) {
                    break;
                }
            }

            ++item;
        }

        // If it is just (...)
        if (highest < Operation::Exponent) {
            if (items.Size() == 1) {
                MathBit *item2 = items.First();

                if (item2->Op == Operation::Parentheses) {
                    const SizeT len = (item2->Length + 2U);

                    if (length != len) {
                        // Cheking for anything extra.
                        StringUtils::SoftTrim(content, offset, length);

                        if (length != len) {
                            items.Reset();
                            return;
                        }
                    }

                    // Sort ops inside (...)
                    StringUtils::SoftTrim(content, item2->Offset,
                                          item2->Length);
                    sortOperations(item2->SubItems, content, item2->Offset,
                                   item2->Length);
                } else if (length != item2->Length) { // Brackets
                    // Cheking for anything extra.
                    StringUtils::SoftTrim(content, offset, length);

                    if (length != item2->Length) {
                        items.Reset();
                    }
                }
            }

            return;
        }

        /*
         * Less than Exponent        : No operation; could be (...) or {...}
         * Less than Multiplication  : ^ %
         * Less than Addition        : * /
         * Less than Equal           : + -
         * Less than And             : == != < <= > >=
         * The top                   : && ||
         */

        switch (highest) {
            case Operation::Remainder: {
                highest = Operation::Exponent;
                break;
            }

            case Operation::Division: {
                highest = Operation::Multiplication;
                break;
            }

            case Operation::Subtraction: {
                highest = Operation::Addition;
                break;
            }

            case Operation::NotEqual:
            case Operation::Less:
            case Operation::LessOrEqual:
            case Operation::Bigger:
            case Operation::BiggerOrEqual: {
                highest = Operation::Equal;
                break;
            }

            case Operation::Or: {
                highest = Operation::And;
                break;
            }

            default: {
            }
        }

        SizeT       id      = 0;
        const SizeT size    = items.Size();
        MathBit *   match   = (items.First() - 1);
        MathBit *   n_match = items.First();
        MathBit     n_item;
        const SizeT endOffset = (offset + length);

        for (SizeT i = 0; i <= size; i++) {
            if (i < size) {
                ++match;

                if (match->Op < highest) {
                    n_item.SubItems += static_cast<MathBit &&>(*match);
                    continue;
                }

                n_item.Offset = offset;
                n_item.Length = (match->Offset - offset);
                n_item.Op     = match->Op;
                offset += (n_item.Length + match->Length);
            } else {
                n_item.Offset = offset;
                n_item.Length = (endOffset - offset);
            }

            if (n_item.Length == 0) {
                items.Reset();
                return;
            }

            StringUtils::SoftTrim(content, n_item.Offset, n_item.Length);

            if (n_item.SubItems.IsNotEmpty()) {
                sortOperations(n_item.SubItems, content, n_item.Offset,
                               n_item.Length);
            }

            if (id < size) {
                (*n_match) = static_cast<MathBit &&>(n_item);
                ++n_match;
                ++id;
            } else {
                items += static_cast<MathBit &&>(n_item);
            }
        }

        if (id != size) {
            items.GoBackTo(id);
        }
    }

    template <typename Char_T_, typename Helper_T_>
    static bool process(double &left_number, const Char_T_ *content,
                        SizeT offset, SizeT length, const Array<MathBit> &items,
                        const Helper_T_ *callback) {
        using ALEOperations_T_ = ALEOperations<Char_T_>;

        if (items.IsEmpty()) {
            return Digit<Char_T_>::StringToNumber(left_number,
                                                  (content + offset), length);
        }

        const MathBit *item = items.First();
        // Every Op. is splitied into two (left and tight).
        const MathBit *item2 = (item + 1);
        Operation      op    = item->Op;

        switch (op) {
            case Operation::Equal:
            case Operation::NotEqual: {
                bool is_equal;

                const Char_T_ *l_item = (content + item->Offset);
                const Char_T_ *r_item = (content + item2->Offset);

                if ((*l_item == ALEOperations_T_::StartBracketOp) ||
                    (*r_item == ALEOperations_T_::StartBracketOp)) {
                    if (!(callback->ALEIsEqual(is_equal, l_item, item->Length,
                                               r_item, item2->Length))) {
                        return false;
                    }
                } else if (process(left_number, content, item->Offset,
                                   item->Length, item->SubItems, callback)) {
                    break;
                } else {
                    is_equal =
                        ((item->Length == item2->Length) &&
                         StringUtils::IsEqual(l_item, r_item, item2->Length));
                }

                if (op == Operation::NotEqual) {
                    is_equal = !is_equal;
                }

                left_number = (is_equal ? 1 : 0);
                return true;
            }

            case Operation::Brackets: {
                return (callback->ALESetNumber(
                    left_number, (content + item->Offset), item->Length));
            }

            default: {
                if (!process(left_number, content, item->Offset, item->Length,
                             item->SubItems, callback)) {
                    return false;
                }
            }
        }

        const MathBit *end = items.End();

        while (item2 != end) {
            double right_number;
            if (!process(right_number, content, item2->Offset, item2->Length,
                         item2->SubItems, callback)) {
                return false;
            }

            switch (op) {
                case Operation::Exponent: { // ^
                    if (right_number != 0.0) {
                        // NOTE: Needs more work.
                        const bool neg = (right_number < 0);

                        if (neg) {
                            right_number *= -1;
                        }

                        if (right_number < 1) {
                            return false;
                        }

                        if (left_number == 0.0) {
                            break;
                        }

                        UInt         times = static_cast<UInt>(right_number);
                        const double num   = left_number;

                        while (--times != 0) {
                            // NOTE: Optimize.
                            left_number *= num;
                        }

                        if (neg) {
                            left_number = (1 / left_number);
                        }

                        break;
                    }

                    left_number = 1;
                    break;
                }

                case Operation::Remainder: { // %
                    left_number =
                        static_cast<double>(static_cast<ULong>(left_number) %
                                            static_cast<ULong>(right_number));
                    break;
                }

                case Operation::Multiplication: { // *
                    left_number *= right_number;
                    break;
                }

                case Operation::Division: { // /
                    if (right_number == 0.0) {
                        return false;
                    }

                    left_number /= right_number;
                    break;
                }

                case Operation::Addition: { // +
                    left_number += right_number;
                    break;
                }

                case Operation::Subtraction: { // -
                    left_number -= right_number;
                    break;
                }

                case Operation::Equal: { // ==
                    left_number = (left_number == right_number) ? 1 : 0;
                    break;
                }

                case Operation::NotEqual: { // !=
                    left_number = (left_number != right_number) ? 1 : 0;
                    break;
                }

                case Operation::Less: { // <
                    left_number = (left_number < right_number) ? 1 : 0;
                    break;
                }

                case Operation::LessOrEqual: { // <=
                    left_number = (left_number <= right_number) ? 1 : 0;
                    break;
                }

                case Operation::Bigger: { // >
                    left_number = (left_number > right_number) ? 1 : 0;
                    break;
                }

                case Operation::BiggerOrEqual: { // >=
                    left_number = (left_number >= right_number) ? 1 : 0;
                    break;
                }

                case Operation::And: { // &&
                    left_number =
                        ((left_number > 0) && (right_number > 0)) ? 1 : 0;
                    break;
                }

                case Operation::Or: { // ||
                    left_number =
                        ((left_number > 0) || (right_number > 0)) ? 1 : 0;
                    break;
                }

                default: {
                    // Should not get to here.
                }
            }

            op = item2->Op;
            ++item2;
        }

        return true;
    }
};

} // namespace Qentem

#endif
