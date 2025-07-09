/**
 * @file QTest.hpp
 * @brief Provides a lightweight unit testing framework for Qentem Engine.
 *
 * This header defines core macros and utilities for writing and running test cases
 * within the Qentem codebase. QTest.hpp is designed to be simple, and fast,
 * enabling efficient automated testing and validation of Qentem Engine
 * components and features.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_Q_TEST_H
#define QENTEM_Q_TEST_H

#include "ToCharsHelper.hpp"
#include "LiteStream.hpp"

#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#else
#include <malloc.h>
#endif

#ifndef QENTEM_ALLOCATE
#if defined(_MSC_VER)
#define QENTEM_ALLOCATE(size) malloc(size)
#define QENTEM_DEALLOCATE(ptr) free(ptr)
#define QENTEM_RAW_ALLOCATE(size) malloc(size)
#define QENTEM_RAW_DEALLOCATE(ptr) free(ptr)
#else
#define QENTEM_ALLOCATE(size) __builtin_malloc(size)
#define QENTEM_DEALLOCATE(ptr) __builtin_free(ptr)
#define QENTEM_RAW_ALLOCATE(size) __builtin_malloc(size)
#define QENTEM_RAW_DEALLOCATE(ptr) __builtin_free(ptr)
#endif

#endif // QENTEM_ALLOCATE

namespace Qentem {

struct TestOutput {
    TestOutput()                              = delete;
    ~TestOutput()                             = delete;
    TestOutput(TestOutput &&)                 = delete;
    TestOutput(const TestOutput &)            = delete;
    TestOutput &operator=(TestOutput &&)      = delete;
    TestOutput &operator=(const TestOutput &) = delete;

    enum struct Colors : SizeT8 { TitleColor, ErrorColor, PassColor, EndColor };

    static bool &IsColored() noexcept {
        static bool isColored{true};

        return isColored;
    }

    QENTEM_NOINLINE static const char *GetColor(Colors color) noexcept {
        if (IsColored()) {
            switch (color) {
                case Colors::TitleColor:
                    return "\x1B[36m";
                case Colors::ErrorColor:
                    return "\x1B[31m";
                case Colors::PassColor:
                    return "\x1B[32m";
                case Colors::EndColor:
                    return "\x1B[0m";
            }
        }

        return "";
    }

    template <typename... Values_T>
    inline static void Print(const Values_T &...values) {
        LiteStream &ss = GetStreamCache();
        ToCharsHelper::Write(ss, values...);

        if (IsOutputEnabled()) {
            fwrite(ss.First(), 1, ss.Length(), stdout);
            ss.Clear();
        }
    }

    static void Flush() noexcept {
        fflush(stdout);
    }

    static void Clear() noexcept {
        GetStreamCache().Clear();
    }

    static void DisableOutput() noexcept {
        getOutputEnabledRef() = false;
    }

    static void EnableOutput() noexcept {
        getOutputEnabledRef() = true;
    }

    static bool IsOutputEnabled() noexcept {
        return getOutputEnabledRef();
    }

    // static void SetDoubleFormat() noexcept {
    // }

    // static void ResetDoubleFormat() noexcept {
    // }

    static LiteStream &GetStreamCache() noexcept {
        static LiteStream ss{32};

        return ss;
    }

  private:
    static bool &getOutputEnabledRef() noexcept {
        static bool enable_output_ = true;

        return enable_output_;
    }
};

struct MemoryRecordData {
    SizeT         allocations{0};
    SizeT         deallocations{0};
    SizeT         subAllocations{0};
    SizeT         subDeallocations{0};
    SystemIntType remainingSize{0};
    SystemIntType peakSize{0};
};

struct MemoryRecord {
    MemoryRecord()                                = delete;
    ~MemoryRecord()                               = delete;
    MemoryRecord(MemoryRecord &&)                 = delete;
    MemoryRecord(const MemoryRecord &)            = delete;
    MemoryRecord &operator=(MemoryRecord &&)      = delete;
    MemoryRecord &operator=(const MemoryRecord &) = delete;

    inline static void ResetMemoryRecord() noexcept {
        GetRecord() = MemoryRecordData{};
    }

    inline static void ResetSubMemoryRecord() noexcept {
        static MemoryRecordData &storage = GetRecord();

        storage.subAllocations   = 0;
        storage.subDeallocations = 0;
    }

    inline static SizeT CheckSubAllocationCount() noexcept {
        static const MemoryRecordData &storage = GetRecord();

        return (storage.subAllocations - storage.subDeallocations);
    }

    inline static void AddAllocation(void *pointer) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.allocations);
        ++(storage.subAllocations);

#if defined(_WIN32) || defined(_M_X64)
        storage.remainingSize += _msize(pointer);
#elif defined(__APPLE__)
        storage.remainingSize += malloc_size(pointer);
#else
        storage.remainingSize += malloc_usable_size(pointer);
#endif

        if (storage.remainingSize > storage.peakSize) {
            storage.peakSize = storage.remainingSize;
        }
    }

    QENTEM_NOINLINE static void RemoveAllocation(void *pointer) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.deallocations);
        ++(storage.subDeallocations);

#if defined(_WIN32) || defined(_M_X64)
        storage.remainingSize -= _msize(pointer);
#elif defined(__APPLE__)
        storage.remainingSize -= malloc_size(pointer);
#else
        storage.remainingSize -= malloc_usable_size(pointer);
#endif
    }

    QENTEM_NOINLINE static void PrintMemoryStatus() {
        static const MemoryRecordData &storage = GetRecord();

        // TestOutput::SetDoubleFormat();

        TestOutput::Print("Memory: ", (double(storage.remainingSize) / 1024),
                          " KB, Peak: ", (double(storage.peakSize) / 1024), " KB.\n");

        TestOutput::Print("Allocations: ", storage.allocations, ", Deallocations: ", storage.deallocations, ".\n");

        const SizeT remaining_allocations = (storage.allocations - storage.deallocations);

        if (remaining_allocations != 0) {
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ErrorColor), "Leak detected",
                              TestOutput::GetColor(TestOutput::Colors::EndColor), ": ", remaining_allocations,
                              " remaining allocations.\n\n");
        }

        // TestOutput::ResetDoubleFormat();
    }

    QENTEM_NOINLINE static void PrintMemoryRecord() {
        static const MemoryRecordData &storage = GetRecord();

        // TestOutput::SetDoubleFormat();

        TestOutput::Print("Memory: ", (double(storage.remainingSize) / 1024),
                          " KB, Peak: ", (double(storage.peakSize) / 1024), " KB.\n");

        TestOutput::Print("Allocations: ", storage.allocations, ", Deallocations: ", storage.deallocations, ".\n");

        // TestOutput::ResetDoubleFormat();
    }

    static MemoryRecordData &GetRecord() noexcept {
        static MemoryRecordData data{};

        return data;
    }
};

struct QTest {
    QTest()                         = delete;
    ~QTest()                        = default;
    QTest(QTest &&)                 = delete;
    QTest(const QTest &)            = delete;
    QTest &operator=(QTest &&)      = delete;
    QTest &operator=(const QTest &) = delete;

    QTest(const char *name, const char *file_fullname) noexcept : test_name_{name}, file_fullname_{file_fullname} {
    }

    QENTEM_NOINLINE void PrintGroupName() const {
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TitleColor), test_name_,
                          TestOutput::GetColor(TestOutput::Colors::EndColor), ":\n");
    }

    QENTEM_NOINLINE int EndTests() {
        if (!error_) {
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TitleColor), test_name_,
                              TestOutput::GetColor(TestOutput::Colors::PassColor), " Passed all tests",
                              TestOutput::GetColor(TestOutput::Colors::EndColor), "\n\n");
            return 0;
        }

        return 1;
    }

    template <typename Char_T, typename FUNC_T>
    QENTEM_NOINLINE void Test(Char_T *name, FUNC_T func) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this);
            afterTest(true);
        }
    }

    template <typename Char_T, typename FUNC_T, typename... Values_T>
    QENTEM_NOINLINE void Test(Char_T *name, FUNC_T func, bool test_for_leaks, Values_T &...values) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this, values...);
            afterTest(test_for_leaks);
        }
    }

    QENTEM_NOINLINE void IsTrue(bool value, unsigned long line) {
        if ((!error_ || continue_on_error_) && !value) {
            error_ = true;
            QTest::PrintErrorMessage(false, "false", "true", line);
        }
    }

    QENTEM_NOINLINE void IsFalse(bool value, unsigned long line) {
        if ((!error_ || continue_on_error_) && value) {
            error_ = true;
            QTest::PrintErrorMessage(false, "true", "false", line);
        }
    }

    QENTEM_NOINLINE void IsNull(const void *value, unsigned long line) {
        if ((!error_ || continue_on_error_) && (value != nullptr)) {
            error_ = true;
            QTest::PrintErrorMessage(false, value, "null", line);
        }
    }

    QENTEM_NOINLINE void IsNotNull(const void *value, unsigned long line) {
        if ((!error_ || continue_on_error_) && (value == nullptr)) {
            error_ = true;
            QTest::PrintErrorMessage(true, "null", "null", line);
        }
    }

    template <typename Value1_T, typename Value2_T>
    QENTEM_NOINLINE void IsEqual(const Value1_T &left, const Value2_T &right, unsigned long line) {
        if ((!error_ || continue_on_error_) && (left != right)) {
            error_ = true;
            QTest::PrintErrorMessage(false, left, right, line);
        }
    }

    template <typename Value1_T, typename Value2_T>
    QENTEM_NOINLINE void IsNotEqual(const Value1_T &left, const Value2_T &right, unsigned long line) {
        if ((!error_ || continue_on_error_) && (left == right)) {
            error_ = true;
            QTest::PrintErrorMessage(true, left, right, line);
        }
    }

    template <typename Value1_T, typename Value2_T>
    QENTEM_NOINLINE void PrintErrorMessage(bool equal, const Value1_T &value1, const Value2_T &value2,
                                           unsigned long line) {
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ErrorColor), "Failed",
                          TestOutput::GetColor(TestOutput::Colors::EndColor), ": ", part_name_, '\n');
        TestOutput::Print(file_fullname_, ":", line, ":\n Should", (equal ? " not " : " "), "equal: `", value2,
                          "`\n     Returned: `", value1, "`\n\n");
    }

    inline void ContinueOnError(bool continue_on_error) noexcept {
        continue_on_error_ = continue_on_error;
    }

    inline bool HasError() const noexcept {
        return error_;
    }

    inline bool ContinueOnErrorEnabled() const noexcept {
        return continue_on_error_;
    }

    QENTEM_NOINLINE static void PrintInfo(bool template_engine_info = true) {
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TitleColor), "Configurations",
                          TestOutput::GetColor(TestOutput::Colors::EndColor), ":\n");
        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit) {
            TestOutput::Print("Arch: 64-bit\n");
        } else {
            TestOutput::Print("Arch: 32-bit\n");
        }

        TestOutput::Print("SizeT: ", sizeof(SizeT), " bytes:\n");

        if QENTEM_CONST_EXPRESSION (QentemConfig::IsBigEndian) {
            TestOutput::Print("Endianness: Big-Endian\n");
        } else {
            TestOutput::Print("Endianness: Little-Endian\n");
        }
#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
        TestOutput::Print("Advanced Vector Extensions: On\n");
#endif

#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
        TestOutput::Print("Streaming SIMD Extensions 2: On\n");
#endif

#if defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
        TestOutput::Print("WASM SIMD128: On\n");
#endif

        if QENTEM_CONST_EXPRESSION (QentemConfig::AutoEscapeHTML) {
            if (template_engine_info) {
                TestOutput::Print("Auto Escape HTML: On\n");
            }
        }

        TestOutput::Print('\n');
    }

  private:
    QENTEM_NOINLINE void afterTest(bool test_for_leaks) {
        if (!error_) {
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::PassColor), "Pass",
                              TestOutput::GetColor(TestOutput::Colors::EndColor), ": ", part_name_, '\n');
        }

        if (test_for_leaks) {
            const SizeT remaining_allocations = MemoryRecord::CheckSubAllocationCount();
            MemoryRecord::ResetSubMemoryRecord();

            if (remaining_allocations != 0) {
                TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ErrorColor), "Leak detected",
                                  TestOutput::GetColor(TestOutput::Colors::EndColor), ": ", remaining_allocations,
                                  " remaining allocations.\n");
            }
        }
    }

    const char *part_name_{nullptr};
    const char *test_name_;
    const char *file_fullname_;
    bool        error_{false};
    bool        continue_on_error_{false};
};

} // namespace Qentem

#endif
