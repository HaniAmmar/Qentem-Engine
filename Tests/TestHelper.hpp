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

#ifndef QENTEM_ENABLE_COLORED_OUTPUT
#define QENTEM_ENABLE_COLORED_OUTPUT 1
#endif

#ifndef QENTEM_OUTPUT_STREAM
#include <iostream>
#define QENTEM_OUTPUT_STREAM_TYPE std::wostream
#define QENTEM_OUTPUT_STREAM std::wcout
#endif

#if defined(QENTEM_ENABLE_COLORED_OUTPUT) && (QENTEM_ENABLE_COLORED_OUTPUT == 1)
#define QENTEM_OUTPUT_START_COLOR_MAIN "\x1B[36m"
#define QENTEM_OUTPUT_START_COLOR_ERROR "\x1B[31m"
#define QENTEM_OUTPUT_START_COLOR_PASS "\x1B[32m"
#define QENTEM_OUTPUT_END_COLOR "\x1B[0m"
#else
#define QENTEM_OUTPUT_START_COLOR_MAIN ""
#define QENTEM_OUTPUT_START_COLOR_ERROR ""
#define QENTEM_OUTPUT_START_COLOR_PASS ""
#define QENTEM_OUTPUT_END_COLOR ""
#endif

#ifndef QENTEM_TESTHELPER_H_
#define QENTEM_TESTHELPER_H_

namespace Qentem {
namespace Test {

template <typename Stream_T_>
class TestHelper_T {
  public:
    static void Init() {
        if (GetStream() == nullptr) {
            SetStream(QENTEM_OUTPUT_STREAM);
        }
    }

    static Stream_T_ &Stream() { return (*GetStream()); }

    static Stream_T_ *&GetStream() {
        static Stream_T_ *out_stream = nullptr;
        return out_stream;
    }

    static void SetStream(Stream_T_ &out) { GetStream() = &out; }

    static unsigned long &LineNumber() {
        static unsigned long line = 0;

        return line;
    }

    template <typename Char_T_>
    static const Char_T_ *&TestGroupName() {
        static const Char_T_ *name = nullptr;

        return name;
    }

    template <typename Char_T_, typename Value_T_>
    QENTEM_NOINLINE static void
    PrintErrorMessage1(bool equal, const Char_T_ *name, const Value_T_ &value) {
        Stream() << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed"
                 << QENTEM_OUTPUT_END_COLOR << ": " << TestGroupName<Char_T_>()
                 << "\n At line :" << LineNumber() << ": `" << name
                 << "` should" << (equal ? " not " : " ") << "equal `" << value
                 << "`\n\n";
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE static void
    PrintErrorMessage2(bool equal, const Char_T_ *name, const Value1_T_ &value1,
                       const Value2_T_ &value2) {
        Stream() << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed"
                 << QENTEM_OUTPUT_END_COLOR << ": " << TestGroupName<Char_T_>()
                 << "\n At line :" << LineNumber() << ": `" << name
                 << "` should" << (equal ? " not " : " ") << "equal `" << value2
                 << "`\n Returned Value: `" << value1 << "`\n\n";
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static void StartingTest(const Char_T_ *name) {
        Stream() << QENTEM_OUTPUT_START_COLOR_MAIN << name
                 << QENTEM_OUTPUT_END_COLOR << " Tests:\n";
    }

    template <typename Char_T_, typename FUNC_>
    QENTEM_NOINLINE static bool StartTest(const Char_T_ *name, FUNC_ func) {
        TestGroupName<Char_T_>() = name;

        if (func() == 0) {
            Stream() << QENTEM_OUTPUT_START_COLOR_PASS << "Pass"
                     << QENTEM_OUTPUT_END_COLOR << ": "
                     << TestGroupName<Char_T_>() << '\n';
            return true;
        }

        return false;
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static void EndTest(const Char_T_ *name) {
        Stream() << QENTEM_OUTPUT_START_COLOR_MAIN << name
                 << QENTEM_OUTPUT_START_COLOR_PASS << " Passed all tests"
                 << QENTEM_OUTPUT_END_COLOR << "\n\n";
    }
};

using TestHelper = TestHelper_T<QENTEM_OUTPUT_STREAM_TYPE>;

#define STARTING_TEST(name) TestHelper::StartingTest(name)

#define START_TEST(name, func)                                                 \
    do {                                                                       \
        if (!TestHelper::StartTest(name, func)) {                              \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_TO(left, right, name, value)                                        \
    do {                                                                       \
        const auto &_tmp_left    = left;                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((_tmp_left) != (right)) {                                          \
            TestHelper::PrintErrorMessage2(false, name, _tmp_left, value);     \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define NOT_EQ_TO(left, right, name, value)                                    \
    do {                                                                       \
        const auto &_tmp_left    = left;                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((_tmp_left) == (right)) {                                          \
            TestHelper::PrintErrorMessage2(true, name, _tmp_left, value);      \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_VALUE(left, right, name)                                            \
    do {                                                                       \
        const auto &_tmp_left    = left;                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((_tmp_left) != (right)) {                                          \
            TestHelper::PrintErrorMessage2(false, name, _tmp_left, right);     \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define NOT_EQ_VALUE(left, right, name)                                        \
    do {                                                                       \
        const auto &_tmp_left    = left;                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if ((_tmp_left) == (right)) {                                          \
            TestHelper::PrintErrorMessage2(true, name, _tmp_left, right);      \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_TRUE(condition, name)                                               \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if (!(condition)) {                                                    \
            TestHelper::PrintErrorMessage1(false, name, "true");               \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define EQ_FALSE(condition, name)                                              \
    do {                                                                       \
        TestHelper::LineNumber() = __LINE__;                                   \
        if (condition) {                                                       \
            TestHelper::PrintErrorMessage1(true, name, "true");                \
            return 1;                                                          \
        }                                                                      \
    } while (false)

#define END_SUB_TEST return 0

#define END_TEST(name)                                                         \
    do {                                                                       \
        TestHelper::EndTest(name);                                             \
        return 0;                                                              \
    } while (false)

class EmptyStream {
    template <typename Stream_T_, typename Value_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const Value_T_ &val) {
        (void)val;
        return out;
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const void *val) {
        (void)val;
        return out;
    }
};

QENTEM_MAYBE_UNUSED
static int TestError_1() { return 1; }

QENTEM_MAYBE_UNUSED
static int TestError() {
    using TestHelperEmptyStream = TestHelper_T<EmptyStream>;
    EmptyStream es;
    int         n = 0;
    int         m = 1;

    TestHelperEmptyStream::SetStream(es);
    TestHelperEmptyStream::PrintErrorMessage1(false, "", n);
    TestHelperEmptyStream::PrintErrorMessage2(false, "", n, m);

    EQ_FALSE(TestHelperEmptyStream::StartTest("Test StartTest()", TestError_1),
             "StartTest()");

    END_SUB_TEST;
}

QENTEM_MAYBE_UNUSED
static int RunTestHelperTests() {
    STARTING_TEST("TestHelper.hpp");

    START_TEST("Test Error", TestError);

    END_TEST("TestHelper.hpp");
}

} // namespace Test
} // namespace Qentem

#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505)
#endif
