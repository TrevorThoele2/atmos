#include "catch.hpp"
#include "EntityTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Arca/Create.h>
#include <Arca/Clear.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ModifyEntityBoundary.h>
#include <Atmos/MoveEntityTo.h>
#include <Atmos/Work.h>

EntityTestsFixture::EntityTestsFixture() : worldManager([this]() { return World::FieldInitialization{}; })
{
    Arca::ReliquaryOrigin reliquaryOrigin;

    RegisterTypes(reliquaryOrigin);
    Data::RegisterTypes(reliquaryOrigin);
    World::RegisterTypes(reliquaryOrigin, worldManager);

    reliquary = reliquaryOrigin.Actualize();
    mappedEntities = reliquary->Find<Mapped>();
}

SCENARIO_METHOD(EntityTestsFixture, "entity actualization", "[entity]")
{
    GIVEN("registered reliquary")
    {
        WHEN("creating 3 prototypes")
        {
            auto names = dataGeneration.RandomGroup<Name>(3);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(3);

            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Scripting::Script>{},
                names[0],
                positions[0],
                false));
            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Scripting::Script>{},
                names[1],
                positions[1],
                false));
            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Scripting::Script>{},
                names[2],
                positions[2],
                false));

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();
            auto prototypes = reliquary->Batch<Prototype>();

            WHEN("working reliquary")
            {
                reliquary->Do(Work{});

                THEN("3 entities were created")
                {
                    REQUIRE(entities.Size() == 3);
                }

                THEN("entities contains correct data")
                {
                    for (size_t i = 0; i < entities.Size(); ++i)
                    {
                        size_t count = 0;
                        for (auto& entity : entities)
                        {
                            if (entity.name == names[i] && entity.position == positions[i])
                                ++count;
                        }

                        REQUIRE(count == 1);
                    }
                }

                THEN("prototypes are destroyed")
                {
                    REQUIRE(prototypes.IsEmpty());
                }

                THEN("entities are mapped by name")
                {
                    auto& nameToEntity = mappedEntities->nameToEntity;
                    REQUIRE(nameToEntity.size() == 3);
                    for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                    {
                        auto found = nameToEntity.find(entity->name);
                        REQUIRE(found != nameToEntity.end());
                        REQUIRE(found->second.ID() == entity.ID());
                    }
                }

                THEN("entities are mapped by position")
                {
                    auto& positionToEntity = mappedEntities->positionToEntity;
                    REQUIRE(positionToEntity.size() == 3);
                    for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                    {
                        auto found = positionToEntity.find(entity->position);
                        REQUIRE(found != positionToEntity.end());
                        REQUIRE(found->second.begin()->ID() == entity.ID());
                    }
                }
            }
        }
    }
}

SCENARIO_METHOD(EntityTestsFixture, "creating entities", "[entity]")
{
    GIVEN("registered reliquary")
    {
        WHEN("creating 3 entities")
        {
            auto names = dataGeneration.RandomGroup<Name>(3);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(3);
            auto isSolids = dataGeneration.RandomGroup<bool>(3);

            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                isSolids[0],
                std::vector<Tag>{}));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                positions[1],
                isSolids[1],
                std::vector<Tag>{}));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[2],
                positions[2],
                isSolids[2],
                std::vector<Tag>{}));

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();

            THEN("entities contains correct data")
            {
                for (size_t i = 0; i < entities.Size(); ++i)
                {
                    size_t count = 0;
                    for (auto& entity : entities)
                    {
                        if (entity.name == names[i] && entity.position == positions[i])
                            ++count;
                    }

                    REQUIRE(count == 1);
                }
            }

            THEN("entities are mapped by name")
            {
                auto& nameToEntity = mappedEntities->nameToEntity;
                REQUIRE(nameToEntity.size() == 3);
                for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                {
                    auto found = nameToEntity.find(entity->name);
                    REQUIRE(found != nameToEntity.end());
                    REQUIRE(found->second.ID() == entity.ID());
                }
            }

            THEN("entities are mapped by position")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.size() == 3);
                for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                {
                    auto found = positionToEntity.find(entity->position);
                    REQUIRE(found != positionToEntity.end());
                    REQUIRE(found->second.begin()->ID() == entity.ID());
                }
            }
        }

        WHEN("creating entity with same name as other entity")
        {
            const auto name = dataGeneration.Random<String>();

            const auto entity0 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                name,
                Spatial::Grid::Point{},
                false,
                std::vector<Tag>{}));
            const auto entity1 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                name,
                Spatial::Grid::Point{},
                false,
                std::vector<Tag>{}));

            THEN("did not create entity")
            {
                REQUIRE(!entity1);
            }
        }

        WHEN("creating entity with same position as other entity when both are solid")
        {
            const auto names = dataGeneration.RandomGroup<String>(2);
            auto position = dataGeneration.RandomStack<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>();

            const auto entity0 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                position,
                true,
                std::vector<Tag>{}));
            const auto entity1 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                position,
                true,
                std::vector<Tag>{}));

            THEN("did not create entity")
            {
                REQUIRE(!entity1);
            }
        }
    }
}

SCENARIO_METHOD(EntityTestsFixture, "clearing entities", "[entity]")
{
    GIVEN("registered reliquary")
    {
        WHEN("clearing entities")
        {
            reliquary->Do(Arca::Clear(Arca::TypeFor<Atmos::Entity::Entity>()));

            THEN("no entities are mapped by name")
            {
                auto& nameToEntity = mappedEntities->nameToEntity;
                REQUIRE(nameToEntity.empty());
            }

            THEN("no entities are mapped by position")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.empty());
            }
        }
    }
}

SCENARIO_METHOD(EntityTestsFixture, "moving entities", "[entity]")
{
    GIVEN("registered reliquary")
    {
        WHEN("creating 3 entities")
        {
            auto names = dataGeneration.RandomGroup<Name>(3);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(6);
            auto isSolids = dataGeneration.RandomGroup<bool>(3);

            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                isSolids[0],
                std::vector<Tag>{}));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                positions[1],
                isSolids[1],
                std::vector<Tag>{}));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[2],
                positions[2],
                isSolids[2],
                std::vector<Tag>{}));

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();

            WHEN("moving entities")
            {
                size_t i = 3;
                for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                {
                    reliquary->Do(MoveTo{ entity.ID(), positions[i] });
                    ++i;
                }

                THEN("entities have been updated in map")
                {
                    auto& positionToEntity = mappedEntities->positionToEntity;
                    REQUIRE(positionToEntity.size() == 3);
                    for (auto position = positions.begin() + 3; position != positions.end(); ++position)
                    {
                        auto found = positionToEntity.find(*position);
                        REQUIRE(found != positionToEntity.end());
                        auto foundEntity = std::any_of(
                            entities.begin(),
                            entities.end(),
                            [position](const Atmos::Entity::Entity& entity)
                            {
                                return entity.position == *position;
                            });
                        REQUIRE(foundEntity);
                    }
                }
            }
        }

        WHEN("moving solid entity into entity boundary")
        {
            auto names = dataGeneration.RandomGroup<Name>(2);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(2);

            auto entity = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                true,
                std::vector<Tag>{}));

            reliquary->Do(World::ModifyEntityBoundary{ {positions[1]}, {} });

            reliquary->Do(MoveTo{ entity.ID(), positions[1] });

            THEN("has not moved")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.size() == 1);

                auto position0Entities = positionToEntity.find(positions[0]);
                REQUIRE(position0Entities != positionToEntity.end());
                REQUIRE(position0Entities->second.size() == 1);
                REQUIRE(position0Entities->second.begin()->ID() == entity.ID());
            }
        }

        WHEN("moving non-solid entity into other solid")
        {
            auto names = dataGeneration.RandomGroup<Name>(2);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(2);

            auto entity0 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                true,
                std::vector<Tag>{}));
            auto entity1 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                positions[1],
                false,
                std::vector<Tag>{}));

            reliquary->Do(MoveTo{ entity0.ID(), positions[1] });

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();

            THEN("has moved")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.size() == 1);

                auto position1Entities = positionToEntity.find(positions[1]);
                REQUIRE(position1Entities != positionToEntity.end());
                REQUIRE(position1Entities->second.size() == 2);
                for (auto entity = entities.begin(); entity != entities.end(); ++entity)
                {
                    auto foundEntity = std::find_if(
                        position1Entities->second.begin(),
                        position1Entities->second.end(),
                        [entity](const Arca::Index<Atmos::Entity::Entity>& checkEntity) { return checkEntity.ID() == entity.ID(); });
                    REQUIRE(foundEntity != position1Entities->second.end());
                }
            }
        }

        WHEN("moving solid entity into other solid")
        {
            auto names = dataGeneration.RandomGroup<Name>(2);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(2);

            auto entity0 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                true,
                std::vector<Tag>{}));
            auto entity1 = reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                positions[1],
                true,
                std::vector<Tag>{}));

            reliquary->Do(MoveTo{ entity0.ID(), positions[1] });

            THEN("has not moved")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.size() == 2);

                auto position0Entities = positionToEntity.find(positions[0]);
                REQUIRE(position0Entities != positionToEntity.end());
                REQUIRE(position0Entities->second.size() == 1);
                REQUIRE(position0Entities->second.begin()->ID() == entity0.ID());

                auto position1Entities = positionToEntity.find(positions[1]);
                REQUIRE(position1Entities != positionToEntity.end());
                REQUIRE(position1Entities->second.size() == 1);
                REQUIRE(position1Entities->second.begin()->ID() == entity1.ID());
            }
        }
    }
}