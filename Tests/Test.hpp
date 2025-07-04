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
#include "QTest.hpp"
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
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::PASS), "All good. (", passed, ") tests",
                          TestOutput::GetColor(TestOutput::Colors::END), "\n\n");
        return 0;
    }

    TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ERROR), "Not good!",
                      TestOutput::GetColor(TestOutput::Colors::END), " ", failed, " out of ", (passed + failed),
                      " failed.\n\n");

    return 1;
}

static void SelfTestLeak(QTest &test, char *&ptr) {
    ptr        = Memory::Allocate<char>(8U);
    char *nptr = nullptr;

    test.IsTrue(false, 1);
    test.IsFalse(true, 2);
    test.IsNull(ptr, 2);
    test.IsNotNull(nptr, 1);
    test.IsEqual(true, false, 4);
    test.IsNotEqual(true, true, 5);
    test.IsNotEqual(u'a', u'a', 7);
    test.IsNotEqual(U'a', U'a', 6);

    TestOutput::Print(u"a");
    TestOutput::Print(U"a");
}

static void SelfTest() {
    TestOutput::DisableOutput();

    const bool is_colored = TestOutput::IsColored();

    QTest test{"Test.hpp", __FILE__};

    test.ContinueOnError(true);

    char *ptr = nullptr;
    test.Test("SelfTestLeak", SelfTestLeak, true, ptr);
    Qentem::MemoryRecord::PrintMemoryStatus();
    Memory::Deallocate(ptr);
    MemoryRecord::ResetSubMemoryRecord();
    TestOutput::IsColored() = false;

    test.EndTests();

    if (test.ContinueOnErrorEnabled()) {
        PrintResult(10, 5);
        // const wchar_t *failed_message = L"Not good! 5 out of 15 failed.\n";
    }

    TestOutput::IsColored() = is_colored;
    TestOutput::EnableOutput();
    TestOutput::GetStreamCache().Clear();
}

static int RunTests() {
    int passed = 0;
    int failed = 0;

    ((Qentem::Test::RunStringUtilsTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringStreamTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunArrayTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunDequeTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunBigIntTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunDigitTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunHArrayTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunUnicodeTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunJSONUtilsTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunValueTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunJSONTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunEvaluateTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunTemplateTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunTemplateUTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunTemplateLTests() == 0) ? ++passed : ++failed);

    return PrintResult(passed, failed);
}

} // namespace Test
} // namespace Qentem

#endif
