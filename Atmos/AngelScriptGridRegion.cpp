#include "AngelScriptGridRegion.h"

#include "AngelScriptHandle.h"
#include "AngelScriptArcaTraits.h"
#include "AngelScriptArcaBatch.h"
#include "AngelScriptArcaCreate.h"
#include "AngelScriptArcaDestroy.h"
#include "AngelScriptGridPoint.h"

#include <angelscript.h>

namespace Atmos::Scripting::Angel
{
    void Registration<Render::GridRegion>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type> registration(ContainingNamespace(), Name());
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Points>, "Atmos::Spatial::Grid::Point[]@", "Points", {})
            .ConstMethod(&Management::Method<&Z>, "int", "Z", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::RegionMaterial", "Material", {})
            .Actualize(engine, documentationManager);

        Registration<ArcaTraits<Render::GridRegion>>::RegisterTo(engine, documentationManager);
        Registration<Arca::Batch<Render::GridRegion>>::RegisterTo(engine, documentationManager);

        ArcaCreateRelicRegistration<Type::ValueT>()
            .Constructor<
                std::unordered_set<Spatial::Grid::Point>,
                int,
                Arca::Index<Asset::RegionMaterial>>
            ({
                "Atmos::Spatial::Grid::Point[]@ points",
                "int z",
                "Atmos::Asset::RegionMaterial material"
            })
            .Actualize(engine, documentationManager);
        RegisterArcaCreated<Type::ValueT>(engine, documentationManager);

        RegisterArcaDestroyRelic<Type::ValueT>(engine, documentationManager);
        RegisterArcaDestroying<Type::ValueT>(engine, documentationManager);
    }

    std::unordered_set<Spatial::Grid::Point> Registration<Render::GridRegion>::Points(Type type)
    {
        return RequiredValue(type)->points;
    }

    Spatial::Grid::Point::Value Registration<Render::GridRegion>::Z(Type type)
    {
        return RequiredValue(type)->z;
    }

    Arca::Index<Asset::RegionMaterial> Registration<Render::GridRegion>::Material(Type type)
    {
        return RequiredValue(type)->material;
    }

    void Registration<ChangeGridRegionPoints>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, std::vector<Spatial::Grid::Point>>>,
                { "Arca::RelicID id", "Atmos::Spatial::Grid::Point[]@ to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("Atmos::Spatial::Grid::Point[]@", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::MoveGridRegion Registration<ChangeGridRegionPoints>::ToArca(Type fromAngelScript)
    {
        auto points = std::unordered_set<Spatial::Grid::Point>{ fromAngelScript.to.begin(), fromAngelScript.to.end() };
        return { fromAngelScript.id, points, {} };
    }

    void Registration<ChangeGridRegionZ>::RegisterTo(asIScriptEngine& engine, DocumentationManager& documentationManager)
    {
        ValueTypeRegistration<Type>(ContainingNamespace(), Name())
            .Constructor(
                &Management::GenerateValue<
                    &PullFromParameter<0, Arca::RelicID>,
                    &PullFromParameter<1, Spatial::Grid::Point::Value>>,
                { "Arca::RelicID id", "int to" })
            .CopyConstructor(&Management::GenerateValueFromCopy)
            .Destructor(&Management::DestructValue)
            .CopyAssignment(&Management::CopyAssign)
            .Property<&Type::id>("Arca::RelicID", "id")
            .Property<&Type::to>("int", "to")
            .Actualize(engine, documentationManager);

        RegisterCommandHandler<&ToArca>(engine, documentationManager);
    }

    Render::MoveGridRegion Registration<ChangeGridRegionZ>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, fromAngelScript.to };
    }
}