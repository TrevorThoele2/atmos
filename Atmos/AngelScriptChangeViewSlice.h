#pragma once

#include "ChangeViewSlice.h"

#include "AngelScriptObjectRegistration.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"
#include "AngelScriptCommand.h"

#include "String.h"

#include <Chroma/Identity.h>

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    struct ChangeViewSliceBox
    {
        Arca::RelicID id = 0;

        Spatial::AxisAlignedBox2D to;
    };

    template<>
    struct Registration<ChangeViewSliceBox>
    {
        using Type = ChangeViewSliceBox;

        static String Name() { return "ChangeViewSliceBox"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a command."; }
        static const ObjectType objectType = ObjectType::Value;

        using Management = ObjectManagement<Type>;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        static Render::ChangeViewSlice ToArca(Type fromAngelScript);
    };
}