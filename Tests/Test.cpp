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

// #include "StringStream.hpp"
// static Qentem::StringStream<char> test_ss;
// #define QENTEM_OUTPUT_STREAM test_ss

#include "Test.hpp"

int main() {
    Qentem::Test::SelfTest();
    Qentem::TestHelper::PrintInfo();

    // for (int i = 0; i < 100000; i++) {
    //     Qentem::Test::RunStringUtilsTests();
    //     Qentem::Test::RunStringTests();
    //     Qentem::Test::RunStringStreamTests();
    //     Qentem::Test::RunArrayTests();
    //     Qentem::Test::RunDigitTests();
    //     Qentem::Test::RunHArrayTests();
    //     Qentem::Test::RunEngineTests();
    //     Qentem::Test::RunUnicodeTests();
    //     Qentem::Test::RunJSONUtilsTests();
    //     Qentem::Test::RunValueTests();
    //     Qentem::Test::RunJSONTests();
    //     Qentem::Test::RunEvaluateTests();
    //     Qentem::Test::RunTemplateTests();
    //     Qentem::Test::RunTemplateUTests();
    //     Qentem::Test::RunTemplateLTests();
    // }

    const int ret = Qentem::Test::RunTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
