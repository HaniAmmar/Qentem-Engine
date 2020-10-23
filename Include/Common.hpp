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

#ifndef QENTEM_DOUBLE_PRECISION
#define QENTEM_DOUBLE_PRECISION 15
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) ||          \
    defined(__ppc64__) || defined(__powerpc64__) || defined(__s390__)
#define QENTEM_64BIT_ARCH
// 64bit arch only uses the lower 48 bits for pointers,
// the upper 16 bits can be used for taging.
#ifndef QENTEM_POINTER_TAGGING
#define QENTEM_POINTER_TAGGING 1
#endif
#endif

#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
#ifndef QENTEM_SSO
// Short string optimization
#define QENTEM_SSO 1
#endif
#endif

#ifndef QENTEM_AVX2
#define QENTEM_AVX2 0
#endif

#ifndef QENTEM_SSE2
#define QENTEM_SSE2 0
#endif

#ifndef __AVX2__
#undef QENTEM_AVX2
#endif

#ifndef __SSE2__
#undef QENTEM_SSE2
#endif

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#define QENTEM_MAYBE_UNUSED
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#define QENTEM_MAYBE_UNUSED __attribute__((unused))
#endif

#ifndef QENTEM_BIG_ENDIAN
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define QENTEM_BIG_ENDIAN
#endif
#endif

namespace Qentem {

#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T
#ifdef QENTEM_64BIT_ARCH
using SizeT = unsigned int;
#else
using SizeT = unsigned long;
#endif
#endif

using NullType = decltype(nullptr);

} // namespace Qentem

#endif
