#include <catch.hpp>

#include "WorldSerializationTests.h"

#include <Atmos/StaticMaterialView.h>
#include <Atmos/DynamicMaterialView.h>
#include <Atmos/TypeRegistration.h>

#include <Atmos/OutputWorldArchiveInterface.h>
#include <Atmos/WorldFileExtension.h>

#include "DerivedEngine.h"

SCENARIO_METHOD(WorldSerializationTestsFixture, "rendering after world serialization")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.globalReliquary);
        World::Field field(0, fieldOrigin.Actualize());

        WHEN("creating static material views and loading through world file then starting execution")
        {
            std::vector<World::Field> fields;
            fields.push_back(std::move(field));

            auto positions = dataGeneration.RandomStackGroup<
                Position3D,
                Position3D::Value,
                Position3D::Value,
                Position3D::Value>(3);
            fields[0].Reliquary().Create<StaticMaterialView>(positions[0]);
            fields[0].Reliquary().Create<StaticMaterialView>(positions[1]);
            fields[0].Reliquary().Create<StaticMaterialView>(positions[2]);

            auto filePath = "Test." + World::Serialization::worldFileExtension;

            {
                auto outputArchive = World::Serialization::OutputWorldArchiveInterface(filePath);
                outputArchive.Save(fields);
            }

            engine.LoadWorld(filePath);
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->materialRenders;
                REQUIRE(materialRenders.size() == 3);

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[0].x && entry.position.y == positions[0].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[1].x && entry.position.y == positions[1].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[2].x && entry.position.y == positions[2].y;
                    }));
            }
        }
    }
}