#include "Log.h"
#include <utility>

namespace Atmos::Logging
{
    Log::Log(String message, Severity severity) :
        message(std::move(message)), severity(severity)
    {}

    Log::Log(String message, Severity severity, const Details& details) :
        message(std::move(message)), severity(severity), details(details)
    {}
}