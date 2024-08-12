/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_ENGINE_TESTS_H
#define QENTEM_ENGINE_TESTS_H

#include "QTest.hpp"
#include "Array.hpp"
#include "Digit.hpp"
#include "Engine.hpp"
#include "StringStream.hpp"

namespace Qentem {
namespace Test {

static void TestEngine1(QTest &test) {
    SizeT       ret;
    SizeT       content_len = 0;
    SizeT       find_len    = 3;
    const char *content     = "";
    const char *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, SizeT{0}, SizeT{1});
    test.IsEqual(ret, 0U, __LINE__);

    content     = "A";
    content_len = 1;
    find_       = "A";

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    test.IsEqual(ret, 1U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "AAAAA";
    content_len = 5;

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    test.IsEqual(ret, 2U, __LINE__);

    ret = Engine::FindOne(*find_, content, content_len, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{4}, content_len);
    test.IsEqual(ret, 5U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "  A A        A";
    content_len = 14;

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    test.IsEqual(ret, 5U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{7}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    test.IsEqual(ret, 5U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{4}, content_len);
    test.IsEqual(ret, 5U, __LINE__);

    ret = Engine::FindOne(*find_, content, SizeT{5}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    content     = "A";
    content_len = 1;
    find_       = "AB";
    find_len    = 2;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content = "AA";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content = "A   A   A";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content = "  A   A   A  ";
    ret     = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "AB";
    content_len = 2;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 2U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = " AB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    content     = "AAB";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    content     = "AAAB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    content     = "AAAAAAAAAB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    content     = "  AB";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{1}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{2}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    content     = "        AB";
    content_len = 10;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    content     = "AAAAAAAAA";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{8}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::Find(find_, find_len, content, (content_len - SizeT{1}), content_len);
    test.IsEqual(ret, 0U, __LINE__);

    find_       = "ABC";
    find_len    = 3;
    content     = "CBA";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "BCBA";
    content_len = 4;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "123456789123456CBA";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "ADC";
    content_len = 3;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "123456789123456ADC";
    content_len = 18;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "ADC0ADC0ADC0ADC0ADC0ADC0ADC0ADC0ADC0ADC0";
    content_len = 40;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);
}

static void TestEngine2(QTest &test) {
    SizeT           ret;
    SizeT           content_len = 16;
    constexpr SizeT find_len    = 3;
    const char     *content     = "ABABABABABAAABAB";
    const char     *find_       = "ABC";

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "ABABABABABAAABABC";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 17U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{15}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::Find(find_, find_len, content, SizeT{14}, content_len);
    test.IsEqual(ret, 17U, __LINE__);

    content     = "ABABABABABCABAAAB";
    content_len = 17;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, SizeT{11}, __LINE__);

    content     = "   ABC   ";
    content_len = 9;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 6U, __LINE__);

    content     = "ABC   ";
    content_len = 6;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::FindOne('1', "00000001", SizeT{0}, SizeT{8});
    test.IsEqual(ret, 8U, __LINE__);

    ret = Engine::FindOne('1', "0000000000000001", SizeT{0}, SizeT{16});
    test.IsEqual(ret, 16U, __LINE__);

    ret = Engine::FindOne('1', "00000000000000000000000000000001", SizeT{0}, SizeT{32});
    test.IsEqual(ret, 32U, __LINE__);

    content     = "ABC   ";
    content_len = 6;

    ret = Engine::Find(find_, find_len, content, SizeT{0}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    content = R"(<loop value="loop1-value">loop1-value, </loop>)";

    ret = Engine::Find("loop1-value", SizeT{11}, content, SizeT{26}, SizeT{39}, SizeT{46});
    test.IsEqual(ret, 37U, __LINE__);
}

static void TestEngine3(QTest &test) {
    SizeT       ret;
    SizeT       content_len = 0;
    const char *content     = "";

    ret = Engine::SkipInnerPatterns(char{0}, char{0}, content, SizeT{0}, SizeT{0});
    test.IsEqual(ret, 0U, __LINE__);

    content     = "   (  ";
    content_len = 6;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "((((((((";
    content_len = 8;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{6}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{7}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{8}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "()";
    content_len = 2;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    test.IsEqual(ret, 2U, __LINE__);

    content     = " ()";
    content_len = 3;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{2}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = " ()             ";
    content_len = 16;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{2}, content_len);
    test.IsEqual(ret, 3U, __LINE__);

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);
}

static void TestEngine4(QTest &test) {
    SizeT       ret;
    SizeT       content_len = 12;
    const char *content     = "{{{{{{}}}}}}";

    ret = Engine::FindOne('}', content, SizeT{0}, content_len);
    test.IsEqual(ret, 7U, __LINE__);

    ret = Engine::SkipInnerPatterns('x', 'y', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    test.IsEqual(ret, 12U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, content_len);
    test.IsEqual(ret, 11U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, content_len);
    test.IsEqual(ret, 9U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, content_len);
    test.IsEqual(ret, 8U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, content_len);
    test.IsEqual(ret, 7U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, content_len);
    test.IsEqual(ret, 8U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{8}, content_len);
    test.IsEqual(ret, 9U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{9}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{10}, content_len);
    test.IsEqual(ret, 11U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{11}, content_len);
    test.IsEqual(ret, 12U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{12}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{13}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{14}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{100}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{7});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{8});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{9});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{10});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{11});
    test.IsEqual(ret, 0U, __LINE__);

    content     = "{}";
    content_len = 2;
    ret         = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    test.IsEqual(ret, 2U, __LINE__);

    content     = "{   }";
    content_len = 5;
    ret         = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    test.IsEqual(ret, 5U, __LINE__);

    content     = "{{{}{{}{}{}{}}}}{}";
    content_len = 18;

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    test.IsEqual(ret, 16U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, content_len);
    test.IsEqual(ret, 15U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, content_len);
    test.IsEqual(ret, 15U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, content_len);
    test.IsEqual(ret, 7U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{8}, content_len);
    test.IsEqual(ret, 9U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{9}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{10}, content_len);
    test.IsEqual(ret, 11U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{11}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{12}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{13}, content_len);
    test.IsEqual(ret, 14U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{14}, content_len);
    test.IsEqual(ret, 15U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{15}, content_len);
    test.IsEqual(ret, 16U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{100}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{14});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{13});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, SizeT{12});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, SizeT{11});
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, SizeT{10});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, SizeT{9});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, SizeT{8});
    test.IsEqual(ret, 7U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, SizeT{7});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{6});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{5});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, SizeT{4});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{3});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{2});
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{1});
    test.IsEqual(ret, 0U, __LINE__);
}

static void TestEngine5(QTest &test) {
    SizeT       ret;
    SizeT       content_len = 18;
    const char *content     = "       )          ";

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = " ()             ";
    content_len = 16;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "(())";
    content_len = 4;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 4U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "      (())";
    content_len = 10;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "      (()) ";
    content_len = 11;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    test.IsEqual(ret, 10U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{11}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{12}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{13}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{255}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "( (())";
    content_len = 6;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 6U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 6U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "( ( ( ( ( ( (())";
    content_len = 16;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{11}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{12}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{13}, content_len);
    test.IsEqual(ret, 16U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{14}, content_len);
    test.IsEqual(ret, 16U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{15}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{16}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{255}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    content     = "(()";
    content_len = 3;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 0U, __LINE__);
}

static void TestEngine6(QTest &test) {
    SizeT           ret;
    constexpr SizeT content_len = 50;
    const char     *content     = "{-{-{-     -}{-{-  -}{-   -}{- -}{- -}-}-}-}{- -}";

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{0}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{1}, content_len);
    test.IsEqual(ret, 44U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{2}, content_len);
    test.IsEqual(ret, 44U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{3}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{4}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{5}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{6}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{7}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{8}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{9}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{10}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{11}, content_len);
    test.IsEqual(ret, 13U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{12}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{13}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{14}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{15}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{16}, content_len);
    test.IsEqual(ret, 21U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{17}, content_len);
    test.IsEqual(ret, 21U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{18}, content_len);
    test.IsEqual(ret, 21U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{19}, content_len);
    test.IsEqual(ret, 21U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{20}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{21}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{22}, content_len);
    test.IsEqual(ret, 28U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{23}, content_len);
    test.IsEqual(ret, 28U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{24}, content_len);
    test.IsEqual(ret, 28U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{25}, content_len);
    test.IsEqual(ret, 28U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{26}, content_len);
    test.IsEqual(ret, 28U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{27}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{28}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{29}, content_len);
    test.IsEqual(ret, 33U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{30}, content_len);
    test.IsEqual(ret, 33U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{31}, content_len);
    test.IsEqual(ret, 33U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{32}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{33}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{34}, content_len);
    test.IsEqual(ret, 38U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{35}, content_len);
    test.IsEqual(ret, 38U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{36}, content_len);
    test.IsEqual(ret, 38U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{37}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{38}, content_len);
    test.IsEqual(ret, 40U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{39}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{40}, content_len);
    test.IsEqual(ret, 42U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{41}, content_len);
    test.IsEqual(ret, 44U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{42}, content_len);
    test.IsEqual(ret, 44U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{43}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{45}, content_len);
    test.IsEqual(ret, 49U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{46}, content_len);
    test.IsEqual(ret, 49U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{47}, content_len);
    test.IsEqual(ret, 49U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{48}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{49}, content_len);
    test.IsEqual(ret, 0U, __LINE__);

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{50}, content_len);
    test.IsEqual(ret, 0U, __LINE__);
}

struct Item2Engine {
    Array<Item2Engine> SubItems;
    SizeT              Offset{0};
    SizeT              Length{0};
    SizeT              padding_[2]{0};
};

static void to_JSON(StringStream<char> &stream, const Array<Item2Engine> &items, const char *content) {
    if (items.IsEmpty()) {
        return;
    }

    const Item2Engine *storage = items.First();

    stream += '{';

    for (SizeT i = 0; i < items.Size(); i++) {
        stream += '"';
        stream.Write((content + storage[i].Offset), storage[i].Length);
        stream += "\":{";

        stream += "\"O\":";
        Digit::NumberToString(stream, storage[i].Offset);
        stream += ",\"L\":";
        Digit::NumberToString(stream, storage[i].Length);

        if (storage[i].SubItems.IsNotEmpty()) {
            stream += ",\"S\":";
            to_JSON(stream, storage[i].SubItems, content);
        }

        stream += '}';
        stream += ',';
    }

    stream.StepBack(1);

    stream += '}';
}

static void find_Engine701(const char *l_tag, SizeT l_tag_len, const char *r_tag, SizeT r_tag_len,
                           Array<Item2Engine> &items, const char *content, SizeT offset, SizeT end_offset) {
    const SizeT l_r_len      = (l_tag_len + r_tag_len);
    SizeT       e_tag_offset = offset;
    SizeT       s_tag_offset;

    while (offset < end_offset) {
        if (l_tag_len == 1) {
            s_tag_offset = Engine::FindOne(l_tag[0], content, e_tag_offset, end_offset);
        } else {
            s_tag_offset = Engine::Find(l_tag, l_tag_len, content, e_tag_offset, end_offset);
        }

        if (s_tag_offset == 0) {
            return;
        }

        if (l_tag_len == 1) {
            e_tag_offset = Engine::SkipInnerPatterns(l_tag[0], r_tag[0], content, s_tag_offset, end_offset);
        } else {
            e_tag_offset =
                Engine::SkipInnerPatterns(l_tag, l_tag_len, r_tag, r_tag_len, content, s_tag_offset, end_offset);
        }

        if (e_tag_offset == 0) {
            return;
        }

        Item2Engine item;
        item.Offset = (s_tag_offset - l_tag_len);
        item.Length = (e_tag_offset - item.Offset);

        if ((item.Length - l_r_len) >= l_r_len) {
            find_Engine701(l_tag, l_tag_len, r_tag, r_tag_len, item.SubItems, content, s_tag_offset,
                           (e_tag_offset - r_tag_len));
        }

        items += Memory::Move(item);

        offset = e_tag_offset;
    }
}

static void TestEngine7(QTest &test) {
    Array<Item2Engine> items;
    StringStream<char> stream;
    const char        *content;
    const char        *result;
    /////////////////////////////////////////////////////////////////////////////////////////

    content = "[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]] [[[]]] [[[[[[]]]]]] [[[-]]]    [[[X]]]";
    find_Engine701("[[[", 3U, "]]]", 3U, items, content, 0U, 75);
    to_JSON(stream, items, content);
    result =
        R"({"[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]]":{"O":0,"L":36,"S":{"[[[  [[[  [[[  ]]]  ]]]  ]]]":{"O":4,"L":28,"S":{"[[[  [[[  ]]]  ]]]":{"O":9,"L":18,"S":{"[[[  ]]]":{"O":14,"L":8}}}}}}},"[[[]]]":{"O":37,"L":6},"[[[[[[]]]]]]":{"O":44,"L":12,"S":{"[[[]]]":{"O":47,"L":6}}},"[[[-]]]":{"O":57,"L":7},"[[[X]]]":{"O":68,"L":7}})";
    test.IsEqual(stream, result, __LINE__);
    stream.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////

    content =
        R"((....) () (( )(  )(   )(    )) ((((((())))))) (A(B)C(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)U(V)W) (1(2(3(4(5(6(7)8)9)10)10)12)13))";
    find_Engine701("(", 1U, ")", 1U, items, content, 0U, 125);
    to_JSON(stream, items, content);
    result =
        R"j({"(....)":{"O":0,"L":6},"()":{"O":7,"L":2},"(( )(  )(   )(    ))":{"O":10,"L":20,"S":{"( )":{"O":11,"L":3},"(  )":{"O":14,"L":4},"(   )":{"O":18,"L":5},"(    )":{"O":23,"L":6}}},"((((((()))))))":{"O":31,"L":14,"S":{"(((((())))))":{"O":32,"L":12,"S":{"((((()))))":{"O":33,"L":10,"S":{"(((())))":{"O":34,"L":8,"S":{"((()))":{"O":35,"L":6,"S":{"(())":{"O":36,"L":4,"S":{"()":{"O":37,"L":2}}}}}}}}}}}}},"(A(B)C(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)U(V)W)":{"O":46,"L":47,"S":{"(B)":{"O":48,"L":3},"(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)":{"O":52,"L":35,"S":{"(E)":{"O":54,"L":3},"(G(H)I(G(K(L)M)N)O(P)Q)":{"O":58,"L":23,"S":{"(H)":{"O":60,"L":3},"(G(K(L)M)N)":{"O":64,"L":11,"S":{"(K(L)M)":{"O":66,"L":7,"S":{"(L)":{"O":68,"L":3}}}}},"(P)":{"O":76,"L":3}}},"(S)":{"O":82,"L":3}}},"(V)":{"O":88,"L":3}}},"(1(2(3(4(5(6(7)8)9)10)10)12)13)":{"O":94,"L":31,"S":{"(2(3(4(5(6(7)8)9)10)10)12)":{"O":96,"L":26,"S":{"(3(4(5(6(7)8)9)10)10)":{"O":98,"L":21,"S":{"(4(5(6(7)8)9)10)":{"O":100,"L":16,"S":{"(5(6(7)8)9)":{"O":102,"L":11,"S":{"(6(7)8)":{"O":104,"L":7,"S":{"(7)":{"O":106,"L":3}}}}}}}}}}}}}})j";
    test.IsEqual(stream, result, __LINE__);
    stream.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////

    content = R"((())";
    find_Engine701("(", 1U, ")", 1U, items, content, 0U, 4);
    to_JSON(stream, items, content);
    test.IsEqual(stream, "", __LINE__);
    stream.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////
}

static int RunEngineTests() {
    QTest test{"Engine.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("Engine Test 1", TestEngine1);
    test.Test("Engine Test 2", TestEngine2);
    test.Test("Engine Test 3", TestEngine3);
    test.Test("Engine Test 4", TestEngine4);
    test.Test("Engine Test 5", TestEngine5);
    test.Test("Engine Test 6", TestEngine6);
    test.Test("Engine Test 6", TestEngine7);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
