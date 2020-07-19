#include "catch.hpp"
#include "EntityTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>

SCENARIO_METHOD(EntityTestsFixture, "entity actualization")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating 3 prototypes")
        {
            auto names = dataGeneration.RandomGroup<Name>(3);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(3);
            auto directions = dataGeneration.RandomGroup<Spatial::Angle2D>(3);

            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Script::Instance>{},
                names[0],
                positions[0],
                directions[0]));
            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Script::Instance>{},
                names[1],
                positions[1],
                directions[1]));
            reliquary->Do(Arca::Create<Prototype>(
                Arca::Index<Script::Instance>{},
                names[2],
                positions[2],
                directions[2]));

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();
            auto prototypes = reliquary->Batch<Prototype>();
            auto mappedEntities = Arca::Index<MappedEntities>(*reliquary);

            WHEN("actualizing")
            {
                reliquary->Do(ActualizeAllPrototypes());

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
                            if (entity.name == names[i]
                                && entity.position == positions[i]
                                && entity.direction == directions[i])
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
                    for(auto& entity : entities)
                    {
                        auto found = nameToEntity.find(entity.name);
                        REQUIRE(found != nameToEntity.end());
                        REQUIRE(found->second.ID() == entity.ID());
                    }
                }

                THEN("entities are mapped by position")
                {
                    auto& positionToEntity = mappedEntities->positionToEntity;
                    REQUIRE(positionToEntity.size() == 3);
                    for (auto& entity : entities)
                    {
                        auto found = positionToEntity.find(entity.position);
                        REQUIRE(found != positionToEntity.end());
                        REQUIRE(found->second.ID() == entity.ID());
                    }
                }

                WHEN("moving entities")
                {
                    auto newPositions = dataGeneration.RandomStackGroup<
                        Spatial::Grid::Point,
                        Spatial::Grid::Point::Value,
                        Spatial::Grid::Point::Value>(3);

                    size_t i = 0;
                    for (auto& entity : entities)
                    {
                        reliquary->Do(Move{ Arca::Index<Atmos::Entity::Entity>(entity.ID(), *reliquary), newPositions[i] });
                        ++i;
                    }

                    THEN("entities have been updated in map")
                    {
                        auto& positionToEntity = mappedEntities->positionToEntity;
                        REQUIRE(positionToEntity.size() == 3);
                        for (auto& position : newPositions)
                        {
                            auto found = positionToEntity.find(position);
                            REQUIRE(found != positionToEntity.end());
                            auto foundEntity = std::any_of(
                                entities.begin(),
                                entities.end(),
                                [position](const Atmos::Entity::Entity& entity)
                                {
                                    return entity.position == position;
                                });
                            REQUIRE(foundEntity);
                        }
                    }
                }

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

        WHEN("creating 3 entities")
        {
            auto names = dataGeneration.RandomGroup<Name>(3);
            auto positions = dataGeneration.RandomStackGroup<
                Spatial::Grid::Point,
                Spatial::Grid::Point::Value,
                Spatial::Grid::Point::Value>(3);
            auto directions = dataGeneration.RandomGroup<Spatial::Angle2D>(3);

            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[0],
                positions[0],
                directions[0]));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[1],
                positions[1],
                directions[1]));
            reliquary->Do(Arca::Create<Atmos::Entity::Entity>(
                names[2],
                positions[2],
                directions[2]));

            auto entities = reliquary->Batch<Atmos::Entity::Entity>();
            auto mappedEntities = Arca::Index<MappedEntities>(*reliquary);

            THEN("entities contains correct data")
            {
                for (size_t i = 0; i < entities.Size(); ++i)
                {
                    size_t count = 0;
                    for (auto& entity : entities)
                    {
                        if (entity.name == names[i]
                            && entity.position == positions[i]
                            && entity.direction == directions[i])
                            ++count;
                    }

                    REQUIRE(count == 1);
                }
            }

            THEN("entities are mapped by name")
            {
                auto& nameToEntity = mappedEntities->nameToEntity;
                REQUIRE(nameToEntity.size() == 3);
                for (auto& entity : entities)
                {
                    auto found = nameToEntity.find(entity.name);
                    REQUIRE(found != nameToEntity.end());
                    REQUIRE(found->second.ID() == entity.ID());
                }
            }

            THEN("entities are mapped by position")
            {
                auto& positionToEntity = mappedEntities->positionToEntity;
                REQUIRE(positionToEntity.size() == 3);
                for (auto& entity : entities)
                {
                    auto found = positionToEntity.find(entity.position);
                    REQUIRE(found != positionToEntity.end());
                    REQUIRE(found->second.ID() == entity.ID());
                }
            }

            WHEN("moving entities")
            {
                auto newPositions = dataGeneration.RandomStackGroup<
                    Spatial::Grid::Point,
                    Spatial::Grid::Point::Value,
                    Spatial::Grid::Point::Value>(3);

                size_t i = 0;
                for (auto& entity : entities)
                {
                    reliquary->Do(Move{ Arca::Index<Atmos::Entity::Entity>(entity.ID(), *reliquary), newPositions[i] });
                    ++i;
                }

                THEN("entities have been updated in map")
                {
                    auto& positionToEntity = mappedEntities->positionToEntity;
                    REQUIRE(positionToEntity.size() == 3);
                    for (auto& position : newPositions)
                    {
                        auto found = positionToEntity.find(position);
                        REQUIRE(found != positionToEntity.end());
                        auto foundEntity = std::any_of(
                            entities.begin(),
                            entities.end(),
                            [position](const Atmos::Entity::Entity& entity)
                            {
                                return entity.position == position;
                            });
                        REQUIRE(foundEntity);
                    }
                }
            }

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
}