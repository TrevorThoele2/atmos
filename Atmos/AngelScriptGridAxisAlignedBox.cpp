#include "AngelScriptGridAxisAlignedBox.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::Grid::AxisAlignedBox>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Spatial::Grid::Point>,
                &PullFromParameter<1, Spatial::Grid::Size>>,
                { "Atmos::Spatial::Grid::Point center", "Atmos::Spatial::Grid::Size size" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::center>("Atmos::Spatial::Grid::Point", "center")
            .Property<&Type::size>("Atmos::Spatial::Grid::Size", "size")
            .Method(&Management::Method<&Type::Left>, "int", "Left", {})
            .Method(&Management::Method<&Type::Top>, "int", "Top", {})
            .Method(&Management::Method<&Type::Right>, "int", "Right", {})
            .Method(&Management::Method<&Type::Bottom>, "int", "Bottom", {})
            .Actualize(engine, documentationManager);
    }
}