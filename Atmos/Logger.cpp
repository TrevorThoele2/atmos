#include "Logger.h"

namespace Atmos::Logging
{
    Logger::Logger(Severity minimumSeverity) : minimumSeverity(minimumSeverity)
    {
        Log("Session started.");
    }

    Logger::Logger(Logger&& arg) : minimumSeverity(arg.minimumSeverity), sinks(std::move(arg.sinks))
    {
        shouldSignalStopSession = false;
    }

    Logger::~Logger()
    {
        if (shouldSignalStopSession)
            Log("Session stopped.");
        Flush();
    }

    Logger& Logger::operator=(Logger&& arg)
    {
        minimumSeverity = arg.minimumSeverity;
        sinks = std::move(arg.sinks);
        shouldSignalStopSession = false;
        return *this;
    }

    void Logger::Log(const Logging::Log& log)
    {
        if (static_cast<int>(log.severity) < static_cast<int>(minimumSeverity))
            return;

        for (auto& sink : sinks)
            sink->Log(log);

        previousLogs.push_back(log);
    }

    void Logger::Log(
        const String& message,
        Severity severity,
        std::optional<Details> details)
    {
        Log(Logging::Log{ message, severity, details ? *details : Details{} });
    }

    void Logger::Flush()
    {
        for (auto& sink : sinks)
            sink->Flush();
    }
}