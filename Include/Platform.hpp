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

#ifndef QENTEM_PLATFORM_H_
#define QENTEM_PLATFORM_H_

#ifdef _MSC_VER
#include <intrin.h>
#if _WIN64
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#endif
#endif

namespace Qentem {
namespace Platform {

#ifdef _MSC_VER
#if _WIN64
inline static unsigned long CTZL(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward64(&index, value) != 0) ? index : 64);
}

inline static unsigned long CLZL(unsigned long long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse64(&index, value) != 0) ? index : 0);
}
#else
inline static unsigned long CTZL(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanForward(&index, value) != 0) ? index : 32);
}

inline static unsigned long CLZL(unsigned long value) noexcept {
    unsigned long index = 0;
    return ((_BitScanReverse(&index, value) != 0) ? index : 0);
}
#endif
#else
inline static unsigned long CTZL(unsigned long value) {
    return static_cast<unsigned long>(__builtin_ctzl(value));
}

inline static unsigned long CLZL(unsigned long value) {
    constexpr unsigned long bits = (sizeof(long) * 8) - 1;

    // if (value != 0) {
    return (bits - static_cast<unsigned long>(__builtin_clzl(value)));
    // }

    // return 0;
}
#endif

} // namespace Platform
} // namespace Qentem

#endif
