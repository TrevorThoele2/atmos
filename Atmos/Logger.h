#pragma once

#include "LoggingSink.h"

namespace Atmos::Logging
{
    class Logger
    {
    public:
        explicit Logger(Severity minimumSeverity);
        Logger(Logger&& arg);
        ~Logger();

        Logger& operator=(Logger&& arg);

        void Log(const Logging::Log& log);
        void Log(
            const String& message,
            Severity severity = Severity::Information,
            std::optional<Details> details = {});

        template<class T, class... Args>
        void Add(Args&& ... args);
    private:
        Severity minimumSeverity;
        bool shouldSignalStopSession = true;

        using SinkPtr = std::unique_ptr<Sink>;
        using Sinks = std::vector<SinkPtr>;
        Sinks sinks;

        std::vector<Logging::Log> previousLogs;
    };

    template<class T, class... Args>
    void Logger::Add(Args&& ... args)
    {
        sinks.push_back(std::make_unique<T>(std::forward<Args>(args)...));

        const auto newSink = sinks.back().get();
        for (auto& log : previousLogs)
            newSink->Log(log);
    }
}