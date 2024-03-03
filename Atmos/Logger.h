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
        Event<const std::optional<ProcessedLog>&> onLog;
    public:
        std::optional<ProcessedLog> Log(const Logging::Log& log);
        std::optional<ProcessedLog> Log
        (
            const String& message,
            Severity severity = Severity::Information,
            std::optional<Details> details = {}
        );

        void StartSession();
        void StopSession();
    private:
        void ClearFile();

        [[nodiscard]] static File::Path OutputFilePath();

        [[nodiscard]] static String CurrentTimeStamp();
    };

    inline Logger logger;
}