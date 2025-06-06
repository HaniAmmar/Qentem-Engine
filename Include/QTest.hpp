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

#include <iostream>
#include <sstream>

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#else
#include <malloc.h>
#endif

#include "QCommon.hpp"

namespace Qentem {

#if (defined(_MSVC_LANG) && (_MSVC_LANG > 201703L)) || (defined(__cplusplus) && (__cplusplus > 201703L))
QENTEM_MAYBE_UNUSED
static std::wostream &operator<<(std::wostream &ss, const char16_t *str) {
    if (str != nullptr) {
        while (*str != char16_t{'\0'}) {
            // it's fine for PrintErrorMessage().
            ss << char(*str);
            ++str;
        }
    }

    return ss;
}

QENTEM_MAYBE_UNUSED
static std::wostream &operator<<(std::wostream &ss, const char16_t ch) {
    // it's fine for PrintErrorMessage().
    ss << char(ch);

    return ss;
}

QENTEM_MAYBE_UNUSED
static std::wostream &operator<<(std::wostream &ss, const char32_t *str) {
    if (str != nullptr) {
        while (*str != char16_t{'\0'}) {
            // it's fine for PrintErrorMessage().
            ss << char(*str);
            ++str;
        }
    }

    return ss;
}

QENTEM_MAYBE_UNUSED
static std::wostream &operator<<(std::wostream &ss, const char32_t ch) {
    // it's fine for PrintErrorMessage().
    ss << char(ch);

    return ss;
}
#endif

struct TestOutput {
    TestOutput()                              = delete;
    ~TestOutput()                             = delete;
    TestOutput(TestOutput &&)                 = delete;
    TestOutput(const TestOutput &)            = delete;
    TestOutput &operator=(TestOutput &&)      = delete;
    TestOutput &operator=(const TestOutput &) = delete;

    template <typename... Values_T>
    inline static void Print(const Values_T &...values) {
        if (GetStreamCache() == nullptr) {
#if __cplusplus > 201402L
            (std::wcout << ... << values);
#else
            const int dummy[sizeof...(Values_T)] = {(std::wcout << values, 0)...};
            (void)dummy;
#endif

        } else {
#if __cplusplus > 201402L
            ((*GetStreamCache()) << ... << values);
#else
            const int dummy[sizeof...(Values_T)] = {((*GetStreamCache()) << values, 0)...};
            (void)dummy;
#endif
        }
    }

    enum struct Colors : SizeT8 { TITLE, ERROR, PASS, END };

    QENTEM_NOINLINE static const char *GetColor(Colors color) noexcept {
        if (IsColored()) {
            switch (color) {
                case Colors::TITLE:
                    return "\x1B[36m";
                case Colors::ERROR:
                    return "\x1B[31m";
                case Colors::PASS:
                    return "\x1B[32m";
                case Colors::END:
                    return "\x1B[0m";
            }
        }

        return "";
    }

    static void SetStreamCache(std::wstringstream *wss) noexcept {
        GetStreamCache() = wss;
    }

    static void SetDoubleFormat() noexcept {
        std::wstringstream *wss = GetStreamCache();

        if (wss == nullptr) {
            std::wcout << std::fixed;
            std::wcout.precision(2);
        } else {
            *wss << std::fixed;
            wss->precision(2);
        }
    }

    static void ResetDoubleFormat() noexcept {
        std::wstringstream *wss = GetStreamCache();

        if (wss == nullptr) {
            std::wcout << std::defaultfloat;
            std::wcout.precision(6);
        } else {
            *wss << std::defaultfloat;
            wss->precision(6);
        }
    }

    static bool &IsColored() noexcept {
        static bool isColored{true};

        return isColored;
    }

    static std::wstringstream *&GetStreamCache() noexcept {
        static std::wstringstream *wss{nullptr};

        return wss;
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
        getStorage() = MemoryRecordData{};
    }

    inline static void ResetSubMemoryRecord() noexcept {
        static MemoryRecordData &storage = getStorage();

        storage.subAllocations   = 0;
        storage.subDeallocations = 0;
    }

    inline static SizeT CheckSubAllocationCount() noexcept {
        static const MemoryRecordData &storage = getStorage();

        return (storage.subAllocations - storage.subDeallocations);
    }

    inline static void AddAllocation(void *pointer) noexcept {
        static MemoryRecordData &storage = getStorage();

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
        static MemoryRecordData &storage = getStorage();

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
        static const MemoryRecordData &storage = getStorage();

        TestOutput::SetDoubleFormat();

        TestOutput::Print("Memory: ", (double(storage.remainingSize) / 1024),
                          " KB, Peak: ", (double(storage.peakSize) / 1024), " KB.\n");

        TestOutput::Print("Allocations: ", storage.allocations, ", Deallocations: ", storage.deallocations, ".\n");

        const SizeT remaining_allocations = (storage.allocations - storage.deallocations);

        if (remaining_allocations != 0) {
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ERROR), "Leak detected",
                              TestOutput::GetColor(TestOutput::Colors::END), ": ", remaining_allocations,
                              " remaining allocations.\n\n");
        }

        TestOutput::ResetDoubleFormat();
    }

  private:
    static MemoryRecordData &getStorage() noexcept {
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
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TITLE), test_name_,
                          TestOutput::GetColor(TestOutput::Colors::END), ":\n");
    }

    QENTEM_NOINLINE int EndTests() {
        if (!error_) {
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TITLE), test_name_,
                              TestOutput::GetColor(TestOutput::Colors::PASS), " Passed all tests",
                              TestOutput::GetColor(TestOutput::Colors::END), "\n\n");
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
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ERROR), "Failed",
                          TestOutput::GetColor(TestOutput::Colors::END), ": ", part_name_, '\n');
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
        TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::TITLE), "Configurations",
                          TestOutput::GetColor(TestOutput::Colors::END), ":\n");
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
            TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::PASS), "Pass",
                              TestOutput::GetColor(TestOutput::Colors::END), ": ", part_name_, '\n');
        }

        if (test_for_leaks) {
            const SizeT remaining_allocations = MemoryRecord::CheckSubAllocationCount();
            MemoryRecord::ResetSubMemoryRecord();

            if (remaining_allocations != 0) {
                TestOutput::Print(TestOutput::GetColor(TestOutput::Colors::ERROR), "Leak detected",
                                  TestOutput::GetColor(TestOutput::Colors::END), ": ", remaining_allocations,
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
