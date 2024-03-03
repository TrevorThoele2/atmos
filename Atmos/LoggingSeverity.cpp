#include "LoggingSeverity.h"

namespace Atmos
{
    String ToString(Logging::Severity severity)
    {
        switch (severity)
        {
        case Logging::Severity::Verbose:
            return "Verbose";
        case Logging::Severity::Information:
            return "Information";
        case Logging::Severity::Warning:
            return "Warning";
        case Logging::Severity::Error:
            return "Error";
        }

        throw std::runtime_error("Unknown logging severity.");
    }
}