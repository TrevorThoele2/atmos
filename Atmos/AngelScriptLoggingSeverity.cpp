#include "AngelScriptLoggingSeverity.h"

#include <angelscript.h>
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Logging::Severity>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        EnumRegistration<Logging::Severity>(containingNamespace, name)
            .Value("Verbose", Logging::Severity::Verbose)
            .Value("Information", Logging::Severity::Information)
            .Value("Warning", Logging::Severity::Warning)
            .Value("Error", Logging::Severity::Error)
            .Actualize(engine, documentationManager);
    }
}