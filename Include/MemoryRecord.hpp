
#ifndef QENTEM_MEMORY_RECORD_HPP
#define QENTEM_MEMORY_RECORD_HPP

#include "QCommon.hpp"

namespace Qentem {

struct MemoryRecord {
    struct MemoryRecordData {
        SystemLong Reserved{0};
        SystemLong Released{0};
        SystemLong SubReserved{0};
        SystemLong subReleased{0};
        SystemLong Size{0};
        SystemLong PeakSize{0};
        SystemLong Blocks{0};
        SystemLong BlocksTotalSize{0};
    };

    MemoryRecord()                                = delete;
    ~MemoryRecord()                               = delete;
    MemoryRecord(MemoryRecord &&)                 = delete;
    MemoryRecord(const MemoryRecord &)            = delete;
    MemoryRecord &operator=(MemoryRecord &&)      = delete;
    MemoryRecord &operator=(const MemoryRecord &) = delete;

    QENTEM_NOINLINE static void EraseMemoryRecord() noexcept {
        GetRecord() = MemoryRecordData{};
    }

    QENTEM_NOINLINE static void EraseSubMemoryRecord() noexcept {
        static MemoryRecordData &storage = GetRecord();

        storage.SubReserved = 0;
        storage.subReleased = 0;
    }

    QENTEM_NOINLINE static SystemLong CheckSubRecord() noexcept {
        static const MemoryRecordData &storage = GetRecord();

        return (storage.SubReserved - storage.subReleased);
    }

    QENTEM_NOINLINE static void Reserved(SystemLong size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Reserved);
        ++(storage.SubReserved);

        storage.Size += size;

        if (storage.Size > storage.PeakSize) {
            storage.PeakSize = storage.Size;
        }
    }

    QENTEM_NOINLINE static void Shrink(SystemLong size) noexcept {
        GetRecord().Size -= size;
    }

    QENTEM_NOINLINE static void Expand(SystemLong size) noexcept {
        GetRecord().Size += size;
    }

    QENTEM_NOINLINE static void Released(SystemLong size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Released);
        ++(storage.subReleased);

        storage.Size -= size;
    }

    QENTEM_NOINLINE static void ReservedBlock(SystemLong size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Blocks);
        storage.BlocksTotalSize += size;
    }

    QENTEM_NOINLINE static void ReleasedBlock(SystemLong size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        --(storage.Blocks);
        storage.BlocksTotalSize -= size;
    }

    QENTEM_NOINLINE static MemoryRecordData &GetRecord() noexcept {
        static MemoryRecordData data{};

        return data;
    }
};

} // namespace Qentem

#endif // QENTEM_MEMORY_RECORD_HPP
