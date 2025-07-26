/**
 * @file CPUHelper.hpp
 * @brief Provides low-level utilities for querying and managing CPU affinity.
 *
 * This header defines the CPUHelper struct, which offers STL-free, cross-platform
 * methods for retrieving the number of logical CPU cores and pinning threads to
 * specific cores. These functions are useful for systems that require per-core
 * affinity (such as thread pools, fiber schedulers, or NUMA-aware designs).
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_CPU_HELPER_HPP
#define QENTEM_CPU_HELPER_HPP

#include "QCommon.hpp" // Defines SizeT32 and other basic types

#ifdef _WIN32
#define NOMINMAX
#include <windows.h> // GetSystemInfo
#else
#include <sched.h>  // sched_setaffinity
#include <unistd.h> // sysconf
#endif

namespace Qentem {

/**
 * @brief Platform-agnostic CPU utility class.
 *
 * CPUHelper provides access to the number of available CPU cores and supports
 * thread affinity pinning. It avoids STL and standard library dependencies,
 * relying only on native system calls and headers.
 */
struct CPUHelper {
    /**
     * @brief Returns the number of logical CPU cores available to the process.
     *
     * This method queries the system once and caches the result for future calls.
     * On Linux and Unix-like systems, it uses `sysconf(_SC_NPROCESSORS_ONLN)`.
     * On Windows, it uses `GetSystemInfo()`.
     *
     * @return Number of online logical CPU cores. Guaranteed to be at least 1.
     */
    static SizeT32 GetCoreCount() noexcept {
#ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return SizeT32(sysinfo.dwNumberOfProcessors);
#else
        auto count = sysconf(_SC_NPROCESSORS_ONLN);
        return (count > 0) ? static_cast<SizeT32>(count) : 1U;
#endif
    }

    /**
     * @brief Retrieves the current CPU core ID the calling thread is running on.
     *
     * This function provides a cross-platform method to query the currently active
     * CPU core of the calling thread. It may return different results over time
     * if the thread is not pinned and is scheduled to run on various cores.
     *
     * @return The zero-based index of the current CPU core, or -1 if unsupported.
     */
    static int GetCurrentCore() noexcept {
#if defined(__linux__)
        // Uses GNU-specific syscall to get current processor
        return ::sched_getcpu();
#elif defined(_WIN32)
        // Uses Windows API to get the index of the processor the thread is running on
        return static_cast<int>(::GetCurrentProcessorNumber());
#else
        // Unsupported platform: returns -1 as a sentinel value
        return -1;
#endif
    }

    /**
     * @brief Pins the current thread to the specified CPU core.
     *
     * This function sets thread affinity so that the calling thread will execute
     * only on the given logical core. Core IDs start at 0.
     *
     * On Linux, uses `sched_setaffinity()` with a `cpu_set_t`.
     * On Windows, uses `SetThreadAffinityMask()` with a bitmask.
     * On other platforms, this function is a no-op.
     *
     * @param core_id Zero-based index of the CPU core to pin to.
     */
    static void PinToCore(SizeT32 core_id) {
#if defined(__linux__)
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);                               // Clear all bits
        CPU_SET(core_id, &cpuset);                       // Enable target core
        ::sched_setaffinity(0, sizeof(cpuset), &cpuset); // Apply affinity to calling thread
#elif defined(_WIN32)
        DWORD_PTR mask = 1ULL << core_id;                  // Create bitmask for the desired core
        ::SetThreadAffinityMask(GetCurrentThread(), mask); // Apply to current thread
#else
        (void)core_id; // Platform is not supported
#endif
    }
};

} // namespace Qentem

#endif // QENTEM_CPU_HELPER_HPP
