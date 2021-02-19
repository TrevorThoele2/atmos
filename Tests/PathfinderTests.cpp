#include "catch.hpp"
#include "PathfinderTests.h"

#include <Atmos/Pathfinder.h>
#include <Atmos/Entity.h>
#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>

PathfinderTestsFixture::PathfinderTestsFixture()
{
    Arca::ReliquaryOrigin reliquaryOrigin;

    Entity::RegisterTypes(reliquaryOrigin);
    Data::RegisterTypes(reliquaryOrigin);
    World::RegisterTypes(reliquaryOrigin, worldManager);

    reliquary = reliquaryOrigin.Actualize();
}

SCENARIO_METHOD(PathfinderTestsFixture, "pathfinder", "[entity]")
{
    auto pathfinder = Entity::Pathfinder();

    GIVEN("created entity")
    {
        const auto name = dataGeneration.Random<Name>();
        const auto position = Spatial::Grid::Point{ 3, 5 };
        const auto isSolid = true;

        const auto entity = reliquary->Do(Arca::Create<Entity::Entity>(name, position, isSolid));

        WHEN("finding path to 3 tiles away")
        {
            const auto toPoint = Spatial::Grid::Point{ 6, 5 };
            const auto path = pathfinder.FindPath(entity, toPoint, *reliquary);

            THEN("path is correct")
            {
                REQUIRE(path.size() == 3);
                REQUIRE(path[0] == Spatial::Grid::Point{ 4, 5 });
                REQUIRE(path[1] == Spatial::Grid::Point{ 5, 5 });
                REQUIRE(path[2] == Spatial::Grid::Point{ 6, 5 });
            }
        }

        WHEN("finding path to top left")
        {
            const auto toPoint = Spatial::Grid::Point{ 0, 2 };
            const auto path = pathfinder.FindPath(entity, toPoint, *reliquary);

            THEN("path has correct size")
            {
                REQUIRE(path.size() == 6);
            }
        }
    }
}