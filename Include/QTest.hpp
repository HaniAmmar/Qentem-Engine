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

#define QENTEM_DEBUG
#define QENTEM_ENABLE_MEMORY_RECORD

#if defined(_WIN32)
#include <windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

#ifndef QENTEM_Q_TEST_H
#define QENTEM_Q_TEST_H

#include "QConsole.hpp"
#include "MemoryRecord.hpp"

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
        QConsole::Print(QConsole::GetColor(QConsole::Color::TitleColor), test_name_,
                        QConsole::GetColor(QConsole::Color::EndColor), ":\n");
    }

    QENTEM_NOINLINE int EndTests() {
        if (!error_) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::TitleColor), test_name_,
                            QConsole::GetColor(QConsole::Color::PassColor), " Passed all tests",
                            QConsole::GetColor(QConsole::Color::EndColor), "\n\n");
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
        QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Failed",
                        QConsole::GetColor(QConsole::Color::EndColor), ": ", part_name_, '\n');
        QConsole::Print(file_fullname_, ":", line, ":\n Should", (equal ? " not " : " "), "equal: `", value2,
                        "`\n     Returned: `", value1, "`\n\n");

        QConsole::Flush();

        if (!continue_on_error_) {
            terminate();
        }
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

    QENTEM_NOINLINE static void PrintMemoryRecord() {
        static const auto &storage = MemoryRecord::GetRecord();

        QConsole::Print("Memory: ", (double(storage.Size) / 1024), " KiB, Peak: ", (double(storage.PeakSize) / 1024),
                        " KiB.\n");

        QConsole::Print("Reserves: ", storage.Reserved, ", Releases: ", storage.Released, ".\n");
        QConsole::Print("Kept Pages: ", storage.Pages, ", Sum: ", (double(storage.PagesSumSize) / 1024), " KiB.\n");
    }

    QENTEM_NOINLINE static void PrintMemoryStatus() {
        static const auto &storage = MemoryRecord::GetRecord();
        PrintMemoryRecord();

        const SystemIntType remaining_allocations = (storage.Reserved - storage.Released);

        if (remaining_allocations != 0) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Leak detected",
                            QConsole::GetColor(QConsole::Color::EndColor), ": ", remaining_allocations,
                            " remaining Allocations.\n\n");
        }
    }

    QENTEM_NOINLINE static void PrintInfo(bool template_engine_info = true) {
        QConsole::Print(QConsole::GetColor(QConsole::Color::TitleColor), "Configurations",
                        QConsole::GetColor(QConsole::Color::EndColor), ":\n");
        if QENTEM_CONST_EXPRESSION (QentemConfig::Is64bit) {
            QConsole::Print("Arch: 64-bit\n");
        } else {
            QConsole::Print("Arch: 32-bit\n");
        }

        QConsole::Print("SizeT: ", sizeof(SizeT), " bytes:\n");

        if QENTEM_CONST_EXPRESSION (QentemConfig::IsBigEndian) {
            QConsole::Print("Endianness: Big-Endian\n");
        } else {
            QConsole::Print("Endianness: Little-Endian\n");
        }
#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
        QConsole::Print("Advanced Vector Extensions: On\n");
#endif

#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
        QConsole::Print("Streaming SIMD Extensions 2: On\n");
#endif

#if defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
        QConsole::Print("WASM SIMD128: On\n");
#endif

        if QENTEM_CONST_EXPRESSION (QentemConfig::AutoEscapeHTML) {
            if (template_engine_info) {
                QConsole::Print("Auto Escape HTML: On\n");
            }
        }

        QConsole::Print('\n');
    }

  private:
    QENTEM_NOINLINE void afterTest(bool test_for_leaks) {
        if (!error_) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::PassColor), "Pass",
                            QConsole::GetColor(QConsole::Color::EndColor), ": ", part_name_, '\n');
        }

        if (test_for_leaks) {
            const SystemIntType remaining_allocations = MemoryRecord::CheckSubRecord();
            MemoryRecord::EraseSubMemoryRecord();

            if (remaining_allocations != 0) {
                QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Leak detected",
                                QConsole::GetColor(QConsole::Color::EndColor), ": ", remaining_allocations,
                                " remaining allocations.\n");
            }
        }
    }

    [[noreturn]] QENTEM_INLINE void terminate() noexcept {
#if defined(_WIN32)
        ::ExitProcess(1); // Native Windows termination
#else
        _exit(1);                // POSIX immediate termination (no cleanup)
        __builtin_trap();        // Should never reach; added as safeguard
        __builtin_unreachable(); // Declares unreachable for compilers/optimizers
#endif
    }

    const char *part_name_{nullptr};
    const char *test_name_;
    const char *file_fullname_;
    bool        error_{false};
    bool        continue_on_error_{false};
};

} // namespace Qentem

#endif
