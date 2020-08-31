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

#ifndef QENTEM_ENGINETEST_H_
#define QENTEM_ENGINETEST_H_

namespace Qentem {
namespace Test {

static int TestEngine1() {
    ULong       ret;
    ULong       content_len = 0;
    UInt        find_len    = 3;
    const char *content     = "";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, 0, 1);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "A";
    content_len = 1;
    find_       = "A";
    find_len    = 1;

    ret = Engine::FindOne(*find_, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 1, "return");

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 1, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "AAAAA";
    content_len = 5;

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 2, "return");

    ret = Engine::Find(find_, find_len, content, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, 2, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 3, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, 4, content_len);
    SHOULD_EQUAL_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, 5, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "  A A        A";
    content_len = 14;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 2, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 3, content_len);
    SHOULD_EQUAL_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, 4, content_len);
    SHOULD_EQUAL_VALUE(ret, 5, "return");

    ret = Engine::Find(find_, find_len, content, 5, content_len);
    SHOULD_EQUAL_VALUE(ret, 14, "return");

    content     = "A";
    content_len = 1;
    find_       = "AB";
    find_len    = 2;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content = "AA";
    ret     = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content = "A   A   A";
    ret     = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content = "  A   A   A  ";
    ret     = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "AB";
    content_len = 2;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 2, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, 2, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = " AB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    content     = "AAB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    content     = "AAAB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, 2, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    content     = "AAAAAAAAAB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, 5, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, 8, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    content     = "  AB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, 1, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    ret = Engine::Find(find_, find_len, content, 2, content_len);
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    content     = "        AB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, 5, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    ret = Engine::Find(find_, find_len, content, 8, content_len);
    SHOULD_EQUAL_VALUE(ret, 10, "return");

    content     = "AAAAAAAAA";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, 5, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, 8, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret =
        Engine::Find(find_, find_len, content, (content_len - 1), content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    find_       = "ABC";
    find_len    = 3;
    content     = "CBA";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "BCBA";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "123456789123456CBA";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "ADC";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "123456789123456ADC";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    END_SUB_TEST;
}

static int TestEngine2() {
    ULong       ret;
    ULong       content_len = 16;
    UInt        find_len    = 3;
    const char *content     = "ABABABABABAAABAB";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "ABABABABABAAABABC";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 17, "return");

    ret = Engine::Find(find_, find_len, content, 15, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = Engine::Find(find_, find_len, content, 14, content_len);
    SHOULD_EQUAL_VALUE(ret, 17, "return");

    content     = "ABABABABABCABAAAB";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 11, "return");

    content     = "   ABC   ";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 6, "return");

    content     = "ABC   ";
    content_len = 6;

    ret = Engine::Find(find_, find_len, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

#ifdef QENTEM_SIMD_ENABLED_
    ret = Engine::Count("(", 1, "123(", 0, 4);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count("(", 1, "123(", 3, 4);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count("(", 1, "(123(", 0, 5);
    SHOULD_EQUAL_VALUE(ret, 2, "Count()");

    ret = Engine::Count("(", 1, "(123(", 1, 5);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count(
        "(", 1, "123123123(1231231231231(23123123123(123123123123123(123", 0,
        55);
    SHOULD_EQUAL_VALUE(ret, 4, "Count()");

    ret = Engine::Count("((", 2, "12((", 0, 4);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count("((", 2, "12((", 2, 4);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count("((", 2, "((1((", 0, 5);
    SHOULD_EQUAL_VALUE(ret, 2, "Count()");

    ret = Engine::Count("((", 2, "((1((", 1, 5);
    SHOULD_EQUAL_VALUE(ret, 1, "Count()");

    ret = Engine::Count(
        "((", 2, "123123123((1231231231231((23123123123((123123123123123((123",
        0, 59);
    SHOULD_EQUAL_VALUE(ret, 4, "Count()");
#endif

    END_SUB_TEST;
}

static int TestEngine3() {
    ULong       ret;
    ULong       content_len = 0;
    const char *content     = "";

    struct test1 : Engine {
        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '1') {
                    return ++offset;
                }

                ++offset;
            }

            // No match.
            return 0;
        }
    };

    // Checking for any weirdness.
    ret = test1().FindNest("1", 0, 1, 1);
    SHOULD_EQUAL_VALUE(ret, 1, "return");

    ret = test1().FindNest("1   ", 0, 4, 4);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    struct test2 : Engine {
        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '(') {
                    return ++offset;
                }

                ++offset;
            }

            // No match.
            return 0;
        }

        inline bool hasTail() const noexcept override {
            return true;
        }

        ULong find2(const char *content, ULong offset,
                    ULong end_before) noexcept override {
            return Find(")", 1, content, offset, end_before);
        }
    };

    ret = test2().FindNest("", 0, 0, 0);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    ret = test2().FindNest("(", 0, 0, 1);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "   (  ";
    content_len = 6;

    ret = test2().FindNest(content, 0, content_len, content_len);
    /*
     * When the engine fails to find a complete match, it returns the last
     * offset that stops at: to prevent Infinite loops from according.
     */
    SHOULD_EQUAL_VALUE(ret, 4, "return");

    content     = "((((((((";
    content_len = 8;

    ret = test2().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 1, "return");

    content     = "()";
    content_len = 2;

    ret = test2().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 2, "return");

    ret = test2().FindNest(content, 1, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = " ()";
    content_len = 3;

    ret = test2().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = test2().FindNest(content, 1, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    ret = test2().FindNest(content, 2, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = " ()             ";
    content_len = 16;

    ret = test2().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return"); // It called Found() already.

    ret = test2().FindNest(content, 0, 3, 3);
    SHOULD_EQUAL_VALUE(ret, 3, "return");

    END_SUB_TEST;
}

static int TestEngine4() {
    ULong       ret;
    ULong       content_len = 0;
    const char *content     = "";

    struct test2 : Engine {
        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '(') {
                    return ++offset;
                }

                ++offset;
            }

            // No match.
            return 0;
        }

        inline bool hasTail() const noexcept override {
            return true;
        }

        ULong find2(const char *content, ULong offset,
                    ULong end_before) noexcept override {
            return Find(")", 1, content, offset, end_before);
        }
    };

    struct test3 : test2 {
        inline bool hasTail() const noexcept override {
            return true;
        }

        ULong find2(const char *content, ULong offset,
                    ULong end_before) noexcept override {
            return Find("))", 2, content, offset, end_before);
        }
    };

    content     = "       )          ";
    content_len = 18;

    // Checking for weirdness.
    ret = test3().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = " ()             ";
    content_len = 16;

    // Checking for weirdness.
    ret = test3().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 2, "return");

    struct test4 : test3 {
        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
            while (offset < end_before) {

                if ((content[offset] == '(') &&
                    (content[(offset + 1)] == '(')) {
                    return (offset + 2);
                }

                ++offset;
            }

            // No match.
            return 0;
        }
    };

    content     = "(())";
    content_len = 4;

    ret = test4().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "      (())";
    content_len = 10;

    ret = test4().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "      (()) ";
    content_len = 11;

    ret = test4().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 0, "return");

    content     = "( (())";
    content_len = 6;

    ret = test4().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    content     = "( ( ( ( ( ( (())";
    content_len = 16;

    ret = test4().FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, content_len, "return");

    struct test5 : test4 {
        void failed() noexcept override {
            has_error = true;
        }

        bool getError() const {
            return has_error;
        }

      private:
        bool has_error = false;
    };

    content     = "(()";
    content_len = 3;
    test5 test5_;

    ret = test5_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(ret, 2, "return");
    SHOULD_EQUAL_TRUE(test5_.getError(), "has_error");

    struct Item__ {
        ULong Offset;
        ULong Length;
    };

    struct VARS__ {
        const char *Content{nullptr};
        ULong       Offset{0};
        ULong       EndBefore{0};
    };

    struct test6 : test2 {
        void Found(const char *content, ULong offset, ULong end_before,
                   ULong start_offset, ULong &current_offset) override {
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

    test6 test6_;
    content     = "()";
    content_len = 2;

    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 2, "items[0].Length");
    SHOULD_EQUAL_VALUE(test6_.getVARS().Content, content, "Content");
    SHOULD_EQUAL_VALUE(test6_.getVARS().Offset, 0, "Offset");
    SHOULD_EQUAL_VALUE(test6_.getVARS().EndBefore, content_len, "EndBefore");

    content     = "()(";
    content_len = 3;

    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 2, "items[0].Length");

    content     = "()((((((";
    content_len = 8;

    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 2, "items[0].Length");

    content     = "()((((((()";
    content_len = 10;
    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 2, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 2, "items[0].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Offset, 2, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Length, 8, "items[1].Length");

    content     = "()(((()((()()(()()";
    content_len = 18;
    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 6, "items.Size()");

    content     = "  ()  ()  ()()  ()";
    content_len = 18;
    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 5, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 2, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 2, "items[0].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Offset, 6, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Length, 2, "items[1].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[2].Offset, 10, "items[2].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[2].Length, 2, "items[2].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[3].Offset, 12, "items[3].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[3].Length, 2, "items[3].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[4].Offset, 16, "items[4].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[4].Length, 2, "items[4].Length");
    SHOULD_EQUAL_VALUE(test6_.getVARS().Content, content, "Content");
    SHOULD_EQUAL_VALUE(test6_.getVARS().Offset, 0, "Offset");
    SHOULD_EQUAL_VALUE(test6_.getVARS().EndBefore, content_len, "EndBefore");

    content     = "( ) (     ) (  )";
    content_len = 16;
    test6_.getItems().SoftClear();
    test6_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test6_.getItems().Size(), 3, "items.Size()");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[0].Length, 3, "items[0].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Offset, 4, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[1].Length, 7, "items[1].Length");
    SHOULD_EQUAL_VALUE(test6_.getItems()[2].Offset, 12, "items[2].Offset");
    SHOULD_EQUAL_VALUE(test6_.getItems()[2].Length, 4, "items[2].Length");

    struct test7 : test5 {
        void Found(const char *content, ULong offset, ULong end_before,
                   ULong start_offset, ULong &current_offset) override {
            start_offset -= 2;
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

    test7 test7_;
    content     = "(())";
    content_len = 4;

    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())(";
    content_len = 5;

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())((";
    content_len = 6;

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 0, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 4, "items[0].Length");

    content     = "(())(()";
    content_len = 7;

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");

    content     = "(())(((()(()(()()";
    content_len = 17;

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");

    content     = "(())(((())";
    content_len = 10;
    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 2, "items.Size()");

    content     = "  (())  (())  (())(())  (())";
    content_len = 28;
    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 5, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 2, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 4, "items[0].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Offset, 8, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Length, 4, "items[1].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[2].Offset, 14, "items[2].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[2].Length, 4, "items[2].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[3].Offset, 18, "items[3].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[3].Length, 4, "items[3].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[4].Offset, 24, "items[4].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[4].Length, 4, "items[4].Length");

    content     = "  ((   )) (( )) ((               ))";
    content_len = 35;

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 3, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 2, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 7, "items[0].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Offset, 10, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Length, 5, "items[1].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[2].Offset, 16, "items[2].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[2].Length, 19, "items[2].Length");

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 10, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 2, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 10, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 5, "items[0].Length");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Offset, 16, "items[1].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[1].Length, 19, "items[1].Length");
    SHOULD_EQUAL_VALUE(test7_.getVARS().Offset, 10, "Offset");
    SHOULD_EQUAL_VALUE(test7_.getVARS().EndBefore, content_len, "EndBefore");

    test7_.getItems().SoftClear();
    test7_.FindNest(content, 15, content_len, content_len);
    SHOULD_EQUAL_VALUE(test7_.getItems().Size(), 1, "items.Size()");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Offset, 16, "items[0].Offset");
    SHOULD_EQUAL_VALUE(test7_.getItems()[0].Length, 19, "items[0].Length");
    SHOULD_EQUAL_VALUE(test7_.getVARS().Offset, 15, "Offset");
    SHOULD_EQUAL_VALUE(test7_.getVARS().EndBefore, content_len, "EndBefore");

    END_SUB_TEST;
}

static int TestEngine5() {
    ULong       ret1;
    ULong       ret2;
    ULong       content_len = 12;
    const char *content     = "{{{{{{}}}}}}";

    ret1 = Engine::Find("}", 1, content, 0, content_len);
    SHOULD_EQUAL_VALUE(ret1, 7, "return1");

    /*
     * The offset has to start after the head (one), because zero is the match
     * that is being searched in.
     */
    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 7, content_len);
    SHOULD_EQUAL_VALUE(ret2, 11, "return2");

    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 0, content_len);
    SHOULD_EQUAL_VALUE(ret2, 0, "return2");

    ret2 =
        Engine::SkipInnerPatterns("x", 1, "y", 1, content, 1, 7, content_len);
    SHOULD_EQUAL_VALUE(ret2, 0, "return2");

    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 2, content_len);
    SHOULD_EQUAL_VALUE(ret2, 2, "return2");

    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 3, content_len);
    SHOULD_EQUAL_VALUE(ret2, 7, "return2");

    ret2 = Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 7, 7);
    SHOULD_EQUAL_VALUE(ret2, 7, "return2");

    ret2 = Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 7, 8);
    SHOULD_EQUAL_VALUE(ret2, 8, "return2");

    ret2 = Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 7, 9);
    SHOULD_EQUAL_VALUE(ret2, 9, "return2");

    ret2 = Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 7, 10);
    SHOULD_EQUAL_VALUE(ret2, 10, "return2");

    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 12, content_len);
    SHOULD_EQUAL_VALUE(ret2, 12, "return2");

    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 12, content_len);
    SHOULD_EQUAL_VALUE(ret2, 12, "return2");

    content     = "{}";
    content_len = 2;
    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 0, 2, content_len);
    SHOULD_EQUAL_VALUE(ret2, 2, "return2");

    content     = "{   }";
    content_len = 5;
    ret2 =
        Engine::SkipInnerPatterns("{", 1, "}", 1, content, 1, 5, content_len);
    SHOULD_EQUAL_VALUE(ret2, 0, "return2");

    END_SUB_TEST;
}

struct Item2__ {
    ULong          Offset{0};
    ULong          Length{0};
    Array<Item2__> SubItems;
};

static String toJSON(const Array<Item2__> &items, const char *content) {
    if (items.Size() == 0) {
        return String("{}");
    }

    StringStream ss;
    ss += '{';

    for (ULong i = 0; i < items.Size(); i++) {
        if (i != 0) {
            ss += ',';
        }

        ss += '"';
        ss.Add(&(content[items[i].Offset]), items[i].Length);
        ss += "\":{";

        ss += "\"O\":";
        ss += Digit::NumberToString(items[i].Offset);
        ss += ",\"L\":";
        ss += Digit::NumberToString(items[i].Length);

        if (items[i].SubItems.Size() != 0) {
            ss += ",\"S\":";
            ss += toJSON(items[i].SubItems, content);
        }

        ss += '}';
    }

    ss += '}';

    return ss.GetString();
}

static int TestEngine6() {
    Array<Item2__> items;
    const char *   content;
    ULong          content_len;
    const char *   result;
    String         toJSON_result;

    struct test5_1 : Engine {
        explicit test5_1(Array<Item2__> *items) noexcept : items_(items) {
        }

        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
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

        inline bool hasTail() const noexcept override {
            return true;
        }

        ULong find2(const char *content, ULong offset,
                    ULong end_before) noexcept override {
            return Find("]]]", 3, content, offset, end_before);
        }

        ULong nest(const char *content, ULong offset, ULong end_before,
                   ULong max_end_before) override {
            return test5_1(&sub_items)
                .FindNest(content, offset, end_before, max_end_before);
        }

        virtual UInt length() noexcept {
            return 3;
        }

        void Found(const char *content, ULong offset, ULong end_before,
                   ULong start_offset, ULong &current_offset) override {

            Item2__ item;

            item.Offset   = (start_offset - length());
            item.Length   = ((current_offset + length()) - start_offset);
            item.SubItems = static_cast<Array<Item2__> &&>(sub_items);

            items_->Add(static_cast<Item2__ &&>(item));

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

    test5_1 t51(&items);

    content     = "[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]] [[[]]] [[[[[[]]]]]]";
    content_len = 56;
    t51.FindNest(content, 0, content_len, content_len);
    toJSON_result = toJSON(items, content);

    result =
        R"({"[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]]":{"O":0,"L":36,"S":{"[[[  [[[  [[[  ]]]  ]]]  ]]]":{"O":4,"L":28,"S":{"[[[  [[[  ]]]  ]]]":{"O":9,"L":18,"S":{"[[[  ]]]":{"O":14,"L":8}}}}}}},"[[[]]]":{"O":37,"L":6},"[[[[[[]]]]]]":{"O":44,"L":12,"S":{"[[[]]]":{"O":47,"L":6}}}})";
    SHOULD_EQUAL_VALUE(toJSON_result, result, "toJSON_result");

    struct test5_2 : test5_1 {
        explicit test5_2(Array<Item2__> *items) noexcept : test5_1(items) {
        }

        ULong find(const char *content, ULong offset,
                   ULong end_before) noexcept override {
            while (offset < end_before) {
                if (content[offset] == '{') {
                    return ++offset;
                }

                ++offset;
            }

            // No match.
            return 0;
        }

        inline bool hasTail() const noexcept override {
            return true;
        }

        ULong find2(const char *content, ULong offset,
                    ULong end_before) noexcept override {
            return Find("}", 1, content, offset, end_before);
        }

        UInt length() noexcept override {
            return 1;
        }

        ULong nest(const char *content, ULong offset, ULong end_before,
                   ULong max_end_before) override {
            return test5_2(getsubItems())
                .FindNest(content, offset, end_before, max_end_before);
        }
    };

    content =
        R"({....} {} {{ }{  }{   }{    }} {{{{{{{}}}}}}}
        {A{B}C{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}U{V}W}
        {1{2{3{4{5{6{7}8}9}10}10}12}13})";

    // content = R"({....})";
    items.Clear();
    test5_2 t52(&items);
    content_len = String::Count(content);
    t52.FindNest(content, 0, content_len, content_len);
    toJSON_result = toJSON(items, content);

    result =
        R"({"{....}":{"O":0,"L":6},"{}":{"O":7,"L":2},"{{ }{  }{   }{    }}":{"O":10,"L":20,"S":{"{ }":{"O":11,"L":3},"{  }":{"O":14,"L":4},"{   }":{"O":18,"L":5},"{    }":{"O":23,"L":6}}},"{{{{{{{}}}}}}}":{"O":31,"L":14,"S":{"{{{{{{}}}}}}":{"O":32,"L":12,"S":{"{{{{{}}}}}":{"O":33,"L":10,"S":{"{{{{}}}}":{"O":34,"L":8,"S":{"{{{}}}":{"O":35,"L":6,"S":{"{{}}":{"O":36,"L":4,"S":{"{}":{"O":37,"L":2}}}}}}}}}}}}},"{A{B}C{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}U{V}W}":{"O":54,"L":47,"S":{"{B}":{"O":56,"L":3},"{D{E}F{G{H}I{G{K{L}M}N}O{P}Q}R{S}T}":{"O":60,"L":35,"S":{"{E}":{"O":62,"L":3},"{G{H}I{G{K{L}M}N}O{P}Q}":{"O":66,"L":23,"S":{"{H}":{"O":68,"L":3},"{G{K{L}M}N}":{"O":72,"L":11,"S":{"{K{L}M}":{"O":74,"L":7,"S":{"{L}":{"O":76,"L":3}}}}},"{P}":{"O":84,"L":3}}},"{S}":{"O":90,"L":3}}},"{V}":{"O":96,"L":3}}},"{1{2{3{4{5{6{7}8}9}10}10}12}13}":{"O":110,"L":31,"S":{"{2{3{4{5{6{7}8}9}10}10}12}":{"O":112,"L":26,"S":{"{3{4{5{6{7}8}9}10}10}":{"O":114,"L":21,"S":{"{4{5{6{7}8}9}10}":{"O":116,"L":16,"S":{"{5{6{7}8}9}":{"O":118,"L":11,"S":{"{6{7}8}":{"O":120,"L":7,"S":{"{7}":{"O":122,"L":3}}}}}}}}}}}}}})";
    SHOULD_EQUAL_VALUE(toJSON_result, result, "toJSON_result");

    content = "";
    items.Clear();
    content_len = 0;
    t52.FindNest(content, 0, content_len, content_len);
    toJSON_result = toJSON(items, content);

    SHOULD_EQUAL_VALUE(toJSON_result, "{}", "toJSON_result");

    content = "{ { }";
    items.Clear();
    content_len = 5;
    t52.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(items.Size(), 0, "items.Size()");

    content = "{ {{ }}";
    items.Clear();
    content_len = 5;
    t52.FindNest(content, 0, content_len, content_len);
    SHOULD_EQUAL_VALUE(items.Size(), 0, "items.Size()");

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
