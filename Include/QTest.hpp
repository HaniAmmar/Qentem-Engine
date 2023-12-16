/*
 * Copyright (c) 2023 Hani Ammar
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

#if defined(__APPLE__)
#include <malloc/malloc.h>
#elif defined(__FreeBSD__)
#include <malloc_np.h>
#else
#include <malloc.h>
#endif

#ifndef _QENTEM_Q_TEST_H
#define _QENTEM_Q_TEST_H

namespace Qentem {

#if (defined(_MSVC_LANG) && (_MSVC_LANG > 201703L)) || (defined(__cplusplus) && (__cplusplus > 201703L))
std::wostream &operator<<(std::wostream &ss, const char16_t *str) {
    if (str != nullptr) {
        while (*str != char16_t{'\0'}) {
            // it's fine for PrintErrorMessage().
            ss << char(*str);
            ++str;
        }
    }

    return ss;
}

std::wostream &operator<<(std::wostream &ss, const char32_t *str) {
    if (str != nullptr) {
        while (*str != char16_t{'\0'}) {
            // it's fine for PrintErrorMessage().
            ss << char(*str);
            ++str;
        }
    }

    return ss;
}
#endif

struct TestOutPut {
    TestOutPut()                              = delete;
    ~TestOutPut()                             = delete;
    TestOutPut(TestOutPut &&)                 = delete;
    TestOutPut(const TestOutPut &)            = delete;
    TestOutPut &operator=(TestOutPut &&)      = delete;
    TestOutPut &operator=(const TestOutPut &) = delete;

    template <typename... _Values_T>
    inline static void Print(const _Values_T &...values) {
        if (getCachedStream() == nullptr) {
#if __cplusplus > 201402L
            (std::wcout << ... << values);
#else
            int dummy[sizeof...(_Values_T)] = {(std::wcout << values, 0)...};
            (void)dummy;
#endif

        } else {
#if __cplusplus > 201402L
            ((*getCachedStream()) << ... << values);
#else
            int dummy[sizeof...(_Values_T)] = {((*getCachedStream()) << values, 0)...};
            (void)dummy;
#endif
        }
    }

    enum Colors { TITLE, ERROR, PASS, END };

    QENTEM_NOINLINE static const char *GetColor(Colors color) noexcept {
        if (IsColored()) {
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

    static void SetCachedStream(std::wstringstream *wss) noexcept {
        getCachedStream() = wss;
    }

    static bool &IsColored() noexcept {
        static bool isColored{true};

        return isColored;
    }

  private:
    static std::wstringstream *&getCachedStream() noexcept {
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

    inline static void ResetSubMemory() noexcept {
        static MemoryRecordData &storage = getStorage();

        storage.subAllocations   = 0;
        storage.subDeallocations = 0;
    }

    inline static SizeT CheckSubMemory() noexcept {
        static MemoryRecordData &storage = getStorage();

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
        static MemoryRecordData &storage = getStorage();

        TestOutPut::Print("\nMemory: ", (double(storage.remainingSize) / 1024),
                          " KB, Peak: ", (double(storage.peakSize) / 1024), " KB.\n");

        TestOutPut::Print("Allocations: ", storage.allocations, ", Deallocations: ", storage.deallocations, ".\n");

        const SizeT remaining_deallocations = (storage.allocations - storage.deallocations);

        if (remaining_deallocations != 0) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Leak detected",
                              TestOutPut::GetColor(TestOutPut::Colors::END), ": ", remaining_deallocations,
                              " Remaining deallocations.\n\n");
        }
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

    QTest(const char *name, const char *file_fullname) noexcept : _test_name{name}, _file_fullname{file_fullname} {
    }

    QENTEM_NOINLINE void PrintGroupName() const {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), _test_name,
                          TestOutPut::GetColor(TestOutPut::Colors::END), ":\n");
    }

    QENTEM_NOINLINE int EndTests() {
        if (!_error) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), _test_name,
                              TestOutPut::GetColor(TestOutPut::Colors::PASS), " Passed all tests",
                              TestOutPut::GetColor(TestOutPut::Colors::END), "\n\n");
            return 0;
        }

        return 1;
    }

    template <typename _Char_T, typename _FUNC_T>
    QENTEM_NOINLINE void Test(_Char_T *name, _FUNC_T func) {
        if (!_error || _continue_on_error) {
            _part_name = name;
            func(*this);
            afterTest(true);
        }
    }

    template <typename _Char_T, typename _FUNC_T, typename... _Values_T>
    QENTEM_NOINLINE void Test(_Char_T *name, _FUNC_T func, bool test_for_leaks, _Values_T &...values) {
        if (!_error || _continue_on_error) {
            _part_name = name;
            func(*this, values...);
            afterTest(test_for_leaks);
        }
    }

    template <typename _Char_T>
    QENTEM_NOINLINE void EqualsTrue(bool value, const _Char_T *name, unsigned long line) {
        if ((!_error || _continue_on_error) && !value) {
            _error = true;
            QTest::PrintErrorMessage1(false, name, line);
        }
    }

    template <typename _Char_T>
    QENTEM_NOINLINE void EqualsFalse(bool value, const _Char_T *name, unsigned long line) {
        if ((!_error || _continue_on_error) && value) {
            _error = true;
            QTest::PrintErrorMessage1(true, name, line);
        }
    }

    template <typename _Char_T, typename _Value1_T, typename _Value2_T, typename _Value3_T>
    QENTEM_NOINLINE void Equal(const _Value1_T &left, const _Value2_T &right, const _Char_T *name,
                               const _Value3_T &value, unsigned long line) {
        if ((!_error || _continue_on_error) && (left != right)) {
            _error = true;
            QTest::PrintErrorMessage2(false, name, left, value, line);
        }
    }

    template <typename _Char_T, typename _Value1_T, typename _Value2_T, typename _Value3_T>
    QENTEM_NOINLINE void NotEqual(const _Value1_T &left, const _Value2_T &right, const _Char_T *name,
                                  const _Value3_T &value, unsigned long line) {
        if ((!_error || _continue_on_error) && (left == right)) {
            _error = true;
            QTest::PrintErrorMessage2(true, name, left, value, line);
        }
    }

    template <typename _Char_T, typename _Value1_T, typename _Value2_T>
    QENTEM_NOINLINE void Equal(const _Value1_T &left, const _Value2_T &right, const _Char_T *name, unsigned long line) {
        if ((!_error || _continue_on_error) && (left != right)) {
            _error = true;
            QTest::PrintErrorMessage2(false, name, left, right, line);
        }
    }

    template <typename _Char_T, typename _Value1_T, typename _Value2_T>
    QENTEM_NOINLINE void NotEqual(const _Value1_T &left, const _Value2_T &right, const _Char_T *name,
                                  unsigned long line) {
        if ((!_error || _continue_on_error) && (left == right)) {
            _error = true;
            QTest::PrintErrorMessage2(true, name, left, right, line);
        }
    }

    template <typename _Char_T>
    QENTEM_NOINLINE void PrintErrorMessage1(bool equal, const _Char_T *name, unsigned long line) {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Failed",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ": ", _part_name, '\n', _file_fullname, ":",
                          line, ":\n`", name, "` should", (equal ? " not " : " "), "equal: `true`\n\n");
    }

    template <typename _Char_T, typename _Value1_T, typename _Value2_T>
    QENTEM_NOINLINE void PrintErrorMessage2(bool equal, const _Char_T *name, const _Value1_T &value1,
                                            const _Value2_T &value2, unsigned long line) {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::ERROR), "Failed",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ": ", _part_name, '\n', _file_fullname, ":",
                          line, ":\n`", name, "` should", (equal ? " not " : " "), "equal: `", value2,
                          "`\n Returned Value: `", value1, "`\n\n");
    }

    inline void ContinueOnError(bool continue_on_error) noexcept {
        _continue_on_error = continue_on_error;
    }

    inline bool HasError() const noexcept {
        return _error;
    }

    inline bool IsContinueOnError() const noexcept {
        return _continue_on_error;
    }

    QENTEM_NOINLINE static void PrintInfo() {
        TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::TITLE), "Configurations",
                          TestOutPut::GetColor(TestOutPut::Colors::END), ":\n");
        if (Config::Is64bit) {
            TestOutPut::Print("Arch: 64-bit\n");
        } else {
            TestOutPut::Print("Arch: 32-bit\n");
        }

        TestOutPut::Print("SizeT: ", sizeof(SizeT), " bytes:\n");

        if (Config::IsBigEndian) {
            TestOutPut::Print("Endianness: Big-Endian\n");
        } else {
            TestOutPut::Print("Endianness: Little-Endian\n");
        }

        if (Config::PointerTagging) {
            TestOutPut::Print("Tagged Pointers: On\n");
        }

        if (Config::ShortStringOptimization) {
            TestOutPut::Print("Short String Optimization: On\n");
        }

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
        TestOutPut::Print("Advanced Vector Extensions: On\n");
#endif

#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
        TestOutPut::Print("Streaming SIMD Extensions 2: On\n");
#endif

#if defined(QENTEM_MSIMD128) && (QENTEM_MSIMD128 == 1)
        TestOutPut::Print("WASM SIMD128: On\n");
#endif

        if (Config::AutoEscapeHTML) {
            TestOutPut::Print("Auto Escape HTML: On\n");
        }

        TestOutPut::Print('\n');
    }

  private:
    QENTEM_NOINLINE void afterTest(bool test_for_leaks) {
        if (!_error) {
            TestOutPut::Print(TestOutPut::GetColor(TestOutPut::Colors::PASS), "Pass",
                              TestOutPut::GetColor(TestOutPut::Colors::END), ": ", _part_name, '\n');
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

    const char *_part_name{nullptr};
    const char *_test_name;
    const char *_file_fullname;
    bool        _error{false};
    bool        _continue_on_error{false};
};

} // namespace Qentem

#endif
