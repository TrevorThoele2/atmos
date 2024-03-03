#include "AngelScriptColor.h"

#include <angelscript.h>
#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Render::Color>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Render::Color>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>,
                    &PullFromParameter<2, Type::Value>,
                    &PullFromParameter<3, Type::Value>>,
                { "uint8 alpha", "uint8 red", "uint8 green", "uint8 blue" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::alpha>("uint8", "alpha")
            .Property<&Type::red>("uint8", "red")
            .Property<&Type::green>("uint8", "green")
            .Property<&Type::blue>("uint8", "blue")
            .Actualize(engine);
    }
}