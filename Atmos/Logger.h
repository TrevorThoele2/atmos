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
        Logger(Logger&& arg);
        ~Logger();

        Logger& operator=(Logger&& arg);

        std::optional<ProcessedLog> Log(const Logging::Log& log);
        std::optional<ProcessedLog> Log(
            const String& message,
            Severity severity = Severity::Information,
            std::optional<Details> details = {});
    private:
        Severity minimumSeverity;
    private:
        bool shouldSignalStopSession = true;

        void StartSession();
        void StopSession();
    private:
        void ClearFile();

        [[nodiscard]] static File::Path OutputFilePath();

        [[nodiscard]] static String SeverityToString(Severity severity);
        [[nodiscard]] static String DetailsToString(std::optional<Details> details);
        [[nodiscard]] static String CurrentTimeStamp();
    };
}