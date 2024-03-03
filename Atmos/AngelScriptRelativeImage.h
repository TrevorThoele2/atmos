#pragma once

#include "RelativeImage.h"

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
    struct Registration<Render::RelativeImage>
    {
        using Type = Arca::Index<Render::RelativeImage>;
        using Management = ObjectManagement<Type>;

        static inline const String name = "RelativeImage";
        static inline const String containingNamespace = Namespaces::Atmos::Render::name;
        static const ObjectType objectType = ObjectType::Value;

        static void RegisterTo(asIScriptEngine& engine);
    private:
        [[nodiscard]] static Arca::Index<Asset::Image> Asset(Type type);
        [[nodiscard]] static Render::RelativeImage::Index AssetIndex(Type type);
        [[nodiscard]] static Spatial::AxisAlignedBox2D AssetSlice(Type type);
        [[nodiscard]] static Arca::Index<Asset::ImageMaterial> Material(Type type);
        [[nodiscard]] static Render::Color Color(Type type);

        [[nodiscard]] static Spatial::Point3D Position(Type type);
        [[nodiscard]] static Spatial::Size2D Size(Type type);
        [[nodiscard]] static Spatial::Angle2D Rotation(Type type);

        [[nodiscard]] static Arca::Index<Render::ImageCore> Core(Type type);
        [[nodiscard]] static Arca::Index<Spatial::Bounds> Bounds(Type type);
    };

    template<>
    struct Registration<Arca::Index<Render::RelativeImage>> : Registration<Render::RelativeImage>
    {};
}