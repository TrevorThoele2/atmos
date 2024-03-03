#include "AngelScriptRelativeImage.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::RelativeImage>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Image", "Asset", {})
            .ConstMethod(&Management::Method<&AssetIndex>, "int", "AssetIndex", {})
            .ConstMethod(&Management::Method<&AssetSlice>, "Atmos::Spatial::AxisAlignedBox2D", "AssetSlice", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::ImageMaterial", "Material", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Point3D", "Position", {})
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", {})
            .ConstMethod(&Management::Method<&Rotation>, "Atmos::Spatial::Angle2D", "Rotation", {})
            .ConstMethod(&Management::Method<&Core>, "Atmos::Render::ImageCore", "Core", {})
            .ConstMethod(&Management::Method<&Bounds>, "Atmos::Spatial::Bounds", "Bounds", {})
            .Actualize(engine);

        Registration<ArcaTraits<Render::RelativeImage>>::RegisterTo(engine);
        Registration<Arca::Batch<Render::RelativeImage>>::RegisterTo(engine);

        RegisterArcaCreateRelic<
            Render::RelativeImage,
            Chroma::VariadicTemplate<
            Arca::Index<Asset::Image>,
            int,
            Arca::Index<Asset::ImageMaterial>,
            Render::Color,
            Spatial::Point3D,
            Spatial::Scalers2D,
            Spatial::Angle2D>>
            (
                {
                    "Atmos::Asset::Image asset",
                    "int assetIndex",
                    "Atmos::Asset::ImageMaterial material",
                    "Atmos::Render::Color color",
                    "Atmos::Spatial::Point3D position",
                    "Atmos::Spatial::Scalers2D scalers",
                    "Atmos::Spatial::Angle2D rotation"
                },
                engine);

        RegisterArcaDestroyRelic<Render::RelativeImage>(engine);
    }

    Arca::Index<Asset::Image> Registration<Render::RelativeImage>::Asset(Type type)
    {
        return RequiredValue(type)->Asset();
    }

    Render::RelativeImage::Index Registration<Render::RelativeImage>::AssetIndex(Type type)
    {
        return RequiredValue(type)->AssetIndex();
    }

    Spatial::AxisAlignedBox2D Registration<Render::RelativeImage>::AssetSlice(Type type)
    {
        return RequiredValue(type)->AssetSlice();
    }

    Arca::Index<Asset::ImageMaterial> Registration<Render::RelativeImage>::Material(Type type)
    {
        return RequiredValue(type)->Material();
    }

    Render::Color Registration<Render::RelativeImage>::Color(Type type)
    {
        return RequiredValue(type)->Color();
    }

    Spatial::Point3D Registration<Render::RelativeImage>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<Render::RelativeImage>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Angle2D Registration<Render::RelativeImage>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }

    Arca::Index<Render::ImageCore> Registration<Render::RelativeImage>::Core(Type type)
    {
        return RequiredValue(type)->Core();
    }

    Arca::Index<Spatial::Bounds> Registration<Render::RelativeImage>::Bounds(Type type)
    {
        return RequiredValue(type)->Bounds();
    }
}