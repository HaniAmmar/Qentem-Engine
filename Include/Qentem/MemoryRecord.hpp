/**
 * @file MemoryRecord.hpp
 * @brief Thread-local memory allocation statistics and tracking utilities.
 *
 * Provides lightweight thread-local counters for tracking memory allocations,
 * releases, active memory usage, peak usage, and reserved memory blocks.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_MEMORY_RECORD_HPP
#define QENTEM_MEMORY_RECORD_HPP

#include "Qentem/QCommon.hpp"

namespace Qentem {

/**
 * @brief Thread-local memory statistics.
 *
 * Stores allocation, release, usage, and block tracking counters for a
 * single thread.
 */
struct MemoryRecordData {
    SystemLong Reserved{0};
    SystemLong Released{0};
    SystemLong SubReserved{0};
    SystemLong SubReleased{0};
    SystemLong Size{0};
    SystemLong PeakSize{0};
    SystemLong Blocks{0};
    SystemLong BlocksTotalSize{0};
};

/**
 * @brief Thread-local memory usage recorder.
 *
 * Provides static functions for recording allocation activity and querying
 * per-thread memory statistics.
 */
struct MemoryRecord {
    MemoryRecord()                                = delete;
    ~MemoryRecord()                               = delete;
    MemoryRecord(MemoryRecord &&)                 = delete;
    MemoryRecord(const MemoryRecord &)            = delete;
    MemoryRecord &operator=(MemoryRecord &&)      = delete;
    MemoryRecord &operator=(const MemoryRecord &) = delete;

    /**
     * @brief Record a reserved memory block.
     *
     * @param size Block size in bytes.
     */
    QENTEM_NOINLINE static void ReservedBlock(SystemLong size) noexcept {
        ++(storage_.Blocks);
        storage_.BlocksTotalSize += size;
    }

    /**
     * @brief Record a released memory block.
     *
     * @param size Block size in bytes.
     */
    QENTEM_NOINLINE static void ReleasedBlock(SystemLong size) noexcept {
        --(storage_.Blocks);
        storage_.BlocksTotalSize -= size;
    }

    /**
     * @brief Record a memory allocation.
     *
     * @param size Allocated size in bytes.
     */
    QENTEM_NOINLINE static void Reserved(SystemLong size) noexcept {
        ++(storage_.Reserved);
        ++(storage_.SubReserved);

        storage_.Size += size;

        if (storage_.Size > storage_.PeakSize) {
            storage_.PeakSize = storage_.Size;
        }
    }

    /**
     * @brief Record a memory size reduction.
     *
     * @param size Reduced size in bytes.
     */
    QENTEM_NOINLINE static void Shrink(SystemLong size) noexcept {
        storage_.Size -= size;
    }

    /**
     * @brief Record a memory size increase.
     *
     * @param size Increased size in bytes.
     */
    QENTEM_NOINLINE static void Expand(SystemLong size) noexcept {
        storage_.Size += size;

        if (storage_.Size > storage_.PeakSize) {
            storage_.PeakSize = storage_.Size;
        }
    }

    /**
     * @brief Record a memory release.
     *
     * @param size Released size in bytes.
     */
    QENTEM_NOINLINE static void Released(SystemLong size) noexcept {
        ++(storage_.Released);
        ++(storage_.SubReleased);

        storage_.Size -= size;
    }

    /**
     * @brief Get the current thread's memory statistics.
     *
     * @return Reference to the thread-local memory record.
     */
    QENTEM_NOINLINE static MemoryRecordData &GetRecord() noexcept {
        return storage_;
    }

    /**
     * @brief Get the current sub-record allocation balance.
     *
     * @return Number of allocations not yet released.
     */
    QENTEM_NOINLINE static SystemLong CheckSubRecord() noexcept {
        return (storage_.SubReserved - storage_.SubReleased);
    }

    /**
     * @brief Reset all memory statistics.
     */
    QENTEM_NOINLINE static void EraseMemoryRecord() noexcept {
        storage_ = MemoryRecordData{};
    }

    /**
     * @brief Reset sub-record allocation statistics.
     */
    QENTEM_NOINLINE static void EraseSubMemoryRecord() noexcept {
        storage_.SubReserved = 0;
        storage_.SubReleased = 0;
    }

    inline static thread_local MemoryRecordData storage_{};
};

} // namespace Qentem

#endif
