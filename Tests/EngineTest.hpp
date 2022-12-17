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

    ret = Engine::Find(find_, find_len, content, (content_len - 1), content_len);
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
    SizeT           ret;
    SizeT           content_len = 16;
    constexpr SizeT find_len    = 3;
    const char     *content     = "ABABABABABAAABAB";
    const char     *find_       = "ABC";

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

static int RunEngineTests() {
    STARTING_TEST("Engine.hpp");

    START_TEST("Engine Test 1", TestEngine1);
    START_TEST("Engine Test 2", TestEngine2);

    END_TEST("Engine.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
