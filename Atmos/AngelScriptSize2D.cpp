#include "AngelScriptSize2D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::Size2D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Spatial::Size2D>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>>,
                { "float width", "float height" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::width>("float", "width")
            .Property<&Type::height>("float", "height")
            .Actualize(engine, documentationManager);
    }
}