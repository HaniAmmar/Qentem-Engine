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
#pragma warning(disable : 4505) // Warning about functions not being used.
#endif

#include "Common.hpp"

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#else
#include <malloc.h>
#endif

#ifndef QENTEM_OUTPUT_STREAM
#include <iostream>
#define QENTEM_OUTPUT_STREAM std::wcout
#endif

#ifndef QENTEM_TEST_HELPER_H_
#define QENTEM_TEST_HELPER_H_

namespace Qentem {

struct TestOutPut {
    TestOutPut()                              = delete;
    ~TestOutPut()                             = delete;
    TestOutPut(TestOutPut &&)                 = delete;
    TestOutPut(const TestOutPut &)            = delete;
    TestOutPut &operator=(TestOutPut &&)      = delete;
    TestOutPut &operator=(const TestOutPut &) = delete;

    template <typename... Values_T_>
    QENTEM_NOINLINE static void Print(const Values_T_ &...values) {
        // (out) may copy the value but at that point it will not matter.
        int dummy[sizeof...(Values_T_)] = {(QENTEM_OUTPUT_STREAM << static_cast<const Values_T_ &>(values), 0)...};
        (void)dummy;
        // (QENTEM_OUTPUT_STREAM << ... << static_cast<const Values_T_ &>(values)); // c++17
    }

    enum Colors { TITLE, ERROR, PASS, END };

    QENTEM_NOINLINE static const char *GetColor(Colors color) noexcept {
        if (coloredOutput_) {
            switch (color) {
                case TITLE:
                    return "\x1B[36m";
                case ERROR:
                    return "\x1B[31m";
                case PASS:
                    return "\x1B[32m";
                case END:
                    return "\x1B[0m";
            }
        }

        return "";
    }

    void static SetColoredOutput(bool value) {
        coloredOutput_ = value;
    }

  private:
    static bool coloredOutput_;
};

bool TestOutPut::coloredOutput_{true};

struct MemoryRecord {
    MemoryRecord()                                = delete;
    ~MemoryRecord()                               = delete;
    MemoryRecord(MemoryRecord &&)                 = delete;
    MemoryRecord(const MemoryRecord &)            = delete;
    MemoryRecord &operator=(MemoryRecord &&)      = delete;
    MemoryRecord &operator=(const MemoryRecord &) = delete;

    struct Records {
        SizeT  allocations{0};
        SizeT  deallocations{0};
        SizeT  subAllocations{0};
        SizeT  subDeallocations{0};
        size_t remainingSize{0};
        size_t peakSize{0};
    };

    QENTEM_NOINLINE static void ResetSubMemory() noexcept {
        records_.subAllocations   = 0;
        records_.subDeallocations = 0;
    }

    QENTEM_NOINLINE static SizeT CheckSubMemory() noexcept {
        return (records_.subAllocations - records_.subDeallocations);
    }

    QENTEM_NOINLINE static void AddAllocation(void *pointer) noexcept {
        ++(records_.allocations);
        ++(records_.subAllocations);

#if defined(_WIN32) || defined(_WIN64)
        records_.remainingSize += _msize(pointer);
#elif defined(__APPLE__)
        records_.remainingSize += malloc_size(pointer);
#else
        records_.remainingSize += malloc_usable_size(pointer);
#endif

        if (records_.remainingSize > records_.peakSize) {
            records_.peakSize = records_.remainingSize;
        }
    }

    QENTEM_NOINLINE static void RemoveAllocation(void *pointer) noexcept {
        ++(records_.deallocations);
        ++(records_.subDeallocations);

#if defined(_WIN32) || defined(_WIN64)
        records_.remainingSize -= _msize(pointer);
#elif defined(__APPLE__)
        records_.remainingSize -= malloc_size(pointer);
#else
        records_.remainingSize -= malloc_usable_size(pointer);
#endif
    }

    QENTEM_NOINLINE static void PrintMemoryStatus() noexcept {
        TestOutPut::Print("\nMemory: ", (static_cast<double>(records_.remainingSize) / 1024),
                          " KB, Peak: ", (static_cast<double>(records_.peakSize) / 1024), " KB.\n");

        TestOutPut::Print("Allocations: ", records_.allocations, ", Deallocations: ", records_.deallocations, ".\n");

        SizeT remaining_deallocations = (records_.allocations - records_.deallocations);

        if (remaining_deallocations != 0) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Leak detected",
                              TestOutPut::GetColor(TestOutPut::Colors::END), ": ", remaining_deallocations,
                              " Remaining deallocations.\n\n");
        }
    }

  private:
    static Records records_;
};

MemoryRecord::Records MemoryRecord::records_{};

struct TestHelper {
    TestHelper()  = delete;
    ~TestHelper() = default;

    QENTEM_NOINLINE
    TestHelper(const char *name, const char *file_fullname) noexcept : test_name_{name}, file_fullname_{file_fullname} {
    }

    QENTEM_NOINLINE void PrintGroupName() const noexcept {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), test_name_,
                          TestOutPut::GetColor(TestOutPut::Colors::END), ":\n");
    }

    QENTEM_NOINLINE int EndTests() noexcept {
        if (!error_) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), test_name_,
                              TestOutPut::GetColor(TestOutPut::Colors::PASS), " Passed all tests",
                              TestOutPut::GetColor(TestOutPut::Colors::END), "\n\n");
            return 0;
        }

        return 1;
    }

    template <typename Char_T_, typename FUNC_T_>
    QENTEM_NOINLINE void Test(Char_T_ *name, FUNC_T_ func) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this);
            afterTest(true);
        }
    }

    template <typename Char_T_, typename FUNC_T_, typename... Values_T_>
    QENTEM_NOINLINE void Test(Char_T_ *name, FUNC_T_ func, bool test_for_leaks, Values_T_ &...values) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this, values...);
            afterTest(test_for_leaks);
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE void EqualsTrue(bool value, const Char_T_ *name, unsigned long line) {
        if ((!error_ || continue_on_error_) && !value) {
            error_ = true;
            TestHelper::PrintErrorMessage1(false, name, line);
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE void EqualsFalse(bool value, const Char_T_ *name, unsigned long line) {
        if ((!error_ || continue_on_error_) && value) {
            error_ = true;
            TestHelper::PrintErrorMessage1(true, name, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_, typename Value3_T_>
    QENTEM_NOINLINE void Equal(const Value1_T_ &left, const Value2_T_ &right, const Char_T_ *name,
                               const Value3_T_ &value, unsigned long line) {
        if ((!error_ || continue_on_error_) && (left != right)) {
            error_ = true;
            TestHelper::PrintErrorMessage2(false, name, left, value, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_, typename Value3_T_>
    QENTEM_NOINLINE void NotEqual(const Value1_T_ &left, const Value2_T_ &right, const Char_T_ *name,
                                  const Value3_T_ &value, unsigned long line) {
        if ((!error_ || continue_on_error_) && (left == right)) {
            error_ = true;
            TestHelper::PrintErrorMessage2(true, name, left, value, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE void Equal(const Value1_T_ &left, const Value2_T_ &right, const Char_T_ *name, unsigned long line) {
        if ((!error_ || continue_on_error_) && (left != right)) {
            error_ = true;
            TestHelper::PrintErrorMessage2(false, name, left, right, line);
        }
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE void NotEqual(const Value1_T_ &left, const Value2_T_ &right, const Char_T_ *name,
                                  unsigned long line) {
        if ((!error_ || continue_on_error_) && (left == right)) {
            error_ = true;
            TestHelper::PrintErrorMessage2(true, name, left, right, line);
        }
    }

    template <typename Char_T_>
    QENTEM_NOINLINE void PrintErrorMessage1(bool equal, const Char_T_ *name, unsigned long line) {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Failed",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ": ", part_name_, '\n', file_fullname_, ":",
                          line, ":\n`", name, "` should", (equal ? " not " : " "), "equal: `true`\n\n");
    }

    template <typename Char_T_, typename Value1_T_, typename Value2_T_>
    QENTEM_NOINLINE void PrintErrorMessage2(bool equal, const Char_T_ *name, const Value1_T_ &value1,
                                            const Value2_T_ &value2, unsigned long line) {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Failed",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ": ", part_name_, '\n', file_fullname_, ":",
                          line, ":\n`", name, "` should", (equal ? " not " : " "), "equal: `", value2,
                          "`\n Returned Value: `", value1, "`\n\n");
    }

    inline void ContinueOnError(bool continue_on_error) noexcept {
        continue_on_error_ = continue_on_error;
    }

    inline bool HasError() const noexcept {
        return error_;
    }

    inline bool IsContinueOnError() const noexcept {
        return continue_on_error_;
    }

    QENTEM_NOINLINE static void PrintInfo() {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), "Configurations",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ":\n");
        if (Config::Is64bit) {
            TestOutPut::Print("Arch: 64Bit\n");

            if (Config::PointerTagging) {
                TestOutPut::Print("Tagged Pointers: On\n");

                if (Config::ShortStringOptimization) {
                    TestOutPut::Print("Short String Optimization: On\n");
                } else {
                    TestOutPut::Print("Short String Optimization: Off\n");
                }
            } else {
                TestOutPut::Print("Tagged Pointers: Off\n");
                TestOutPut::Print("Short String Optimization: Off\n");
            }
        } else {
            TestOutPut::Print("Arch: 32Bit\n");
            TestOutPut::Print("Tagged Pointers: Off\n");
            TestOutPut::Print("Short String Optimization: Off\n");
        }

        if (Config::BigEndian) {
            TestOutPut::Print("Endianness: Big-Endian\n");
        } else {
            TestOutPut::Print("Endianness: Little-Endian\n");
        }

        if (Config::AutoEscapeHTML) {
            TestOutPut::Print("Auto Escape HTML: On\n");
        } else {
            TestOutPut::Print("Auto Escape HTML: Off\n");
        }

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
        TestOutPut::Print("AVX2: On\n");
#endif
#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
        TestOutPut::Print("SSE2: On\n");
#endif

        TestOutPut::Print("SizeT size: ", sizeof(SizeT), "\n\n");
    }

  private:
    QENTEM_NOINLINE void afterTest(bool test_for_leaks) {
        if (!error_) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::PASS), "Pass",
                              TestOutPut::GetColor(TestOutPut::Colors::END), ": ", part_name_, '\n');
        }

        if (test_for_leaks) {
            const SizeT remaining_deallocations = MemoryRecord::CheckSubMemory();
            MemoryRecord::ResetSubMemory();

            if (remaining_deallocations != 0) {
                TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Leak detected",
                                  TestOutPut::GetColor(TestOutPut::Colors::END), ": ", remaining_deallocations,
                                  " Remaining deallocations.\n");
            }
        }
    }

    const char *part_name_;
    const char *test_name_;
    const char *file_fullname_;
    bool        error_{false};
    bool        continue_on_error_{false};
};

} // namespace Qentem

#endif
