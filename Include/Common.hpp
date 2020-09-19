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

#ifndef QENTEM_COMMON_H_
#define QENTEM_COMMON_H_

#if !defined(_WIN64)
#ifndef QENTEM_AVX512BW_
#define QENTEM_AVX512BW_ 0
#endif
#endif

#ifndef QENTEM_AVX2_
#define QENTEM_AVX2_ 0
#endif

#ifndef QENTEM_SSE2_
#define QENTEM_SSE2_ 0
#endif

#if !defined(__AVX512BW__)
#undef QENTEM_AVX512BW_
#endif

#if !defined(__AVX2__)
#undef QENTEM_AVX2_
#endif

#if !defined(__SSE2__)
#undef QENTEM_SSE2_
#endif

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#endif

#ifdef _MSC_VER
#define QENTEM_MAYBE_UNUSED_
#else
#define QENTEM_MAYBE_UNUSED_ __attribute__((unused))
#endif

namespace Qentem {

#ifndef QENTEM_DOUBLE_PRECISION_
#define QENTEM_DOUBLE_PRECISION_ 14
#endif

#if defined(_WIN64)
using ULong = unsigned long long;
#else
using ULong = unsigned long;
#endif

using UInt = unsigned int;

} // namespace Qentem

#endif
