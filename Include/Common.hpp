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

#ifndef QENTEM_DOUBLE_PRECISION_
#define QENTEM_DOUBLE_PRECISION_ 15
#endif

#if _WIN64 || __x86_64__ || __ppc64__
#define QENTEM_64BIT_
#endif

#ifndef QENTEM_AVX2_
#define QENTEM_AVX2_ 0
#endif

#ifndef QENTEM_SSE2_
#define QENTEM_SSE2_ 0
#endif

#ifndef __AVX2__
#undef QENTEM_AVX2_
#endif

#ifndef __SSE2__
#undef QENTEM_SSE2_
#endif

#ifdef _MSC_VER
#define QENTEM_NOINLINE __declspec(noinline)
#define QENTEM_MAYBE_UNUSED_
#else
#define QENTEM_NOINLINE __attribute__((noinline))
#define QENTEM_MAYBE_UNUSED_ __attribute__((unused))
#endif

#ifdef QENTEM_64BIT_
// We only use the lower 48 bits for pointers on 64bit arch
// the upper 16 bits can be used to taging.
#ifndef QENTEM_TAGGED_POINTER_
#define QENTEM_TAGGED_POINTER_ 1
#endif
#endif

namespace Qentem {
// 1 == 0x00000001; // little;
// 1 == 0x01000000; // big

static constexpr bool IsBigEndian() { return ((1 & 0x01000000) ? 1 : 0); }

#ifdef QENTEM_64BIT_
using ULong = unsigned long long;
#else
using ULong = unsigned long;
#endif

#ifndef QENTEM_SIZE_T_
#define QENTEM_SIZE_T_
using SizeT = unsigned int;
#endif

using NullType = decltype(nullptr);

} // namespace Qentem

#endif
