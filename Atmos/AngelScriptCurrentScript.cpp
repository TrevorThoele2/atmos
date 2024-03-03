#include "AngelScriptCurrentScript.h"

#include "CurrentExecutingScript.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptUserData.h"

namespace Atmos::Scripting::Angel
{
    void Registration<CurrentScript>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        GlobalRegistration(ContainingNamespace())
            .Function(&Management::Function<&Retrieve, &UserData::RequiredReliquaryFrom>, "Atmos::Scripting::Script", "CurrentScript", {})
            .Actualize(engine, documentationManager);
    }

    Arca::Index<Script> Registration<CurrentScript>::Retrieve(Arca::Reliquary* reliquary)
    {
        const auto currentExecutingScript = reliquary->Find<CurrentExecutingScript>();
        return reliquary->Find<Script>(currentExecutingScript->id);
    }
}