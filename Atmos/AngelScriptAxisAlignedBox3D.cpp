#include "AngelScriptAxisAlignedBox3D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::AxisAlignedBox3D>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Spatial::Point3D>,
                    &PullFromParameter<1, Spatial::Size3D>>,
                { "Point3D center", "Size3D size" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::center>("Point3D", "center")
            .Property<&Type::size>("Size3D", "size")
            .Method(&Management::Method<&Type::Left>, "float", "Left", {})
            .Method(&Management::Method<&Type::Top>, "float", "Top", {})
            .Method(&Management::Method<&Type::FarZ>, "float", "FarZ", {})
            .Method(&Management::Method<&Type::Right>, "float", "Right", {})
            .Method(&Management::Method<&Type::Bottom>, "float", "Bottom", {})
            .Method(&Management::Method<&Type::NearZ>, "float", "NearZ", {})
            .Actualize(engine);
    }
}