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

#include "Common.hpp"

#include <iostream>
#include <sstream>

#ifndef QENTEM_TESTHELPER_H_
#define QENTEM_TESTHELPER_H_

namespace Qentem {
namespace Test {

class TestHelper {
  public:
    QENTEM_NOINLINE static unsigned long &LineNumber() {
        static unsigned long line = 0;

        return line;
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static const Char_T_ *&TestGroupName() {
        static const Char_T_ *name = nullptr;

        return name;
    }

    template <typename Char_T_, typename Value_T_>
    QENTEM_NOINLINE static void
    PrintErrorMessage(bool equal, const Char_T_ *name, Value_T_ value,
                      std::wostream &out = std::wcout) {
        out << "\x1B[31mFailed\x1B[0m: " << TestGroupName<Char_T_>()
            << "\n At line :" << LineNumber() << ": '" << name << "' should"
            << (equal ? " not " : " ") << "equal '" << value << "'\n"
            << std::endl;
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE static void
    PrintErrorMessage(bool equal, const Char_T_ *name, Value1_T_ value1,
                      Value2_T_ value2, std::wostream &out = std::wcout) {
        out << "\x1B[31mFailed\x1B[0m: " << TestGroupName<Char_T_>()
            << "\n At line :" << LineNumber() << ": '" << name << "' should"
            << (equal ? " not " : " ") << "equal '" << value2 << "'\n"
            << "Returned Value: " << value1 << "\n"
            << std::endl;
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static void StartingTest(const Char_T_ *name,
                                             std::wostream &out = std::wcout) {
        out << "\x1B[36m" << name << "\x1B[0m"
            << " Tests:" << std::endl;
    }

    template <typename Char_T_, typename FUNC_>
    QENTEM_NOINLINE static bool StartTest(const Char_T_ *name, FUNC_ func,
                                          std::wostream &out = std::wcout) {
        TestGroupName<Char_T_>() = name;

        try {
            if (func() == 0) {
                out << "\x1B[32mPass\x1B[0m: " << TestGroupName<Char_T_>()
                    << std::endl;
                return true;
            }

        } catch (...) {
            out << "\x1B[31mFailed (throw)\x1B[0m: " << TestGroupName<Char_T_>()
                << "\n after line :" << TestHelper::LineNumber() << '\n'
                << std::endl;
        }

        return false;
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static void EndTest(const Char_T_ *name,
                                        std::wostream &out = std::wcout) {
        out << "\x1B[36m" << name << "\x1B[32m Passed all tests\x1B[0m\n"
            << std::endl;
    }
};

#define STARTING_TEST(name) TestHelper::StartingTest(name)

#define START_TEST(name, func)                                                 \
    do {                                                                       \
        if (!TestHelper::StartTest(name, func)) {                              \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_TO(left, right, name, value)                                        \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) != (right)) {                                               \
            TestHelper::PrintErrorMessage(false, name, left, value);           \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define NOT_EQ_TO(left, right, name, value)                                    \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) == (right)) {                                               \
            TestHelper::PrintErrorMessage(true, name, left, value);            \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_VALUE(left, right, name)                                            \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) != (right)) {                                               \
            TestHelper::PrintErrorMessage(false, name, left, right);           \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define NOT_EQ_VALUE(left, right, name)                                        \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) == (right)) {                                               \
            TestHelper::PrintErrorMessage(true, name, left, right);            \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_TRUE(condition, name)                                               \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if (!(condition)) {                                                    \
            TestHelper::PrintErrorMessage(false, name, "true");                \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_FALSE(condition, name)                                              \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if (condition) {                                                       \
            TestHelper::PrintErrorMessage(true, name, "true");                 \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define END_SUB_TEST return 0

#define END_TEST(name)                                                         \
    do {                                                                       \
        TestHelper::EndTest(name);                                             \
        return 0;                                                              \
    } while (false)

QENTEM_MAYBE_UNUSED
static int TestThrow1_1() { return 1; }

QENTEM_MAYBE_UNUSED
static int TestThrow1() {
    std::wstringstream ss;

    TestHelper::StartTest("Test Throw 1", TestThrow1_1, ss);
    // EQ_VALUE(L"Test Throw 1", ss.str(), "Test Throw 1");

    END_SUB_TEST;
}

QENTEM_MAYBE_UNUSED
static int TestThrow2_2() { throw 1; }

QENTEM_MAYBE_UNUSED
static int TestThrow2() {
    std::wstringstream ss;

    TestHelper::PrintErrorMessage(false, "", 1, 0, ss);

    EQ_VALUE(TestHelper::StartTest("Test Throw 2", TestThrow2_2, ss), false,
             "Test Throw 2");

    END_SUB_TEST;
}

QENTEM_MAYBE_UNUSED
static int RunTestHelperTests() {
    STARTING_TEST("TestHelper.hpp");

    START_TEST("Throw Test 1", TestThrow1);
    START_TEST("Throw Test 2", TestThrow2);

    END_TEST("TestHelper.hpp");
}

} // namespace Test
} // namespace Qentem

#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505)
#endif
