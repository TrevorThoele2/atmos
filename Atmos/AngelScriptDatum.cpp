#include "AngelScriptDatum.h"

#include "AngelScriptVariant.h"

#include "AngelScriptObjectRegistration.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Datum>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, String>, &PullFromParameter<1, Variant>>,
                { "string name", "Atmos::Variant value" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::name>("string", "name")
            .Property<&Type::value>("Atmos::Variant", "value")
            .Actualize(engine, documentationManager);
    }
}