#include "AngelScriptArcaType.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Arca::Type>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                &PullFromParameter<0, Arca::TypeName>,
                &PullFromParameter<1, bool>>,
                { "string name", "bool isConst" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::name>("string", "name")
            .Property<&Type::isConst>("bool", "isConst")
            .Actualize(engine, documentationManager);
    }
}