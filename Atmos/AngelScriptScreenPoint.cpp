#include "AngelScriptScreenPoint.h"

#include "SpatialAlgorithms.h"

#include <angelscript.h>
#include "AngelScriptObjectManagement.h"
#include "AngelScriptObjectRegistration.h"

namespace Atmos::Scripting::Angel
{
    void Registration<Spatial::ScreenPoint>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Type::Value>,
                    &PullFromParameter<1, Type::Value>>,
                { "int x", "int y" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Equals(&Management::Equals)
            .Add(&Management::Add)
            .Subtract(&Management::Subtract)
            .Property<&Type::x>("int", "x")
            .Property<&Type::y>("int", "y")
            .Actualize(engine, documentationManager);
    }
}