#include "AngelScriptLog.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptPrimitive.h"
#include "AngelScriptCommand.h"

#include "Chroma/Identity.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Logging::Log>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, String>>,
                { "const string &in" })
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, String>,
                    &PullFromParameter<1, Logging::Severity>>,
                { "const string &in", "Severity" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::message>("string", "message")
            .Property<&Type::severity>("Severity", "severity")
            .Actualize(engine);

        RegisterCommandHandler<&Chroma::Identity<Type>>(engine);
    }
}