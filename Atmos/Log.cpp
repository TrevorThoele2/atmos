#include "Log.h"
#include <utility>

namespace Atmos::Logging
{
    Log::Log(String message) :
        Log(std::move(message), Severity::Information)
    {}

    Log::Log(String message, Severity severity) :
        message(std::move(message)), severity(severity)
    {}

    Log::Log(String message, const Details& details) :
        Log(std::move(message), Severity::Information, details)
    {}

    Log::Log(String message, Severity severity, const Details& details) :
        message(std::move(message)), severity(severity), details(details)
    {}
}