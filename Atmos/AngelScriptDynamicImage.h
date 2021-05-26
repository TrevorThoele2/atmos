#pragma once

#include "DynamicImage.h"

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
    struct Registration<Render::DynamicImage>
    {
        using Type = Arca::Index<Render::DynamicImage>;
        using Management = ObjectManagement<Type>;

        static String Name() { return "DynamicImage"; }
        static String ContainingNamespace() { return "Atmos::Render"; }
        static String Documentation() { return "This is a relic."; }
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager);
    private:
        [[nodiscard]] static Arca::Index<Asset::Image> Asset(Type type);
        [[nodiscard]] static Render::DynamicImage::Index AssetIndex(Type type);
        [[nodiscard]] static Spatial::AxisAlignedBox2D AssetSlice(Type type);
        [[nodiscard]] static Arca::Index<Asset::Material> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);

        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);
        [[nodiscard]] static Spatial::Angle2D Rotation(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::DynamicImage>> : Registration<Render::DynamicImage>
    {};
}