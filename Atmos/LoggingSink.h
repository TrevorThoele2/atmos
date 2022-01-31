#pragma once

#include "Log.h"

namespace Atmos::Logging
{
    class Sink
    {
    public:
        virtual ~Sink() = 0;

        virtual void Log(const Logging::Log& log) = 0;
        virtual void Flush() = 0;
    protected:
        Sink() = default;

        [[nodiscard]] static String StandardFormattedLog(const Logging::Log& log);
    private:
        [[nodiscard]] static String SeverityToString(Severity severity);
        [[nodiscard]] static String DetailsToString(std::optional<Details> details);
        [[nodiscard]] static String CurrentTimeStamp();
    };
}