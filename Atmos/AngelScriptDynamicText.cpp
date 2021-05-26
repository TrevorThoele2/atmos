#include "AngelScriptDynamicText.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::DynamicText>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&String>, "string", "String", {})
            .ConstMethod(&Management::Method<&Font>, "Atmos::Asset::Font", "Font", {})
            .ConstMethod(&Management::Method<&WrapWidth>, "float", "WrapWidth", {})
            .ConstMethod(&Management::Method<&Bold>, "bool", "Bold", {})
            .ConstMethod(&Management::Method<&Italics>, "bool", "Italics", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::Material", "Material", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .ConstMethod(&Management::Method<&Position>, "Atmos::Spatial::Point3D", "Position", {})
            .ConstMethod(&Management::Method<&Size>, "Atmos::Spatial::Size2D", "Size", {})
            .ConstMethod(&Management::Method<&Rotation>, "Atmos::Spatial::Angle2D", "Rotation", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::DynamicText>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::DynamicText>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
            Atmos::String,
            Arca::Index<Asset::Font>,
            Arca::Index<Asset::Material>,
            float,
            bool,
            bool,
            Render::Color,
            Spatial::Point3D,
            Spatial::Scalers2D,
            Spatial::Angle2D>
            ({
                "string value",
                "Atmos::Asset::Font font",
                "Atmos::Asset::Material material",
                "float wrapWidth",
                "bool bold",
                "bool italics",
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

    String Registration<Render::DynamicText>::String(Type type)
    {
        return RequiredValue(type)->String();
    }

    Arca::Index<Asset::Font> Registration<Render::DynamicText>::Font(Type type)
    {
        return RequiredValue(type)->Font();
    }

    float Registration<Render::DynamicText>::WrapWidth(Type type)
    {
        return RequiredValue(type)->WrapWidth();
    }

    bool Registration<Render::DynamicText>::Bold(Type type)
    {
        return RequiredValue(type)->Bold();
    }

    bool Registration<Render::DynamicText>::Italics(Type type)
    {
        return RequiredValue(type)->Italics();
    }

    Arca::Index<Asset::Material> Registration<Render::DynamicText>::Material(Type type)
    {
        return RequiredValue(type)->Material();
    }

    Render::Color Registration<Render::DynamicText>::Color(Type type)
    {
        return RequiredValue(type)->Color();
    }

    Spatial::Point3D Registration<Render::DynamicText>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<Render::DynamicText>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Angle2D Registration<Render::DynamicText>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }
}