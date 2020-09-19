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

#include "JSONUtils.hpp"
#include "TestHelper.hpp"

#ifndef QENTEM_JSONUTILS_TESTS_H_
#define QENTEM_JSONUTILS_TESTS_H_

namespace Qentem {
namespace Test {

static int TestEscapeJSON() {
    const char *       str;
    StringStream<char> ss;
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
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\")", "ss");
    ss.SoftReset();

    str = "\\";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\\)", "ss");
    ss.SoftReset();

    str = "/";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\/)", "ss");
    ss.SoftReset();

    str = "\b";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\b)", "ss");
    ss.SoftReset();

    str = "\f";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\f)", "ss");
    ss.SoftReset();

    str = "\n";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\n)", "ss");
    ss.SoftReset();

    str = "\r";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\r)", "ss");
    ss.SoftReset();

    str = "\t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t)", "ss");
    ss.SoftReset();

    str = "\"\"";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\"\")", "ss");
    ss.SoftReset();

    str = "\\\\";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\\\\)", "ss");
    ss.SoftReset();

    str = "//";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\/\/)", "ss");
    ss.SoftReset();

    str = "\b\b";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\b\b)", "ss");
    ss.SoftReset();

    str = "\f\f";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\f\f)", "ss");
    ss.SoftReset();

    str = "\n\n";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\n\n)", "ss");
    ss.SoftReset();

    str = "\r\r";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\r\r)", "ss");
    ss.SoftReset();

    str = "\t\t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t\t)", "ss");
    ss.SoftReset();

    str = R"(""")";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\"\"\")", "ss");
    ss.SoftReset();

    str = R"(\\\)";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\\\\\\)", "ss");
    ss.SoftReset();

    str = "///";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\/\/\/)", "ss");
    ss.SoftReset();

    str = "\b\b\b";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\b\b\b)", "ss");
    ss.SoftReset();

    str = "\f\f\f";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\f\f\f)", "ss");
    ss.SoftReset();

    str = "\n\n\n";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\n\n\n)", "ss");
    ss.SoftReset();

    str = "\r\r\r";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\r\r\r)", "ss");
    ss.SoftReset();

    str = "\t\t\t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t\t\t)", "ss");
    ss.SoftReset();

    str = " \"";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \")", "ss");
    ss.SoftReset();

    str = " \\";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \\)", "ss");
    ss.SoftReset();

    str = " /";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \/)", "ss");
    ss.SoftReset();

    str = " \b";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \b)", "ss");
    ss.SoftReset();

    str = " \f";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \f)", "ss");
    ss.SoftReset();

    str = " \n";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \n)", "ss");
    ss.SoftReset();

    str = " \r";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \r)", "ss");
    ss.SoftReset();

    str = " \t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \t)", "ss");
    ss.SoftReset();

    str = "\" ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\" )", "ss");
    ss.SoftReset();

    str = "\\ ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\\ )", "ss");
    ss.SoftReset();

    str = "/ ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\/ )", "ss");
    ss.SoftReset();

    str = "\b ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\b )", "ss");
    ss.SoftReset();

    str = "\f ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\f )", "ss");
    ss.SoftReset();

    str = "\n ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\n )", "ss");
    ss.SoftReset();

    str = "\r ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\r )", "ss");
    ss.SoftReset();

    str = "\t ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t )", "ss");
    ss.SoftReset();

    str = " \" ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \" )", "ss");
    ss.SoftReset();

    str = " \\ ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \\ )", "ss");
    ss.SoftReset();

    str = " / ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \/ )", "ss");
    ss.SoftReset();

    str = " \b ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \b )", "ss");
    ss.SoftReset();

    str = " \f ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \f )", "ss");
    ss.SoftReset();

    str = " \n ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \n )", "ss");
    ss.SoftReset();

    str = " \r ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \r )", "ss");
    ss.SoftReset();

    str = " \t ";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"( \t )", "ss");
    ss.SoftReset();

    str = R"(" ")";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\" \")", "ss");
    ss.SoftReset();

    str = R"(\ \)";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\\ \\)", "ss");
    ss.SoftReset();

    str = "/ /";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\/ \/)", "ss");
    ss.SoftReset();

    str = "\b \b";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\b \b)", "ss");
    ss.SoftReset();

    str = "\f \f";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\f \f)", "ss");
    ss.SoftReset();

    str = "\n \n";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\n \n)", "ss");
    ss.SoftReset();

    str = "\r \r";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\r \r)", "ss");
    ss.SoftReset();

    str = "\t \t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t \t)", "ss");
    ss.SoftReset();

    str = "\"\\/\b\f\n\r\t";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\"\\\/\b\f\n\r\t)", "ss");
    ss.SoftReset();

    str = "\t\r\n\f\b/\\\"";
    JSON::EscapeJSON(str, StringUtils::Count(str), ss);
    SHOULD_EQUAL_VALUE(ss, R"(\t\r\n\f\b\/\\\")", "ss");
    ss.SoftReset();

    END_SUB_TEST;
}

static int TestUnEscapeJSON1() {
    const char *str1;
    const char *str2;
    char *      str3;
    ULong       len;

    str1 = R"(\")";
    str2 = "\"";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\\)";
    str2 = "\\";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\/)";
    str2 = "/";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\b)";
    str2 = "\b";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\f)";
    str2 = "\f";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\n)";
    str2 = "\n";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\r)";
    str2 = "\r";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t)";
    str2 = "\t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\"\")";
    str2 = "\"\"";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\\\\)";
    str2 = R"(\\)";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\/\/)";
    str2 = "//";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\b\b)";
    str2 = "\b\b";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\f\f)";
    str2 = "\f\f";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\n\n)";
    str2 = "\n\n";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\r\r)";
    str2 = "\r\r";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t\t)";
    str2 = "\t\t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\"\"\")";
    str2 = R"(""")";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\\\\\\)";
    str2 = R"(\\\)";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\/\/\/)";
    str2 = "///";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\b\b\b)";
    str2 = "\b\b\b";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\f\f\f)";
    str2 = "\f\f\f";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\n\n\n)";
    str2 = "\n\n\n";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\r\r\r)";
    str2 = "\r\r\r";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t\t\t)";
    str2 = "\t\t\t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \")";
    str2 = " \"";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \\)";
    str2 = " \\";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \/)";
    str2 = " /";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \b)";
    str2 = " \b";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \f)";
    str2 = " \f";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \n)";
    str2 = " \n";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \r)";
    str2 = " \r";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \t)";
    str2 = " \t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\" )";
    str2 = "\" ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\\ )";
    str2 = "\\ ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\/ )";
    str2 = "/ ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\b )";
    str2 = "\b ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\f )";
    str2 = "\f ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\n )";
    str2 = "\n ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\r )";
    str2 = "\r ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t )";
    str2 = "\t ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \" )";
    str2 = " \" ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \\ )";
    str2 = " \\ ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \/ )";
    str2 = " / ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \b )";
    str2 = " \b ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \f )";
    str2 = " \f ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \n )";
    str2 = " \n ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \r )";
    str2 = " \r ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"( \t )";
    str2 = " \t ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    END_SUB_TEST;
}

static int TestUnEscapeJSON2() {
    const char *str1;
    const char *str2;
    char *      str3;
    ULong       len;

    str1 = R"(\" \")";
    str2 = "\" \"";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\\ \\)";
    str2 = "\\ \\";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\/ \/)";
    str2 = "/ /";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\b \b)";
    str2 = "\b \b";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\f \f)";
    str2 = "\f \f";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\n \n)";
    str2 = "\n \n";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\r \r)";
    str2 = "\r \r";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t \t)";
    str2 = "\t \t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\"\\\/\b\f\n\r\t)";
    str2 = "\"\\/\b\f\n\r\t";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\t\r\n\f\b\/\\\")";
    str2 = "\t\r\n\f\b/\\\"";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\u003D)";
    str2 = "=";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\u00a1)";
    str2 = "¡";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\u08A7)";
    str2 = "ࢧ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\ud802\uDE7B)";
    str2 = "𐩻";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\uD83E\uFC59)";
    str2 = "🡙";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    str1 = R"(\UD800\UDE83W\U003DW\UD800\UDE83\U00A1\UD83E\UFC59\U08A7)";
    str2 = "𐊃W=W𐊃¡🡙ࢧ";
    len  = StringUtils::Count(str1);
    str3 = JSON::UnEscapeJSON(str1, len);
    SHOULD_EQUAL_TRUE(Memory::IsEqual(str2, str3, len), "str2 == str3");
    HAllocator::Deallocate(str3);

    END_SUB_TEST;
}

static int RunJSONUtilsTests() {
    STARTING_TEST("JSONUtils.hpp");

    START_TEST("EscapeJSON Test", TestEscapeJSON);
    START_TEST("UnEscapeJSON Test 1", TestUnEscapeJSON1);
    START_TEST("UnEscapeJSON Test 2", TestUnEscapeJSON2);

    END_TEST("JSONUtils.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
