#include "AngelScriptAxisAlignedBox3D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::AxisAlignedBox3D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Spatial::Point3D>,
                    &PullFromParameter<1, Spatial::Size3D>>,
                { "Atmos::Spatial::Point3D center", "Atmos::Spatial::Size3D size" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::center>("Atmos::Spatial::Point3D", "center")
            .Property<&Type::size>("Atmos::Spatial::Size3D", "size")
            .ConstMethod(&Management::Method<&Type::Left>, "float", "Left", {})
            .ConstMethod(&Management::Method<&Type::Top>, "float", "Top", {})
            .ConstMethod(&Management::Method<&Type::FarZ>, "float", "FarZ", {})
            .ConstMethod(&Management::Method<&Type::Right>, "float", "Right", {})
            .ConstMethod(&Management::Method<&Type::Bottom>, "float", "Bottom", {})
            .ConstMethod(&Management::Method<&Type::NearZ>, "float", "NearZ", {})
            .Actualize(engine, documentationManager);
    }
}