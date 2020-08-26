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

#include "ALE.hpp"
#include "JSON.hpp"

#ifndef QENTEM_TEMPLATE_H_
#define QENTEM_TEMPLATE_H_

/*
 * Tags:
 *
 *  {...}
 *  - {var:s|n}  var: Variable, s: String, n: Number.
 *
 *  - {math:var|e|n}
 *      - var|e|n: Variable, Equation or Number.
 *
 *  - {if case="var|s" true="var|s" false="var|s"}
 *      - Inline if,  var: Variable, s: String.
 *
 *  <...>
 *  - <loop set="s"? key="s"? value="s"? times="n|var"? index="n|var"?>
 *                                                          ...</loop>
 *      - s: String, n: Number, var: Variable,
 *      - set: child name in the passed colloction: Optional.
 *      - key: the string the will be replaced with keys: Optional.
 *      - value: the current value in the colloction: Optional.
 *          Note: Choose a unique names for key and value.
 *      - times: if set, it will be used instead of the "set" size.
 *      - index: starting index.
 *
 *  - <if case="var|e|n">...<else(if)? ... >?...</if>
 *      - var|e|n: Variable, Equation or Number.
 *      - <else if ....> same as if: Optional.
 *      - <else> no case: Optional.
 */

// NOTE: Implement TemplateCache.

namespace Qentem {

/*
 * Template randering engine.
 */
template <typename Value_ = Value, typename KeyString_ = String>
class Template : Engine, ALEHelper {
    enum class Tag;

  public:
    Template(StringStream *ss, const Value_ *root_value) noexcept
        : ss_(ss), root_value_(root_value) {
    }

    static void Render(StringStream &ss, const char *content,
                       const ULong length, const Value_ *root_value) {
        ULong tmp_offset = 0;

        Template temp(&ss, root_value);

        // To render all remaining tags if there were errors.
        while (temp.not_done_ && ((tmp_offset + 1) < length)) {
            temp.not_done_ = false;
            tmp_offset     = temp.FindNest(content, tmp_offset, length, length);
        }

        if (temp.last_offset_ < length) {
            // Add the remaining string.
            ss.Add((&(content[temp.last_offset_])),
                   (length - temp.last_offset_));
        }
    }

    static String Render(const char *content, const ULong length,
                         const Value_ *root_value) {
        StringStream ss;
        Render(ss, content, length, root_value);

        return ss.GetString();
    }

    inline static String Render(const char *content, const Value_ *root_value) {
        return Render(content, String::Count(content), root_value);
    }

    inline static void Render(StringStream &ss, const String &content,
                              const Value_ *root_value) {
        Render(ss, content.Char(), content.Length(), root_value);
    }

    inline static String Render(const String &content,
                                const Value_ *root_value) {
        return Render(content.Char(), content.Length(), root_value);
    }

  private:
    enum class Tag {
        Variable = 0, // {var:x}
        Math,         // {math:x}
        InLineIf,     // {if:x}
        Loop,         // <loop set="..." key="..." value="...">
        If,           // <if case="...">
    };

    inline bool hasTail() const noexcept final {
        return true;
    }

    ULong find2(const char *content, ULong offset,
                ULong end_before) noexcept final {
        switch (tag_) {
            case Tag::If: {
                return Find("</if>", 5, content, offset, end_before);
            }

            case Tag::Loop: {
                return Find("</loop>", 7, content, offset, end_before);
            }

            default: {
                return Find("}", 1, content, offset, end_before);
            }
        }
    }

    ULong find(const char *content, ULong offset,
               ULong end_before) noexcept final {
        bool  found          = false;
        ULong current_offset = 0;
        UInt  tmp_offset;

        while (offset < end_before) {
            switch (content[offset]) {
                case '{': {
                    current_offset = offset;
                    ++current_offset;

                    switch (content[current_offset]) {
                        case 'v': { // {var:
                            ++current_offset;
                            tmp_offset = 0;

                            while ((tmp_offset < 3) &&
                                   (current_offset < end_before) &&
                                   (content[current_offset] ==
                                    "ar:"[tmp_offset])) {
                                ++current_offset;
                                ++tmp_offset;
                            }

                            if (tmp_offset == 3) {
                                tag_  = Tag::Variable;
                                found = true;
                            }

                            break;
                        }

                        case 'm': { // {math:
                            ++current_offset;
                            tmp_offset = 0;

                            while ((tmp_offset < 4) &&
                                   (current_offset < end_before) &&
                                   (content[current_offset] ==
                                    "ath:"[tmp_offset])) {
                                ++current_offset;
                                ++tmp_offset;
                            }

                            if (tmp_offset == 4) {
                                tag_  = Tag::Math;
                                found = true;
                            }

                            break;
                        }

                        case 'i': { // {if
                            ++current_offset;

                            if (content[current_offset] == 'f') {
                                ++current_offset;
                                tag_  = Tag::InLineIf;
                                found = true;
                            }
                        }
                    }

                    break;
                }

                case '<': {
                    current_offset = offset;
                    ++current_offset;

                    switch (content[current_offset]) {
                        case 'l': { // <loop
                            ++current_offset;
                            tmp_offset = 0;

                            while ((tmp_offset < 3) &&
                                   (current_offset < end_before) &&
                                   (content[current_offset] ==
                                    "oop"[tmp_offset])) {
                                ++current_offset;
                                ++tmp_offset;
                            }

                            if (tmp_offset == 3) {
                                tag_  = Tag::Loop;
                                found = true;
                            }

                            break;
                        }

                        case 'i': { // <if
                            ++current_offset;

                            if (content[current_offset] == 'f') {
                                ++current_offset;
                                tag_  = Tag::If;
                                found = true;
                            }
                        }
                    }
                }
            }

            if (found) {
                return current_offset;
            }

            ++offset;
        }

        // No match.
        return 0;
    }

    ULong nest(const char *content, ULong offset, ULong end_before,
               ULong max_end_before) override {
        switch (tag_) {
            case Tag::Math:
            case Tag::InLineIf: {
                return SkipInnerKeywords("{var:", 5, "}", 1, content, offset,
                                         end_before, max_end_before);
            }

            case Tag::Loop: {
                return SkipInnerKeywords("<loop", 5, "</loop>", 7, content,
                                         offset, end_before, max_end_before);
            }

            case Tag::If: {
                // <if> is spacial.
                return renderIf(content, offset, end_before, max_end_before);
            }

            default: {
                return 0;
            }
        }
    }

    void addPreviousContent(const char *content, ULong offset) {
        // Noneed to check if the  length is zero; because Stringstream will
        // ignore it If it is.
        ss_->Add(&(content[last_offset_]), (offset - last_offset_));
    }

    void Found(const char *content, ULong offset, ULong end_before,
               ULong start_offset, ULong &current_offset) override {
        switch (tag_) {
            case Tag::Variable: {
                addPreviousContent(content, (start_offset - 5)); // {var:

                renderVariable(
                    &(content[start_offset]),
                    static_cast<UInt>((current_offset - 1) - start_offset));
                break;
            }

            case Tag::Math: {
                addPreviousContent(content, (start_offset - 6)); // {math:

                renderMath(
                    &(content[start_offset]),
                    static_cast<UInt>((current_offset - 1) - start_offset));
                break;
            }

            case Tag::InLineIf: {
                addPreviousContent(content, (start_offset - 3)); // {if

                renderInLineIf(
                    &(content[start_offset]),
                    static_cast<UInt>((current_offset - 1) - start_offset));
                break;
            }

            case Tag::Loop: {
                addPreviousContent(content, (start_offset - 5)); // <loop

                renderLoop(
                    &(content[start_offset]),
                    static_cast<UInt>((current_offset - 7) - start_offset));
                break;
            }

            case Tag::If: {
                // In case if fails because of a wrong value or syntax,
                // this should help skip any nested ifs.
                current_offset = last_offset_;
                return;
            }

            default: {
            }
        }

        last_offset_ = current_offset;

        (void)offset;
        (void)end_before;
    }

    /*
     * Gets anything between "..."
     */
    bool getQuotaed(UInt &offset, UInt &length, const char *content,
                    const UInt end_before) const noexcept {
        offset = static_cast<UInt>(Find("\"", 1, content, offset, end_before));

        if (offset != 0) {
            ULong start_offset = Find("\"", 1, content, offset, end_before);

            if (start_offset != 0) {
                length = static_cast<UInt>((start_offset - 1) - offset);
                return true;
            }
        }

        return false;
    }

    bool getQuotaed(ULong &offset, UInt &length, const char *content,
                    const ULong end_before) const noexcept {
        offset = Find("\"", 1, content, offset, end_before);

        if (offset != 0) {
            ULong start_offset = Find("\"", 1, content, offset, end_before);

            if (start_offset != 0) {
                length = static_cast<UInt>((start_offset - 1) - offset);
                return true;
            }
        }

        return false;
    }

    bool parseNumber(ULong &number, const char *content, const UInt length) {
        if (length > 6) {
            ULong offset = 0;
            offset       = Find("{var:", 5, content, offset, length);

            if (offset != 0) {
                ULong end_offset = Find("}", 1, content, offset, length);

                if (end_offset == 0) {
                    return false;
                }

                const Value_ *value =
                    findValue(&(content[offset]),
                              static_cast<UInt>((end_offset - 1) - offset));

                if (value != nullptr) {
                    double num;

                    if (value->GetNumber(num)) {
                        number = static_cast<ULong>(num);
                        return true;
                    }
                }

                return false;
            }
        }

        return Digit::StringToNumber(number, content, length);
    }

    /*
     * {var:name}, {var:name[id]}
     * {var:id}, {var:id[id2]}, {var:id[id2][id3]}
     * {var:id[name]}, {var:id[id2][name]}, {var:id[id2][name1][id3][name2]}
     */
    void parseVariables(const char *content, UInt length) {
        ULong offset = 0;
        ULong last_offset;

        do {
            last_offset = offset;

            offset = Find("{var:", 5, content, offset, length);

            if (offset == 0) {
                break;
            }

            if (last_offset < offset) {
                // Add any content that comes before any {var:x}
                ss_->Add(&(content[last_offset]),
                         static_cast<ULong>((offset - 5) - last_offset));
            }

            ULong start_offset = offset;
            offset             = Find("}", 1, content, offset, length);

            // if (offset == 0) {
            //     break;
            // }

            renderVariable(&(content[start_offset]),
                           static_cast<UInt>((offset - 1) - start_offset));
        } while (true);

        if (last_offset < length) {
            // Add any content that comes after }
            ss_->Add(&(content[last_offset]),
                     static_cast<UInt>(length - last_offset));
        }
    }

    /*
     * {var:name}, {var:name[name2]}, {var:name[name2][name3][...]}
     * {var:name}, {var:name[id]}, {var:name[id][id2][...]}
     * {var:id}, {var:id[id2]}, {var:id[id2][...]}
     */
    void renderVariable(const char *content, const UInt length) {
        const Value_ *value = findValue(content, length);

        if ((value == nullptr) || !(value->InsertString(*ss_))) {
            ss_->Add((content - 5), (length + 6));
        }
    }

    /*
     * {math:1+5} {math:2^3} {math:8^-2} {math:2*4} {math:2+2*4} {math:2+2*4-1}
     * {math:8/2} {math:8%2}
     * {math: 1 && 1 } {math:0 || 1} {math: (1 && 1) || (3&&3) }
     * {math: 3==3} {math: 3!=4} {math: 3>4}  {math: 3>=4}
     * {math: 2<3} {math: 2 <= 3}
     * {math: {var:n1} * {var:n2}}
     * {math: (5+3*(1+2)/2^2 == 7.25) || (3==((8-2)/2))}
     * {math: 0.2 + 0.3}
     * See ALE::Evaluate
     */
    void renderMath(const char *content, const UInt length) {
        double number;

        if (ALE::Evaluate(number, content, length, this)) {
            *ss_ += Digit::NumberToString(number, 1, 0, 3);
        } else {
            ss_->Add((content - 6), (length + 7));
        }
    }

    /*
     * {if case="3 == 3" true="Yes" false="No"}
     * {if case="{var:var_five} == 5" true="5" false="no"}
     * {if case="{var:var1}" true="{var:var_five} is equal to 5" false="no"}
     * {if case="3 == 3" true="Yes" false="No"}
     */
    void renderInLineIf(const char *content, const UInt length) {
        UInt offset      = 0;
        UInt last_offset = 0;
        UInt len         = 0;

        UInt times = 3;

        bool case_value = false;

        do {
            ++len;
            offset += len; // Move to the next char.

            if (!getQuotaed(offset, len, content, length)) {
                break;
            }

            if (len == 0) {
                // The syntax is wrong. case. true and false are bigger than 4
                return;
            }

            UInt tmp_offset   = offset - 3;
            bool iif_not_done = true;

            do {
                switch (content[tmp_offset]) {
                    case 'a': { // c[a]se
                        iif_not_done = false;
                        double number;

                        if (!(ALE::Evaluate(number, &(content[offset]), len,
                                            this))) {
                            // A messed-up case.
                            return;
                        }

                        case_value = (number > 0.0);
                        break;
                    }

                    case 'u': { // tr[u]e
                        iif_not_done = false;

                        if (case_value) {
                            parseVariables(&(content[offset]), len);
                            return;
                        }

                        break;
                    }

                    case 'l': { // fa[l]se
                        iif_not_done = false;

                        if (!case_value) {
                            parseVariables(&(content[offset]), len);
                            return;
                        }

                        break;
                    }
                }
            } while (iif_not_done && (--tmp_offset > last_offset));

            last_offset = offset;
        } while (--times > 0);
    }

    struct loopItem_ {
        UInt Offset;
        UInt Type;
        UInt SubOffset;
        UInt SubLength;
    };

    /*
     * <loop set="tree_name" key="loop1-key" value="loop1-value">
     *     loop1-key: loop1-value
     * </loop>
     *
     * <loop set="items" value="item">
     *     item[subitem1], item[subitem2], item[subitem3]
     * </loop>
     *
     * <loop set="tree_name" key="loop1-key" value="loop1-value">
     *      loop1-key: loop1-value
     *
     *      <loop set="tree_name2" key="loop2-key" value="loop2-value">
     *          loop2-key: loop2-value
     *      </loop>
     * </loop>
     *
     * <loop key="loop1-key" times="number|var">
     *     loop1-key
     * </loop>
     *
     * <loop key="loop1-key" index="number|var">
     *     loop1-key
     * </loop>
     * */
    void renderLoop(const char *content, UInt length) {
        const Value_ *root_value = root_value_;
        const char *  key_str    = nullptr;
        const char *  value_str  = nullptr;

        UInt key_length   = 0;
        UInt value_length = 0;
        UInt len          = 0;

        UInt offset      = 0;
        UInt last_offset = 0;

        ULong size  = 0;
        ULong index = 0;

        UInt times = 3;

        UInt start_offset = static_cast<UInt>(Find(">", 1, content, 0, length));
        if (start_offset == 0) {
            // The syntax is wrong.
            return;
        }

        length = (length - start_offset);

        do {
            ++len;
            offset += len; // Move to the next char.

            if (!getQuotaed(offset, len, content, start_offset)) {
                break;
            }

            if (len == 0) {
                // The syntax is wrong.
                return;
            }

            UInt tmp_offset = (offset - 3);

            do {
                switch (content[tmp_offset]) {
                    case 't': { // se[t]
                        const Value_ *value =
                            findValue(&(content[offset]), len);

                        if ((value != nullptr) &&
                            (value->IsArray() || value->IsObject())) {
                            root_value  = value;
                            last_offset = tmp_offset; // Break loop.
                            break;
                        }

                        return; // set="..." has invalid value.
                    }

                    case 'y': { // ke[y]
                        key_str     = &(content[offset]);
                        key_length  = static_cast<UInt>(len);
                        last_offset = tmp_offset; // Break loop.
                        break;
                    }

                    case 'u': { // val[u]e
                        value_str    = &(content[offset]);
                        value_length = static_cast<UInt>(len);
                        last_offset  = tmp_offset; // Break loop.
                        break;
                    }

                    case 'm': { // ti[m]es
                        if (!parseNumber(size, &(content[offset]), len)) {
                            return;
                        }

                        last_offset = tmp_offset; // Break loop.
                        break;
                    }

                    case 'x': { // inde[x]
                        if (!parseNumber(index, &(content[offset]), len)) {
                            return;
                        }

                        last_offset = tmp_offset; // Break loop.
                        break;
                    }
                }
            } while (--tmp_offset > last_offset);

            last_offset = offset;
        } while (--times > 0);

        content = &(content[start_offset]);

        const Array<loopItem_> loop_items = loopMatch_(
            content, length, key_str, value_str, key_length, value_length);

        loopReplace_(loop_items, content, length, index, size, root_value,
                     key_length, value_length);
    }

    Array<loopItem_> loopMatch_(const char *content, UInt length,
                                const char *key, const char *value,
                                UInt key_length, UInt value_length) {
        Array<loopItem_> items;
        loopItem_        key_item{0, 0, 0, 0};
        loopItem_        value_item{0, 0, 0, 0};

        UInt value_offset = 0;
        UInt key_offset   = 0;

        UInt sub_offset;
        UInt sub_length;

        do {
            sub_offset = 0;
            sub_length = 0;

            if ((value != nullptr) && (value_item.Type == 0)) {
                value_offset = static_cast<UInt>(Engine::Find(
                    value, value_length, content, value_offset, length));

                if (value_offset == 0) {
                    value = nullptr; // To stop looking for it.
                } else {
                    if (content[value_offset] == '[') {
                        sub_offset = (value_offset + 1);

                        for (UInt i = sub_offset; i < length; i++) {
                            if (content[i] == ']') {
                                sub_length = (i - sub_offset);
                                break;
                            }
                        }
                    }

                    value_item = {(value_offset - value_length), 2, sub_offset,
                                  sub_length}; // value is 2
                }
            }

            if ((key != nullptr) && (key_item.Type == 0)) {
                key_offset = static_cast<UInt>(
                    Engine::Find(key, key_length, content, key_offset, length));

                if (key_offset == 0) {
                    key = nullptr; // To stop looking for it.
                } else {
                    key_item = {(key_offset - key_length), 1, 0,
                                0}; // key is one
                }
            }

            // Sorting by the offset
            if (key_item.Type == 0) {
                if (value_item.Type != 0) {
                    items += value_item;
                    value_item.Type = 0; // Reset
                }
            } else if (value_item.Type == 0) {
                items += key_item;
                key_item.Type = 0; // Reset
            } else {
                // Both are set
                if (value_item.Offset > key_item.Offset) {
                    items += key_item;
                    key_item.Type = 0; // Reset
                } else {
                    items += value_item;
                    value_item.Type = 0; // Reset
                }
            }

        } while ((key != nullptr) || (value != nullptr));

        // items.Compress();

        return items;
    }

    void loopReplace_(const Array<loopItem_> &items, const char *content,
                      ULong length, ULong index, ULong size,
                      const Value_ *root_value, UInt key_length,
                      UInt value_length) const {
        StringStream      ss;
        ULong             last_offset;
        ULong             sub_id;
        const KeyString_ *key;
        const Value_ *    value;
        const bool        is_array = (root_value->IsArray() || (size != 0));

        if (size == 0) {
            size = root_value->Size();
        }

        while (index < size) {
            last_offset           = 0;
            const loopItem_ *item = items.Storage();
            const loopItem_ *end  = (items.Storage() + items.Size());

            while (item < end) {
                if (last_offset < item->Offset) {
                    ss.Add(&(content[last_offset]),
                           (item->Offset - last_offset));
                }

                if (item->Type == 2) {
                    last_offset = (item->Offset + value_length);

                    value = root_value->GetValue(index);

                    if (value != nullptr) {
                        if (item->SubLength != 0) {
                            last_offset += item->SubLength + 2;

                            if (value->IsObject()) {
                                value =
                                    value->GetValue(&(content[item->SubOffset]),
                                                    item->SubLength);
                            } else if ((Digit::StringToNumber(
                                           sub_id, &(content[item->SubOffset]),
                                           item->SubLength))) {
                                value = value->GetValue(sub_id);
                            } else {
                                value = nullptr;
                            }

                            if (value != nullptr) {
                                value->InsertString(ss);
                            }
                        } else {
                            value->InsertString(ss);
                        }
                    }
                } else {
                    last_offset = (item->Offset + key_length);

                    if (!is_array) {
                        key = root_value->GetKey(index);

                        if (key != nullptr) {
                            ss.Add(key->Char(), key->Length());
                        }
                    } else {
                        ss += Digit::NumberToString(index);
                    }
                }

                ++item;
            }

            // Add the remaining string.
            if (last_offset != length) {
                ss.Add(&(content[last_offset]), (length - last_offset));
            }

            ++index;
        }

        Render(*ss_, ss.GetString(), root_value_);
    }

    /*
     * <if case="{case}">...</if>
     * <if case="{case}">...<else />...</if>
     * <if case="{case1}">...<elseif case="{case2}" />...</if>
     * <if case="{case}">...<if case="{case2}" />...</if></if>
     */
    ULong renderIf(const char *content, ULong offset, ULong end_before,
                   const ULong max_end_before) {
        addPreviousContent(content, (offset - 3)); // 3 is the size of <if

        if (!findIfTail(content, offset, end_before, max_end_before)) {
            //  Error: no tail.
            last_offset_ = end_before;
            return 0;
        }

        ULong  case_offset = offset;
        UInt   case_length = 0;
        double result      = 0;

        last_offset_ = end_before;

        if (getQuotaed(case_offset, case_length, content, end_before)) {
            offset =
                Find(">", 1, content, (case_offset + case_length), end_before);
        }

        while (true) {
            if (((case_length != 0) &&
                 !(ALE::Evaluate(result, &(content[case_offset]), case_length,
                                 this))) ||
                (offset == 0)) {
                // Error: <if> syntax's wrong; no case=""
                return 0;
            }

            // Find <else..
            case_offset = findNextElse(content, offset, end_before);

            if ((result > 0) || (case_offset == 0)) {
                break;
            }

            // <else has no "i" after it.
            if (content[case_offset] != 'i') {
                result      = 1; // Mark <else as true.
                case_length = 0;
            } else if (!getQuotaed(case_offset, case_length, content,
                                   end_before)) {
                // Error: <elseif has no case=""
                return 0;
            }

            // Find the end of else/elseif
            offset =
                Find("/>", 2, content, (case_offset + case_length), end_before);
        }

        if (result > 0) {
            if (case_offset != 0) {
                end_before = case_offset;
            }

            Render(*ss_, &(content[offset]), ((end_before - 5) - offset),
                   root_value_);
        }

        return 0;
    }

    static ULong findNextElse(const char *content, ULong offset,
                              ULong end_before) noexcept {
        ULong else_offset = 0;
        ULong next_if;

        do {
            else_offset = Find("<else", 5, content, offset, end_before);

            if (else_offset == 0) {
                // No <else was found.
                return 0;
            }

            next_if = Find("<if", 3, content, offset, end_before);

            if ((next_if == 0) || (else_offset < next_if)) {
                // No nesting <ifs were found, or <else comes before the child
                // <if
                return else_offset;
            }

            offset = Find("</if>", 5, content, next_if, end_before);

            if (else_offset > offset) {
                // <else comes after the child if.
                return else_offset;
            }
        } while (true);

        return else_offset;
    }

    static bool findIfTail(const char *content, ULong offset, ULong &end_before,
                           ULong max_end_before) noexcept {
        do {
            offset = SkipInnerKeywords("<if", 3, "</if>", 5, content, offset,
                                       end_before, max_end_before);

            if (end_before > offset) {
                break;
            }

            end_before = Find("</if>", 5, content, offset, max_end_before);

            if (end_before == 0) {
                // No </if>
                end_before = offset;
                return false;
            }
        } while (true);

        return true;
    }

    void failed() noexcept override {
        not_done_ = true;
    }

    // Key can be: name/id, name/id[name/id], name/id[name/id][sub-name/id],
    // name/id[name/id][sub-name/id][sub-sub-name/id]... "name": a string that's
    // stored in "keys_". "id" is the index that starts with 0:
    // values_[id]
    const Value_ *findValue(const char *key, UInt length) const {
        if ((key != nullptr) && (length != 0)) {
            const Value_ *root_value = root_value_;
            UInt          offset     = 0;
            UInt          next_offset;

            if (key[(length - 1)] == ']') {
                next_offset = 0;
                // Starting with a string followed by [...]
                while ((next_offset < length) && (key[++next_offset] != '[')) {
                }

                --length; // No use for ending bracket.
            } else {
                next_offset = length;
            }

            while (true) {
                Value_ *value = nullptr;

                if (root_value->IsArray()) {
                    ULong value_id;

                    if (!(Digit::StringToNumber(value_id, &(key[offset]),
                                                (next_offset - offset)))) {
                        break;
                    }

                    value = root_value->GetValue(value_id);
                } else {
                    value = root_value->GetValue(&(key[offset]),
                                                 (next_offset - offset));
                }

                if (value == nullptr) {
                    break;
                }

                if (next_offset == length) {
                    return value;
                }

                // if (!(value->IsArray()) && !(value->IsObject())) {
                //     break;
                // }

                root_value = value;

                // Next part: starts at next_offset
                offset = next_offset;
                while ((offset < length) && (key[offset] != '[')) {
                    ++offset;
                }

                // Ends at offset + 1
                next_offset = ++offset; // ++ to the next char after [
                while ((next_offset < length) && (key[++next_offset] != ']')) {
                }
            }
        }

        return nullptr;
    }

    bool ALESetNumber(double &number, const char *content,
                      UInt length) const final {
        if (length > 4) {
            const Value_ *value =
                findValue(&(content[5]), (length - 6)); // {var:x}

            if (value != nullptr) {
                if (value->IsNumber()) {
                    number = value->GetNumber();
                    return true;
                }

                if (value->IsString()) {
                    return ALE::Evaluate(number, value->Char(),
                                         static_cast<UInt>(value->Length()),
                                         this);
                }

                return value->GetNumber(number);
            }
        }

        return false;
    }

    bool ALEIsEqual(bool &result, const char *left, UInt left_length,
                    const char *right, UInt right_length) const final {
        const Value_ *value_left{nullptr};
        const Value_ *value_right{nullptr};

        bool is_number{false};

        // If the right side is a variable
        if ((left[0] == '{') && (left_length > 6)) {
            value_left = findValue(&(left[5]), (left_length - 6)); // {var:x}

            if (value_left == nullptr) {
                return false;
            }

            is_number = value_left->IsNumber();
        }

        // If the left side is a variable
        if ((right[0] == '{') && (right_length > 6)) {
            value_right = findValue(&(right[5]), (right_length - 6)); // {var:x}

            if (value_right == nullptr) {
                return false;
            }

            if (!is_number) {
                is_number = value_right->IsNumber();
            }
        }

        if (is_number) {
            double num_left;

            if (value_left != nullptr) {
                if (!(value_left->GetNumber(num_left))) {
                    return false;
                }
            } else if (!(Digit::StringToNumber(num_left, left, left_length))) {
                return false;
            }

            double num_right;

            if (value_right != nullptr) {
                if (!(value_right->GetNumber(num_right))) {
                    return false;
                }
            } else if (!(Digit::StringToNumber(num_right, right,
                                               right_length))) {
                return false;
            }

            result = (num_left == num_right);
            return true;
        }

        String string_left;

        const char *str_left;
        ULong       str_length_left;

        if (value_left != nullptr) {
            if (value_left->IsString()) {
                str_left        = value_left->Char();
                str_length_left = value_left->Length();
            } else if (value_left->SetString(string_left)) {
                str_left        = string_left.Char();
                str_length_left = string_left.Length();
            } else {
                return false;
            }
        } else {
            str_left        = left;
            str_length_left = left_length;
        }

        String string_right;

        const char *str_right;
        ULong       str_length_right;

        if (value_right != nullptr) {
            if (value_right->IsString()) {
                str_right        = value_right->Char();
                str_length_right = value_right->Length();
            } else if (value_right->SetString(string_right)) {
                str_right        = string_right.Char();
                str_length_right = string_right.Length();
            } else {
                return false;
            }
        } else {
            str_right        = right;
            str_length_right = right_length;
        }

        result = ((str_length_left == str_length_right) &&
                  Memory::Compare(str_left, str_right, str_length_right));
        return true;
    }

    StringStream *ss_;
    const Value_ *root_value_;

    ULong last_offset_{0};
    Tag   tag_{Tag::Variable};
    bool  not_done_{true};
};

} // namespace Qentem

#endif
