#include "AngelScriptGridSpatialAlgorithms.h"

#include "SpatialAlgorithms.h"

#include <angelscript.h>
#include "AngelScriptGridPoint.h"
#include "AngelScriptGlobalRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<GridSpatialAlgorithms>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        GlobalRegistration(ContainingNamespace())
            .Function(&Management::Function<&Spatial::Grid::Distance>,
                "int", "Distance", { "Point starting", "Point destination" })
            .Function(&Management::Function<static_cast<bool(*)(Spatial::Grid::AxisAlignedBox, Spatial::Grid::Point)>(Spatial::Grid::Contains)>,
                "bool", "Contains", { "AxisAlignedBox box", "Point point" })
            .Function(&Management::Function<static_cast<bool(*)(Spatial::Grid::AxisAlignedBox, Spatial::Grid::AxisAlignedBox)>(Spatial::Grid::Contains)>,
                "bool", "Contains", { "AxisAlignedBox box", "AxisAlignedBox other" })
            .Function(&Management::Function<static_cast<bool(*)(Spatial::Grid::AxisAlignedBox, Spatial::Grid::AxisAlignedBox)>(Spatial::Grid::Intersects)>,
                "bool", "Intersects", { "AxisAlignedBox one", "AxisAlignedBox two" })
            .Actualize(engine, documentationManager);
    }
}