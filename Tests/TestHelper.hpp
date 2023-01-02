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

struct TestHelper {
    TestHelper()  = delete;
    ~TestHelper() = default;

    QENTEM_NOINLINE TestHelper(const char *name, const char *file_fullname) noexcept
        : test_name_{name}, file_fullname_{file_fullname} {}

    QENTEM_NOINLINE void PrintGroupName() const noexcept {
        QENTEM_OUTPUT_STREAM << QENTEM_OUTPUT_START_COLOR_MAIN << test_name_ << QENTEM_OUTPUT_END_COLOR << ":\n";
    }

    QENTEM_NOINLINE int EndTests() noexcept {
        if (!error_) {
            QENTEM_OUTPUT_STREAM << QENTEM_OUTPUT_START_COLOR_MAIN << test_name_ << QENTEM_OUTPUT_START_COLOR_PASS
                                 << " Passed all tests" << QENTEM_OUTPUT_END_COLOR << "\n\n";
            return 0;
        }

        return 1;
    }

    template <typename FUNC_>
    QENTEM_NOINLINE void Test(const char *name, FUNC_ func) {
        if (!error_) {
            func(*this);

            if (!error_) {
                QENTEM_OUTPUT_STREAM << QENTEM_OUTPUT_START_COLOR_PASS << "Pass" << QENTEM_OUTPUT_END_COLOR << ": "
                                     << name << '\n';
            }
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE void EqualsTrue(bool value, const Char_T_ *name, unsigned long line) {
        if (!value) {
            error_ = true;
            TestHelper::PrintErrorMessage1(false, name, file_fullname_, line);
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE void EqualsFalse(bool value, const Char_T_ *name, unsigned long line) {
        if (value) {
            error_ = true;
            TestHelper::PrintErrorMessage1(true, name, file_fullname_, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_, typename Value3_T_>
    QENTEM_NOINLINE void Equal(Value1_T_ left, Value2_T_ right, const Char_T_ *name, Value3_T_ value,
                               unsigned long line) {
        if (left != right) {
            error_ = true;
            TestHelper::PrintErrorMessage2(false, name, left, value, file_fullname_, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_, typename Value3_T_>
    QENTEM_NOINLINE void NotEqual(Value1_T_ left, Value2_T_ right, const Char_T_ *name, Value3_T_ value,
                                  unsigned long line) {
        if (left == right) {
            error_ = true;
            TestHelper::PrintErrorMessage2(true, name, left, value, file_fullname_, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE void Equal(Value1_T_ left, Value2_T_ right, const Char_T_ *name, unsigned long line) {
        if (left != right) {
            error_ = true;
            TestHelper::PrintErrorMessage2(false, name, left, right, file_fullname_, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE void NotEqual(Value1_T_ left, Value2_T_ right, const Char_T_ *name, unsigned long line) {
        if (left == right) {
            error_ = true;
            TestHelper::PrintErrorMessage2(true, name, left, right, file_fullname_, line);
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE static void PrintErrorMessage1(bool equal, const Char_T_ *name, const char *file,
                                                   unsigned long line) {
        QENTEM_OUTPUT_STREAM << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed" << QENTEM_OUTPUT_END_COLOR << ": " << file
                             << ":" << line << ":\n`" << name << "` should" << (equal ? " not " : " ")
                             << "equal `true`\n\n";
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE static void PrintErrorMessage2(bool equal, const Char_T_ *name, const Value1_T_ &value1,
                                                   const Value2_T_ &value2, const char *file, unsigned long line) {
        // (out <<) will copy the value1 and value2, but at that point, it will not matter.
        QENTEM_OUTPUT_STREAM << QENTEM_OUTPUT_START_COLOR_ERROR << "Failed" << QENTEM_OUTPUT_END_COLOR << ": " << file
                             << ":" << line << ":\n`" << name << "` should" << (equal ? " not " : " ") << "equal `"
                             << value2 << "`\nReturned Value: `" << value1 << "`\n\n";
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
        QENTEM_OUTPUT_STREAM << " " << (AllocateHit() - DeallocateHit()) << " remaining.\n";
    }

  private:
    const char *test_name_;
    const char *file_fullname_;
    bool        error_{false};
};

// class EmptyStream {
//     template <typename Value_T_>
//     friend Stream_T_ &operator<<(Stream_T_ &out, const Value_T_ &val) noexcept {
//         (void)val;
//         return out;
//     }

//     template <typename Stream_T_>
//     friend Stream_T_ &operator<<(Stream_T_ &out, const void *val) noexcept {
//         (void)val;
//         return out;
//     }
// };

namespace Test {
QENTEM_MAYBE_UNUSED
void TestError(TestHelper &helper) {
    (void)helper;
    //////////////////////////////
}

QENTEM_MAYBE_UNUSED
static int RunTestHelperTests() {
    // EmptyStream stream;
    TestHelper helper{"TestHelper.hpp", __FILE__};

    helper.PrintGroupName();

    helper.Test("Test Error", TestError);

    helper.EndTests();

    return 0;
}

} // namespace Test
} // namespace Qentem

#endif

#ifdef _MSC_VER
#pragma warning(disable : 4505)
#endif
