#include "AngelScriptAngle2D.h"

#include <angelscript.h>
#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Angle2D>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Typedef("Angle2D", "float")
            .Actualize(engine);
    }
}