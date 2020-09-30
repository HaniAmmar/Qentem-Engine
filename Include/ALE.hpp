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
    bool ALESetNumber(double &number, const Char_T_ *content,
                      SizeT length) const {
        (void)number;
        (void)content;
        (void)length;
        return false;
    }

    bool ALEIsEqual(bool &result, const Char_T_ *left, SizeT left_length,
                    const Char_T_ *right, SizeT right_length) const {
        (void)result;
        (void)left;
        (void)left_length;
        (void)right;
        (void)right_length;
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

    struct Item_;

    template <typename Char_T_, typename Helper_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length,
                         const Helper_T_ *callback) {
        number = 0;

        if ((content != nullptr) && (length != 0)) {
            Array<Item_>               items;
            ALE_T_<Char_T_, Helper_T_> ale{&items, callback};
            Engine::FindNest(content, 0U, length, length, &ale);

            if (items.IsNotEmpty()) {
                ALE_T_<Char_T_, Helper_T_>::sortOperations(items, content, 0,
                                                           length);
                return ALE_T_<Char_T_, Helper_T_>::process(
                    number, items, content, 0, length, callback);
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

    ///////////////////////////////////////////////////////////////////////

    template <typename Char_T_>
    static bool Evaluate(double &number, const Char_T_ *content, SizeT length) {
        ALEHelper<Char_T_> helper;
        return Evaluate(number, content, length, &helper);
    }

    template <typename Char_T_>
    inline static bool Evaluate(double &number, const Char_T_ *content) {
        ALEHelper<Char_T_> helper;
        return Evaluate(number, content, &helper);
    }

    template <typename Char_T_>
    static double Evaluate(const Char_T_ *content, SizeT length) {
        ALEHelper<Char_T_> helper;
        return Evaluate(content, length, &helper);
    }

    template <typename Char_T_>
    inline static double Evaluate(const Char_T_ *content) {
        ALEHelper<Char_T_> helper;
        return Evaluate(content, &helper);
    }

    enum class Operation_ {
        Brackets = 0U,
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

    struct Item_ {
        Array<Item_> SubItems{};
        UInt         Offset{0};
        UInt         Length{0};
        Operation_   Op{Operation_::Parentheses};
    };
};

template <typename Char_T_, typename Helper_T_>
class ALE_T_ {
    friend class ALE;
    friend class Qentem::Engine;

    using Item_            = ALE::Item_;
    using Operation_       = ALE::Operation_;
    using ALEOperations_T_ = ALEOperations<Char_T_>;

  public:
    ALE_T_() = delete;

  private:
    ALE_T_(Array<Item_> *items, const Helper_T_ *callback)
    noexcept : items_(items), callback_(callback) {
    }

    inline bool HasTail() const noexcept {
        return ((item_.Op == Operation_::Brackets) ||
                (item_.Op == Operation_::Parentheses));
    }

    SizeT Nest(const Char_T_ *content, SizeT offset, SizeT end_before,
               SizeT max_end_before) {
        if ((offset + 1) != end_before) {
            ALE_T_<Char_T_, Helper_T_> ale{&(item_.SubItems), callback_};
            return Engine::FindNest(content, offset, end_before, max_end_before,
                                    &ale);
        }

        return 0;
    }

    inline SizeT FindH(const Char_T_ *content, SizeT offset,
                       SizeT end_before) noexcept {
        bool found   = false;
        item_.Length = 1;

        while (offset < end_before) {
            switch (content[offset]) {
                case ALEOperations_T_::BracketOp: {
                    found    = true;
                    item_.Op = Operation_::Brackets;
                    break;
                }

                case ALEOperations_T_::ParenthesOp: {
                    found    = true;
                    item_.Op = Operation_::Parentheses;
                    break;
                }

                case ALEOperations_T_::ExponentOp: {
                    found    = true;
                    item_.Op = Operation_::Exponent;
                    break;
                }

                case ALEOperations_T_::RemainderOp: {
                    found    = true;
                    item_.Op = Operation_::Remainder;
                    break;
                }

                case ALEOperations_T_::MultipleOp: {
                    found    = true;
                    item_.Op = Operation_::Multiplication;
                    break;
                }

                case ALEOperations_T_::DivideOp: {
                    found    = true;
                    item_.Op = Operation_::Division;
                    break;
                }

                case ALEOperations_T_::AddOp: {
                    found    = true;
                    item_.Op = Operation_::Addition;
                    break;
                }

                case ALEOperations_T_::SubtractOp: {
                    found    = true;
                    item_.Op = Operation_::Subtraction;
                    break;
                }

                case ALEOperations_T_::EqualOp: { // ==
                    ++offset;

                    if (content[offset] == ALEOperations_T_::EqualOp) {
                        found        = true;
                        item_.Op     = Operation_::Equal;
                        item_.Length = 2;
                    }

                    break;
                }

                case ALEOperations_T_::NotOp: { // !=
                    ++offset;

                    if (content[offset] == ALEOperations_T_::EqualOp) {
                        found        = true;
                        item_.Op     = Operation_::NotEqual;
                        item_.Length = 2;
                    }

                    break;
                }

                case ALEOperations_T_::LessOp: { // < or <=
                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        found        = true;
                        item_.Op     = Operation_::LessOrEqual;
                        item_.Length = 2;
                        ++offset;
                    } else {
                        found    = true;
                        item_.Op = Operation_::Less;
                    }

                    break;
                }

                case ALEOperations_T_::BiggerOp: { // > or >=
                    if (content[(offset + 1)] == ALEOperations_T_::EqualOp) {
                        found        = true;
                        item_.Op     = Operation_::BiggerOrEqual;
                        item_.Length = 2;
                        ++offset;
                    } else {
                        found    = true;
                        item_.Op = Operation_::Bigger;
                    }

                    break;
                }

                case ALEOperations_T_::AndOp: { // &&
                    ++offset;

                    if (content[offset] == ALEOperations_T_::AndOp) {
                        found        = true;
                        item_.Op     = Operation_::And;
                        item_.Length = 2;
                    }

                    break;
                }

                case ALEOperations_T_::OrOp: { // ||
                    ++offset;

                    if (content[offset] == ALEOperations_T_::OrOp) {
                        found        = true;
                        item_.Op     = Operation_::Or;
                        item_.Length = 2;
                    }
                }

                default: {
                }
            }

            ++offset;

            if (found && (offset < end_before)) {
                return offset;
            }
        }

        return 0; // No match
    }

    SizeT FindT(const Char_T_ *content, SizeT offset,
                SizeT end_before) const noexcept {
        if (item_.Op == Operation_::Parentheses) {
            return Engine::FindOne(ALEOperations_T_::Parenthes2Op, content,
                                   offset, end_before);
        }

        return Engine::FindOne(ALEOperations_T_::Bracket2Op, content, offset,
                               end_before);
    }

    void Found(const Char_T_ *content, SizeT offset, SizeT end_before,
               SizeT start_offset, const SizeT &current_offset) {
        item_.Offset = (start_offset - item_.Length);

        switch (item_.Op) {
            case Operation_::Brackets: {
                item_.Length = ((current_offset + 1) - start_offset);
                break;
            }

            case Operation_::Parentheses: {
                // Set the length and drop )
                item_.Length = (current_offset - (start_offset + 1));

                // Drop (
                ++item_.Offset;

                break;
            }

            case Operation_::Addition:
            case Operation_::Subtraction: {
                /*
                 * If there is a number before - + or ) or } then add it as
                 * an operation not a sign.
                 */
                --start_offset; // Going back to the sign.

                if (start_offset != offset) {
                    do {
                        --start_offset;
                        const Char_T_ tmp_c = content[start_offset];

                        if (tmp_c != ALEOperations_T_::SpaceChar) {
                            switch (tmp_c) {
                                case ALEOperations_T_::Parenthes2Op:
                                case ALEOperations_T_::Bracket2Op:
                                    (*items_) += static_cast<Item_ &&>(item_);
                                    return;

                                default:
                                    if ((tmp_c < ALEOperations_T_::ColonChar) &&
                                        (tmp_c > ALEOperations_T_::SlashChar)) {
                                        (*items_) +=
                                            static_cast<Item_ &&>(item_);
                                    }
                            }

                            break;
                        }
                    } while (start_offset != 0);
                } else if (items_->IsNotEmpty()) {
                    // An OP.
                    break;
                }

                return;
            }

            default: {
            }
        }

        (*items_) += static_cast<Item_ &&>(item_);
        (void)end_before;
    }

    void Failed() const noexcept {
        items_->Reset();
    }

    static void sortOperations(Array<Item_> &items, const Char_T_ *content,
                               SizeT offset, SizeT length) {
        // Determine the highest operation.
        Operation_ highest = Operation_::Parentheses;

        const Item_ *item = items.First();
        const Item_ *end  = items.End();

        while (item != end) {
            if (item->Op > highest) {
                highest = item->Op;

                if ((highest == Operation_::And) ||
                    (highest == Operation_::Or)) {
                    break;
                }
            }

            ++item;
        }

        // If it is just (...)
        if (highest < Operation_::Exponent) {
            if (items.Size() == 1) {
                Item_ *item2 = items.First();

                if (item2->Op == Operation_::Parentheses) {
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
            case Operation_::Remainder: {
                highest = Operation_::Exponent;
                break;
            }

            case Operation_::Division: {
                highest = Operation_::Multiplication;
                break;
            }

            case Operation_::Subtraction: {
                highest = Operation_::Addition;
                break;
            }

            case Operation_::NotEqual:
            case Operation_::Less:
            case Operation_::LessOrEqual:
            case Operation_::Bigger:
            case Operation_::BiggerOrEqual: {
                highest = Operation_::Equal;
                break;
            }

            case Operation_::Or: {
                highest = Operation_::And;
                break;
            }

            default: {
            }
        }

        SizeT       id      = 0;
        const SizeT size    = items.Size();
        Item_ *     match   = (items.First() - 1);
        Item_ *     n_match = items.First();
        Item_       n_item;
        const SizeT endOffset = (offset + length);

        for (SizeT i = 0; i <= size; i++) {
            if (i < size) {
                ++match;

                if (match->Op < highest) {
                    n_item.SubItems += static_cast<Item_ &&>(*match);
                    continue;
                }

                n_item.Length = (match->Offset - offset);
                n_item.Offset = offset;
                n_item.Op     = match->Op;
                offset += (n_item.Length + match->Length);
            } else {
                n_item.Offset = offset;
                n_item.Length = (endOffset - offset);
            }

            StringUtils::SoftTrim(content, n_item.Offset, n_item.Length);

            if (n_item.SubItems.IsNotEmpty()) {
                sortOperations(n_item.SubItems, content, n_item.Offset,
                               n_item.Length);
            }

            if (id < size) {
                (*n_match) = static_cast<Item_ &&>(n_item);
                ++n_match;
                ++id;
            } else {
                items += static_cast<Item_ &&>(n_item);
            }
        }

        if (id != size) {
            items.GoBackTo(id);
        }
    }

    static bool process(double &left_number, const Array<Item_> &items,
                        const Char_T_ *content, SizeT offset, SizeT length,
                        const Helper_T_ *callback) {
        if (items.IsEmpty()) {
            return Digit<Char_T_>::StringToNumber(left_number,
                                                  (content + offset), length);
        }

        const Item_ *item  = items.First();
        const Item_ *item2 = (item + 1);
        Operation_   op    = item->Op;

        switch (op) {
            case Operation_::Equal:
            case Operation_::NotEqual: {
                bool is_equal;

                const Char_T_ *l_item = (content + item->Offset);
                const Char_T_ *r_item = (content + item2->Offset);

                if ((*l_item == ALEOperations_T_::BracketOp) ||
                    (*r_item == ALEOperations_T_::BracketOp)) {
                    if (!(callback->ALEIsEqual(is_equal, l_item, item->Length,
                                               r_item, item2->Length))) {
                        return false;
                    }
                } else if (process(left_number, item->SubItems, content,
                                   item->Offset, item->Length, callback)) {
                    break;
                } else {
                    is_equal =
                        ((item->Length == item2->Length) &&
                         StringUtils::IsEqual(l_item, r_item, item2->Length));
                }

                if (op == Operation_::NotEqual) {
                    is_equal = !is_equal;
                }

                left_number = (is_equal ? 1 : 0);
                return true;
            }

            case Operation_::Brackets: {
                return (callback->ALESetNumber(
                    left_number, (content + item->Offset), item->Length));
            }

            default: {
                if (!process(left_number, item->SubItems, content, item->Offset,
                             item->Length, callback)) {
                    return false;
                }
            }
        }

        const Item_ *end = items.End();

        while (item2 != end) {
            double right_number;
            if (!process(right_number, item2->SubItems, content, item2->Offset,
                         item2->Length, callback)) {
                return false;
            }

            switch (op) {
                case Operation_::Exponent: { // ^
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

                case Operation_::Remainder: { // %
                    left_number =
                        static_cast<double>(static_cast<ULong>(left_number) %
                                            static_cast<ULong>(right_number));
                    break;
                }

                case Operation_::Multiplication: { // *
                    left_number *= right_number;
                    break;
                }

                case Operation_::Division: { // /
                    if (right_number == 0.0) {
                        return false;
                    }

                    left_number /= right_number;
                    break;
                }

                case Operation_::Addition: { // +
                    left_number += right_number;
                    break;
                }

                case Operation_::Subtraction: { // -
                    left_number -= right_number;
                    break;
                }

                case Operation_::Equal: { // ==
                    left_number = (left_number == right_number) ? 1 : 0;
                    break;
                }

                case Operation_::NotEqual: { // !=
                    left_number = (left_number != right_number) ? 1 : 0;
                    break;
                }

                case Operation_::Less: { // <
                    left_number = (left_number < right_number) ? 1 : 0;
                    break;
                }

                case Operation_::LessOrEqual: { // <=
                    left_number = (left_number <= right_number) ? 1 : 0;
                    break;
                }

                case Operation_::Bigger: { // >
                    left_number = (left_number > right_number) ? 1 : 0;
                    break;
                }

                case Operation_::BiggerOrEqual: { // >=
                    left_number = (left_number >= right_number) ? 1 : 0;
                    break;
                }

                case Operation_::And: { // &&
                    left_number =
                        ((left_number > 0) && (right_number > 0)) ? 1 : 0;
                    break;
                }

                case Operation_::Or: { // ||
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

    Array<Item_> *   items_;
    const Helper_T_ *callback_;
    Item_            item_;
};

} // namespace Qentem

#endif
