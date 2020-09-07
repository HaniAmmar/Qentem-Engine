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

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#endif

#include <iostream>
#include <sstream>

#ifndef QENTEM_TESTHELPER_H_
#define QENTEM_TESTHELPER_H_

namespace Qentem {

class TestHelper {
  public:
    QENTEM_NOINLINE static unsigned long &LineNumber() {
        static unsigned long line = 0;

        return line;
    }

    template <typename Value_>
    QENTEM_NOINLINE static void
    PrintErrorMessage(bool equal, const char *name, Value_ value,
                      std::ostream &out = std::cout) {
        out << ":\x1B[31m Failed\x1B[0m\n"
            << "At line " << LineNumber() << ": '" << name << "' should"
            << (equal ? " not " : " ") << "equal '" << value << "'\n"
            << std::endl;
    }

    QENTEM_NOINLINE static void StartingTest(const char *  name,
                                             std::ostream &out = std::cout) {
        out << "\x1B[36m" << name << "\x1B[0m"
            << " Tests:" << std::endl;
    }

    template <typename FUNC_>
    QENTEM_NOINLINE static bool StartTest(const char *name, FUNC_ func,
                                          std::ostream &out = std::cout) {
        try {
            out << name;

            if (func() != 0) {
                return false;
            }

            out << ":\x1B[32m Passed\x1B[0m" << std::endl;
        } catch (...) {
            out << ":\x1B[31m Failed to pass (throw)\x1B[0m"
                << " after line: " << TestHelper::LineNumber() << '\n'
                << std::endl;
            return false;
        }

        return true;
    }

    QENTEM_NOINLINE static void EndTest(const char *  name,
                                        std::ostream &out = std::cout) {
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

#define SHOULD_EQUAL(left, right, name, value)                                 \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) != (right)) {                                               \
            TestHelper::PrintErrorMessage(false, name, value);                 \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define SHOULD_EQUAL_VALUE(left, right, name)                                  \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) != (right)) {                                               \
            TestHelper::PrintErrorMessage(false, name, right);                 \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define SHOULD_NOT_EQUAL(left, right, name, value)                             \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) == (right)) {                                               \
            TestHelper::PrintErrorMessage(true, name, value);                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define SHOULD_NOT_EQUAL_VALUE(left, right, name)                              \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((left) == (right)) {                                               \
            TestHelper::PrintErrorMessage(true, name, right);                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define SHOULD_EQUAL_TRUE(condition, name)                                     \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if (!(condition)) {                                                    \
            TestHelper::PrintErrorMessage(false, name, "true");                \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define SHOULD_NOT_EQUAL_TRUE(condition, name)                                 \
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

namespace Test {

static int TestThrow1_1() {
    return 1;
}

static int TestThrow1() {
    std::stringstream ss;

    TestHelper::StartTest("Test Throw 1", TestThrow1_1, ss);
    SHOULD_EQUAL_VALUE(ss.str(), "Test Throw 1", "Test Throw 1");

    END_SUB_TEST;
}

static int TestThrow2_2() {
    throw 1;
}

static int TestThrow2() {
    std::stringstream ss;

    TestHelper::PrintErrorMessage(false, "", 0, ss);

    SHOULD_EQUAL_VALUE(TestHelper::StartTest("Test Throw 2", TestThrow2_2, ss),
                       false, "Test Throw 2");

    END_SUB_TEST;
}

static int RunTestHelperTests() {
    STARTING_TEST("TestHelper.hpp");

    START_TEST("Test Throw 1", TestThrow1);
    START_TEST("Test Throw 2", TestThrow2);

    END_TEST("TestHelper.hpp");
}

} // namespace Test
} // namespace Qentem

#endif
