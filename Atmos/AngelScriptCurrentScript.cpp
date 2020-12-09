#include "AngelScriptCurrentScript.h"

#include "CurrentExecutingScript.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"
#include "AngelScriptUserData.h"

namespace Atmos::Scripting::Angel
{
    void Registration<CurrentScript>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Function(&Management::Function<&Retrieve, &UserData::RequiredReliquaryFrom>, "Atmos::Scripting::Script", "CurrentScript", {})
            .Actualize(engine);
    }

    Arca::Index<Script> Registration<CurrentScript>::Retrieve(Arca::Reliquary* reliquary)
    {
        const auto currentExecutingScript = Arca::Index<CurrentExecutingScript>(reliquary);
        return Arca::Index<Script>(currentExecutingScript->id, reliquary);
    }
}