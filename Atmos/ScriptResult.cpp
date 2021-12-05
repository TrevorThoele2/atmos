#include "ScriptResult.h"

#include "StringUtility.h"

namespace Atmos::Scripting
{
    Logging::Log ToLog(Failure failure)
    {
        Logging::Details details
        {
            NameValuePair{ "Message", ToString(failure.message) },
            NameValuePair{ "Line", ToString(failure.line) },
            NameValuePair{ "Column", ToString(failure.column) }
        };

        if (failure.function)
            details.emplace_back("Function", *failure.function);

        if (failure.section)
            details.emplace_back("Section", *failure.section);

        return {
            "Script has encountered an error.",
            Logging::Severity::Error,
            details
        };
    }
}