#include "AngelScriptHandle.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptEnumRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Arca::Handle>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Arca::Type>>,
                { "Arca::RelicID id", "Arca::Type type" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::type>("Arca::Type", "type")
            .Actualize(engine, documentationManager);
    }
}