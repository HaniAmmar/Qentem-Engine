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

#ifndef QENTEM_COMMON_H_
#define QENTEM_COMMON_H_

namespace Qentem {

struct Config {
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__) || defined(__powerpc64__) ||  \
    defined(__s390__)
#define QENTEM_64BIT_ARCH 1
    // static constexpr bool Is64bit = (sizeof(void *) == 8);
    static constexpr bool Is64bit = true;
#else
    static constexpr bool Is64bit                 = false;
#endif
    ///////////////////////////////////////////////
#ifndef QENTEM_BIG_ENDIAN
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define QENTEM_BIG_ENDIAN 1
#endif
#endif
/////////////////
#if defined(QENTEM_BIG_ENDIAN) && (QENTEM_BIG_ENDIAN == 1)
    static constexpr bool BigEndian = true;
#else
    static constexpr bool BigEndian               = false;
#endif
///////////////////////////////////////////////
#ifdef QENTEM_64BIT_ARCH
// 64bit arch only uses the lower 48 bits for pointers,
// the upper 16 bits can be used for tagging.
#ifndef QENTEM_POINTER_TAGGING
#define QENTEM_POINTER_TAGGING 1
#endif
#else
#undef QENTEM_POINTER_TAGGING
#endif
/////////////////
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    static constexpr bool PointerTagging = true;
#else
    static constexpr bool PointerTagging          = false;
#endif
///////////////////////////////////////////////
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
#ifndef QENTEM_SSO
// Short string optimization
#define QENTEM_SSO 1
#endif
#else
#undef QENTEM_SSO
#endif
/////////////////
#if defined(QENTEM_SSO) && (QENTEM_SSO == 1)
    static constexpr bool ShortStringOptimization = true;
#else
    static constexpr bool ShortStringOptimization = false;
#endif

    ///////////////////////////////////////////////
#ifndef QENTEM_AUTO_ESCAPE_HTML
#define QENTEM_AUTO_ESCAPE_HTML 1
#endif
/////////////////
#if defined(QENTEM_AUTO_ESCAPE_HTML) && (QENTEM_AUTO_ESCAPE_HTML == 1)
    static constexpr bool AutoEscapeHTML = true;
#else
    static constexpr bool AutoEscapeHTML          = false;
#endif

    static constexpr unsigned int FloatDoublePrecision = 17;
};
///////////////////////////////////////////////

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
#define QENTEM_INLINE __forceinline
#define QENTEM_MAYBE_UNUSED
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#define QENTEM_INLINE __attribute__((always_inline))
#define QENTEM_MAYBE_UNUSED __attribute__((unused))
#endif

#ifndef QENTEM_SIZE_T
#define QENTEM_SIZE_T
using SizeT = unsigned int;
#endif

using NullType = decltype(nullptr);

union QNumber {
    unsigned long long Natural{0};
    long long          Integer;
    double             Real;
};

} // namespace Qentem

#endif
