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
    void Registration<Render::GridRegion>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type> registration(containingNamespace, name);
        RegisterArcaIndex(registration);
        registration
            .ConstMethod(&Management::Method<&Points>, "Atmos::Spatial::Grid::Point[]@", "Points", {})
            .ConstMethod(&Management::Method<&Z>, "int", "Z", {})
            .ConstMethod(&Management::Method<&Material>, "Atmos::Asset::RegionMaterial", "Material", {})
            .Actualize(engine);

        Registration<ArcaTraits<Render::GridRegion>>::RegisterTo(engine);
        Registration<Arca::Batch<Render::GridRegion>>::RegisterTo(engine);

        RegisterArcaCreateRelic<
            Render::GridRegion,
            Chroma::VariadicTemplate<
                std::unordered_set<Spatial::Grid::Point>,
                int,
                Arca::Index<Asset::RegionMaterial>>>
            (
                {
                    "Atmos::Spatial::Grid::Point[]@ points",
                    "int z",
                    "Atmos::Asset::RegionMaterial material"
                },
                engine);

        RegisterArcaDestroyRelic<Render::GridRegion>(engine);
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

    void Registration<ChangeGridRegionPoints>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
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
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::MoveGridRegion Registration<ChangeGridRegionPoints>::ToArca(Type fromAngelScript)
    {
        auto points = std::unordered_set<Spatial::Grid::Point>{ fromAngelScript.to.begin(), fromAngelScript.to.end() };
        return { fromAngelScript.id, points, {} };
    }

    void Registration<ChangeGridRegionZ>::RegisterTo(asIScriptEngine& engine)
    {
        ValueTypeRegistration<Type>(containingNamespace, name)
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
            .Actualize(engine);

        RegisterCommandHandler<&ToArca>(engine);
    }

    Render::MoveGridRegion Registration<ChangeGridRegionZ>::ToArca(Type fromAngelScript)
    {
        return { fromAngelScript.id, {}, fromAngelScript.to };
    }
}