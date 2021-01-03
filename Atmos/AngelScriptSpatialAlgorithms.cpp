#include "AngelScriptSpatialAlgorithms.h"

#include "SpatialAlgorithms.h"

#include <angelscript.h>
#include "AngelScriptAngle3D.h"
#include "AngelScriptPoint2D.h"
#include "AngelScriptPoint3D.h"
#include "AngelScriptGridPoint.h"
#include "AngelScriptAxisAlignedBox2D.h"
#include "AngelScriptAxisAlignedBox3D.h"
#include "AngelScriptGlobalRegistration.h"

using namespace Atmos::Spatial;

namespace Atmos::Scripting::Angel
{
    void Registration<SpatialAlgorithms>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        GlobalRegistration(containingNamespace)
            .Function(&Management::Function<static_cast<Point2D::Value(*)(Point2D, Point2D)>(Distance)>,
                "float", "Distance", { "Point2D starting", "Point2D destination" })
            .Function(&Management::Function<static_cast<Point3D::Value(*)(Point3D, Point3D)>(Distance)>,
                "float", "Distance", { "Point3D starting", "Point3D destination" })
            .Function(&Management::Function<static_cast<bool(*)(Point2D, Point2D, Point2D)>(IsBetween)>,
                "bool", "IsBetween", { "Point2D one", "Point2D check", "Point2D two" })
            .Function(&Management::Function<static_cast<float(*)(Point2D, Point2D, Point2D)>(AngleOf)>,
                "float", "AngleOf", { "Point2D side1", "Point2D middle", "Point2D side2" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox2D, Point2D)>(Contains)>,
                "bool", "Contains", { "AxisAlignedBox2D box", "Point2D point" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox2D, AxisAlignedBox2D)>(Contains)>,
                "bool", "Contains", { "AxisAlignedBox2D box", "AxisAlignedBox2D other" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox3D, Point3D)>(Contains)>,
                "bool", "Contains", { "AxisAlignedBox3D box", "Point3D point" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox3D, AxisAlignedBox3D)>(Contains)>,
                "bool", "Contains", { "AxisAlignedBox3D box", "AxisAlignedBox3D other" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox2D, AxisAlignedBox2D)>(Intersects)>,
                "bool", "Intersects", { "AxisAlignedBox2D one", "AxisAlignedBox2D two" })
            .Function(&Management::Function<static_cast<bool(*)(AxisAlignedBox3D, AxisAlignedBox3D)>(Intersects)>,
                "bool", "Intersects", { "AxisAlignedBox3D one", "AxisAlignedBox3D two" })
            .Function(&Management::Function<static_cast<Point2D(*)(Point3D)>(ToPoint2D)>,
                "Point2D", "ToPoint2D", { "Point3D point" })
            .Function(&Management::Function<static_cast<Point2D(*)(Grid::Point)>(ToPoint2D)>,
                "Point2D", "ToPoint2D", { "Atmos::Spatial::Grid::Point point" })
            .Function(&Management::Function<static_cast<Point2D(*)(Angle2D, float)>(ToPoint2D)>,
                "Point2D", "ToPoint2D", { "Angle2D angle", "float distance" })
            .Function(&Management::Function<static_cast<Point3D(*)(Point2D, float)>(ToPoint3D)>,
                "Point3D", "ToPoint3D", { "Point2D point", "float z" })
            .Function(&Management::Function<static_cast<Point3D(*)(Grid::Point, float)>(ToPoint3D)>,
                "Point3D", "ToPoint3D", { "Atmos::Spatial::Grid::Point point", "float z" })
            .Function(&Management::Function<static_cast<Point3D(*)(Angle3D, float)>(ToPoint3D)>,
                "Point3D", "ToPoint3D", { "Angle3D angle", "float distance" })
            .Function(&Management::Function<&ToAxisAlignedBox2D>,
                "AxisAlignedBox2D", "ToAxisAlignedBox2D", { "float left", "float top", "float right", "float bottom" })
            .Function(&Management::Function<&ToAxisAlignedBox3D>,
                "AxisAlignedBox3D", "ToAxisAlignedBox3D", { "float left", "float top", "float farZ", "float right", "float bottom", "float nearZ" })
            .Actualize(engine, documentationManager);

        GlobalRegistration("Atmos::Spatial::Grid")
            .Function(&Management::Function<static_cast<Grid::Point(*)(Point2D)>(Grid::ToPoint)>,
                "Point", "ToPoint", { "Point2D point" })
            .Function(&Management::Function<static_cast<Grid::Point(*)(Point3D)>(Grid::ToPoint)>,
                "Point", "ToPoint", { "Point3D point" })
            .Actualize(engine, documentationManager);
    }
}