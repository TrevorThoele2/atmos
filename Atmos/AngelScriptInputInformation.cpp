#include "AngelScriptInputInformation.h"

#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"

#include "AngelScriptObjectRegistration.h"
#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Input::Information>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&PreviousMousePosition>, "Atmos::Spatial::ScreenPoint", "PreviousMousePosition", {})
            .ConstMethod(&Management::Method<&CurrentMousePosition>, "Atmos::Spatial::ScreenPoint", "CurrentMousePosition", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Input::Information>>::RegisterTo(engine, documentationManager);
    }

    Spatial::ScreenPoint Registration<Input::Information>::PreviousMousePosition(Type type)
    {
        return RequiredValue(type)->mousePosition.previous;
    }

    Spatial::ScreenPoint Registration<Input::Information>::CurrentMousePosition(Type type)
    {
        return RequiredValue(type)->mousePosition.current;
    }
}