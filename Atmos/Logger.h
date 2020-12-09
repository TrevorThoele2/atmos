#pragma once

#include "Log.h"
#include "ProcessedLog.h"
#include "FilePath.h"
#include "Event.h"

namespace Atmos::Logging
{
    class Logger
    {
    public:
        Event<const ProcessedLog&> onLog;
    public:
        explicit Logger(Severity minimumSeverity);
        ~Logger();

        std::optional<ProcessedLog> Log(const Logging::Log& log);
        std::optional<ProcessedLog> Log(
            const String& message,
            Severity severity = Severity::Information,
            std::optional<Details> details = {});
    private:
        Severity minimumSeverity;
    private:
        void StartSession();
        void StopSession();

        void ClearFile();

        [[nodiscard]] static File::Path OutputFilePath();

        [[nodiscard]] static String SeverityToString(Severity severity);
        [[nodiscard]] static String DetailsToString(std::optional<Details> details);
        [[nodiscard]] static String CurrentTimeStamp();
    };
}