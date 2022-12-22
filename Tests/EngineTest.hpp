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

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 1, "return");

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "AAAAA";
    content_len = 5;

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 2, "return");

    ret = Engine::FindOne(*find_, content, content_len, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 4, "return");

    ret = Engine::FindOne(*find_, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::FindOne(*find_, content, SizeT{5}, content_len);
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

    ret = Engine::FindOne(*find_, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::FindOne(*find_, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::FindOne(*find_, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 5, "return");

    ret = Engine::FindOne(*find_, content, SizeT{5}, content_len);
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

    ret = Engine::FindOne('1', "00000001", 0u, 8u);
    EQ_VALUE(ret, 8u, "return");

    ret = Engine::FindOne('1', "0000000000000001", 0u, 16u);
    EQ_VALUE(ret, 16u, "return");

    ret = Engine::FindOne('1', "00000000000000000000000000000001", 0u, 32u);
    EQ_VALUE(ret, 32u, "return");

    ret = Engine::FindOne('1', "0000000000000000000000000000000000000000000000000000000000000001", 0u, 64u);
    EQ_VALUE(ret, 64u, "return");

    END_SUB_TEST;
}

static int TestEngine3() {
    SizeT       ret;
    SizeT       content_len = 0;
    const char *content     = "";

    ret = Engine::SkipInnerPatterns(char(0), char(0), content, SizeT{0}, SizeT{0});
    EQ_VALUE(ret, 0, "return");

    content     = "   (  ";
    content_len = 6;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "((((((((";
    content_len = 8;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{6}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{7}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{8}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = "()";
    content_len = 2;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 2, "return");

    content     = " ()";
    content_len = 3;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "return");

    content     = " ()             ";
    content_len = 16;

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{2}, content_len);
    EQ_VALUE(ret, 3, "return");

    ret = Engine::SkipInnerPatterns('(', ')', content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "return");

    END_SUB_TEST;
}

static int TestEngine4() {
    SizeT       ret;
    SizeT       content_len = 12;
    const char *content     = "{{{{{{}}}}}}";

    ret = Engine::FindOne('}', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 7, "FindOne");

    ret = Engine::SkipInnerPatterns('x', 'y', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 12, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, content_len);
    EQ_VALUE(ret, 11, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, content_len);
    EQ_VALUE(ret, 9, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, content_len);
    EQ_VALUE(ret, 8, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, content_len);
    EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, content_len);
    EQ_VALUE(ret, 8, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{8}, content_len);
    EQ_VALUE(ret, 9, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{9}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{10}, content_len);
    EQ_VALUE(ret, 11, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{11}, content_len);
    EQ_VALUE(ret, 12, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{12}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{13}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{14}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{100}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{7});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{8});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{9});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{10});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{11});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "{}";
    content_len = 2;
    ret         = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 2, "SkipInnerPatterns()");

    content     = "{   }";
    content_len = 5;
    ret         = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 5, "SkipInnerPatterns()");

    content     = "{{{}{{}{}{}{}}}}{}";
    content_len = 18;

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, content_len);
    EQ_VALUE(ret, 16, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, content_len);
    EQ_VALUE(ret, 15, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, content_len);
    EQ_VALUE(ret, 4, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, content_len);
    EQ_VALUE(ret, 15, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, content_len);
    EQ_VALUE(ret, 14, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, content_len);
    EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, content_len);
    EQ_VALUE(ret, 14, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{8}, content_len);
    EQ_VALUE(ret, 9, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{9}, content_len);
    EQ_VALUE(ret, 14, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{10}, content_len);
    EQ_VALUE(ret, 11, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{11}, content_len);
    EQ_VALUE(ret, 14, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{12}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{13}, content_len);
    EQ_VALUE(ret, 14, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{14}, content_len);
    EQ_VALUE(ret, 15, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{15}, content_len);
    EQ_VALUE(ret, 16, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{100}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{14});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{13});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, SizeT{12});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{3}, SizeT{11});
    EQ_VALUE(ret, 4, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{4}, SizeT{10});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{5}, SizeT{9});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{6}, SizeT{8});
    EQ_VALUE(ret, 7, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{7}, SizeT{7});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{6});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{5});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{2}, SizeT{4});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{3});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{1}, SizeT{2});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns('{', '}', content, SizeT{0}, SizeT{1});
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    END_SUB_TEST;
}

static int TestEngine5() {
    SizeT       ret;
    SizeT       content_len = 18;
    const char *content     = "       )          ";

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = " ()             ";
    content_len = 16;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "(())";
    content_len = 4;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 4, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 4, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "      (())";
    content_len = 10;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "      (()) ";
    content_len = 11;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 10, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{11}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{12}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{13}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{500}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "( (())";
    content_len = 6;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 6, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 6, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "( ( ( ( ( ( (())";
    content_len = 16;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{6}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{7}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{9}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{10}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{11}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{12}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{13}, content_len);
    EQ_VALUE(ret, 16, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{14}, content_len);
    EQ_VALUE(ret, 16, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{15}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{16}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{500}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    content     = "(()";
    content_len = 3;

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("((", SizeT{2}, "))", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    END_SUB_TEST;
}

static int TestEngine6() {
    SizeT           ret;
    constexpr SizeT content_len = 50;
    const char     *content     = "{-{-{-     -}{-{-  -}{-   -}{- -}{- -}-}-}-}{- -}";

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{0}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{1}, content_len);
    EQ_VALUE(ret, 44, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{2}, content_len);
    EQ_VALUE(ret, 44, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{3}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{4}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{5}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{6}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{7}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{8}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{9}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{10}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{11}, content_len);
    EQ_VALUE(ret, 13, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{12}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{13}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{14}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{15}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{16}, content_len);
    EQ_VALUE(ret, 21, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{17}, content_len);
    EQ_VALUE(ret, 21, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{18}, content_len);
    EQ_VALUE(ret, 21, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{19}, content_len);
    EQ_VALUE(ret, 21, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{20}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{21}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{22}, content_len);
    EQ_VALUE(ret, 28, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{23}, content_len);
    EQ_VALUE(ret, 28, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{24}, content_len);
    EQ_VALUE(ret, 28, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{25}, content_len);
    EQ_VALUE(ret, 28, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{26}, content_len);
    EQ_VALUE(ret, 28, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{27}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{28}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{29}, content_len);
    EQ_VALUE(ret, 33, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{30}, content_len);
    EQ_VALUE(ret, 33, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{31}, content_len);
    EQ_VALUE(ret, 33, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{32}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{33}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{34}, content_len);
    EQ_VALUE(ret, 38, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{35}, content_len);
    EQ_VALUE(ret, 38, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{36}, content_len);
    EQ_VALUE(ret, 38, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{37}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{38}, content_len);
    EQ_VALUE(ret, 40, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{39}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{40}, content_len);
    EQ_VALUE(ret, 42, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{41}, content_len);
    EQ_VALUE(ret, 44, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{42}, content_len);
    EQ_VALUE(ret, 44, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{43}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{45}, content_len);
    EQ_VALUE(ret, 49, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{46}, content_len);
    EQ_VALUE(ret, 49, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{47}, content_len);
    EQ_VALUE(ret, 49, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{48}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{49}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    ret = Engine::SkipInnerPatterns("{-", SizeT{2}, "-}", SizeT{2}, content, SizeT{50}, content_len);
    EQ_VALUE(ret, 0, "SkipInnerPatterns()");

    END_SUB_TEST;
}

struct Item2__ {
    SizeT          Offset{0};
    SizeT          Length{0};
    Array<Item2__> SubItems;
};

static void to_JSON(StringStream<char> &ss, const Array<Item2__> &items, const char *content) {
    if (items.IsEmpty()) {
        return;
    }

    ss += '{';

    for (SizeT i = 0; i < items.Size(); i++) {
        ss += '"';
        ss.Insert((content + items.Storage()[i].Offset), items.Storage()[i].Length);
        ss += "\":{";

        ss += "\"O\":";
        ss += Digit<char>::NumberToString(items.Storage()[i].Offset);
        ss += ",\"L\":";
        ss += Digit<char>::NumberToString(items.Storage()[i].Length);

        if (items.Storage()[i].SubItems.IsNotEmpty()) {
            ss += ",\"S\":";
            to_JSON(ss, items.Storage()[i].SubItems, content);
        }

        ss += '}';
        ss += ',';
    }

    ss.StepBack(1);

    ss += '}';
}

void find_Engine701(const char *l_tag, SizeT l_tag_len, const char *r_tag, SizeT r_tag_len, Array<Item2__> &items,
                    const char *content, SizeT offset, SizeT stop_offset) {
    const SizeT l_r_len      = (l_tag_len + r_tag_len);
    SizeT       e_tag_offset = offset;
    SizeT       s_tag_offset;

    while (offset < stop_offset) {
        if (l_tag_len == 1) {
            s_tag_offset = Engine::FindOne(l_tag[0], content, e_tag_offset, stop_offset);
        } else {
            s_tag_offset = Engine::Find(l_tag, l_tag_len, content, e_tag_offset, stop_offset);
        }

        if (s_tag_offset == 0) {
            return;
        }

        if (l_tag_len == 1) {
            e_tag_offset = Engine::SkipInnerPatterns(l_tag[0], r_tag[0], content, s_tag_offset, stop_offset);
        } else {
            e_tag_offset =
                Engine::SkipInnerPatterns(l_tag, l_tag_len, r_tag, r_tag_len, content, s_tag_offset, stop_offset);
        }

        if (e_tag_offset == 0) {
            return;
        }

        Item2__ item;
        item.Offset = (s_tag_offset - l_tag_len);
        item.Length = (e_tag_offset - item.Offset);

        if ((item.Length - l_r_len) >= l_r_len) {
            find_Engine701(l_tag, l_tag_len, r_tag, r_tag_len, item.SubItems, content, s_tag_offset,
                           (e_tag_offset - r_tag_len));
        }

        items += static_cast<Item2__ &&>(item);

        offset = e_tag_offset;
    }
}

static int TestEngine7() {
    Array<Item2__>     items;
    StringStream<char> ss;
    const char        *content;
    const char        *result;
    /////////////////////////////////////////////////////////////////////////////////////////

    content = "[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]] [[[]]] [[[[[[]]]]]] [[[-]]]    [[[X]]]";
    find_Engine701("[[[", 3, "]]]", 3, items, content, 0, 75);
    to_JSON(ss, items, content);
    result =
        R"({"[[[ [[[  [[[  [[[  ]]]  ]]]  ]]] ]]]":{"O":0,"L":36,"S":{"[[[  [[[  [[[  ]]]  ]]]  ]]]":{"O":4,"L":28,"S":{"[[[  [[[  ]]]  ]]]":{"O":9,"L":18,"S":{"[[[  ]]]":{"O":14,"L":8}}}}}}},"[[[]]]":{"O":37,"L":6},"[[[[[[]]]]]]":{"O":44,"L":12,"S":{"[[[]]]":{"O":47,"L":6}}},"[[[-]]]":{"O":57,"L":7},"[[[X]]]":{"O":68,"L":7}})";
    EQ_VALUE(ss, result, "stringify");
    ss.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////

    content =
        R"((....) () (( )(  )(   )(    )) ((((((())))))) (A(B)C(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)U(V)W) (1(2(3(4(5(6(7)8)9)10)10)12)13))";
    find_Engine701("(", 1, ")", 1, items, content, 0, 125);
    to_JSON(ss, items, content);
    result =
        R"j({"(....)":{"O":0,"L":6},"()":{"O":7,"L":2},"(( )(  )(   )(    ))":{"O":10,"L":20,"S":{"( )":{"O":11,"L":3},"(  )":{"O":14,"L":4},"(   )":{"O":18,"L":5},"(    )":{"O":23,"L":6}}},"((((((()))))))":{"O":31,"L":14,"S":{"(((((())))))":{"O":32,"L":12,"S":{"((((()))))":{"O":33,"L":10,"S":{"(((())))":{"O":34,"L":8,"S":{"((()))":{"O":35,"L":6,"S":{"(())":{"O":36,"L":4,"S":{"()":{"O":37,"L":2}}}}}}}}}}}}},"(A(B)C(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)U(V)W)":{"O":46,"L":47,"S":{"(B)":{"O":48,"L":3},"(D(E)F(G(H)I(G(K(L)M)N)O(P)Q)R(S)T)":{"O":52,"L":35,"S":{"(E)":{"O":54,"L":3},"(G(H)I(G(K(L)M)N)O(P)Q)":{"O":58,"L":23,"S":{"(H)":{"O":60,"L":3},"(G(K(L)M)N)":{"O":64,"L":11,"S":{"(K(L)M)":{"O":66,"L":7,"S":{"(L)":{"O":68,"L":3}}}}},"(P)":{"O":76,"L":3}}},"(S)":{"O":82,"L":3}}},"(V)":{"O":88,"L":3}}},"(1(2(3(4(5(6(7)8)9)10)10)12)13)":{"O":94,"L":31,"S":{"(2(3(4(5(6(7)8)9)10)10)12)":{"O":96,"L":26,"S":{"(3(4(5(6(7)8)9)10)10)":{"O":98,"L":21,"S":{"(4(5(6(7)8)9)10)":{"O":100,"L":16,"S":{"(5(6(7)8)9)":{"O":102,"L":11,"S":{"(6(7)8)":{"O":104,"L":7,"S":{"(7)":{"O":106,"L":3}}}}}}}}}}}}}})j";
    EQ_VALUE(ss, result, "stringify");
    ss.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////

    content = R"((())";
    find_Engine701("(", 1, ")", 1, items, content, 0, 4);
    to_JSON(ss, items, content);
    EQ_VALUE(ss, "", "stringify");
    ss.Clear();
    items.Clear();

    /////////////////////////////////////////////////////////////////////////////////////////
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
    START_TEST("Engine Test 6", TestEngine7);

    END_TEST("Engine.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
