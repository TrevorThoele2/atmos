#include "AngelScriptSuspendScript.h"

#include "AngelScriptScript.h"
#include "AngelScriptCommand.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Suspend>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(
                &Management::GenerateValue<&PullFromParameter<0, Arca::Index<Script>>>,
                { "Atmos::Scripting::Script script" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::script>("Atmos::Scripting::Script", "script")
            .Actualize(engine);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine);
    }
}