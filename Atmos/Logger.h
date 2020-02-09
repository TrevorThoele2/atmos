#pragma once

#include "Log.h"
#include "ProcessedLog.h"
#include "FilePath.h"

namespace Atmos::Logging
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        std::optional<ProcessedLog> Log(const Logging::Log& log);
        std::optional<ProcessedLog> Log
        (
            const String& message,
            Severity severity,
            std::optional<Details> details = {}
        );
    private:
        void OnExit();
        [[nodiscard]] static File::Path OutputFilePath();

        static void ClearFile();
        [[nodiscard]] static String CurrentTimeStamp();
    };

    inline Logger logger;
}