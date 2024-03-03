#include "Log.h"
#include <utility>

namespace Atmos::Log
{
    Log::Log(String message, Severity severity) :
        message(std::move(message)), severity(severity)
    {}

    Log::Log(String message, Severity severity, const NameValuePairs& nameValuePairs) :
        message(std::move(message)), severity(severity), nameValuePairs(nameValuePairs)
    {}
}

namespace Arca
{
    const TypeName Traits<Atmos::Log::Log>::typeName = "Log";
}