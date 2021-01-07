#include "AngelScriptDynamicImage.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::DynamicImage>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
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
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::DynamicImage>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::DynamicImage>>::RegisterTo(engine, documentationManager);

        RegisterArcaCreateRelic<
            Render::DynamicImage,
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
            engine,
            documentationManager);

        RegisterArcaDestroyRelic<Render::DynamicImage>(engine, documentationManager);
    }

    Arca::Index<Asset::Image> Registration<Render::DynamicImage>::Asset(Type type)
    {
        return RequiredValue(type)->Asset();
    }

    Render::DynamicImage::Index Registration<Render::DynamicImage>::AssetIndex(Type type)
    {
        return RequiredValue(type)->AssetIndex();
    }

    Spatial::AxisAlignedBox2D Registration<Render::DynamicImage>::AssetSlice(Type type)
    {
        return RequiredValue(type)->AssetSlice();
    }

    Arca::Index<Asset::ImageMaterial> Registration<Render::DynamicImage>::Material(Type type)
    {
        return RequiredValue(type)->Material();
    }

    Render::Color Registration<Render::DynamicImage>::Color(Type type)
    {
        return RequiredValue(type)->Color();
    }

    Spatial::Point3D Registration<Render::DynamicImage>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<Render::DynamicImage>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Angle2D Registration<Render::DynamicImage>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }
}