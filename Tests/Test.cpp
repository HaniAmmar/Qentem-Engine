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

#include "ALETest.hpp"
#include "ArrayTest.hpp"
#include "DigitTest.hpp"
#include "EngineTest.hpp"
#include "FixedArrayTest.hpp"
#include "HArrayTest.hpp"
#include "JSONTest.hpp"
#include "JSONUtilsTest.hpp"
#include "MemoryTest.hpp"
#include "StringStreamTest.hpp"
#include "StringTest.hpp"
#include "StringUtilsTest.hpp"
#include "TemplateTest.hpp"
#include "TestHelper.hpp"
#include "UnicodeTest.hpp"
#include "ValueTest.hpp"
#ifndef QENTEM_SIMD_ENABLED
#include "TemplateLTest.hpp"
#endif

int RunTests() {
    int passed = 0;
    int failed = 0;
    int total  = 0;

    ((Qentem::Test::RunTestHelperTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunMemoryTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringUtilsTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringStreamTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunFixedArrayTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunArrayTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunDigitTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunHArrayTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunEngineTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunALETests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunUnicodeTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunJSONUtilsTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunValueTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunJSONTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunTemplateTests() == 0) ? ++passed : ++failed);
#ifndef QENTEM_SIMD_ENABLED
    ((Qentem::Test::RunTemplateLTests() == 0) ? ++passed : ++failed);
#endif

    if (failed == 0) {
        std::wcout << "\x1B[32mAll good.\x1B[0m" << std::endl;
        return 0;
    }

    total = (passed + failed);

    std::wcout << "\x1B[31mNot good!\x1B[0m " << failed << " out of " << total
               << " failed." << std::endl;

    return 1;
}

int main() {
    // for (int i = 0; i < 100000; i++) {
    //     Qentem::Test::RunTestHelperTests();
    //     Qentem::Test::RunMemoryTests();
    //     Qentem::Test::RunStringUtilsTests();
    //     Qentem::Test::RunStringTests();
    //     Qentem::Test::RunStringStreamTests();
    //     Qentem::Test::RunFixedArrayTests();
    //     Qentem::Test::RunArrayTests();
    //     Qentem::Test::RunDigitTests();
    //     Qentem::Test::RunHArrayTests();
    //     Qentem::Test::RunEngineTests();
    //     Qentem::Test::RunALETests();
    //     Qentem::Test::RunUnicodeTests();
    //     Qentem::Test::RunJSONUtilsTests();
    //     Qentem::Test::RunValueTests();
    //     Qentem::Test::RunJSONTests();
    //     Qentem::Test::RunTemplateTests();
    //     Qentem::Test::RunTemplateXTests();
    // }

#ifdef QENTEM_64BIT_ARCH
    std::wcout << "Arch: 64BIT";
#if defined(QENTEM_POINTER_TAGGING) && QENTEM_POINTER_TAGGING == 1
    std::wcout << "\nTagged Pointers: On";
#endif
#endif

    std::wcout << "\nEndianness: ";

#ifndef QENTEM_BIG_ENDIAN
    std::wcout << "Little-endian";
#else
    std::wcout << "Big-endian";
#endif

#ifdef QENTEM_SIMD_ENABLED
#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
    std::wcout << "\nAVX2: On";
#endif
#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
    std::wcout << "\nSSE2: On";
#endif
#endif

    std::wcout << "\nSize of SizeT: " << sizeof(Qentem::SizeT) << '\n';
    std::wcout << std::endl;

    return RunTests();
}
