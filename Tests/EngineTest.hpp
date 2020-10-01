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
#include "StringStream.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_ENGINE_TESTS_H_
#define QENTEM_ENGINE_TESTS_H_

namespace Qentem {
namespace Test {

struct Engine1 {
    static constexpr bool HasTail() noexcept {
        return true;
    }

    virtual SizeT FindH(const char *content, SizeT offset,
                        SizeT end_before) noexcept {
        while (offset < end_before) {
            if (content[offset] == '(') {
                return ++offset;
            }

            ++offset;
        }

        return 0U; // No match.
    }

    virtual SizeT FindT(const char *content, SizeT offset,
                        SizeT end_before) const noexcept {
        return Engine::Find(")", 1U, content, offset, end_before);
    }

    virtual SizeT Nest(const char *content, SizeT offset, SizeT end_before,
                       SizeT max_end_before) {
        (void)content;
        (void)offset;
        (void)end_before;
        (void)max_end_before;

        return 0U;
    }

    virtual void Found(const char *content, SizeT offset, SizeT end_before,
                       SizeT start_offset, SizeT &current_offset) {
        (void)content;
        (void)offset;
        (void)end_before;
        (void)start_offset;
        (void)current_offset;
    }

    virtual void Failed() {
    }
};

static int TestEngine1() {
    SizeT       ret;
    SizeT       content_len = 0U;
    SizeT       find_len    = 3U;
    const char *content     = "";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, 0U, 1U);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "A";
    content_len = 1U;
    find_       = "A";
    find_len    = 1U;

    ret = Engine::FindOne(*find_, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 1U, "return");

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 1U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "AAAAA";
    content_len = 5U;

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 2U, "return");

    ret = Engine::Find(find_, find_len, content, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::Find(find_, find_len, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 3U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    ret = Engine::Find(find_, find_len, content, 4U, content_len);
    SHOULD_EQUAL_VALUE(ret, 5U, "return");

    ret = Engine::Find(find_, find_len, content, 5U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "  A A        A";
    content_len = 14U;

    ret = Engine::FindOne(*find_, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::FindOne(*find_, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::FindOne(*find_, content, 3U, content_len);
    SHOULD_EQUAL_VALUE(ret, 5U, "return");

    ret = Engine::FindOne(*find_, content, 7U, content_len);
    SHOULD_EQUAL_VALUE(ret, 14U, "return");

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 3U, content_len);
    SHOULD_EQUAL_VALUE(ret, 5U, "return");

    ret = Engine::Find(find_, find_len, content, 4U, content_len);
    SHOULD_EQUAL_VALUE(ret, 5U, "return");

    ret = Engine::Find(find_, find_len, content, 5U, content_len);
    SHOULD_EQUAL_VALUE(ret, 14U, "return");

    content     = "A";
    content_len = 1U;
    find_       = "AB";
    find_len    = 2U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content = "AA";
    ret     = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content = "A   A   A";
    ret     = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content = "  A   A   A  ";
    ret     = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "AB";
    content_len = 2U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 2U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::Find(find_, find_len, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = " AB";
    content_len = 3U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    content     = "AAB";
    content_len = 3U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    content     = "AAAB";
    content_len = 4U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    ret = Engine::Find(find_, find_len, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    content     = "AAAAAAAAAB";
    content_len = 10U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    ret = Engine::Find(find_, find_len, content, 5U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    ret = Engine::Find(find_, find_len, content, 8U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    content     = "  AB";
    content_len = 4U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    ret = Engine::Find(find_, find_len, content, 1U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    ret = Engine::Find(find_, find_len, content, 2U, content_len);
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    content     = "        AB";
    content_len = 10U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    ret = Engine::Find(find_, find_len, content, 5U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    ret = Engine::Find(find_, find_len, content, 8U, content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return");

    content     = "AAAAAAAAA";
    content_len = 9U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::Find(find_, find_len, content, 5U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::Find(find_, find_len, content, 8U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret =
        Engine::Find(find_, find_len, content, (content_len - 1), content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    find_       = "ABC";
    find_len    = 3U;
    content     = "CBA";
    content_len = 3U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "BCBA";
    content_len = 4U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "123456789123456CBA";
    content_len = 18U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "ADC";
    content_len = 3U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "123456789123456ADC";
    content_len = 18U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    END_SUB_TEST;
}

static int TestEngine2() {
    SizeT       ret;
    SizeT       content_len = 16U;
    SizeT       find_len    = 3U;
    const char *content     = "ABABABABABAAABAB";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "ABABABABABAAABABC";
    content_len = 17U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 17U, "return");

    ret = Engine::Find(find_, find_len, content, 15U, content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::Find(find_, find_len, content, 14U, content_len);
    SHOULD_EQUAL_VALUE(ret, 17U, "return");

    content     = "ABABABABABCABAAAB";
    content_len = 17U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 11U, "return");

    content     = "   ABC   ";
    content_len = 9U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 6U, "return");

    content     = "ABC   ";
    content_len = 6U;

    ret = Engine::Find(find_, find_len, content, 0U, content_len);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    END_SUB_TEST;
}

static int TestEngine3() {
    SizeT       ret;
    SizeT       content_len = 0U;
    const char *content     = "";

    Engine1 eng1;

    ret = Engine::FindNest("", 0U, 0U, 0U, &eng1);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    ret = Engine::FindNest("(", 0U, 0U, 1U, &eng1);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "   (  ";
    content_len = 6U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng1);
    /*
     * When the engine fails to find a complete match, it returns the last
     * offset that stops at: to prevent Infinite loops from according.
     */
    SHOULD_EQUAL_VALUE(ret, 4U, "return");

    content     = "((((((((";
    content_len = 8U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 1U, "return");

    content     = "()";
    content_len = 2U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 2U, "return");

    ret = Engine::FindNest(content, 1U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = " ()";
    content_len = 3U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::FindNest(content, 1U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    ret = Engine::FindNest(content, 2U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = " ()             ";
    content_len = 16U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng1);
    SHOULD_EQUAL_VALUE(ret, 0U, "return"); // It called Found() already.

    ret = Engine::FindNest(content, 0U, 3U, 3U, &eng1);
    SHOULD_EQUAL_VALUE(ret, 3U, "return");

    END_SUB_TEST;
}

static int TestEngine4() {
    SizeT       ret;
    SizeT       content_len = 0U;
    const char *content     = "";

    struct Engine2 : Engine1 {
        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::Find("))", 2U, content, offset, end_before);
        }
    };

    content     = "       )          ";
    content_len = 18U;

    Engine2 eng2;

    // Checking for weirdness.
    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng2);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = " ()             ";
    content_len = 16U;

    // Checking for weirdness.
    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng2);
    SHOULD_EQUAL_VALUE(ret, 2U, "return");

    struct Engine3 : Engine2 {
        SizeT FindH(const char *content, SizeT offset,
                    SizeT end_before) noexcept override {
            while (offset < end_before) {

                if ((content[offset] == '(') &&
                    (content[(offset + 1)] == '(')) {
                    return (offset + 2);
                }

                ++offset;
            }

            return 0U; // No match
        }
    };

    content     = "(())";
    content_len = 4U;

    Engine3 eng3;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng3);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "      (())";
    content_len = 10U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng3);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "      (()) ";
    content_len = 11U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng3);
    SHOULD_EQUAL_VALUE(ret, 0U, "return");

    content     = "( (())";
    content_len = 6U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng3);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "( ( ( ( ( ( (())";
    content_len = 16U;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng3);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    struct Engine4 : Engine3 {
        void Failed() noexcept override {
            has_error = true;
        }

        bool getError() const {
            return has_error;
        }

      private:
        bool has_error = false;
    };

    content     = "(()";
    content_len = 3U;
    Engine4 eng4;

    ret = Engine::FindNest(content, 0U, content_len, content_len, &eng4);
    SHOULD_EQUAL_VALUE(ret, 2U, "return");
    SHOULD_EQUAL_TRUE(eng4.getError(), "has_error");

    struct Item__ {
        SizeT Offset;
        SizeT Length;
    };

    struct VARS__ {
        const char *Content{nullptr};
        SizeT       Offset{0};
        SizeT       EndBefore{0};
    };

    struct Engine5 : Engine1 {
        void Found(const char *content, SizeT offset, SizeT end_before,
                   SizeT start_offset, SizeT &current_offset) override {
            --start_offset;
            items += {start_offset, (current_offset - start_offset)};

            vars.Content   = content;
            vars.Offset    = offset;
            vars.EndBefore = end_before;
        }

        Array<Item__> &getItems() {
            return items;
        }

        VARS__ &getVARS() {
            return vars;
        }

      private:
        Array<Item__> items;
        VARS__        vars{};
    };

    Engine5 eng5;
    content     = "()";
    content_len = 2U;

    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 2U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng5.getVARS().Content, content, "Content");
    SHOULD_EQUAL_VALUE(eng5.getVARS().Offset, 0U, "Offset");
    SHOULD_EQUAL_VALUE(eng5.getVARS().EndBefore, content_len, "EndBefore");

    content     = "()(";
    content_len = 3U;

    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 2U, "items[0].Length");

    content     = "()((((((";
    content_len = 8U;

    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 2U, "items[0].Length");

    content     = "()((((((()";
    content_len = 10U;
    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 2U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 2U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Offset, 2U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Length, 8U, "items[1].Length");

    content     = "()(((()((()()(()()";
    content_len = 18U;
    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 6U, "items.Size()");

    content     = "  ()  ()  ()( )  (  )";
    content_len = 21U;
    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 5U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 2U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 2U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Offset, 6U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Length, 2U, "items[1].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[2].Offset, 10U, "items[2].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[2].Length, 2U, "items[2].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[3].Offset, 12U, "items[3].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[3].Length, 3U, "items[3].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[4].Offset, 17U, "items[4].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[4].Length, 4U, "items[4].Length");
    SHOULD_EQUAL_VALUE(eng5.getVARS().Content, content, "Content");
    SHOULD_EQUAL_VALUE(eng5.getVARS().Offset, 0U, "Offset");
    SHOULD_EQUAL_VALUE(eng5.getVARS().EndBefore, content_len, "EndBefore");

    content     = "( ) (     ) (  )";
    content_len = 16U;
    eng5.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng5);
    SHOULD_EQUAL_VALUE(eng5.getItems().Size(), 3U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[0].Length, 3U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Offset, 4U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[1].Length, 7U, "items[1].Length");
    SHOULD_EQUAL_VALUE(eng5.getItems()[2].Offset, 12U, "items[2].Offset");
    SHOULD_EQUAL_VALUE(eng5.getItems()[2].Length, 4U, "items[2].Length");

    struct Engine6 : Engine4 {
        void Found(const char *content, SizeT offset, SizeT end_before,
                   SizeT start_offset, SizeT &current_offset) override {
            start_offset -= 2U;
            items += {start_offset, (current_offset - start_offset)};

            vars.Content   = content;
            vars.Offset    = offset;
            vars.EndBefore = end_before;
        }

        Array<Item__> &getItems() {
            return items;
        }

        VARS__ &getVARS() {
            return vars;
        }

      private:
        Array<Item__> items;
        VARS__        vars{};
    };

    Engine6 eng6;
    content     = "(())";
    content_len = 4U;

    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 4U, "items[0].Length");

    content     = "(())(";
    content_len = 5U;

    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 4U, "items[0].Length");

    content     = "(())((";
    content_len = 6U;

    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 0U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 4U, "items[0].Length");

    content     = "(())(()";
    content_len = 7U;

    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");

    content     = "(())(((()(()(()()";
    content_len = 17U;

    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");

    content     = "(())(((())";
    content_len = 10U;
    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 2U, "items.Size()");

    content     = "  (())  (())  (())(())  (())";
    content_len = 28U;
    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 5U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 2U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 4U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Offset, 8U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Length, 4U, "items[1].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[2].Offset, 14U, "items[2].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[2].Length, 4U, "items[2].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[3].Offset, 18U, "items[3].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[3].Length, 4U, "items[3].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[4].Offset, 24U, "items[4].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[4].Length, 4U, "items[4].Length");

    content     = "  ((   )) (( )) ((               ))";
    content_len = 35U;

    eng6.getItems().Clear();
    Engine::FindNest(content, 0U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 3U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 2U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 7U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Offset, 10U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Length, 5U, "items[1].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[2].Offset, 16U, "items[2].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[2].Length, 19U, "items[2].Length");

    eng6.getItems().Clear();
    Engine::FindNest(content, 10U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 2U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 10U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 5U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Offset, 16U, "items[1].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[1].Length, 19U, "items[1].Length");
    SHOULD_EQUAL_VALUE(eng6.getVARS().Offset, 10U, "Offset");
    SHOULD_EQUAL_VALUE(eng6.getVARS().EndBefore, content_len, "EndBefore");

    eng6.getItems().Clear();
    Engine::FindNest(content, 15U, content_len, content_len, &eng6);
    SHOULD_EQUAL_VALUE(eng6.getItems().Size(), 1U, "items.Size()");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Offset, 16U, "items[0].Offset");
    SHOULD_EQUAL_VALUE(eng6.getItems()[0].Length, 19U, "items[0].Length");
    SHOULD_EQUAL_VALUE(eng6.getVARS().Offset, 15U, "Offset");
    SHOULD_EQUAL_VALUE(eng6.getVARS().EndBefore, content_len, "EndBefore");

    END_SUB_TEST;
}

static int TestEngine5() {
    SizeT       ret;
    SizeT       content_len = 12U;
    const char *content     = "{{{{{{}}}}}}";

    SHOULD_EQUAL_VALUE(Engine::Find("}", 1U, content, 0U, content_len), 7U,
                       "return1");

    /*
     * The offset has to start after the head (one), because zero is the match
     * that is being searched for its tail.
     */
    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 7U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 12U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 0U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 0U, "return2");

    ret = Engine::SkipInnerPatterns("x", 1U, "y", 1U, content, 1U, 7U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 7U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 5U, 6U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 7U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 4U, 6U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 8U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 3U, 6U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 9U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 3U, 7U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 10U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 7U, 7U);
    SHOULD_EQUAL_VALUE(ret, 7U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 7U, 8U);
    SHOULD_EQUAL_VALUE(ret, 8U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 7U, 9U);
    SHOULD_EQUAL_VALUE(ret, 9U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 7U, 10U);
    SHOULD_EQUAL_VALUE(ret, 10U, "return2");

    ret = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 12U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 12U, "return2");

    content     = "{}";
    content_len = 2U;
    ret         = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 0U, 2U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 2U, "return2");

    content     = "{   }";
    content_len = 5U;
    ret         = Engine::SkipInnerPatterns("{", 1U, "}", 1U, content, 1U, 5U,
                                    content_len);
    SHOULD_EQUAL_VALUE(ret, 5U, "return2");

    END_SUB_TEST;
}

struct Item2__ {
    SizeT          Offset{0};
    SizeT          Length{0};
    Array<Item2__> SubItems;
};

static void toJSON(StringStream<char> &ss, const Array<Item2__> &items,
                   const char *content) {
    if (items.IsEmpty()) {
        return;
    }

    ss += '{';

    for (SizeT i = 0U; i < items.Size(); i++) {
        if (i != 0) {
            ss += ',';
        }

        ss += '"';
        ss.Insert((content + items[i].Offset), items[i].Length);
        ss += "\":{";

        ss += "\"O\":";
        ss += Digit<char>::NumberToString(items[i].Offset);
        ss += ",\"L\":";
        ss += Digit<char>::NumberToString(items[i].Length);

        if (items[i].SubItems.IsNotEmpty()) {
            ss += ",\"S\":";
            toJSON(ss, items[i].SubItems, content);
        }

        ss += '}';
    }

    ss += '}';
}

static int TestEngine6() {
    Array<Item2__>     items;
    const char *       content;
    SizeT              content_len;
    const char *       result;
    StringStream<char> toJSON_ss;

    struct Engine7 : Engine1 {
        explicit Engine7(Array<Item2__> *items) noexcept : items_(items) {
        }

        SizeT FindH(const char *content, SizeT offset,
                    SizeT end_before) noexcept override {
            while (offset < end_before) {

                if ((content[offset] == '[') &&
                    (content[(offset + 1)] == '[') &&
                    (content[(offset + 2)] == '[')) {
                    return (offset + 3);
                }

                ++offset;
            }

            // No match.
            return 0U;
        }

        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::Find("]]]", 3U, content, offset, end_before);
        }

        SizeT Nest(const char *content, SizeT offset, SizeT end_before,
                   SizeT max_end_before) override {
            Engine7 eng7{&sub_items};
            return Engine::FindNest(content, offset, end_before, max_end_before,
                                    &eng7);
        }

        virtual SizeT length() noexcept {
            return 3U;
        }

        void Found(const char *content, SizeT offset, SizeT end_before,
                   SizeT start_offset, SizeT &current_offset) override {

            Item2__ item;

            item.Offset   = (start_offset - length());
            item.Length   = ((current_offset + length()) - start_offset);
            item.SubItems = static_cast<Array<Item2__> &&>(sub_items);

            items_->Insert(static_cast<Item2__ &&>(item));

            (void)content;
            (void)offset;
            (void)end_before;
        }

        Array<Item2__> *getsubItems() {
            return &sub_items;
        }

      private:
        Array<Item2__> *items_;
        Array<Item2__>  sub_items;
    };

    Engine7 eng7{&items};

    content     = "[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]] [[[]]] [[[[[[]]]]]]";
    content_len = 56U;
    Engine::FindNest(content, 0U, content_len, content_len, &eng7);
    toJSON(toJSON_ss, items, content);

    result =
        R"({"[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]]":{"O":0,"L":36,"S":{"[[[  [[[  [[[  ]]]  ]]]  ]]]":{"O":4,"L":28,"S":{"[[[  [[[  ]]]  ]]]":{"O":9,"L":18,"S":{"[[[  ]]]":{"O":14,"L":8}}}}}}},"[[[]]]":{"O":37,"L":6},"[[[[[[]]]]]]":{"O":44,"L":12,"S":{"[[[]]]":{"O":47,"L":6}}}})";
    SHOULD_EQUAL_VALUE(toJSON_ss, result, "toJSON_ss");

    struct Engine8 : Engine7 {
        explicit Engine8(Array<Item2__> *items) noexcept : Engine7(items) {
        }

        SizeT FindH(const char *content, SizeT offset,
                    SizeT end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '{') {
                    return ++offset;
                }

                ++offset;
            }

            return 0U; // No match
        }

        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::FindOne('}', content, offset, end_before);
        }

        SizeT length() noexcept override {
            return 1U;
        }

        SizeT Nest(const char *content, SizeT offset, SizeT end_before,
                   SizeT max_end_before) override {
            Engine8 eng8{getsubItems()};
            return Engine::FindNest(content, offset, end_before, max_end_before,
                                    &eng8);
        }
    };

    content =
        R"({....} {} {{ }{  }{   }{    }} {{{{{{{}}}}}}}
        {A{B}C{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}U{V}W}
        {1{2{3{4{5{6{7}8}9}10}10}12}13})";

    // content = R"({....})";
    items.Clear();
    Engine8 eng8(&items);
    content_len = StringUtils::Count<char, SizeT>(content);
    Engine::FindNest(content, 0U, content_len, content_len, &eng8);
    toJSON_ss.Reset();
    toJSON(toJSON_ss, items, content);

    result =
        R"({"{....}":{"O":0,"L":6},"{}":{"O":7,"L":2},"{{ }{  }{   }{    }}":{"O":10,"L":20,"S":{"{ }":{"O":11,"L":3},"{  }":{"O":14,"L":4},"{   }":{"O":18,"L":5},"{    }":{"O":23,"L":6}}},"{{{{{{{}}}}}}}":{"O":31,"L":14,"S":{"{{{{{{}}}}}}":{"O":32,"L":12,"S":{"{{{{{}}}}}":{"O":33,"L":10,"S":{"{{{{}}}}":{"O":34,"L":8,"S":{"{{{}}}":{"O":35,"L":6,"S":{"{{}}":{"O":36,"L":4,"S":{"{}":{"O":37,"L":2}}}}}}}}}}}}},"{A{B}C{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}U{V}W}":{"O":54,"L":47,"S":{"{B}":{"O":56,"L":3},"{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}":{"O":60,"L":35,"S":{"{E}":{"O":62,"L":3},"{G{H}I{G{K{L}M}N}O{P}Q}":{"O":66,"L":23,"S":{"{H}":{"O":68,"L":3},"{G{K{L}M}N}":{"O":72,"L":11,"S":{"{K{L}M}":{"O":74,"L":7,"S":{"{L}":{"O":76,"L":3}}}}},"{P}":{"O":84,"L":3}}},"{S}":{"O":90,"L":3}}},"{V}":{"O":96,"L":3}}},"{1{2{3{4{5{6{7}8}9}10}10}12}13}":{"O":110,"L":31,"S":{"{2{3{4{5{6{7}8}9}10}10}12}":{"O":112,"L":26,"S":{"{3{4{5{6{7}8}9}10}10}":{"O":114,"L":21,"S":{"{4{5{6{7}8}9}10}":{"O":116,"L":16,"S":{"{5{6{7}8}9}":{"O":118,"L":11,"S":{"{6{7}8}":{"O":120,"L":7,"S":{"{7}":{"O":122,"L":3}}}}}}}}}}}}}})";
    SHOULD_EQUAL_VALUE(toJSON_ss, result, "toJSON_ss");

    content = "";
    items.Clear();
    content_len = 0U;
    Engine::FindNest(content, 0U, content_len, content_len, &eng8);
    toJSON_ss.Reset();
    toJSON(toJSON_ss, items, content);

    SHOULD_EQUAL_VALUE(toJSON_ss.Length(), 0U, "toJSON_ss");

    content = "{ { }";
    items.Clear();
    content_len = 5U;
    Engine::FindNest(content, 0U, content_len, content_len, &eng8);
    SHOULD_EQUAL_VALUE(items.Size(), 1U, "items.Size()");

    content = "{ {{ }}";
    items.Clear();
    content_len = 5U;
    Engine::FindNest(content, 0U, content_len, content_len, &eng8);
    SHOULD_EQUAL_VALUE(items.Size(), 0U, "items.Size()");

    END_SUB_TEST;
}

static int RunEngineTests() {
    STARTING_TEST("Engine.hpp");

    START_TEST("Engine Test 1", TestEngine1);
    START_TEST("Engine Test 2", TestEngine2);
    START_TEST("Engine Test 3", TestEngine3);
    START_TEST("Engine Test 4", TestEngine4);
    START_TEST("Engine Test 5", TestEngine5);
    START_TEST("Engine Test 6", TestEngine6);

    END_TEST("Engine.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
