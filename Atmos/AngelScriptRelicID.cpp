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

    void Registration<NullRelicID>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        static auto nullRelicID = Arca::nullRelicID;

        GlobalRegistration(ContainingNamespace())
            .ConstProperty<&nullRelicID>("Arca::RelicID", "nullRelicID")
            .Actualize(engine, documentationManager);
    }
}