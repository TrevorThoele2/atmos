#include <catch.hpp>

#include "WorldSerializationTests.h"

#include <Atmos/StaticMaterialView.h>
#include <Atmos/DynamicMaterialView.h>
#include <Atmos/TypeRegistration.h>

#include <Atmos/OutputWorldArchiveInterface.h>
#include <Atmos/WorldFileExtension.h>
#include <Atmos/ResizeCamera.h>

#include "DerivedEngine.h"

SCENARIO_METHOD(WorldSerializationTestsFixture, "rendering after world serialization")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        field.Reliquary().Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        WHEN("creating static material views and loading through world file then starting execution")
        {
            std::vector<World::Field> fields;
            fields.push_back(std::move(field));

            auto positions = std::vector<Position3D>
            {
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                }
            };
            auto sizes = std::vector<Size2D>
            {
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                }
            };
            fields[0].Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[0], sizes[0]);
            fields[0].Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[1], sizes[1]);
            fields[0].Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[2], sizes[2]);

            auto filePath = "Test." + World::Serialization::worldFileExtension;

            {
                auto outputArchive = World::Serialization::OutputWorldArchiveInterface(filePath);
                outputArchive.Save(fields);
            }

            engine.LoadWorld(filePath);
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->renderer.materialRenders;
                REQUIRE(materialRenders.size() == 3);

                for (auto i = 0; i < 3; ++i)
                {
                    REQUIRE(std::any_of(
                        materialRenders.begin(),
                        materialRenders.end(),
                        [i, &positions, &sizes](const MaterialRender& entry)
                        {
                            return entry.position == positions[i] && entry.size == sizes[i];
                        }));
                }
            }
        }
    }
}