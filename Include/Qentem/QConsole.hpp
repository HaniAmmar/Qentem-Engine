

#ifndef QENTEM_CONSOLE_H
#define QENTEM_CONSOLE_H

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#elif defined(__linux__)
#include "SystemCall.hpp"
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

    enum struct Color : SizeT8 { TitleColor, ErrorColor, PassColor, EndColor };

    QENTEM_NOINLINE static bool &IsColored() noexcept {
        return is_colored_;
    }

    QENTEM_NOINLINE static const char *GetColor(Color color) noexcept {
        if (IsColored()) {
            switch (color) {
                case Color::TitleColor:
                    return "\x1B[36m";
                case Color::ErrorColor:
                    return "\x1B[31m";
                case Color::PassColor:
                    return "\x1B[32m";
                case Color::EndColor:
                    return "\x1B[0m";
            }
        }

        return "";
    }

    template <typename... Values_T>
    QENTEM_NOINLINE static void Print(const Values_T &...values) {
        ToCharsHelper::Write(buffer_, values...);

        if (IsOutputEnabled() && (buffer_.Length() >= SizeT{512})) {
            flush();
        }
    }

    QENTEM_NOINLINE static void Write(const char *data, unsigned length) noexcept {
        buffer_.Write(data, length);

        if (IsOutputEnabled() && (buffer_.Length() >= SizeT{512})) {
            flush();
        }
    }

    QENTEM_NOINLINE static void Flush() noexcept {
        if (IsOutputEnabled() && (buffer_.Length() != 0)) {
            flush();
        }
    }

    QENTEM_NOINLINE static void Clear() noexcept {
        buffer_.Clear();
    }

    QENTEM_NOINLINE static void DisableOutput() noexcept {
        enable_output_ = false;
    }

    QENTEM_NOINLINE static void EnableOutput() noexcept {
        enable_output_ = true;
    }

    QENTEM_NOINLINE static bool IsOutputEnabled() noexcept {
        return enable_output_;
    }

    // QENTEM_NOINLINE static void SetDoubleFormat() noexcept {
    // }

    // QENTEM_NOINLINE static void ResetDoubleFormat() noexcept {
    // }

    QENTEM_NOINLINE static LiteStream &GetBuffer() noexcept {
        return buffer_;
    }

  private:
    QENTEM_NOINLINE static void write(const char *data, unsigned length) noexcept {
#if defined(_WIN32)
        DWORD written = 0;
        ::WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), data, length, &written, nullptr);
#elif defined(__linux__)
        SystemCall(__NR_write, 1, reinterpret_cast<long>(data), length);
#else
        ::write(1, data, length); // stdout
#endif
    }

    QENTEM_NOINLINE static void flush() noexcept {
        write(buffer_.First(), buffer_.Length());
        buffer_.Clear();
    }

    struct OnExit {
        OnExit() = default;

        ~OnExit() {
            Flush(); // flush buffer on exit;
        }
    };

    inline static LiteStream buffer_{32};

    inline static bool enable_output_{true};
    inline static bool is_colored_{true};

    inline static OnExit oe{};
};

} // namespace Qentem

#endif
