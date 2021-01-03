#include "AngelScriptMouseMoved.h"

#include "AngelScriptSignal.h"
#include "AngelScriptObjectRegistration.h"

#include <angelscript.h>

#include <Chroma/Identity.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Input::MouseMoved>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
            .DefaultConstructor(&Management::GenerateDefaultValue)
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Spatial::ScreenPoint>,
                    &PullFromParameter<1, Spatial::ScreenPoint>>,
                { "Atmos::Asset::Action action" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::previous>("Atmos::Spatial::ScreenPoint", "previous")
            .Property<&Type::current>("Atmos::Spatial::ScreenPoint", "current")
            .Actualize(engine, documentationManager);

        RegisterSignalHandler<&Chroma::Identity<Type>>(engine, documentationManager);
    }
}