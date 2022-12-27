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

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#else
#include <malloc.h>
#endif

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

class TestHelper {
  public:
    template <typename Char_T_>
    static const Char_T_ *&TestGroupName() noexcept {
        static const Char_T_ *name = nullptr;
        return name;
    }

    template <typename Stream_T_, typename Char_T_, typename Value_T_>
    QENTEM_NOINLINE static void PrintErrorMessage1(Stream_T_ &stream, bool equal, const Char_T_ *name,
                                                   const Value_T_ &value, const char *file, unsigned long line) {
        // (out <<) will copy the value, but at that point, it will not matter.
        stream << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed" << QENTEM_OUTPUT_END_COLOR << ": "
               << TestGroupName<Char_T_>() << "\n  " << file << ":" << line << ": `" << name << "` should"
               << (equal ? " not " : " ") << "equal `" << value << "`\n\n";
    }

    template <typename Stream_T_, typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE static void PrintErrorMessage2(Stream_T_ &stream, bool equal, const Char_T_ *name,
                                                   const Value1_T_ &value1, const Value2_T_ &value2, const char *file,
                                                   unsigned long line) {
        stream << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed" << QENTEM_OUTPUT_END_COLOR << ": "
               << TestGroupName<Char_T_>() << "\n  " << file << ":" << line << ": `" << name << "` should"
               << (equal ? " not " : " ") << "equal `" << value2 << "`\n Returned Value: `" << value1 << "`\n\n";
    }

    template <typename Stream_T_, typename Char_T_>
    QENTEM_NOINLINE static void StartingTest(Stream_T_ &stream, const Char_T_ *name) {
        stream << QENTEM_OUTPUT_START_COLOR_MAIN << name << QENTEM_OUTPUT_END_COLOR << " Tests:\n";
    }

    template <typename Stream_T_, typename Char_T_, typename FUNC_>
    QENTEM_NOINLINE static bool StartTest(Stream_T_ &stream, const Char_T_ *name, FUNC_ func) {
        TestGroupName<Char_T_>() = name;

        if (func() == 0) {
            stream << QENTEM_OUTPUT_START_COLOR_PASS << "Pass" << QENTEM_OUTPUT_END_COLOR << ": "
                   << TestGroupName<Char_T_>() << '\n';
            return true;
        }

        return false;
    }

    template <typename Stream_T_, typename Char_T_>
    QENTEM_NOINLINE static void EndTest(Stream_T_ &stream, const Char_T_ *name) {
        stream << QENTEM_OUTPUT_START_COLOR_MAIN << name << QENTEM_OUTPUT_START_COLOR_PASS << " Passed all tests"
               << QENTEM_OUTPUT_END_COLOR << "\n\n";
    }

    static SizeT &AllocateHit() noexcept {
        static SizeT count = 0;
        return count;
    }

    static SizeT &DeallocateHit() noexcept {
        static SizeT count = 0;
        return count;
    }

    static void AddMemorySize(void *pointer) noexcept {
        size_t &m_size = GetMemorySize();
        size_t &m_peak = GetMemoryPeak();

        ++(AllocateHit());

#if defined(_WIN32) || defined(_WIN64)
        m_size += _msize(pointer);
#elif defined(__APPLE__)
        m_size += malloc_size(pointer);
#else
        m_size += malloc_usable_size(pointer);
#endif

        if (m_size > m_peak) {
            m_peak = m_size;
        }
    }

    static void RemoveMemorySize(void *pointer) noexcept {
        size_t &m_size = GetMemorySize();

        ++(DeallocateHit());

#if defined(_WIN32) || defined(_WIN64)
        m_size -= _msize(pointer);
#elif defined(__APPLE__)
        m_size -= malloc_size(pointer);
#else
        m_size -= malloc_usable_size(pointer);
#endif
    }

    static size_t &GetMemorySize() noexcept {
        static size_t size = 0;
        return size;
    }

    static size_t &GetMemoryPeak() noexcept {
        static size_t peak = 0;
        return peak;
    }

    static void PrintMemoryStatus() {
        QENTEM_OUTPUT_STREAM << "\nMemory: " << (static_cast<double>(GetMemorySize()) / 1024)
                             << " KB, Peak: " << (static_cast<double>(GetMemoryPeak()) / 1024) << " KB\n";
        QENTEM_OUTPUT_STREAM << "Allocations: " << AllocateHit() << ", Deallocations: " << DeallocateHit() << ".";

        SizeT remain = (AllocateHit() - DeallocateHit());

        if (remain != 0) {
            QENTEM_OUTPUT_STREAM << " " << QENTEM_OUTPUT_START_COLOR_ERROR << (AllocateHit() - DeallocateHit())
                                 << " not deallocated." << QENTEM_OUTPUT_END_COLOR;
        }

        QENTEM_OUTPUT_STREAM << '\n';
    }
};

#define STARTING_TEST(name) TestHelper::StartingTest(QENTEM_OUTPUT_STREAM, name)

#define START_TEST(name, func)                                                                                         \
    do {                                                                                                               \
        if (!(TestHelper::StartTest(QENTEM_OUTPUT_STREAM, name, func))) {                                              \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define EQ_TO(left, right, name, value)                                                                                \
    do {                                                                                                               \
        if ((left) != (right)) {                                                                                       \
            TestHelper::PrintErrorMessage2(QENTEM_OUTPUT_STREAM, false, name, left, value, __FILE__, __LINE__);        \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define NOT_EQ_TO(left, right, name, value)                                                                            \
    do {                                                                                                               \
        if ((left) == (right)) {                                                                                       \
            TestHelper::PrintErrorMessage2(QENTEM_OUTPUT_STREAM, true, name, left, value, __FILE__, __LINE__);         \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define EQ_VALUE(left, right, name)                                                                                    \
    do {                                                                                                               \
        if ((left) != (right)) {                                                                                       \
            TestHelper::PrintErrorMessage2(QENTEM_OUTPUT_STREAM, false, name, left, right, __FILE__, __LINE__);        \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define NOT_EQ_VALUE(left, right, name)                                                                                \
    do {                                                                                                               \
        if ((left) == (right)) {                                                                                       \
            TestHelper::PrintErrorMessage2(QENTEM_OUTPUT_STREAM, true, name, left, right, __FILE__, __LINE__);         \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define EQ_TRUE(condition, name)                                                                                       \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            TestHelper::PrintErrorMessage1(QENTEM_OUTPUT_STREAM, false, name, "true", __FILE__, __LINE__);             \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define EQ_FALSE(condition, name)                                                                                      \
    do {                                                                                                               \
        if (condition) {                                                                                               \
            TestHelper::PrintErrorMessage1(QENTEM_OUTPUT_STREAM, true, name, "true", __FILE__, __LINE__);              \
            return 1;                                                                                                  \
        }                                                                                                              \
    } while (false)

#define END_SUB_TEST return 0

#define END_TEST(name)                                                                                                 \
    do {                                                                                                               \
        TestHelper::EndTest(QENTEM_OUTPUT_STREAM, name);                                                               \
        return 0;                                                                                                      \
    } while (false)

class EmptyStream {
    template <typename Stream_T_, typename Value_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const Value_T_ &val) noexcept {
        (void)val;
        return out;
    }

    template <typename Stream_T_>
    friend Stream_T_ &operator<<(Stream_T_ &out, const void *val) noexcept {
        (void)val;
        return out;
    }
};

QENTEM_MAYBE_UNUSED
static int TestError_1() noexcept { return 1; }

QENTEM_MAYBE_UNUSED
static int TestError() {
    EmptyStream   stream;
    constexpr int n = 0;
    constexpr int m = 1;

    TestHelper::PrintErrorMessage1(stream, false, "", n, __FILE__, __LINE__);
    TestHelper::PrintErrorMessage2(stream, false, "", n, m, __FILE__, __LINE__);

    EQ_FALSE(TestHelper::StartTest(stream, "Test StartTest()", TestError_1), "StartTest()");

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
