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

#include "Engine.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_ENGINE_TESTS_H_
#define QENTEM_ENGINE_TESTS_H_

namespace Qentem {
namespace Test {

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

// static int TestEngine3() {
//     SizeT       ret;
//     SizeT       content_len = 12U;
//     const char *content     = "{{{{{{}}}}}}";

//     END_SUB_TEST;
// }

static int RunEngineTests() {
    STARTING_TEST("Engine.hpp");

    START_TEST("Engine Test 1", TestEngine1);
    START_TEST("Engine Test 2", TestEngine2);
    // START_TEST("Engine Test 3", TestEngine3);

    END_TEST("Engine.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
