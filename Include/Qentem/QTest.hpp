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
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_Q_TEST_H
#define QENTEM_Q_TEST_H

#define QENTEM_DEBUG
#define QENTEM_ENABLE_MEMORY_RECORD

#include "Qentem/QConsole.hpp"
#include "Qentem/Reserver.hpp"

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#endif

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
        // if (!(Reserver::IsEmpty())) {
        //     QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Reserver should be empty.\n\n",
        //                     QConsole::GetColor(QConsole::Color::EndColor));
        // }

        if (!error_) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::TitleColor), test_name_,
                            QConsole::GetColor(QConsole::Color::PassColor), " Passed all tests",
                            QConsole::GetColor(QConsole::Color::EndColor), "\n\n");
            return 0;
        }

        return 1;
    }

    template <typename Char_T, typename FUNC_T>
    QENTEM_NOINLINE void Test(Char_T *name, FUNC_T func, bool test_for_leaks = true) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this);
            afterTest(test_for_leaks);
        }
    }

    template <typename Char_T, typename FUNC_T, typename... Values_T>
    QENTEM_NOINLINE void Test(Char_T *name, FUNC_T func, bool test_for_leaks, Values_T &&...values) {
        if (!error_ || continue_on_error_) {
            part_name_ = name;
            func(*this, QUtility::Forward<Values_T>(values)...);
            afterTest(test_for_leaks);
        }
    }

    template <typename LineNumber_T>
    QENTEM_NOINLINE void IsTrue(bool value, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && !value) {
            error_ = true;
            QTest::PrintErrorMessage(false, "false", "true", line);
        }
    }

    template <typename LineNumber_T>
    QENTEM_NOINLINE void IsFalse(bool value, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && value) {
            error_ = true;
            QTest::PrintErrorMessage(false, "true", "false", line);
        }
    }

    template <typename LineNumber_T>
    QENTEM_NOINLINE void IsNull(const void *value, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && (value != nullptr)) {
            error_ = true;
            QTest::PrintErrorMessage(false, value, "null", line);
        }
    }

    template <typename LineNumber_T>
    QENTEM_NOINLINE void IsNotNull(const void *value, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && (value == nullptr)) {
            error_ = true;
            QTest::PrintErrorMessage(true, "null", "null", line);
        }
    }

    template <typename Value1_T, typename Value2_T, typename LineNumber_T>
    QENTEM_NOINLINE void IsEqual(const Value1_T &left, const Value2_T &right, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && (left != right)) {
            error_ = true;
            QTest::PrintErrorMessage(false, left, right, line);
        }
    }

    template <typename Value1_T, typename Value2_T, typename LineNumber_T>
    QENTEM_NOINLINE void IsNotEqual(const Value1_T &left, const Value2_T &right, LineNumber_T line) {
        if ((!error_ || continue_on_error_) && (left == right)) {
            error_ = true;
            QTest::PrintErrorMessage(true, left, right, line);
        }
    }

    template <typename Value1_T, typename Value2_T, typename LineNumber_T>
    QENTEM_NOINLINE void PrintErrorMessage(bool equal, const Value1_T &value1, const Value2_T &value2,
                                           LineNumber_T line) {
        QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Failed",
                        QConsole::GetColor(QConsole::Color::EndColor), ": ", part_name_, '\n');
        QConsole::Print(file_fullname_, ":", line, ":\n Should", (equal ? " not " : " "), "equal: `", value2,
                        "`\n     Returned: `", value1, "`\n\n");

        QConsole::Flush();

        if (!continue_on_error_ && QConsole::IsOutputEnabled()) {
            terminate();
        }
    }

    QENTEM_INLINE void ContinueOnError(bool continue_on_error) noexcept {
        continue_on_error_ = continue_on_error;
    }

    QENTEM_INLINE bool HasError() const noexcept {
        return error_;
    }

    QENTEM_INLINE bool ContinueOnErrorEnabled() const noexcept {
        return continue_on_error_;
    }

    QENTEM_NOINLINE static void PrintMemoryRecord() {
        const auto &storage = MemoryRecord::GetRecord();

        QConsole::Print("Memory: ", (double(storage.Size) / 1024), " KiB, Peak: ", (double(storage.PeakSize) / 1024),
                        " KiB.\n");

        QConsole::Print("Reserves: ", storage.Reserved, ", Releases: ", storage.Released, ".\n");
        QConsole::Print("Kept Blocks: ", storage.Blocks, ", Total: ", (double(storage.BlocksTotalSize) / 1024),
                        " KiB.\n");

        QConsole::Flush();
    }

    QENTEM_NOINLINE static void PrintMemoryStatus() {
        const auto &storage = MemoryRecord::GetRecord();

        PrintMemoryRecord();

        const SystemLong remaining = (storage.Reserved - storage.Released);

        if (remaining != 0) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Leak detected",
                            QConsole::GetColor(QConsole::Color::EndColor), ": ", remaining, " not released.\n\n");

            QConsole::Flush();
        }
    }

    QENTEM_NOINLINE static void PrintInfo(bool template_engine_info = true) {
        QConsole::Print(QConsole::GetColor(QConsole::Color::TitleColor), "Configurations",
                        QConsole::GetColor(QConsole::Color::EndColor), ":\n");
        if constexpr (QentemConfig::Is64bit) {
            QConsole::Print("Arch: 64-bit\n");
        } else {
            QConsole::Print("Arch: 32-bit\n");
        }

        QConsole::Print("SizeT: ", sizeof(SizeT), " bytes:\n");

        if constexpr (QentemConfig::IsBigEndian) {
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

        if constexpr (QentemConfig::AutoEscapeHTML) {
            if (template_engine_info) {
                QConsole::Print("Auto Escape HTML: On\n");
            }
        }

        QConsole::Print('\n');
        QConsole::Flush();
    }

  private:
    QENTEM_NOINLINE void afterTest(bool test_for_leaks) {
        if (!error_) {
            QConsole::Print(QConsole::GetColor(QConsole::Color::PassColor), "Pass",
                            QConsole::GetColor(QConsole::Color::EndColor), ": ", part_name_, '\n');
        }

        if (test_for_leaks) {
            const SystemLong remaining = MemoryRecord::CheckSubRecord();
            MemoryRecord::EraseSubMemoryRecord();

            if (remaining != 0) {
                QConsole::Print(QConsole::GetColor(QConsole::Color::ErrorColor), "Leak detected",
                                QConsole::GetColor(QConsole::Color::EndColor), ": ", remaining, " not released.\n");
                QConsole::Flush();
            }
        }
    }

    [[noreturn]] QENTEM_INLINE void terminate() noexcept {
#if defined(_WIN32)
        ::ExitProcess(1);
#elif defined(__linux__)
        SystemCall(__NR_exit_group, 1);
        __builtin_unreachable();
#else
        _exit(1);
        __builtin_unreachable();
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
