

#ifndef QENTEM_CONSOLE_H
#define QENTEM_CONSOLE_H

#include <stdio.h>

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

    static bool &IsColored() noexcept {
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
    inline static void Print(const Values_T &...values) {
        LiteStream &ss = GetStreamCache();
        ToCharsHelper::Write(ss, values...);

        if (IsOutputEnabled()) {
            fwrite(ss.First(), 1, ss.Length(), stdout);
            ss.Clear();
        }
    }

    static void Flush() noexcept {
        fflush(stdout);
    }

    static void Clear() noexcept {
        GetStreamCache().Clear();
    }

    static void DisableOutput() noexcept {
        getOutputEnabledRef() = false;
    }

    static void EnableOutput() noexcept {
        getOutputEnabledRef() = true;
    }

    static bool IsOutputEnabled() noexcept {
        return getOutputEnabledRef();
    }

    // static void SetDoubleFormat() noexcept {
    // }

    // static void ResetDoubleFormat() noexcept {
    // }

    static LiteStream &GetStreamCache() noexcept {
        static LiteStream ss{32};

        return ss;
    }

  private:
    static bool &getOutputEnabledRef() noexcept {
        static bool enable_output_ = true;

        return enable_output_;
    }
};

} // namespace Qentem

#endif
