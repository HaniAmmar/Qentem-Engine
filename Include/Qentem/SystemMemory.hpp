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

#include "Platform.hpp"

// clang-format off
#if defined(_WIN32)
    // Prevent <windows.h> from defining min/max macros that interfere with standard headers
    #define NOMINMAX
    #include <windows.h>

#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #if defined(__linux__)
        #include "SystemCall.hpp"
        #include <linux/mman.h>
    #else
        // POSIX-style platforms: mmap, sysconf, etc.
        #include <sys/mman.h>
        #include <unistd.h>
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
    QENTEM_INLINE static SizeT32 GetPageSize() noexcept {
        return page_size_;
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
    QENTEM_INLINE static bool ProtectGuardPage(void *ptr, SystemLong size) noexcept {
        // clang-format off
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    #if defined(_WIN32)
            DWORD old_protect;
            return (::VirtualProtect(ptr, static_cast<SystemLong>(size), PAGE_NOACCESS, &old_protect) != 0);
    #else
            #if defined(__linux__)
                constexpr int no_access = 0;
                return (SystemCall(__NR_mprotect, ptr, size, no_access) == 0);
            #else
                return (::mprotect(ptr, size, PROT_NONE) == 0);
            #endif
    #endif
#else
        (void)ptr;
        (void)size;
        return false;
#endif
        // clang-format on
    }

    /**
     * @brief Reserves page-aligned memory suitable for general-purpose use.
     *
     * The returned memory is readable and writable and does not include guard pages.
     *
     * @param size Number of bytes to reserve. Should be a multiple of the system page size.
     * @return Pointer to memory on success, or nullptr on failure.
     */
    QENTEM_INLINE static void *Reserve(SystemLong size, SizeT32 flags = 0) noexcept {
        return reserve<false>(size, flags);
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
    QENTEM_INLINE static void *ReserveStack(SystemLong size, SizeT32 flags = 0) noexcept {
        return reserve<true>(size, flags);
    }

    /**
     * @brief Release memory previously acquired with Reserve().
     *
     * @param ptr  Pointer returned by Reserve().
     * @param size Size in bytes (same value passed to Reserve).
     */
    QENTEM_INLINE static void Release(void *ptr, SystemLong size) noexcept {
        // clang-format off
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    #if defined(_WIN32)
            (void)size;
            ::VirtualFree(ptr, 0, MEM_RELEASE);
    #else
        #if defined(__linux__)
            SystemCall(__NR_munmap,ptr, size);
        #else
            ::munmap(ptr, size);
        #endif
    #endif
#else
        // Size ignored when using malloc-based fallback.
        (void)size;
        __builtin_free(ptr);
#endif
        // clang-format on
    }

/**
 * @brief Releases a range of memory pages previously reserved via Reserve().
 *
 * This function is used to return memory to the operating system. The specified region
 * must be both page-aligned and a multiple of the system page size.
 *
 * On POSIX systems, `munmap()` is used to fully unmap the address space and release
 * the memory back to the kernel.
 *
 * This function is excluded on Windows platforms.
 *
 * @param start  Pointer to the start of the region (must be page-aligned).
 * @param size   Size in bytes to release (must be multiple of page size).
 */
#if !defined(_WIN32)
    QENTEM_INLINE static void ReleasePages(void *start, SystemLong size) noexcept {
        // clang-format off
    #if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
        #if defined(__linux__)
            SystemCall(__NR_munmap,start, size);
        #else
            ::munmap(start, size);
        #endif
    #else
            // Fallback mode: platform does not support release; act as no-op.
            (void)start;
            (void)size;
    #endif
        // clang-format on
    }
#endif

  private:
    /**
     * @brief Returns the native system page size (e.g., 4096 bytes on x86).
     *
     * @return Size of a memory page in bytes.
     */
    static SystemLong getPageSize() noexcept {
        // clang-format off
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    #if defined(_WIN32)
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return static_cast<SystemLong>(info.dwPageSize);
    #else
        struct {
            SystemLong Type;
            SystemLong Value;
        } aux;

        #if defined(__linux__)
            constexpr int at_fdcwd      = -100; // AT_FDCWD
            constexpr int read_only     = 0;    // O_RDONLY
            constexpr int page_size_id  = 6;    // AT_PAGESZ
            constexpr const char AUXV_PATH[] = "/proc/self/auxv";

            SystemLong page_size = QENTEM_FALLBACK_SYSTEM_PAGE_SIZE;

            const int fd = static_cast<int>(SystemCall(__NR_openat, at_fdcwd,
                                                reinterpret_cast<long>(AUXV_PATH),
                                                read_only, 0));
            if (fd >= 0) {
                unsigned char *ptr = reinterpret_cast<unsigned char*>(&aux);
                SizeT32 filled = 0;

                while (true) {
                   const SystemLongI ret = SystemCall(__NR_read, fd,
                                          reinterpret_cast<long>(ptr + filled),
                                          (sizeof(aux) - filled));

                    if (ret > 0) {
                        filled += static_cast<SizeT32>(ret);

                        if (filled == sizeof(aux)) {
                            // Got one full AUX record
                            if (aux.Type == page_size_id) {
                                page_size = aux.Value;
                                break;
                            }

                            // Reset buffer for next record
                            filled = 0;
                        }

                        continue;
                    }

                    // EOF or error — stop
                    break;
                }

                SystemCall(__NR_close, fd);
            }

            return page_size;
        #else
            return static_cast<SystemLong>(::sysconf(
                #if defined(_SC_PAGESIZE)
                            _SC_PAGESIZE
                #else
                            _SC_PAGE_SIZE
                #endif
                ));
        #endif
    #endif
#else
    // Fallback for freestanding
    return QENTEM_FALLBACK_SYSTEM_PAGE_SIZE;
#endif
        // clang-format on
    }

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
    static void *reserve(SystemLong size, SizeT32 flags) noexcept {
        // clang-format off
#if !defined(QENTEM_SYSTEM_MEMORY_FALLBACK)
    #if defined(_WIN32)
        return ::VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    #else
        #if defined(__linux__)
            // MAP_STACK was added in Linux 2.6.27; define manually if missing
            #if !defined(MAP_STACK)
                #define QENTEM_LINUX_MAP_STACK 0x20000
            #else
                #define QENTEM_LINUX_MAP_STACK MAP_STACK
            #endif

            flags |= MAP_PRIVATE | MAP_ANONYMOUS | (IS_STACK_MEMORY_T ? QENTEM_LINUX_MAP_STACK : 0);

            // if (Platform::PopCount(size) == 1) {
            //     SizeT32 index = (size >> 13U);

            //     if (index != 0) {
            //         index = Platform::FindLastBit(index);
            //     }

            //     switch (index) {
            //         case 0: {
            //             break;
            //         }

            //         case 1: // 16KiB
            //         case 2: // 32KiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_16KB;
            //             break;
            //         }

            //         case 3: // 64KiB
            //         case 4: // 128KiB
            //         case 5: // 256KiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_64KB;
            //             break;
            //         }

            //         case 6: // 512KiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_512KB;
            //             break;
            //         }

            //         case 7: // 1MiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_1MB;
            //             break;
            //         }

            //         case 8: // 2MiB
            //         case 9: // 4MiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_2MB;
            //             break;
            //         }

            //         case 10: // 8MiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_8MB;
            //             break;
            //         }

            //         case 11: // 16MiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_16MB;
            //             break;
            //         }

            //         case 12: // 32MiB
            //         {
            //             flags |= MAP_HUGETLB | MAP_HUGE_32MB;
            //             break;
            //         }

            //         default: {
            //         }
            //     }
            // }

            return reinterpret_cast<void *>(
                SystemCall(
                    #if defined(__i386__) || defined(__arm__) || defined(__ARM_EABI__)
                        __NR_mmap2,
                    #else
                        __NR_mmap,
                    #endif
                        0, // null
                        size,
                        (PROT_READ | PROT_WRITE),
                        flags,
                        -1,
                        0)
            );
        #else
            return ::mmap(nullptr, size, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS | flags), -1, 0);
        #endif
    #endif
#else
        return __builtin_malloc(size);
#endif
        // clang-format on
    }

    inline static const SizeT32 page_size_{static_cast<SizeT32>(getPageSize())};
};

} // namespace Qentem

#endif // QENTEM_SYSTEM_MEMORY_HPP
