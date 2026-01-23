/**
 * @file CPUHelper.hpp
 * @brief Low-level utilities for querying CPU topology and managing thread affinity.
 *
 * This header defines the `CPUHelper` struct, providing functions to:
 *   - Retrieve the number of online logical CPU cores.
 *   - Pin threads to specific cores for fine-grained control.
 *
 * These utilities are particularly useful in systems requiring per-core
 * affinity, such as high-performance thread pools, fiber schedulers,
 * or NUMA-aware applications.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_CPU_HELPER_HPP
#define QENTEM_CPU_HELPER_HPP

#include "Qentem/Platform.hpp"

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h> // GetSystemInfo
#elif defined(__linux__)
#include "Qentem/SystemCall.hpp"
#else
#include <sched.h>  // sched_setaffinity
#include <unistd.h> // sysconf
#endif

namespace Qentem {

struct CPUSet {
    static constexpr SystemLong MAX_CORE  = QENTEM_MAX_CPU_CORES;
    static constexpr SystemLong BIT_WIDTH = (sizeof(SystemLong) * SystemLong{8});
    static constexpr SystemLong SIZE      = ((MAX_CORE + (BIT_WIDTH - 1U)) / BIT_WIDTH);
    static constexpr SizeT32    SHIFT     = (BIT_WIDTH == SystemLong{64} ? 6U : 5U);

    QENTEM_INLINE void Clear() noexcept {
        SystemLong index = SIZE;

        while (index != 0) {
            --index;
            mask_[index] = 0;
        }
    }

    QENTEM_INLINE void Set(SystemLong core) noexcept {
        if (core < MAX_CORE) {
            SystemLong index;
            SystemLong offset;

            setIndexOffset(index, offset, core);

            mask_[index] |= (SystemLong{1} << offset);
        }
    }

    QENTEM_INLINE void Reset(SystemLong core) noexcept {
        if (core < MAX_CORE) {
            SystemLong index;
            SystemLong offset;

            setIndexOffset(index, offset, core);

            mask_[index] &= ~(SystemLong{1} << offset);
        }
    }

    QENTEM_INLINE bool IsSet(SystemLong core) const noexcept {
        if (core < MAX_CORE) {
            SystemLong index;
            SystemLong offset;

            setIndexOffset(index, offset, core);

            return (mask_[index] & (SystemLong{1} << offset)) != 0;
        }

        return false;
    }

    QENTEM_INLINE const SystemLong *Data() const noexcept {
        return &(mask_[0]);
    }

    QENTEM_INLINE constexpr SystemLong Size() const noexcept {
        return SIZE;
    }

    QENTEM_INLINE constexpr SystemLong TotalBytes() const noexcept {
        return (SIZE * sizeof(SystemLong));
    }

  private:
    QENTEM_INLINE static void setIndexOffset(SystemLong &index, SystemLong &offset, SystemLong core) {
        index  = (core >> SHIFT);
        offset = (core & (BIT_WIDTH - 1U));
    }

    SystemLong mask_[SIZE]{};
};

struct CPUHelper {
    /**
     * @brief Returns the number of logical CPU cores available to the process.
     *
     * This method queries the system once and caches the result for future calls.
     * On Linux, it returns the number of logical cores currently available to this
     * process according to its CPU affinity (via `sched_getaffinity`).
     * On Windows, it returns the total number of logical cores in the system,
     * across all processor groups, regardless of the calling thread's affinity.
     *
     * @return Number of logical CPU cores. Guaranteed to be at least 1.
     */
    QENTEM_INLINE static SizeT32 GetCoreCount() noexcept {
        return core_count_;
    }

    /**
     * @brief Retrieves the current CPU core ID the calling thread is running on.
     *
     * This function provides a cross-platform method to query the currently active
     * CPU core of the calling thread. It may return different results over time
     * if the thread is not pinned and is scheduled to run on various cores.
     *
     * @return The zero-based index of the current CPU core, or 0 if unsupported.
     */
    static SizeT32 GetCurrentCore() noexcept {
#if defined(__linux__)
        long core = 0;

        SystemCall(__NR_getcpu, reinterpret_cast<long>(&core), 0, 0);

        return static_cast<SizeT32>(core);
#elif defined(_WIN32)
        // Uses Windows API to get the index of the processor the thread is running on
        return static_cast<SizeT32>(::GetCurrentProcessorNumber());
#else
        return 0;
#endif
    }

    /**
     * @brief Pins the calling thread to a specific logical CPU core.
     *
     * This function enforces CPU affinity so that the current thread will execute
     * exclusively on the designated logical core. Core indices begin at 0.
     *
     * - **Linux:** Uses `sched_setaffinity()` with a `cpu_set_t` mask.
     * - **Windows:** Uses `SetThreadGroupAffinity()` with a `GROUP_AFFINITY`
     *   structure, supporting processors across multiple groups.
     * - **Other platforms:** No effect; always returns `false`.
     *
     * @param core_id Zero-based index of the target logical CPU core.
     * @return `true` if affinity was successfully applied, `false` otherwise.
     */
    static bool PinToCore(SizeT32 core_id) {
#if defined(__linux__)
        CPUSet cores;
        cores.Clear();
        cores.Set(core_id);

        // 0 = current thread
        const long ret = setAffinity(0, cores.TotalBytes(), cores.Data());

        return (ret == 0);
#elif defined(_WIN32)
        GROUP_AFFINITY affinity{};
        affinity.Mask  = (KAFFINITY(1) << (core_id % 64));
        affinity.Group = static_cast<WORD>(core_id / 64);

        return ::SetThreadGroupAffinity(GetCurrentThread(), &affinity, nullptr);
#else
        (void)core_id; // Platform is not supported
        return false;
#endif
    }

    /**
     * @brief Retrieves the affinity mask of online CPU cores.
     *
     * Queries the operating system for the set of logical cores available
     * to the calling thread. The resulting mask indicates which cores are
     * currently online and eligible for scheduling.
     *
     * On Linux, this invokes the `sched_getaffinity` system call.
     * On unsupported platforms, the mask will remain cleared and the
     * function will return `false`.
     *
     * @param mask Reference to a CPUSet that will be populated with the
     *             online core mask.
     * @return `true` if the query succeeds and the mask is populated,
     *         `false` otherwise.
     */
    QENTEM_INLINE static bool OnlineCoresMask(CPUSet &mask) noexcept {
#if defined(__linux__)
        if (SystemCall(__NR_sched_getaffinity, 0, mask.TotalBytes(), reinterpret_cast<long>(mask.Data())) >= 0) {
            return true;
        }
#else
        (void)mask;
#endif
        return false;
    }

  private:
    /**
     * @brief Set the CPU affinity mask for a process or thread.
     *
     * This function directly invokes the Linux `sched_setaffinity` system call,
     * binding the process (or thread, if `pid` is a thread ID) to the CPUs
     * specified in `mask`.
     *
     * @param pid         Target process ID (0 applies to the calling thread).
     * @param cpusetsize  Size of the CPU mask in bytes (typically `sizeof(cpu_set_t)`).
     * @param mask        Pointer to the CPU set bitmask indicating allowed CPUs.
     */
    QENTEM_INLINE static long setAffinity(int pid, long cpusetsize, const void *mask) noexcept {
#if defined(__linux__)
        return SystemCall(__NR_sched_setaffinity, pid, cpusetsize, reinterpret_cast<long>(mask));
#else
        (void)pid;
        (void)cpusetsize;
        (void)mask;
        return -1;
#endif
    }

    /**
     * @brief Returns the number of available CPU cores for this process.
     *
     * Detects the number of logical processors using platform-appropriate APIs:
     *
     * @return Number of logical CPU cores (at least 1).
     */
    static SizeT32 coreCount() noexcept {
#if defined(__linux__)
        const SizeT32 count = onlineCoresCount();
#elif defined(_WIN32)
        const SizeT32 count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
#else
        const SizeT32 tmp   = static_cast<SizeT32>(::sysconf(_SC_NPROCESSORS_ONLN));
        const SizeT32 count = ((tmp > 0) ? static_cast<SizeT32>(tmp) : 1U);
#endif

        return count;
    }

    /**
     * @brief Counts the number of currently online CPU cores on Linux.
     *
     * This function queries the kernel for the CPU affinity mask of the current process
     * using `sched_getaffinity`. It then iterates through the CPUSet, summing the
     * number of bits set to determine how many cores are currently available.
     *
     * Each set bit corresponds to a logical CPU core the process is allowed to run on.
     * Returns 1 if the affinity query fails or no cores are detected.
     *
     * @return Number of online logical cores available to the process (>= 1).
     */
    QENTEM_INLINE static SizeT32 onlineCoresCount() noexcept {
        CPUSet cores{};
        cores.Clear();

        if (OnlineCoresMask(cores)) {
            SizeT32 count = 0;
            SizeT32 index = 0;

            while (index < cores.Size()) {
                if (cores.Data()[index] != 0) {
                    count += Platform::PopCount(cores.Data()[index]);
                }

                ++index;
            }

            return count;
        }

        return 1;
    }

    inline static const SizeT32 core_count_{coreCount()};
};

} // namespace Qentem

#endif // QENTEM_CPU_HELPER_HPP
