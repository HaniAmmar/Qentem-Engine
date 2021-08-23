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
#include "HArrayTest.hpp"
#include "JSONTest.hpp"
#include "JSONUtilsTest.hpp"
#include "MemoryTest.hpp"
#include "StringStreamTest.hpp"
#include "StringTest.hpp"
#include "StringUtilsTest.hpp"
#include "TemplateLTest.hpp"
#include "TemplateTest.hpp"
#include "TemplateUTest.hpp"
#include "TestHelper.hpp"
#include "UnicodeTest.hpp"
#include "ValueTest.hpp"

int RunTests() {
    int passed = 0;
    int failed = 0;
    int total  = 0;

    ((Qentem::Test::RunTestHelperTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunMemoryTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringUtilsTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunStringStreamTests() == 0) ? ++passed : ++failed);
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
    ((Qentem::Test::RunTemplateUTests() == 0) ? ++passed : ++failed);
    ((Qentem::Test::RunTemplateLTests() == 0) ? ++passed : ++failed);

    if (failed == 0) {
        QENTEM_OUTPUT_STREAM << "\x1B[32mAll good.\x1B[0m\n";
        return 0;
    }

    total = (passed + failed);

    QENTEM_OUTPUT_STREAM << "\x1B[31mNot good!\x1B[0m " << failed << " out of "
                         << total << " failed.\n";

    return 1;
}

int main() {
    // for (int i = 0; i < 100000; i++) {
    //     Qentem::Test::RunTestHelperTests();
    //     Qentem::Test::RunMemoryTests();
    //     Qentem::Test::RunStringUtilsTests();
    //     Qentem::Test::RunStringTests();
    //     Qentem::Test::RunStringStreamTests();
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
    //     Qentem::Test::RunTemplateUTests();
    //     Qentem::Test::RunTemplateLTests();
    // }

    QENTEM_OUTPUT_STREAM << "\x1B[36mConfiguration\x1B[0m:\n";

#ifdef QENTEM_64BIT_ARCH
    QENTEM_OUTPUT_STREAM << "Arch: 64BIT";
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    QENTEM_OUTPUT_STREAM << "\nTagged Pointers: On";
#endif
#endif

    QENTEM_OUTPUT_STREAM << "\nEndianness: ";

#ifndef QENTEM_BIG_ENDIAN
    QENTEM_OUTPUT_STREAM << "Little-endian";
#else
    QENTEM_OUTPUT_STREAM << "Big-endian";
#endif

#if defined(QENTEM_AVX2) && (QENTEM_AVX2 == 1)
    QENTEM_OUTPUT_STREAM << "\nAVX2: On";
#endif
#if defined(QENTEM_SSE2) && (QENTEM_SSE2 == 1)
    QENTEM_OUTPUT_STREAM << "\nSSE2: On";
#endif

    QENTEM_OUTPUT_STREAM << "\nSize of SizeT: " << sizeof(Qentem::SizeT)
                         << "\n\n";

    return RunTests();
}
