/*
 * Copyright (c) 2025 Hani Ammar
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

#ifndef QENTEM_TEST_H
#define QENTEM_TEST_H

// clang-format off
#include "Qentem/QTest.hpp"
#include "MemoryBlockTest.hpp"
#include "ReserverTest.hpp"
#include "StringUtilsTest.hpp"
#include "StringTest.hpp"
#include "StringStreamTest.hpp"
#include "ArrayTest.hpp"
#include "DequeTest.hpp"
#include "BigIntTest.hpp"
#include "DigitTest.hpp"
#include "HArrayTest.hpp"
#include "UnicodeTest.hpp"
#include "JSONUtilsTest.hpp"
#include "ValueTest.hpp"
#include "JSONTest.hpp"
#include "EvaluateTest.hpp"
#include "TemplateTest.hpp"
#include "TemplateLTest.hpp"
#include "TemplateUTest.hpp"
// clang-format on

namespace Qentem {
namespace Test {

static int PrintResult(int passed, int failed) {
    if (failed == 0) {
        QConsole::Print(QConsole::GetColor(QConsole::Color::PassColor), "All good. (", passed, ") tests",
                        QConsole::GetColor(QConsole::Color::EndColor), "\n\n");

        return 0;
    }

    QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Not good!",
                    QConsole::GetColor(QConsole::Color::EndColor), " ", failed, " out of ", (passed + failed),
                    " failed.\n\n");

    return 1;
}

QENTEM_MAYBE_UNUSED
static void SelfTestLeak(QTest &test, char *&ptr) {
    ptr        = Reserver::Reserve<char>(8U);
    char *nptr = nullptr;

    test.IsTrue(false, 1);
    test.IsFalse(true, 2);
    test.IsNull(ptr, 2);
    test.IsNotNull(nptr, 1);
    test.IsEqual(true, false, 4);
    test.IsNotEqual(true, true, 5);
    test.IsNotEqual(u'a', u'a', 7);
    test.IsNotEqual(U'a', U'a', 6);
}

QENTEM_MAYBE_UNUSED
static void SelfTest() {
    QConsole::DisableOutput();

    const bool is_colored = QConsole::IsColored();

    QTest test{"Test.hpp", __FILE__};

    test.ContinueOnError(true);

    char *ptr = nullptr;
    test.Test("SelfTestLeak", SelfTestLeak, true, ptr);
    QTest::PrintMemoryStatus();
    Reserver::Release(ptr, 8U);
    MemoryRecord::EraseSubMemoryRecord();
    QConsole::IsColored() = false;

    test.EndTests();

    if (test.ContinueOnErrorEnabled()) {
        PrintResult(10, 5);
        // const wchar_t *failed_message = L"Not good! 5 out of 15 failed.\n";
    }

    QConsole::IsColored() = is_colored;
    QConsole::GetBuffer().Clear();
    QConsole::EnableOutput();
}

static int RunTests() {
    int passed = 0;
    int failed = 0;

    ((Test::RunMemoryBlockTests() == 0) ? ++passed : ++failed);
    ((Test::RunReserverTests() == 0) ? ++passed : ++failed);
    ((Test::RunStringUtilsTests() == 0) ? ++passed : ++failed);
    ((Test::RunStringTests() == 0) ? ++passed : ++failed);
    ((Test::RunStringStreamTests() == 0) ? ++passed : ++failed);
    ((Test::RunArrayTests() == 0) ? ++passed : ++failed);
    ((Test::RunDequeTests() == 0) ? ++passed : ++failed);
    ((Test::RunBigIntTests() == 0) ? ++passed : ++failed);
    ((Test::RunDigitTests() == 0) ? ++passed : ++failed);
    ((Test::RunHArrayTests() == 0) ? ++passed : ++failed);
    ((Test::RunUnicodeTests() == 0) ? ++passed : ++failed);
    ((Test::RunJSONUtilsTests() == 0) ? ++passed : ++failed);
    ((Test::RunValueTests() == 0) ? ++passed : ++failed);
    ((Test::RunJSONTests() == 0) ? ++passed : ++failed);
    ((Test::RunEvaluateTests() == 0) ? ++passed : ++failed);
    ((Test::RunTemplateTests() == 0) ? ++passed : ++failed);
    ((Test::RunTemplateUTests() == 0) ? ++passed : ++failed);
    ((Test::RunTemplateLTests() == 0) ? ++passed : ++failed);

    return PrintResult(passed, failed);
}

} // namespace Test
} // namespace Qentem

#endif
