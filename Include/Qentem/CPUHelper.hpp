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
#include "Qentem/Digit.hpp"

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
    static constexpr SizeT32 MAX_CORE     = QENTEM_MAX_CPU_CORES;
    static constexpr SizeT32 BIT_WIDTH    = (sizeof(SystemLong) * 8U);
    static constexpr SizeT32 BIT_WIDTH_M1 = (BIT_WIDTH - 1U);
    static constexpr SizeT32 SIZE         = ((QENTEM_MAX_CPU_CORES + (BIT_WIDTH - 1U)) / BIT_WIDTH);
    // static constexpr SizeT32 SHIFT        = Platform::FindFirstBit(BIT_WIDTH);
    static constexpr SizeT32 SHIFT = (BIT_WIDTH == SystemLong{64} ? 6U : 5U);

    QENTEM_INLINE CPUSet() noexcept : mask_{} {
    }

    QENTEM_INLINE void Clear() noexcept {
        SizeT32 index = SIZE;

        while (index != 0) {
            --index;
            mask_[index] = 0;
        }
    }

    QENTEM_INLINE void Set(SizeT32 core) noexcept {
        if (core < MAX_CORE) {
            SizeT32 index;
            SizeT32 offset;

            setIndexOffset(index, offset, core);

            mask_[index] |= (SystemLong{1} << offset);
        }
    }

    QENTEM_INLINE void Reset(SizeT32 core) noexcept {
        if (core < MAX_CORE) {
            SizeT32 index;
            SizeT32 offset;

            setIndexOffset(index, offset, core);

            mask_[index] &= ~(SystemLong{1} << offset);
        }
    }

    QENTEM_INLINE bool IsSet(SizeT32 core) const noexcept {
        if (core < MAX_CORE) {
            SizeT32 index;
            SizeT32 offset;

            setIndexOffset(index, offset, core);

            return ((mask_[index] & (SystemLong{1} << offset)) != 0);
        }

        return false;
    }

    QENTEM_INLINE const SystemLong *Data() const noexcept {
        return &(mask_[0]);
    }

    QENTEM_INLINE constexpr SizeT32 Size() const noexcept {
        return SIZE;
    }

    QENTEM_INLINE constexpr SizeT32 TotalBytes() const noexcept {
        return (SIZE * sizeof(SystemLong));
    }

  private:
    QENTEM_INLINE static void setIndexOffset(SizeT32 &index, SizeT32 &offset, SizeT32 core) {
        index  = (core >> SHIFT);
        offset = (core & BIT_WIDTH_M1);
    }

    SystemLong mask_[SIZE];
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
        CPUSet cores{};
        cores.Clear();
        cores.Set(core_id);

        // 0 = current thread
        const long ret = SetAffinity(0, cores.TotalBytes(), cores.Data());

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

    /**
     * @brief Parses a numeric CPU range list into a validated CPU bitmask array.
     *
     * This function reads a textual list of CPU indices and ranges from
     * @p content and sets the corresponding bits in @p list, **only if**
     * the referenced CPUs are present in the system online-core mask.
     *
     * The input format supports:
     * - Single values:        "3"
     * - Comma-separated:     "1,4,7"
     * - Ranges with '-':     "2-5"
     * - Arbitrary spacing:   "1, 3- 7 , 9"
     *
     * Each parsed CPU index is mapped to a bit position inside the array,
     * where each element of the array represents a word-sized bitmask
     * (as defined by CPUSet::SHIFT and CPUSet::BIT_WIDTH_M1).
     *
     * Example:
     *   "1,3-5,64,65-69"
     * expands to:
     *   1,3,4,5,64,65,66,67,68,69
     *
     * The function validates the input while parsing:
     * - Returns false if a malformed range is detected (e.g. "7-3").
     * - Returns false if an unexpected character is encountered.
     * - Returns false if any referenced CPU index is not marked online
     *   in the system CPU mask.
     *
     * @tparam Array_SystemLong_T  Container type holding SystemLong values.
     *                            Must provide:
     *                            - Size()
     *                            - ResizeInit(SizeT, ...)
     *                            - Storage()
     *
     * @param list     Destination array that receives the resulting bitmask.
     * @param content  Pointer to the character buffer containing the range text.
     * @param length   Length of the character buffer.
     *
     * @return true if the entire input string was parsed successfully and all
     *         referenced CPUs are valid and online,
     *         false if a syntax error, range error, or invalid CPU id is detected.
     */
    template <typename Array_SystemLong_T, typename Char_T>
    static bool RangeToArray(Array_SystemLong_T &list, const Char_T *content, SizeT length,
                             CPUSet &online_cores) noexcept {
        SystemLong bit;
        SizeT      offset{0};
        SizeT      index;

        auto checkArray = [&](SystemLong id) {
            index = static_cast<SizeT>(id >> CPUSet::SHIFT);

            if (list.Size() <= index) {
                list.ResizeInit(index + SizeT{1}, SystemLong{0});
            }
        };

        while (offset < length) {
            while ((offset < length) && ((content[offset] == ' ') || (content[offset] == ','))) {
                ++offset;
            }

            if (offset < length) {
                SizeT start = offset;

                while ((offset < length) && (content[offset] >= DigitUtils::DigitChar::Zero) &&
                       (content[offset] <= DigitUtils::DigitChar::Nine)) {
                    ++offset;
                }

                SystemLong id;
                Digit::FastStringToNumber(id, (content + start), (offset - start));

                checkArray(id);

                bit = (SystemLong{1} << (id & CPUSet::BIT_WIDTH_M1));

                if ((online_cores.Data()[index] & bit) != bit) {
                    return false;
                }

                list.Storage()[index] |= bit;

                if (offset < length) {
                    while ((offset < length) && (content[offset] != ',') && (content[offset] != '-')) {
                        ++offset;
                    }

                    if (offset < length) {
                        if (content[offset] == '-') {
                            // range
                            do {
                                ++offset;
                            } while ((offset < length) && (content[offset] == ' '));

                            start = offset;

                            while ((offset < length) && (content[offset] >= DigitUtils::DigitChar::Zero) &&
                                   (content[offset] <= DigitUtils::DigitChar::Nine)) {
                                ++offset;
                            }

                            SystemLong id2;
                            Digit::FastStringToNumber(id2, (content + start), (offset - start));

                            if (id2 > id) {
                                checkArray(id2);

                                do {
                                    ++id;
                                    index = static_cast<SizeT>(id >> CPUSet::SHIFT);

                                    bit = (SystemLong{1} << (id & CPUSet::BIT_WIDTH_M1));

                                    if ((online_cores.Data()[index] & bit) != bit) {
                                        return false;
                                    }

                                    list.Storage()[index] |= bit;
                                } while (id < id2);

                                if (offset < length) {
                                    continue;
                                }

                                return true;
                            }

                            return false;
                        }

                        if (content[offset] == ',') {
                            ++offset;
                            continue;
                        }

                        return false;
                    }
                }

                return true;
            }
        }

        return false;
    }

    /**
     * @brief Set the CPU affinity mask for a process or thread.
     *
     * This function directly invokes the Linux `sched_setaffinity` system call,
     * binding the process (or thread, if `pid` is a thread ID) to the CPUs
     * specified in `mask`.
     *
     * @param pid         Target process ID (0 applies to the calling thread).
     * @param cpusetsize  Size of the CPU mask in bytes.
     * @param mask        Pointer to the CPU set bitmask indicating allowed CPUs.
     */
    QENTEM_INLINE static long SetAffinity(int pid, SystemLong cpusetsize, const void *mask) noexcept {
#if defined(__linux__)
        return SystemCall(__NR_sched_setaffinity, pid, cpusetsize, reinterpret_cast<long>(mask));
#else
        (void)pid;
        (void)cpusetsize;
        (void)mask;
        return -1;
#endif
    }

  private:
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
            SizeT32 index = cores.Size();
            SizeT32 count = 0;

            do {
                --index;

                if (cores.Data()[index] != 0) {
                    count += Platform::PopCount(cores.Data()[index]);
                }
            } while (index != 0);

            return count;
        }

        return 1U;
    }

    inline static const SizeT32 core_count_{coreCount()};
};

} // namespace Qentem

#endif // QENTEM_CPU_HELPER_HPP
