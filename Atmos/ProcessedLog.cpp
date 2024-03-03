#include "ProcessedLog.h"

namespace Atmos::Logging
{
    ProcessedLog::ProcessedLog(
        String resultantMessage, String originalMessage, Severity severity, std::optional<Details> details)
        :
        resultantMessage(resultantMessage), originalMessage(originalMessage), severity(severity), details(details)
    {}
}