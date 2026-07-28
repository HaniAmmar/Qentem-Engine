/**
 * @file SystemAuxv.hpp
 * @brief Low-level Linux interface for querying process auxiliary vector entries.
 *
 * Defines Qentem::SystemAuxv — a minimal utility for retrieving AT_* values
 * directly from /proc/self/auxv using raw system calls, without relying on
 * libc-provided interfaces such as getauxval().
 *
 * The auxiliary vector contains kernel-provided process information including
 * page size, program headers, vDSO location, hardware capabilities, and other
 * runtime parameters established during process startup.
 *
 * Entries are read and scanned in small batches to reduce syscall overhead
 * while correctly handling partial reads and preserving portability across
 * supported Linux architectures.
 *
 * @copyright Copyright (c) 2026 Hani Ammar
 * @license MIT
 */

#ifndef QENTEM_SYSTEM_AUXV_HPP
#define QENTEM_SYSTEM_AUXV_HPP

#if defined(__linux__)

#include "Qentem/SystemCall.hpp"
#include "Qentem/LinuxConstants.hpp"

namespace Qentem {

struct SystemAuxv {
    QENTEM_NOINLINE static bool Scan(SystemLong target_type, SystemLong &out_value) noexcept {
        constexpr const char *AUXV_PATH = "/proc/self/auxv";
        bool                  found     = false;

        struct AUXInfo {
            SystemLong Type;
            SystemLong Value;
        } aux[4];

        constexpr SizeT32 aux_size = sizeof(AUXInfo);

        const int fd = static_cast<int>(
            SystemCall(__NR_openat, Q_AT_FDCWD, reinterpret_cast<SystemLongI>(AUXV_PATH), Q_RDONLY, 0));

        if (fd >= 0) {
            unsigned char *ptr    = reinterpret_cast<unsigned char *>(&aux);
            SizeT32        filled = 0;

            while (!found) {
                const SystemLongI ret =
                    SystemCall(__NR_read, fd, reinterpret_cast<SystemLongI>(ptr + filled), (sizeof(aux) - filled));

                if (ret > 0) {
                    SizeT32 index = 0;
                    filled += static_cast<SizeT32>(ret);

                    while (filled >= aux_size) {
                        if (aux[index].Type == target_type) {
                            out_value = aux[index].Value;
                            found     = true;
                            break;
                        }

                        // Consume processed record
                        filled -= aux_size;
                        ++index;
                    }

                    if ((filled != 0) && !found) {
                        SizeT32 offset = 0;
                        // Leftover bytes start immediately after the last fully processed record
                        SizeT32 src_index = (index * aux_size);

                        if (src_index != 0) {
                            while (offset < filled) {
                                ptr[offset] = ptr[src_index];
                                ++src_index;
                                ++offset;
                            }
                        }
                    }

                    continue;
                }

                // EOF or error — stop
                break;
            }

            SystemCall(__NR_close, fd);
        }

        return found;
    }
};

} // namespace Qentem

#endif
#endif
