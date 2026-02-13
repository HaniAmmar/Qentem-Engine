
#ifndef QENTEM_MEMORY_RECORD_HPP
#define QENTEM_MEMORY_RECORD_HPP

#include "Qentem/QCommon.hpp"

namespace Qentem {

struct MemoryRecordData {
    MemoryRecordData()                                    = default;
    ~MemoryRecordData()                                   = default;
    MemoryRecordData(MemoryRecordData &&)                 = default;
    MemoryRecordData(const MemoryRecordData &)            = default;
    MemoryRecordData &operator=(MemoryRecordData &&)      = default;
    MemoryRecordData &operator=(const MemoryRecordData &) = default;

    SystemLong Reserved{0};
    SystemLong Released{0};
    SystemLong SubReserved{0};
    SystemLong subReleased{0};
    SystemLong Size{0};
    SystemLong PeakSize{0};
    SystemLong Blocks{0};
    SystemLong BlocksTotalSize{0};
};

struct MemoryRecord {
    MemoryRecord()  = default;
    ~MemoryRecord() = default;

    MemoryRecord(MemoryRecord &&)                 = delete;
    MemoryRecord(const MemoryRecord &)            = delete;
    MemoryRecord &operator=(MemoryRecord &&)      = delete;
    MemoryRecord &operator=(const MemoryRecord &) = delete;

    QENTEM_NOINLINE static void EraseMemoryRecord() noexcept {
        storage_ = MemoryRecordData{};
    }

    QENTEM_NOINLINE static void EraseSubMemoryRecord() noexcept {
        storage_.SubReserved = 0;
        storage_.subReleased = 0;
    }

    QENTEM_NOINLINE static SystemLong CheckSubRecord() noexcept {
        return (storage_.SubReserved - storage_.subReleased);
    }

    QENTEM_NOINLINE static void Reserved(SystemLong size) noexcept {
        ++(storage_.Reserved);
        ++(storage_.SubReserved);

        storage_.Size += size;

        if (storage_.Size > storage_.PeakSize) {
            storage_.PeakSize = storage_.Size;
        }
    }

    QENTEM_NOINLINE static void Shrink(SystemLong size) noexcept {
        storage_.Size -= size;
    }

    QENTEM_NOINLINE static void Expand(SystemLong size) noexcept {
        storage_.Size += size;
    }

    QENTEM_NOINLINE static void Released(SystemLong size) noexcept {
        ++(storage_.Released);
        ++(storage_.subReleased);

        storage_.Size -= size;
    }

    QENTEM_NOINLINE static void ReservedBlock(SystemLong size) noexcept {
        ++(storage_.Blocks);
        storage_.BlocksTotalSize += size;
    }

    QENTEM_NOINLINE static void ReleasedBlock(SystemLong size) noexcept {
        --(storage_.Blocks);
        storage_.BlocksTotalSize -= size;
    }

    QENTEM_NOINLINE static MemoryRecordData &GetRecord() noexcept {
        return storage_;
    }

    inline static MemoryRecordData storage_{};
};

} // namespace Qentem

#endif // QENTEM_MEMORY_RECORD_HPP
