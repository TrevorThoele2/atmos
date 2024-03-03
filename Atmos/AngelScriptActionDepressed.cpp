#include "AngelScriptActionDepressed.h"

#include "AngelScriptSignal.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Input::ActionDepressed>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Arca::Index<Asset::Action>>>,
                { "Atmos::Asset::Action action" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::action>("Atmos::Asset::Action", "action")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}