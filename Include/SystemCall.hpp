/**
 * @file SystemCall.hpp
 * @brief Raw Linux syscall interface using inline assembly.
 *
 * Provides always-inline, type-flexible syscall wrappers for i386 (Linux 32-bit),
 * x86_64 (Linux 64-bit), AArch64, ARM32-EABI, and RISC-V 64. Avoids libc entirely,
 * invoking the kernel directly per ABI calling conventions.
 *
 * Usage pattern:
 *   long r = SystemCall(__NR_somecall, arg1, arg2, ...);
 *   // negative return values correspond to -errno
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT
 */

#ifndef QENTEM_SYSTEM_CALL_HPP
#define QENTEM_SYSTEM_CALL_HPP

#if defined(__linux__)

#include "QCommon.hpp"

#include <asm/unistd.h>

#if defined(__i386__) && !defined(QENTEM_NO_i386_SYSCALL)
#include <unistd.h>
#endif

namespace Qentem {

/**
 * @brief Raw, always-inline system call primitive.
 *
 * - Accepts up to 6 syscall arguments (v1..v6) and the syscall number (name).
 * - Returns the raw kernel return value; negative values correspond to -errno.
 * - Architecture-specific inline assembly ensures ABI compliance without libc.
 *
 * @note This function is internal; use the templated `SystemCall()` wrapper
 *       for type-flexible argument passing.
 */

// #pragma GCC optimize("-fomit-frame-pointer")

// #if defined(__GNUC__) && !defined(__clang__)
// __attribute__((optimize("omit-frame-pointer")))
// #endif

QENTEM_INLINE inline static long SystemCall_(long name, long v1, long v2, long v3, long v4, long v5, long v6) noexcept {
#if defined(__x86_64__)
    // x86-64 ABI: rax=nr, rdi, rsi, rdx, r10, r8, r9 -> rax
    register long r10 __asm__("r10") = v4;
    register long r8 __asm__("r8")   = v5;
    register long r9 __asm__("r9")   = v6;
    long          ret;
    __asm__ __volatile__("syscall"
                         : "=a"(ret)
                         : "a"(name), "D"(v1), "S"(v2), "d"(v3), "r"(r10), "r"(r8), "r"(r9)
                         : "rcx", "r11", "cc", "memory");
    return ret;
#elif defined(__i386__)
#if defined(QENTEM_NO_i386_SYSCALL)
    //   - Required `-O1` or higher (or `-fomit-frame-pointer`) because the 6th
    //     argument must be passed in EBP, which conflicts with the frame pointer.
    //   - The asm version preserved and restored EBP manually to remain debug-safe.

    // i386 ABI : eax = nr, ebx, ecx, edx, esi, edi, EBP->eax
    long ret;
    __asm__ __volatile__("push %%ebp\n\t"        // preserve frame pointer (debug-safe)
                         "mov  %[a6], %%ebp\n\t" // 6th arg must be in EBP
                         "int  $0x80\n\t"        // syscall
                         "pop  %%ebp\n\t"        // restore frame pointer
                         : "=a"(ret)
                         : "a"(name), "b"(v1), "c"(v2), "d"(v3), "S"(v4), "D"(v5), [a6] "r"(v6)
                         : "cc", "memory");
    return ret;
#else
    return syscall(name, v1, v2, v3, v4, v5, v6);
#endif // QENTEM_NO_i386_SYSCALL

#elif defined(__aarch64__)
    // AArch64 ABI: x8=nr, x0..x5=args -> x0
    register long x8 __asm__("x8") = name;
    register long x0 __asm__("x0") = v1;
    register long x1 __asm__("x1") = v2;
    register long x2 __asm__("x2") = v3;
    register long x3 __asm__("x3") = v4;
    register long x4 __asm__("x4") = v5;
    register long x5 __asm__("x5") = v6;
    __asm__ __volatile__("svc 0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "cc", "memory");
    return x0;

#elif defined(__arm__) && defined(__ARM_EABI__)
    // ARM32 EABI: r7=nr, r0..r5=args -> r0
    register long r7 __asm__("r7") = name;
    register long r0 __asm__("r0") = v1;
    register long r1 __asm__("r1") = v2;
    register long r2 __asm__("r2") = v3;
    register long r3 __asm__("r3") = v4;
    register long r4 __asm__("r4") = v5;
    register long r5 __asm__("r5") = v6;
    __asm__ __volatile__("svc 0" : "+r"(r0) : "r"(r7), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5) : "cc", "memory");
    return r0;

#elif defined(__riscv) && (__riscv_xlen == 64)
    // RISC-V 64-bit: a7=nr, a0..a5=args -> a0
    register long a7 __asm__("a7") = name;
    register long a0 __asm__("a0") = v1;
    register long a1 __asm__("a1") = v2;
    register long a2 __asm__("a2") = v3;
    register long a3 __asm__("a3") = v4;
    register long a4 __asm__("a4") = v5;
    register long a5 __asm__("a5") = v6;
    __asm__ __volatile__("ecall" : "+r"(a0) : "r"(a7), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5) : "cc", "memory");
    return a0;

#else
#error "Unsupported architecture: add a SystemCall path."
#endif
}

/**
 * @brief Type-flexible, always-inline syscall wrapper.
 *
 * Converts arbitrary integral types to `long` for the raw `SystemCall_`.
 * Allows libc-free, header-only usage across multiple platforms.
 *
 * @tparam Var1_T..Var6_T  Argument types, defaults to long.
 * @param name             Syscall number (e.g., __NR_openat).
 * @param v1..v6           Optional syscall arguments.
 * @return Kernel return value (negative indicates -errno).
 */
template <typename Var1_T = long, typename Var2_T = long, typename Var3_T = long, typename Var4_T = long,
          typename Var5_T = long, typename Var6_T = long>
QENTEM_INLINE inline static long SystemCall(long name, Var1_T v1 = 0, Var2_T v2 = 0, Var3_T v3 = 0, Var4_T v4 = 0,
                                            Var5_T v5 = 0, Var6_T v6 = 0) noexcept {
    return SystemCall_(name, (long)v1, (long)v2, (long)v3, (long)v4, (long)v5, (long)v6);
}

} // namespace Qentem

#endif // __linux__
#endif // QENTEM_SYSTEM_CALL_HPP
