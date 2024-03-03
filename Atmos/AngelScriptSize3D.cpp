#include "AngelScriptSize3D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::Size3D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>,
                    &PullFromParameter<2, Type::Value>>,
                { "float width", "float height", "float depth" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::width>("float", "width")
            .Property<&Type::height>("float", "height")
            .Property<&Type::depth>("float", "depth")
            .Actualize(engine, documentationManager);
    }
}