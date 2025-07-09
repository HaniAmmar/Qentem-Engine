/**
 * @file SystemMemory.hpp
 * @brief Platform-agnostic, low-level memory allocator for page-aligned system memory.
 *
 * This header defines the Qentem::SystemMemory struct, which provides direct access
 * to virtual memory allocation via mmap (on Unix-like systems) or VirtualAlloc (on Windows).
 * It enables fine-grained memory control.
 *
 * Supports optional stack-specific mappings (e.g., MAP_STACK on Linux) via template toggle,
 * and offers a consistent interface for allocation and deallocation across platforms.
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
    #define NOMINMAX
    #include <windows.h>

#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #include <sys/mman.h>
    #include <unistd.h>

    #if defined(__linux__) && !defined(MAP_STACK)
        #define QENTEM_LINUX_MAP_STACK 0x20000
    #else
        #define QENTEM_LINUX_MAP_STACK MAP_STACK
    #endif
#else
    // Unknown or freestanding platform — enable malloc fallback
    #define QENTEM_SYSTEM_MEMORY_FALLBACK 1
#endif

#ifndef QENTEM_FALLBACK_SYSTEM_PAGE_SIZE
    #define QENTEM_FALLBACK_SYSTEM_PAGE_SIZE 4096
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
#if defined(_WIN32)
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        static const SystemIntType page_size = static_cast<SystemIntType>(info.dwPageSize);
#elif defined(_SC_PAGESIZE) || defined(_SC_PAGE_SIZE)
        static const SystemIntType page_size = static_cast<SystemIntType>(::sysconf(
#if defined(_SC_PAGESIZE)
            _SC_PAGESIZE
#else
            _SC_PAGE_SIZE
#endif
            ));
#else
        // Fallback for freestanding
        static constexpr SystemIntType page_size = QENTEM_FALLBACK_SYSTEM_PAGE_SIZE;
#endif

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
#if defined(_WIN32)
        DWORD old_protect;
        return (::VirtualProtect(ptr, static_cast<SystemIntType>(size), PAGE_NOACCESS, &old_protect) != 0);
#elif defined(_SC_PAGESIZE) || defined(_SC_PAGE_SIZE)
        return (::mprotect(ptr, static_cast<SystemIntType>(size), PROT_NONE) == 0);
#else
        return false;
#endif
    }

    /**
     * @brief Allocates page-aligned memory suitable for general-purpose use.
     *
     * The returned memory is readable and writable and does not include guard pages.
     *
     * @param size Number of bytes to allocate. Should be a multiple of the system page size.
     * @return Pointer to memory on success, or nullptr on failure.
     */
    static void *Allocate(SystemIntType size) noexcept {
        return allocate<false>(size);
    }

    /**
     * @brief Allocates page-aligned memory intended for use as a stack.
     *
     * On Linux, this enables the MAP_STACK flag (if available) to inform the kernel
     * that the memory will be used as a downward-growing stack. No guard page is applied.
     *
     * @param size Number of bytes to allocate. Should be a multiple of the system page size.
     * @return Pointer to memory on success, or nullptr on failure.
     */
    static void *AllocateStack(SystemIntType size) noexcept {
        return allocate<true>(size);
    }

    /**
     * @brief Frees memory previously allocated with Allocate().
     *
     * @param ptr  Pointer returned by Allocate().
     * @param size Size in bytes (same value passed to Allocate).
     */
    static void Free(void *ptr, SystemIntType size) noexcept {
#if defined(_WIN32)
        ::VirtualFree(ptr, 0, MEM_RELEASE);
#elif defined(_SC_PAGESIZE) || defined(_SC_PAGE_SIZE)
        ::munmap(ptr, size);
#else
        // Size ignored when using malloc-based fallback.
        (void)size;
        __builtin_free(ptr);
#endif
    }

    /**
     * @brief Releases a range of memory pages previously allocated by Allocate().
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
#if defined(_WIN32)
        // Always hard-unmap on Windows — madvise not applicable
        ::VirtualFree(start, 0, MEM_RELEASE);
#elif defined(_SC_PAGESIZE) || defined(_SC_PAGE_SIZE)
        if (reclaim) {
            ::munmap(start, size);
        } else {
            ::madvise(start, size, MADV_DONTNEED);
        }
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
     * @param size Number of bytes to allocate. Should be a multiple of the system page size.
     * @return Pointer to allocated memory, or nullptr on failure.
     */
    template <bool IS_STACK_MEMORY_T>
    static void *allocate(SystemIntType size) noexcept {
#if defined(_WIN32)
        return ::VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#elif defined(_SC_PAGESIZE) || defined(_SC_PAGE_SIZE)
        constexpr auto flags = MAP_PRIVATE | MAP_ANONYMOUS | (IS_STACK_MEMORY_T ? QENTEM_LINUX_MAP_STACK : 0);
        return ::mmap(nullptr, size, PROT_READ | PROT_WRITE, flags, -1, 0);
#else
        return __builtin_malloc(size);
#endif
    }
};

} // namespace Qentem

#endif // QENTEM_SYSTEM_MEMORY_HPP
