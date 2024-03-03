#include "AngelScriptScalers2D.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::Scalers2D>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>>,
                { "float x", "float y" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Property<&Type::x>("float", "x")
            .Property<&Type::y>("float", "y")
            .Actualize(engine, documentationManager);
    }
}