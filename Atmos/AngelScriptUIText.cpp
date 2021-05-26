#include "AngelScriptUIText.h"

#include "AngelScriptAsset.h"
#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<UI::Text>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
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

        Registration<ArcaTraits<UI::Text>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<UI::Text>>::RegisterTo(engine, documentationManager);

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

    String Registration<UI::Text>::String(Type type)
    {
        return RequiredValue(type)->String();
    }

    Arca::Index<Asset::Font> Registration<UI::Text>::Font(Type type)
    {
        return RequiredValue(type)->Font();
    }

    float Registration<UI::Text>::WrapWidth(Type type)
    {
        return RequiredValue(type)->WrapWidth();
    }

    bool Registration<UI::Text>::Bold(Type type)
    {
        return RequiredValue(type)->Bold();
    }

    bool Registration<UI::Text>::Italics(Type type)
    {
        return RequiredValue(type)->Italics();
    }
    
    Arca::Index<Asset::Material> Registration<UI::Text>::Material(Type type)
    {
        return RequiredValue(type)->Material();
    }

    Render::Color Registration<UI::Text>::Color(Type type)
    {
        return RequiredValue(type)->Color();
    }

    Spatial::Point3D Registration<UI::Text>::Position(Type type)
    {
        return RequiredValue(type)->Position();
    }

    Spatial::Size2D Registration<UI::Text>::Size(Type type)
    {
        return RequiredValue(type)->Size();
    }

    Spatial::Angle2D Registration<UI::Text>::Rotation(Type type)
    {
        return RequiredValue(type)->Rotation();
    }
}