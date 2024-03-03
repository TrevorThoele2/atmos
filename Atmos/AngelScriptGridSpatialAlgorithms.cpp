#include "AngelScriptGridSpatialAlgorithms.h"

#include "SpatialAlgorithms.h"

#include <angelscript.h>
#include "AngelScriptGridPoint.h"
#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GridSpatialAlgorithms>::RegisterTo(asIScriptEngine& engine)
    {
        GlobalRegistration(containingNamespace)
            .Function(&Management::Function<&Spatial::Grid::Distance>,
                "int", "Distance", { "Point starting", "Point destination" })
            .Actualize(engine);
    }
}