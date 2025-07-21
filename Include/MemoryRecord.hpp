
#ifndef QENTEM_MEMORY_RECORD_HPP
#define QENTEM_MEMORY_RECORD_HPP

#include "QCommon.hpp"

namespace Qentem {

struct MemoryRecord {
    struct MemoryRecordData {
        SystemIntType Reserved{0};
        SystemIntType Released{0};
        SystemIntType SubReserved{0};
        SystemIntType subReleased{0};
        SystemIntType Size{0};
        SystemIntType PeakSize{0};
        SystemIntType Blocks{0};
        SystemIntType BlocksTotalSize{0};
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

    QENTEM_NOINLINE static SystemIntType CheckSubRecord() noexcept {
        static const MemoryRecordData &storage = GetRecord();

        return (storage.SubReserved - storage.subReleased);
    }

    QENTEM_NOINLINE static void Reserved(SystemIntType size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Reserved);
        ++(storage.SubReserved);

        storage.Size += size;

        if (storage.Size > storage.PeakSize) {
            storage.PeakSize = storage.Size;
        }
    }

    QENTEM_NOINLINE static void Released(SystemIntType size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Released);
        ++(storage.subReleased);

        storage.Size -= size;
    }

    QENTEM_NOINLINE static void ReservedBlock(SystemIntType size) noexcept {
        static MemoryRecordData &storage = GetRecord();

        ++(storage.Blocks);
        storage.BlocksTotalSize += size;
    }

    QENTEM_NOINLINE static void ReleasedBlock(SystemIntType size) noexcept {
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
