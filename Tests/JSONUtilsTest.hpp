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

#include "QTest.hpp"
#include "JSONUtils.hpp"
#include "StringStream.hpp"

#ifndef QENTEM_JSON_UTILS_TESTS_H
#define QENTEM_JSON_UTILS_TESTS_H

namespace Qentem {
namespace Test {

static void escapeJSON(const char *str, StringStream<char> &buffer) {
    JSONUtils::EscapeJSON(str, StringUtils::Count(str), buffer);
}

static void TestEscapeJSON(QTest &test) {
    const char        *str;
    StringStream<char> buffer;
    /*
     * "
     * \
     * /
     * \b
     * \f
     * \n
     * \r
     * \t
     */

    str = "\"";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\")", __LINE__);
    buffer.Clear();

    str = "\\";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\\)", __LINE__);
    buffer.Clear();

    str = "/";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\/)", __LINE__);
    buffer.Clear();

    str = "\b";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\b)", __LINE__);
    buffer.Clear();

    str = "\f";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\f)", __LINE__);
    buffer.Clear();

    str = "\n";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\n)", __LINE__);
    buffer.Clear();

    str = "\r";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\r)", __LINE__);
    buffer.Clear();

    str = "\t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t)", __LINE__);
    buffer.Clear();

    str = "\"\"";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\"\")", __LINE__);
    buffer.Clear();

    str = "\\\\";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\\\\)", __LINE__);
    buffer.Clear();

    str = "//";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\/\/)", __LINE__);
    buffer.Clear();

    str = "\b\b";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\b\b)", __LINE__);
    buffer.Clear();

    str = "\f\f";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\f\f)", __LINE__);
    buffer.Clear();

    str = "\n\n";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\n\n)", __LINE__);
    buffer.Clear();

    str = "\r\r";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\r\r)", __LINE__);
    buffer.Clear();

    str = "\t\t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t\t)", __LINE__);
    buffer.Clear();

    str = R"(""")";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\"\"\")", __LINE__);
    buffer.Clear();

    str = R"(\\\)";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\\\\\\)", __LINE__);
    buffer.Clear();

    str = "///";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\/\/\/)", __LINE__);
    buffer.Clear();

    str = "\b\b\b";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\b\b\b)", __LINE__);
    buffer.Clear();

    str = "\f\f\f";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\f\f\f)", __LINE__);
    buffer.Clear();

    str = "\n\n\n";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\n\n\n)", __LINE__);
    buffer.Clear();

    str = "\r\r\r";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\r\r\r)", __LINE__);
    buffer.Clear();

    str = "\t\t\t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t\t\t)", __LINE__);
    buffer.Clear();

    str = " \"";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \")", __LINE__);
    buffer.Clear();

    str = " \\";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \\)", __LINE__);
    buffer.Clear();

    str = " /";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \/)", __LINE__);
    buffer.Clear();

    str = " \b";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \b)", __LINE__);
    buffer.Clear();

    str = " \f";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \f)", __LINE__);
    buffer.Clear();

    str = " \n";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \n)", __LINE__);
    buffer.Clear();

    str = " \r";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \r)", __LINE__);
    buffer.Clear();

    str = " \t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \t)", __LINE__);
    buffer.Clear();

    str = "\" ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\" )", __LINE__);
    buffer.Clear();

    str = "\\ ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\\ )", __LINE__);
    buffer.Clear();

    str = "/ ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\/ )", __LINE__);
    buffer.Clear();

    str = "\b ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\b )", __LINE__);
    buffer.Clear();

    str = "\f ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\f )", __LINE__);
    buffer.Clear();

    str = "\n ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\n )", __LINE__);
    buffer.Clear();

    str = "\r ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\r )", __LINE__);
    buffer.Clear();

    str = "\t ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t )", __LINE__);
    buffer.Clear();

    str = " \" ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \" )", __LINE__);
    buffer.Clear();

    str = " \\ ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \\ )", __LINE__);
    buffer.Clear();

    str = " / ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \/ )", __LINE__);
    buffer.Clear();

    str = " \b ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \b )", __LINE__);
    buffer.Clear();

    str = " \f ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \f )", __LINE__);
    buffer.Clear();

    str = " \n ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \n )", __LINE__);
    buffer.Clear();

    str = " \r ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \r )", __LINE__);
    buffer.Clear();

    str = " \t ";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"( \t )", __LINE__);
    buffer.Clear();

    str = R"(" ")";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\" \")", __LINE__);
    buffer.Clear();

    str = R"(\ \)";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\\ \\)", __LINE__);
    buffer.Clear();

    str = "/ /";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\/ \/)", __LINE__);
    buffer.Clear();

    str = "\b \b";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\b \b)", __LINE__);
    buffer.Clear();

    str = "\f \f";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\f \f)", __LINE__);
    buffer.Clear();

    str = "\n \n";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\n \n)", __LINE__);
    buffer.Clear();

    str = "\r \r";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\r \r)", __LINE__);
    buffer.Clear();

    str = "\t \t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t \t)", __LINE__);
    buffer.Clear();

    str = "\"\\/\b\f\n\r\t";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\"\\\/\b\f\n\r\t)", __LINE__);
    buffer.Clear();

    str = "\t\r\n\f\b/\\\"";
    escapeJSON(str, buffer);
    test.IsEqual(buffer, R"(\t\r\n\f\b\/\\\")", __LINE__);
    buffer.Clear();
}

static void TestUnEscapeJSON1(QTest &test) {
    StringStream<char> buffer;
    const char        *str1;
    const char        *str2;
    SizeT              len1;
    SizeT              len2;

    str1 = R"(\")";
    str2 = "\"";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\\)";
    str2 = "\\";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\/)";
    str2 = "/";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\b)";
    str2 = "\b";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\f)";
    str2 = "\f";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\n)";
    str2 = "\n";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\r)";
    str2 = "\r";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t)";
    str2 = "\t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\"\")";
    str2 = "\"\"";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\\\\)";
    str2 = R"(\\)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\/\/)";
    str2 = "//";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\b\b)";
    str2 = "\b\b";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\f\f)";
    str2 = "\f\f";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\n\n)";
    str2 = "\n\n";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\r\r)";
    str2 = "\r\r";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t\t)";
    str2 = "\t\t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\"\"\")";
    str2 = R"(""")";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\\\\\\)";
    str2 = R"(\\\)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\/\/\/)";
    str2 = "///";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\b\b\b)";
    str2 = "\b\b\b";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\f\f\f)";
    str2 = "\f\f\f";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\n\n\n)";
    str2 = "\n\n\n";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\r\r\r)";
    str2 = "\r\r\r";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t\t\t)";
    str2 = "\t\t\t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \")";
    str2 = " \"";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \\)";
    str2 = " \\";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \/)";
    str2 = " /";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \b)";
    str2 = " \b";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \f)";
    str2 = " \f";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \n)";
    str2 = " \n";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \r)";
    str2 = " \r";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \t)";
    str2 = " \t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\" )";
    str2 = "\" ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\\ )";
    str2 = "\\ ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\/ )";
    str2 = "/ ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\b )";
    str2 = "\b ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\f )";
    str2 = "\f ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();
}

static void TestUnEscapeJSON2(QTest &test) {
    StringStream<char> buffer;
    const char        *str1;
    const char        *str2;
    SizeT              len1;
    SizeT              len2;

    str1 = R"(\n )";
    str2 = "\n ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\r )";
    str2 = "\r ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t )";
    str2 = "\t ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \" )";
    str2 = " \" ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \\ )";
    str2 = " \\ ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \/ )";
    str2 = " / ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \b )";
    str2 = " \b ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \f )";
    str2 = " \f ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \n )";
    str2 = " \n ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \r )";
    str2 = " \r ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"( \t )";
    str2 = " \t ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\" \")";
    str2 = "\" \"";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\\ \\)";
    str2 = "\\ \\";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\/ \/)";
    str2 = "/ /";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\b \b)";
    str2 = "\b \b";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\f \f)";
    str2 = "\f \f";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\n \n)";
    str2 = "\n \n";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\r \r)";
    str2 = "\r \r";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t \t)";
    str2 = "\t \t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\"\\\/\b\f\n\r\t)";
    str2 = "\"\\/\b\f\n\r\t";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\t\r\n\f\b\/\\\")";
    str2 = "\t\r\n\f\b/\\\"";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\u003D)";
    str2 = "=";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\u00a1)";
    str2 = "¡";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\u08A7)";
    str2 = "ࢧ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\ud802\uDE7B)";
    str2 = "𐩻";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\uD83E\uFC59)";
    str2 = "🡙";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7)";
    str2 = "𐊃W=W𐊃¡🡙ࢧ";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = "";
    str2 = "";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = "abcd";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsEqual(buffer.Length(), 0U, __LINE__);
    buffer.Clear();

    str1 = R"(abcd\"efgh)";
    str2 = R"(abcd"efgh)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(abcd\"efgh\"1234)";
    str2 = R"(abcd"efgh"1234)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(abcd\""asd)";
    str2 = R"(abcd")";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, 7U, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(a\"bcd"asd)";
    str2 = R"(a"bcd)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, 7U, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(")";
    str2 = R"()";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, len1, __LINE__);
    test.IsTrue((buffer == str2), __LINE__);
    buffer.Clear();

    str1 = R"(\uD83E\uFC5)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, 0U, __LINE__);
    buffer.Clear();

    str1 = R"(\u00a)";
    len1 = StringUtils::Count(str1);
    len2 = JSONUtils::UnEscapeJSON(str1, len1, buffer);
    test.IsEqual(len2, 0U, __LINE__);
}

static int RunJSONUtilsTests() {
    QTest test{"JSONUtils.hpp", __FILE__};

    test.PrintGroupName();

    test.Test("EscapeJSON Test", TestEscapeJSON);
    test.Test("UnEscapeJSON Test 1", TestUnEscapeJSON1);
    test.Test("UnEscapeJSON Test 2", TestUnEscapeJSON2);

    return test.EndTests();
}

} // namespace Test
} // namespace Qentem

#endif
