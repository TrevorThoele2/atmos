#include "AngelScriptAxisAlignedBox2D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::AxisAlignedBox2D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Spatial::Point2D>,
                    &PullFromParameter<1, Spatial::Size2D>>,
                { "Atmos::Spatial::Point2D center", "Atmos::Spatial::Size2D size" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::center>("Atmos::Spatial::Point2D", "center")
            .Property<&Type::size>("Atmos::Spatial::Size2D", "size")
            .Method(&Management::Method<&Type::Left>, "float", "Left", {})
            .Method(&Management::Method<&Type::Top>, "float", "Top", {})
            .Method(&Management::Method<&Type::Right>, "float", "Right", {})
            .Method(&Management::Method<&Type::Bottom>, "float", "Bottom", {})
            .Actualize(engine, documentationManager);
    }
}