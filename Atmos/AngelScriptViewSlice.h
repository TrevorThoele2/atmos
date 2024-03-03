#pragma once

#include "ViewSlice.h"

#include "AngelScriptIndex.h"
#include "AngelScriptImageAsset.h"

#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<Render::ViewSlice>
    {
        using Type = Arca::Index<Render::ViewSlice>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "ViewSlice"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a shard."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Spatial::AxisAlignedBox2D Box(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::ViewSlice>> : Registration<Render::ViewSlice>
    {};
}