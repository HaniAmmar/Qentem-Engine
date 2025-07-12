

#ifndef QENTEM_CONSOLE_H
#define QENTEM_CONSOLE_H

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "ToCharsHelper.hpp"
#include "LiteStream.hpp"

namespace Qentem {

struct QConsole {
    QConsole()                            = delete;
    ~QConsole()                           = delete;
    QConsole(QConsole &&)                 = delete;
    QConsole(const QConsole &)            = delete;
    QConsole &operator=(QConsole &&)      = delete;
    QConsole &operator=(const QConsole &) = delete;

    enum struct Colors : SizeT8 { TitleColor, ErrorColor, PassColor, EndColor };

    QENTEM_NOINLINE static bool &IsColored() noexcept {
        static bool isColored{true};

        return isColored;
    }

    QENTEM_NOINLINE static const char *GetColor(Colors color) noexcept {
        if (IsColored()) {
            switch (color) {
                case Colors::TitleColor:
                    return "\x1B[36m";
                case Colors::ErrorColor:
                    return "\x1B[31m";
                case Colors::PassColor:
                    return "\x1B[32m";
                case Colors::EndColor:
                    return "\x1B[0m";
            }
        }

        return "";
    }

    template <typename... Values_T>
    QENTEM_NOINLINE static void Print(const Values_T &...values) {
        LiteStream &ss = GetStreamCache();
        ToCharsHelper::Write(ss, values...);

        if (IsOutputEnabled()) {
            // fwrite(ss.First(), 1, ss.Length(), stdout);
            if (ss.Length() >= SizeT{512}) {
                RawWrite(ss.First(), ss.Length());
                ss.Clear();
            }
        }
    }

    QENTEM_NOINLINE static void Flush() noexcept {
        LiteStream &ss = GetStreamCache();

        if (ss.Length() != 0) {
            RawWrite(ss.First(), ss.Length());
            ss.Clear();
        }
    }

    QENTEM_NOINLINE static void Clear() noexcept {
        GetStreamCache().Clear();
    }

    QENTEM_NOINLINE static void DisableOutput() noexcept {
        getOutputEnabledRef() = false;
    }

    QENTEM_NOINLINE static void EnableOutput() noexcept {
        getOutputEnabledRef() = true;
    }

    QENTEM_NOINLINE static bool IsOutputEnabled() noexcept {
        return getOutputEnabledRef();
    }

    // QENTEM_NOINLINE static void SetDoubleFormat() noexcept {
    // }

    // QENTEM_NOINLINE static void ResetDoubleFormat() noexcept {
    // }

    QENTEM_NOINLINE static void RawWrite(const char *data, unsigned length) noexcept {
#if defined(_WIN32)
        DWORD written = 0;
        ::WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), data, length, &written, nullptr);
#else
        ::write(1, data, length); // stdout
#endif
    }

    QENTEM_NOINLINE static LiteStream &GetStreamCache() noexcept {
        static LiteStream ss{32};

        struct OnExit {
            ~OnExit() {
                Flush(); // Now safe
            }
        };

        static OnExit oe;

        return ss;
    }

  private:
    QENTEM_NOINLINE static bool &getOutputEnabledRef() noexcept {
        static bool enable_output_ = true;

        return enable_output_;
    }
};

} // namespace Qentem

#endif
