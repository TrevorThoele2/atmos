#pragma once

#include "UIText.h"

#include "AngelScriptImageAsset.h"
#include "AngelScriptMaterialAsset.h"
#include "AngelScriptImageCore.h"
#include "AngelScriptBounds.h"

#include "AngelScriptIndex.h"
#include "AngelScriptObjectManagement.h"
#include "AngelScriptRegistration.h"

#include "String.h"

class asIScriptEngine;
class asIScriptGeneric;

namespace Atmos::Scripting::Angel
{
    template<>
    struct Registration<UI::Text>
    {
        using Type = Arca::Index<UI::Text>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "Text"; }
        static String ContainingNamespace() { return "Atmos::UI"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static String String(Type type);
        [[nodiscard]] static Arca::Index<Asset::Font> Font(Type type);
        [[nodiscard]] static float WrapWidth(Type type);
        [[nodiscard]] static bool Bold(Type type);
        [[nodiscard]] static bool Italics(Type type);
        [[nodiscard]] static Arca::Index<Asset::Material> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);

        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);
        [[nodiscard]] static Spatial::Angle2D Rotation(Type type);
    };

    template<>
    struct Registration<Arca::Index<UI::Text>> : Registration<UI::Text>
    {};
}