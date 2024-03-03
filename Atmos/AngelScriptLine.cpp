#include "AngelScriptLine.h"

#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include "AngelScriptPoint2D.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::Line>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Points>, "Atmos::Spatial::Point2D[]@", "Points", {})
            .ConstMethod(&Management::Method<&Z>, "float", "Z", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::LineMaterial", "Material", {})
            .ConstMethod(&Management::Method<&Width>, "float", "Width", {})
            .ConstMethod(&Management::Method<&Color>, "Atmos::Render::Color", "Color", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::Line>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::Line>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                std::vector<Spatial::Point2D>,
                float,
                Arca::Index<Asset::LineMaterial>,
                float,
                Render::Color>
            ({
                "Atmos::Spatial::Point2D[]@ points",
                "float z",
                "Atmos::Asset::LineMaterial material",
                "float width",
                "Atmos::Render::Color color"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    std::vector<Spatial::Point2D> Registration<Render::Line>::Points(Type type)
    {
        return RequiredValue(type)->points;
    }

    Spatial::Point2D::Value Registration<Render::Line>::Z(Type type)
    {
        return RequiredValue(type)->z;
    }

    Arca::Index<Asset::LineMaterial> Registration<Render::Line>::Material(Type type)
    {
        return RequiredValue(type)->material;
    }

    Render::LineWidth Registration<Render::Line>::Width(Type type)
    {
        return RequiredValue(type)->width;
    }

    Render::Color Registration<Render::Line>::Color(Type type)
    {
        return RequiredValue(type)->color;
    }

    void Registration<ChangeLinePoints>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, std::vector<Spatial::Point2D>>>,
                { "Arca::RelicID id", "Atmos::Spatial::Point2D[]@ to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::Point2D[]@", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::MoveLine Registration<ChangeLinePoints>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, fromAngelScript.to, {} };
    }

    void Registration<ChangeLineZ>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Point2D::Value>>,
                { "Arca::RelicID id", "float to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("float", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::MoveLine Registration<ChangeLineZ>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, fromAngelScript.to };
    }
}