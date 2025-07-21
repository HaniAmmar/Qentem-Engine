/**
 * @file SystemMemory.hpp
 * @brief Low-level, platform-independent system for acquiring and releasing page-aligned virtual memory.
 *
 * Defines Qentem::SystemMemory — a minimal interface for direct memory management using
 * system calls (mmap on Unix-like platforms, VirtualAlloc on Windows), bypassing libc.
 *
 * Designed for allocators, buffers, and runtime systems requiring page-level control.
 * Supports optional stack-specific allocation (e.g., MAP_STACK on Linux) and provides
 * unified access to allocation, release, and protection routines.
 *
 * On unknown or freestanding platforms, it transparently falls back to malloc.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_SYSTEM_MEMORY_HPP
#define QENTEM_SYSTEM_MEMORY_HPP

#include "QCommon.hpp"

// clang-format off
#if defined(_WIN32)
    // Prevent <windows.h> from defining min/max macros that interfere with standard headers
    #define NOMINMAX
    #include <windows.h>

#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || \
      defined(__NetBSD__) || defined(__OpenBSD__)
    // POSIX-style platforms: mmap, sysconf, etc.
    #include <sys/mman.h>
    #include <unistd.h>

    #if defined(__linux__)
        // MAP_STACK was added in Linux 2.6.27; define manually if missing
        #if !defined(MAP_STACK)
            #define QENTEM_LINUX_MAP_STACK 0x20000
        #else
            #define QENTEM_LINUX_MAP_STACK MAP_STACK
        #endif
    #else
        // Non-Linux POSIX platforms don’t use MAP_STACK
        #define QENTEM_LINUX_MAP_STACK 0
    #endif

#elif !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    // Unknown, unsupported, or freestanding platform — fallback to malloc
    #define QENTEM_SYSTEM_MEMORY_FALLBACK 1
#endif
// clang-format on

namespace Qentem {

/**
 * @brief Provides low-level, page-aligned memory allocation using platform-specific primitives.
 *
 * Avoids use of malloc/free or any libc dependency. Intended for arena allocators, fiber stacks,
 * large buffers, and other low-level memory systems.
 */
struct SystemMemory {
    /**
     * @brief Returns the native system page size (e.g., 4096 bytes on x86).
     *
     * @return Size of a memory page in bytes.
     */
    static SystemIntType PageSize() noexcept {
        // clang-format off
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    #if defined(_WIN32)
            SYSTEM_INFO info;
            GetSystemInfo(&info);
            static const SystemIntType page_size = static_cast<SystemIntType>(info.dwPageSize);
    #else
            static const SystemIntType page_size = static_cast<SystemIntType>(::sysconf(
        #if defined(_SC_PAGESIZE)
                    _SC_PAGESIZE
        #else
                    _SC_PAGE_SIZE
        #endif
                ));
    #endif
#else
        // Fallback for freestanding
        static constexpr SystemIntType page_size = QENTEM_FALLBACK_SYSTEM_PAGE_SIZE;
#endif
        // clang-format on

        return page_size;
    }

    /**
     * @brief Marks a memory region as inaccessible (guard page).
     *
     * For placing red zones in stacks or arenas.
     *
     * @param ptr  Aligned pointer to memory.
     * @param size Size of region to protect (should be a multiple of page size).
     * @return True on success, false on failure.
     */
    static bool ProtectGuardPage(void *ptr, SystemIntType size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
#if defined(_WIN32)
        DWORD old_protect;
        return (::VirtualProtect(ptr, static_cast<SystemIntType>(size), PAGE_NOACCESS, &old_protect) != 0);
#else
        return (::mprotect(ptr, size, PROT_NONE) == 0);
#endif
#else
        (void)ptr;
        (void)size;
        return false;
#endif
    }

    /**
     * @brief Reserves page-aligned memory suitable for general-purpose use.
     *
     * The returned memory is readable and writable and does not include guard pages.
     *
     * @param size Number of bytes to reserve. Should be a multiple of the system page size.
     * @return Pointer to memory on success, or nullptr on failure.
     */
    static void *Reserve(SystemIntType size) noexcept {
        return reserve<false>(size);
    }

    /**
     * @brief Reserves page-aligned memory intended for use as a stack.
     *
     * On Linux, this enables the MAP_STACK flag (if available) to inform the kernel
     * that the memory will be used as a downward-growing stack. No guard page is applied.
     *
     * @param size Number of bytes to reserve. Should be a multiple of the system page size.
     * @return Pointer to memory on success, or nullptr on failure.
     */
    static void *ReserveStack(SystemIntType size) noexcept {
        return reserve<true>(size);
    }

    /**
     * @brief Release memory previously acquired with Reserve().
     *
     * @param ptr  Pointer returned by Reserve().
     * @param size Size in bytes (same value passed to Reserve).
     */
    static void Release(void *ptr, SystemIntType size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
#if defined(_WIN32)
        (void)size;
        ::VirtualFree(ptr, 0, MEM_RELEASE);
#else
        ::munmap(ptr, size);
#endif
#else
        // Size ignored when using malloc-based fallback.
        (void)size;
        __builtin_free(ptr);
#endif
    }

    /**
     * @brief Releases a range of memory pages previously reserved by Reserve().
     *
     * Depending on the platform and policy, this may either unmap the memory entirely
     * (via munmap) or mark it as discardable (via madvise). Use this only for page-aligned,
     * page-sized regions you no longer intend to access.
     *
     * @param start Pointer to the start of the region (must be page-aligned).
     * @param size  Size in bytes to release (must be multiple of page size).
     * @param reclaim Completely release the memory (true = munmap, false = madvise).
     */
    QENTEM_INLINE static void ReleasePages(void *start, SystemIntType size, bool reclaim = false) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
#if defined(_WIN32)
        (void)size;
        (void)reclaim;
        // Always hard-unmap on Windows — madvise not applicable
        ::VirtualFree(start, 0, MEM_RELEASE);
#else
        if (reclaim) {
            ::munmap(start, size);
        } else {
            ::madvise(start, size, MADV_DONTNEED);
        }
#endif
#else
        (void)start;
        (void)size;
        (void)reclaim;
#endif
    }

  private:
    /**
     * @brief Internal implementation of memory allocation.
     *
     * This template function wraps platform-specific virtual memory allocation,
     * optionally including stack-specific flags (e.g., MAP_STACK on Linux).
     *
     * @tparam IS_STACK_MEMORY_T If true, marks the memory as stack-eligible where supported.
     * @param size Number of bytes to reserve. Should be a multiple of the system page size.
     * @return Pointer to reserved memory, or nullptr on failure.
     */
    template <bool IS_STACK_MEMORY_T>
    static void *reserve(SystemIntType size) noexcept {
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
#if defined(_WIN32)
        return ::VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
        constexpr auto flags = MAP_PRIVATE | MAP_ANONYMOUS | (IS_STACK_MEMORY_T ? QENTEM_LINUX_MAP_STACK : 0);
        return ::mmap(nullptr, size, PROT_READ | PROT_WRITE, flags, -1, 0);
#endif
#else
        return __builtin_malloc(size);
#endif
    }
};

} // namespace Qentem

#endif // QENTEM_SYSTEM_MEMORY_HPP
