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
    static constexpr bool HasTail() noexcept { return true; }

    virtual SizeT FindH(const char *content, SizeT offset,
                        SizeT end_before) noexcept {
        while (offset < end_before) {
            if (content[offset] == '(') {
                return ++offset;
            }

            ++offset;
        }

        return 0; // No match.
    }

    virtual SizeT FindT(const char *content, SizeT offset,
                        SizeT end_before) const noexcept {
        return Engine::Find(")", SizeT{1}, content, offset, end_before);
    }

    virtual SizeT Nest(const char *content, SizeT offset, SizeT end_before,
                       SizeT max_end_before) {
        (void)content;
        (void)offset;
        (void)end_before;
        (void)max_end_before;

        return 0;
    }

    virtual void Found(const char *content, SizeT offset, SizeT end_before,
                       SizeT start_offset, SizeT &current_offset) {
        (void)content;
        (void)offset;
        (void)end_before;
        (void)start_offset;
        (void)current_offset;
    }

    virtual void Failed() {}
};

static int TestEngine1() {
    SizeT       ret;
    SizeT       content_len = 0;
    SizeT       find_len    = 3;
    const char *content     = "";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, SizeT{0}, SizeT{1});
    EQ_VALUE(ret, 0, "return");

    content     = "A";
    content_len = 1;
    find_       = "A";
    find_len    = 1;

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 1, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 1, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "AAAAA";
    content_len = 5;

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 2, "return");

    ret = Engine::Find(find_, find_len, content, content_len, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "  A A        A";
    content_len = 14;

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::FindOne(*find_, content, SizeT{7}, content_len);
    EQ_VALUE(ret, 14, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 14, "return");

    content     = "A";
    content_len = 1;
    find_       = "AB";
    find_len    = 2;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content = "AA";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content = "A   A   A";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content = "  A   A   A  ";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "AB";
    content_len = 2;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 2, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = " AB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 3, "return");

    content     = "AAB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 3, "return");

    content     = "AAAB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 4, "return");

    content     = "AAAAAAAAAB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 10, "return");

    content     = "  AB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 4, "return");

    content     = "        AB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 10, "return");

    content     = "AAAAAAAAA";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret =
        Engine::Find(find_, find_len, content, (content_len - 1), content_len);
    EQ_VALUE(ret, 0, "return");

    find_       = "ABC";
    find_len    = 3;
    content     = "CBA";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "BCBA";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "123456789123456CBA";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "ADC";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "123456789123456ADC";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    END_SUB_TEST;
}

static int TestEngine2() {
    SizeT       ret;
    SizeT       content_len = 16;
    SizeT       find_len    = 3;
    const char *content     = "ABABABABABAAABAB";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "ABABABABABAAABABC";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 17, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{15}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, SizeT{14}, content_len);
    EQ_VALUE(ret, 17, "return");

    content     = "ABABABABABCABAAAB";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, SizeT{11}, "return");

    content     = "   ABC   ";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 6, "return");

    content     = "ABC   ";
    content_len = 6;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    END_SUB_TEST;
}

static int TestEngine3() {
    SizeT       ret;
    SizeT       content_len = 0;
    const char *content     = "";

    Engine1 eng1;

    ret = Engine::FindNest("", SizeT{0}, SizeT{0}, SizeT{0}, &eng1);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::FindNest("(", SizeT{0}, SizeT{0}, SizeT{1}, &eng1);
    EQ_VALUE(ret, 0, "return");

    content     = "   (  ";
    content_len = 6;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng1);
    /*
     * When the engine fails to find a complete match, it returns the last
     * offset that stops at: to prevent Infinite loops from according.
     */
    EQ_VALUE(ret, 4, "return");

    content     = "((((((((";
    content_len = 8;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng1);
    EQ_VALUE(ret, SizeT{1}, "return");

    content     = "()";
    content_len = 2;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 2, "return");

    ret = Engine::FindNest(content, SizeT{1}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 0, "return");

    content     = " ()";
    content_len = 3;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindNest(content, SizeT{1}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindNest(content, SizeT{2}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 0, "return");

    content     = " ()             ";
    content_len = 16;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng1);
    EQ_VALUE(ret, 0, "return"); // It called Found() already.

    ret = Engine::FindNest(content, SizeT{0}, SizeT{3}, SizeT{3}, &eng1);
    EQ_VALUE(ret, 3, "return");

    END_SUB_TEST;
}

static int TestEngine4() {
    SizeT       ret;
    SizeT       content_len = 0;
    const char *content     = "";

    struct Engine2 : Engine1 {
        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::Find("))", SizeT{2}, content, offset, end_before);
        }
    };

    content     = "       )          ";
    content_len = 18;

    Engine2 eng2;

    // Checking for weirdness.
    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng2);
    EQ_VALUE(ret, 0, "return");

    content     = " ()             ";
    content_len = 16;

    // Checking for weirdness.
    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng2);
    EQ_VALUE(ret, 2, "return");

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

            return 0; // No match
        }
    };

    content     = "(())";
    content_len = 4;

    Engine3 eng3;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng3);
    EQ_VALUE(ret, content_len, "return");

    content     = "      (())";
    content_len = 10;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng3);
    EQ_VALUE(ret, content_len, "return");

    content     = "      (()) ";
    content_len = 11;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng3);
    EQ_VALUE(ret, 0, "return");

    content     = "( (())";
    content_len = 6;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng3);
    EQ_VALUE(ret, content_len, "return");

    content     = "( ( ( ( ( ( (())";
    content_len = 16;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng3);
    EQ_VALUE(ret, content_len, "return");

    struct Engine4 : Engine3 {
        void Failed() noexcept override { has_error = true; }

        bool getError() const { return has_error; }

      private:
        bool has_error = false;
    };

    content     = "(()";
    content_len = 3;
    Engine4 eng4;

    ret = Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng4);
    EQ_VALUE(ret, 2, "return");
    EQ_TRUE(eng4.getError(), "has_error");

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

        Array<Item__> &getItems() { return items; }

        VARS__ &getVARS() { return vars; }

      private:
        Array<Item__> items;
        VARS__        vars{};
    };

    Engine5 eng5;
    content     = "()";
    content_len = 2;

    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 2, "items[0].Length");
    EQ_VALUE(eng5.getVARS().Content, content, "Content");
    EQ_VALUE(eng5.getVARS().Offset, 0, "Offset");
    EQ_VALUE(eng5.getVARS().EndBefore, content_len, "EndBefore");

    content     = "()(";
    content_len = 3;

    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 2, "items[0].Length");

    content     = "()((((((";
    content_len = 8;

    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 2, "items[0].Length");

    content     = "()((((((()";
    content_len = 10;
    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), 2, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 2, "items[0].Length");
    EQ_VALUE(eng5.getItems()[1].Offset, 2, "items[1].Offset");
    EQ_VALUE(eng5.getItems()[1].Length, 8, "items[1].Length");

    content     = "()(((()((()()(()()";
    content_len = 18;
    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), 6, "items.Size()");

    content     = "  ()  ()  ()( )  (  )";
    content_len = 21;
    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), 5, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 2, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 2, "items[0].Length");
    EQ_VALUE(eng5.getItems()[1].Offset, 6, "items[1].Offset");
    EQ_VALUE(eng5.getItems()[1].Length, 2, "items[1].Length");
    EQ_VALUE(eng5.getItems()[2].Offset, 10, "items[2].Offset");
    EQ_VALUE(eng5.getItems()[2].Length, 2, "items[2].Length");
    EQ_VALUE(eng5.getItems()[3].Offset, 12, "items[3].Offset");
    EQ_VALUE(eng5.getItems()[3].Length, 3, "items[3].Length");
    EQ_VALUE(eng5.getItems()[4].Offset, 17, "items[4].Offset");
    EQ_VALUE(eng5.getItems()[4].Length, 4, "items[4].Length");
    EQ_VALUE(eng5.getVARS().Content, content, "Content");
    EQ_VALUE(eng5.getVARS().Offset, 0, "Offset");
    EQ_VALUE(eng5.getVARS().EndBefore, content_len, "EndBefore");

    content     = "( ) (     ) (  )";
    content_len = 16;
    eng5.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng5);
    EQ_VALUE(eng5.getItems().Size(), 3, "items.Size()");
    EQ_VALUE(eng5.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng5.getItems()[0].Length, 3, "items[0].Length");
    EQ_VALUE(eng5.getItems()[1].Offset, 4, "items[1].Offset");
    EQ_VALUE(eng5.getItems()[1].Length, 7, "items[1].Length");
    EQ_VALUE(eng5.getItems()[2].Offset, 12, "items[2].Offset");
    EQ_VALUE(eng5.getItems()[2].Length, 4, "items[2].Length");

    struct Engine6 : Engine4 {
        void Found(const char *content, SizeT offset, SizeT end_before,
                   SizeT start_offset, SizeT &current_offset) override {
            start_offset -= 2;
            items += {start_offset, (current_offset - start_offset)};

            vars.Content   = content;
            vars.Offset    = offset;
            vars.EndBefore = end_before;
        }

        Array<Item__> &getItems() { return items; }

        VARS__ &getVARS() { return vars; }

      private:
        Array<Item__> items;
        VARS__        vars{};
    };

    Engine6 eng6;
    content     = "(())";
    content_len = 4;

    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())(";
    content_len = 5;

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())((";
    content_len = 6;

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 0, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())(()";
    content_len = 7;

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");

    content     = "(())(((()(()(()()";
    content_len = 17;

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");

    content     = "(())(((())";
    content_len = 10;
    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), 2, "items.Size()");

    content     = "  (())  (())  (())(())  (())";
    content_len = 28;
    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), 5, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 2, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 4, "items[0].Length");
    EQ_VALUE(eng6.getItems()[1].Offset, 8, "items[1].Offset");
    EQ_VALUE(eng6.getItems()[1].Length, 4, "items[1].Length");
    EQ_VALUE(eng6.getItems()[2].Offset, 14, "items[2].Offset");
    EQ_VALUE(eng6.getItems()[2].Length, 4, "items[2].Length");
    EQ_VALUE(eng6.getItems()[3].Offset, 18, "items[3].Offset");
    EQ_VALUE(eng6.getItems()[3].Length, 4, "items[3].Length");
    EQ_VALUE(eng6.getItems()[4].Offset, 24, "items[4].Offset");
    EQ_VALUE(eng6.getItems()[4].Length, 4, "items[4].Length");

    content     = "  ((   )) (( )) ((               ))";
    content_len = 35;

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), 3, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 2, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 7, "items[0].Length");
    EQ_VALUE(eng6.getItems()[1].Offset, 10, "items[1].Offset");
    EQ_VALUE(eng6.getItems()[1].Length, 5, "items[1].Length");
    EQ_VALUE(eng6.getItems()[2].Offset, 16, "items[2].Offset");
    EQ_VALUE(eng6.getItems()[2].Length, 19, "items[2].Length");

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{10}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), 2, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 10, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 5, "items[0].Length");
    EQ_VALUE(eng6.getItems()[1].Offset, 16, "items[1].Offset");
    EQ_VALUE(eng6.getItems()[1].Length, 19, "items[1].Length");
    EQ_VALUE(eng6.getVARS().Offset, 10, "Offset");
    EQ_VALUE(eng6.getVARS().EndBefore, content_len, "EndBefore");

    eng6.getItems().Clear();
    Engine::FindNest(content, SizeT{15}, content_len, content_len, &eng6);
    EQ_VALUE(eng6.getItems().Size(), SizeT{1}, "items.Size()");
    EQ_VALUE(eng6.getItems()[0].Offset, 16, "items[0].Offset");
    EQ_VALUE(eng6.getItems()[0].Length, 19, "items[0].Length");
    EQ_VALUE(eng6.getVARS().Offset, 15, "Offset");
    EQ_VALUE(eng6.getVARS().EndBefore, content_len, "EndBefore");

    END_SUB_TEST;
}

static int TestEngine5() {
    // SizeT       ret;
    SizeT       content_len = 12;
    const char *content     = "{{{{{{}}}}}}";

    EQ_VALUE(Engine::Find("}", SizeT{1}, content, SizeT{0}, content_len),
             SizeT{7}, "return1");

    // /*
    //  * The offset has to start after the head (one), because zero is the
    //  match
    //  * that is being searched for its tail.
    //  */
    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 7,
    //                                 content_len);
    // EQ_VALUE(ret, 12, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 0,
    //                                 content_len);
    // EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("x", SizeT{1}, "y", SizeT{1}, content,
    // SizeT{1}, 7,
    //                                 content_len);
    // EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // 5, 6,
    //                                 content_len);
    // EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // 4, 6,
    //                                 content_len);
    // EQ_VALUE(ret, 8, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // 3, 6,
    //                                 content_len);
    // EQ_VALUE(ret, 9, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // 3, 7,
    //                                 content_len);
    // EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 7, 7); EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 7, 8); EQ_VALUE(ret, 8, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 7, 9); EQ_VALUE(ret, 9, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 7, 10); EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    // ret = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1}, content,
    // SizeT{1}, 12,
    //                                 content_len);
    // EQ_VALUE(ret, 12, "SkipInnerPatterns()");

    // content     = "{}";
    // content_len = 2;
    // ret         = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1},
    // content, 0, 2,
    //                                 content_len);
    // EQ_VALUE(ret, 2, "SkipInnerPatterns()");

    // content     = "{   }";
    // content_len = 5;
    // ret         = Engine::SkipInnerPatterns("{", SizeT{1}, "}", SizeT{1},
    // content, SizeT{1}, 5,
    //                                 content_len);
    // EQ_VALUE(ret, 5, "SkipInnerPatterns()");

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

    for (SizeT i = 0; i < items.Size(); i++) {
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
        explicit Engine7(Array<Item2__> *items) noexcept : items_(items) {}

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
            return 0;
        }

        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::Find("]]]", 3, content, offset, end_before);
        }

        SizeT Nest(const char *content, SizeT offset, SizeT end_before,
                   SizeT max_end_before) override {
            Engine7 eng7{&sub_items};
            return Engine::FindNest(content, offset, end_before, max_end_before,
                                    &eng7);
        }

        virtual SizeT length() noexcept { return 3; }

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

        Array<Item2__> *getsubItems() { return &sub_items; }

      private:
        Array<Item2__> *items_;
        Array<Item2__>  sub_items;
    };

    Engine7 eng7{&items};

    content     = "[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]] [[[]]] [[[[[[]]]]]]";
    content_len = 56;
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng7);
    toJSON(toJSON_ss, items, content);

    result =
        R"({"[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]]":{"O":0,"L":36,"S":{"[[[  [[[  [[[  ]]]  ]]]  ]]]":{"O":4,"L":28,"S":{"[[[  [[[  ]]]  ]]]":{"O":9,"L":18,"S":{"[[[  ]]]":{"O":14,"L":8}}}}}}},"[[[]]]":{"O":37,"L":6},"[[[[[[]]]]]]":{"O":44,"L":12,"S":{"[[[]]]":{"O":47,"L":6}}}})";
    EQ_VALUE(toJSON_ss, result, "toJSON_ss");

    struct Engine8 : Engine7 {
        explicit Engine8(Array<Item2__> *items) noexcept : Engine7(items) {}

        SizeT FindH(const char *content, SizeT offset,
                    SizeT end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '{') {
                    return ++offset;
                }

                ++offset;
            }

            return 0; // No match
        }

        SizeT FindT(const char *content, SizeT offset,
                    SizeT end_before) const noexcept override {
            return Engine::FindOne('}', content, offset, end_before);
        }

        SizeT length() noexcept override { return 1; }

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
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng8);
    toJSON_ss.Reset();
    toJSON(toJSON_ss, items, content);

    result =
        R"({"{....}":{"O":0,"L":6},"{}":{"O":7,"L":2},"{{ }{  }{   }{    }}":{"O":10,"L":20,"S":{"{ }":{"O":11,"L":3},"{  }":{"O":14,"L":4},"{   }":{"O":18,"L":5},"{    }":{"O":23,"L":6}}},"{{{{{{{}}}}}}}":{"O":31,"L":14,"S":{"{{{{{{}}}}}}":{"O":32,"L":12,"S":{"{{{{{}}}}}":{"O":33,"L":10,"S":{"{{{{}}}}":{"O":34,"L":8,"S":{"{{{}}}":{"O":35,"L":6,"S":{"{{}}":{"O":36,"L":4,"S":{"{}":{"O":37,"L":2}}}}}}}}}}}}},"{A{B}C{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}U{V}W}":{"O":54,"L":47,"S":{"{B}":{"O":56,"L":3},"{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}":{"O":60,"L":35,"S":{"{E}":{"O":62,"L":3},"{G{H}I{G{K{L}M}N}O{P}Q}":{"O":66,"L":23,"S":{"{H}":{"O":68,"L":3},"{G{K{L}M}N}":{"O":72,"L":11,"S":{"{K{L}M}":{"O":74,"L":7,"S":{"{L}":{"O":76,"L":3}}}}},"{P}":{"O":84,"L":3}}},"{S}":{"O":90,"L":3}}},"{V}":{"O":96,"L":3}}},"{1{2{3{4{5{6{7}8}9}10}10}12}13}":{"O":110,"L":31,"S":{"{2{3{4{5{6{7}8}9}10}10}12}":{"O":112,"L":26,"S":{"{3{4{5{6{7}8}9}10}10}":{"O":114,"L":21,"S":{"{4{5{6{7}8}9}10}":{"O":116,"L":16,"S":{"{5{6{7}8}9}":{"O":118,"L":11,"S":{"{6{7}8}":{"O":120,"L":7,"S":{"{7}":{"O":122,"L":3}}}}}}}}}}}}}})";
    EQ_VALUE(toJSON_ss, result, "toJSON_ss");

    content = "";
    items.Clear();
    content_len = 0;
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng8);
    toJSON_ss.Reset();
    toJSON(toJSON_ss, items, content);

    EQ_VALUE(toJSON_ss.Length(), 0, "toJSON_ss");

    content = "{ { }";
    items.Clear();
    content_len = 5;
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng8);
    EQ_VALUE(items.Size(), SizeT{1}, "items.Size()");

    content = "{ {{ }}";
    items.Clear();
    content_len = 5;
    Engine::FindNest(content, SizeT{0}, content_len, content_len, &eng8);
    EQ_VALUE(items.Size(), 0, "items.Size()");

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
