#include "AngelScriptRelicID.h"

#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<RelicID>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        GlobalRegistration(ContainingNamespace())
            .Typedef("RelicID", "int64")
            .Actualize(engine, documentationManager);
    }
}