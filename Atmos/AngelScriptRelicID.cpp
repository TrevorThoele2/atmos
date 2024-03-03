#include "AngelScriptRelicID.h"

#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<RelicID>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Typedef("RelicID", "int64")
            .Actualize(engine);
    }
}