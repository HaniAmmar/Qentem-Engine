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

#include "Array.hpp"
#include "Digit.hpp"
#include "Engine.hpp"
#include "String.hpp"

#ifndef QENTEM_ALE_H_
#define QENTEM_ALE_H_

namespace Qentem {

struct ALEHelper {
    virtual bool ALESetNumber(double &number, const char *content,
                              UInt length) const                        = 0;
    virtual bool ALEIsEqual(bool &result, const char *left, UInt left_length,
                            const char *right, UInt right_length) const = 0;
};

/*
 * Arithmetic and logic evaluation
 */
class ALE : Engine {
    struct Item_;
    struct FindCache_;

  public:
    static bool Evaluate(double &number, const char *content, UInt length,
                         const ALEHelper *callback = nullptr) {
        number = 0;

        if ((content != nullptr) && (length != 0)) {
            Array<Item_> items;
            FindCache_   fc = FindCache_();
            ALE(&items, callback, &fc).FindNest(content, 0, length, length);

            if (items.Size() != 0) {
                sortOperations(items, content, 0, length);
                return process(number, items, content, 0, length, callback);
            }

            UInt offset = 0;
            String::SoftTrim(content, offset, length);
            return Digit::StringToNumber(number, (content + offset), length);
        }

        return false;
    }

    inline static bool Evaluate(double &number, const char *content,
                                const ALEHelper *callback = nullptr) {
        return Evaluate(number, content, String::Count<UInt>(content),
                        callback);
    }

    static double Evaluate(const char *content, UInt length,
                           const ALEHelper *callback = nullptr) {
        double number;

        if (Evaluate(number, content, length, callback)) {
            return number;
        }

        return 0;
    }

    inline static double Evaluate(const char *     content,
                                  const ALEHelper *callback = nullptr) {
        return Evaluate(content, String::Count<UInt>(content), callback);
    }

    //////////// Private ////////////

  private:
    enum class Operation_ : UInt {
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

    struct Item_ {
        UInt         Offset{0};
        UInt         Length{0};
        Operation_   Op{Operation_::Parentheses};
        Array<Item_> SubItems{};
    };

    explicit ALE(Array<Item_> *items, const ALEHelper *callback,
                 FindCache_ *find_cache) noexcept
        : items_(items), callback_(callback), find_cache_(find_cache) {
    }

    ULong nest(const char *content, ULong offset, ULong end_before,
               ULong max_end_before) final {
        if ((offset + 1) != end_before) {
            return ALE(&(item_.SubItems), callback_, find_cache_)
                .FindNest(content, offset, end_before, max_end_before);
        }

        return 0;
    }

    void Found(const char *content, ULong offset, ULong end_before,
               ULong start_offset, ULong &current_offset) final {
        item_.Offset = static_cast<UInt>(start_offset - item_.Length);

        switch (item_.Op) {
            case Operation_::Brackets: {
                item_.Length =
                    static_cast<UInt>((current_offset + 1) - start_offset);
                break;
            }

            case Operation_::Parentheses: {
                // Set the length and drop )
                item_.Length =
                    static_cast<UInt>(current_offset - (start_offset + 1));

                // Drop (
                ++item_.Offset;

                break;
            }

            case Operation_::Addition:
            case Operation_::Subtraction: {
                /*
                 * If there is a number before - + or ) or } then add it as an
                 * operation not a sign.
                 */
                --start_offset; // Going back to the sign.

                if (start_offset != offset) {
                    do {
                        --start_offset;
                        const char tmp_c = content[start_offset];

                        if (tmp_c != ' ') {
                            switch (tmp_c) {
                                case ')':
                                case '}':
                                    (*items_) += static_cast<Item_ &&>(item_);
                                    return;

                                default:
                                    if ((tmp_c < ':') && (tmp_c > '/')) {
                                        (*items_) +=
                                            static_cast<Item_ &&>(item_);
                                    }
                            }

                            break;
                        }
                    } while (start_offset != 0);
                } else if (items_->Size() != 0) {
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

    void failed() noexcept final {
        items_->Clear();
    }

    inline bool hasTail() const noexcept final {
        return ((item_.Op == Operation_::Brackets) ||
                (item_.Op == Operation_::Parentheses));
    }

    ULong find2(const char *content, ULong offset,
                ULong end_before) const noexcept final {
        if (item_.Op == Operation_::Parentheses) {
            return FindOne(')', content, offset, end_before);
        }

        return FindOne('}', content, offset, end_before);
    }

    inline ULong find(const char *content, ULong offset,
                      ULong end_before) noexcept final {
        bool found   = false;
        item_.Length = 1;

        while (offset < end_before) {
            switch (content[offset]) {
                case '{': {
                    found    = true;
                    item_.Op = Operation_::Brackets;
                    break;
                }

                case '(': {
                    found    = true;
                    item_.Op = Operation_::Parentheses;
                    break;
                }

                case '^': {
                    found    = true;
                    item_.Op = Operation_::Exponent;
                    break;
                }

                case '%': {
                    found    = true;
                    item_.Op = Operation_::Remainder;
                    break;
                }

                case '*': {
                    found    = true;
                    item_.Op = Operation_::Multiplication;
                    break;
                }

                case '/': {
                    found    = true;
                    item_.Op = Operation_::Division;
                    break;
                }

                case '+': {
                    found    = true;
                    item_.Op = Operation_::Addition;
                    break;
                }

                case '-': {
                    found    = true;
                    item_.Op = Operation_::Subtraction;
                    break;
                }

                case '=': { // ==
                    ++offset;

                    if (content[offset] == '=') {
                        found        = true;
                        item_.Op     = Operation_::Equal;
                        item_.Length = 2;
                    }

                    break;
                }

                case '!': { // !=
                    ++offset;

                    if (content[offset] == '=') {
                        found        = true;
                        item_.Op     = Operation_::NotEqual;
                        item_.Length = 2;
                    }

                    break;
                }

                case '<': { // < or <=
                    if (content[(offset + 1)] == '=') {
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

                case '>': { // > or >=
                    if (content[(offset + 1)] == '=') {
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

                case '&': { // &&
                    ++offset;

                    if (content[offset] == '&') {
                        found        = true;
                        item_.Op     = Operation_::And;
                        item_.Length = 2;
                    }

                    break;
                }

                case '|': { // ||
                    ++offset;

                    if (content[offset] == '|') {
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

        // No match.
        return 0;
    }

    static void sortOperations(Array<Item_> &items, const char *content,
                               UInt offset, UInt length) {
        // Determine the highest operation.
        Operation_ highest = Operation_::Parentheses;

        const Item_ *item = items.Storage();
        const Item_ *end  = (items.Storage() + items.Size());

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

        // If it's just (...)
        if (highest < Operation_::Exponent) {
            if (items.Size() == 1) {
                Item_ *item2 = items.First();

                if (item2->Op == Operation_::Parentheses) {
                    const UInt len = (item2->Length + 2U);

                    if (length != len) {
                        // Cheking for anything extra.
                        String::SoftTrim(content, offset, length);

                        if (length != len) {
                            items.Clear();
                            return;
                        }
                    }

                    // Sort ops inside (...)
                    String::SoftTrim(content, item2->Offset, item2->Length);
                    sortOperations(item2->SubItems, content, item2->Offset,
                                   item2->Length);
                } else if (length != item2->Length) { // Brackets
                    // Cheking for anything extra.
                    String::SoftTrim(content, offset, length);

                    if (length != item2->Length) {
                        items.Clear();
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

        ULong       id      = 0;
        const ULong size    = items.Size();
        Item_ *     match   = (items.First() - 1);
        Item_ *     n_match = items.First();
        Item_       n_item;
        const UInt  endOffset = (offset + length);

        for (ULong i = 0; i <= size; i++) {
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

            String::SoftTrim(content, n_item.Offset, n_item.Length);

            if (n_item.SubItems.Size() != 0) {
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
            items.SoftResize(id);
        }
    }

    static bool process(double &left_number, const Array<Item_> &items,
                        const char *content, UInt offset, UInt length,
                        const ALEHelper *callback) {
        if (items.Size() == 0) {
            return Digit::StringToNumber(left_number, (content + offset),
                                         length);
        }

        const Item_ *item  = items.Storage();
        const Item_ *item2 = (item + 1);
        Operation_   op    = item->Op;

        switch (op) {
            case Operation_::Equal:
            case Operation_::NotEqual: {
                bool is_equal;

                const char *l_item = (content + item->Offset);
                const char *r_item = (content + item2->Offset);

                if ((*l_item == '{') || (*r_item == '{')) {
                    if ((callback == nullptr) ||
                        !(callback->ALEIsEqual(is_equal, l_item, item->Length,
                                               r_item, item2->Length))) {
                        return false;
                    }
                } else if (process(left_number, item->SubItems, content,
                                   item->Offset, item->Length, callback)) {
                    break;
                } else {
                    is_equal = ((item->Length == item2->Length) &&
                                Memory::Compare(l_item, r_item, item2->Length));
                }

                if (op == Operation_::NotEqual) {
                    is_equal = !is_equal;
                }

                left_number = (is_equal ? 1 : 0);
                return true;
            }

            case Operation_::Brackets: {
                return ((callback != nullptr) &&
                        callback->ALESetNumber(left_number,
                                               (content + item->Offset),
                                               item->Length));
            }

            default: {
                if (!process(left_number, item->SubItems, content, item->Offset,
                             item->Length, callback)) {
                    return false;
                }
            }
        }

        const Item_ *end = (items.Storage() + items.Size());

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

    struct FindCache_ {
        ULong        Offset{0};
        ULong        NextOffset{0};
        QMM_Number_T Bits{0};
    };

    Item_            item_;
    Array<Item_> *   items_;
    const ALEHelper *callback_;
    FindCache_ *     find_cache_{nullptr};
};

} // namespace Qentem

#endif
