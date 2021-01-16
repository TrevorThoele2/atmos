#include "AngelScriptProperty.h"

#include "AngelScriptVariant.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptGlobalManagement.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Property>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, String>, &PullFromParameter<1, Property::Value>>,
                { "string name", "Atmos::Variant value" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::name>("string", "name")
            .Property<&Type::value>("Atmos::Variant", "value")
            .Actualize(engine, documentationManager);
    }
}