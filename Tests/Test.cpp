/*
 * Copyright (c) 2026 Hani Ammar
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

#include "Test.hpp"

int main() {
    using namespace Qentem;

    Test::SelfTest();
    QTest::PrintInfo();

    // for (int i = 0; i < 100000; i++) {
    //     Test::RunStringUtilsTests();
    //     Test::RunReserverTests();
    //     Test::RunStringTests();
    //     Test::RunStringStreamTests();
    //     Test::RunArrayTests();
    //     Test::RunQPoolTests();
    //     Test::RunDequeTests();
    //     Test::RunBigIntTests();
    //     Test::RunDigitTests();
    //     Test::RunHArrayTests();
    //     Test::RunHListTests();
    //     Test::RunUnicodeTests();
    //     Test::RunJSONUtilsTests();
    //     Test::RunValueTests();
    //     Test::RunJSONTests();
    //     Test::RunEvaluateTests();
    //     Test::RunTemplateTests();
    //     Test::RunTemplateUTests();
    //     Test::RunTemplateLTests();
    // }

    const int ret = Test::RunTests();
    QTest::PrintMemoryStatus();

    return ret;
}
