#include "AngelScriptUIImage.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<UI::Image>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Asset>, "Atmos::Asset::Image", "Asset", {})
            .ConstMethod(&Management::Method<&AssetIndex>, "int", "AssetIndex", {})
            .ConstMethod(&Management::Method<&AssetSlice>, "Atmos::Spatial::AxisAlignedBox2D", "AssetSlice", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::Material", "Material", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Point3D", "Position", {})
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", {})
            .ConstMethod(&Management::Method<&Rotation>, "Atmos::Spatial::Angle2D", "Rotation", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<UI::Image>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<UI::Image>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                Arca::Index<Asset::Image>,
                int,
                Arca::Index<Asset::Material>,
                Render::Color,
                Spatial::Point3D,
                Spatial::Scalers2D,
                Spatial::Angle2D>
            ({
                "Atmos::Asset::Image asset",
                "int assetIndex",
                "Atmos::Asset::Material material",
                "Atmos::Render::Color color",
                "Atmos::Spatial::Point3D position",
                "Atmos::Spatial::Scalers2D scalers",
                "Atmos::Spatial::Angle2D rotation"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    Arca::Index<Asset::Image> Registration<UI::Image>::Asset(Type type)
    {
        return RequiredValue(type)->Asset();
    }

    UI::Image::Index Registration<UI::Image>::AssetIndex(Type type)
    {
        return RequiredValue(type)->AssetIndex();
    }

    Spatial::AxisAlignedBox2D Registration<UI::Image>::AssetSlice(Type type)
    {
        return RequiredValue(type)->AssetSlice();
    }

    Arca::Index<Asset::Material> Registration<UI::Image>::Material(Type type)
    {
        return RequiredValue(type)->Material();
    }

    Render::Color Registration<UI::Image>::Color(Type type)
    {
        return RequiredValue(type)->Color();
    }

    Spatial::Point3D Registration<UI::Image>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<UI::Image>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Angle2D Registration<UI::Image>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }
}