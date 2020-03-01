#include <catch.hpp>

#include "RenderTests.h"

#include <Atmos/StaticMaterialView.h>
#include <Atmos/DynamicMaterialView.h>
#include <Atmos/Line.h>
#include <Atmos/ResizeCamera.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/GridCellSize.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>

#include "DerivedEngine.h"

using namespace Atmos;

SCENARIO_METHOD(RenderTestsFixture, "rendering material views")
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

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating static material views and starting execution")
        {
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
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
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
                        [i, &positions, &sizes, cameraLeft, cameraTop](const MaterialRender& entry)
                        {
                            auto expectedPosition = positions[i];
                            expectedPosition.x -= cameraLeft;
                            expectedPosition.y -= cameraTop;

                            return entry.position == expectedPosition && entry.size == sizes[i];
                        }));
                }
            }
        }

        WHEN("creating dynamic material views and starting execution")
        {
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
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
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
                        [i, &positions, &sizes, cameraLeft, cameraTop](const MaterialRender& entry)
                        {
                            auto expectedPosition = positions[i];
                            expectedPosition.x -= cameraLeft;
                            expectedPosition.y -= cameraTop;

                            return entry.position == expectedPosition && entry.size == sizes[i];
                        }));
                }
            }
        }
    }
}

SCENARIO_METHOD(RenderTestsFixture, "rendering lines")
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

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating lines and starting execution")
        {
            auto fromPositions = std::vector<Position3D>
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

            auto toPositions = std::vector<Position3D>
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
            field.Reliquary().Do<Arca::Create<Line>>(fromPositions[0], toPositions[0]);
            field.Reliquary().Do<Arca::Create<Line>>(fromPositions[1], toPositions[1]);
            field.Reliquary().Do<Arca::Create<Line>>(fromPositions[2], toPositions[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all lines rendered in graphics manager")
            {
                auto& lineRenders = engine.mockGraphicsManager->renderer.lineRenders;
                REQUIRE(lineRenders.size() == 3);

                for (auto i = 0; i < 3; ++i)
                {
                    REQUIRE(std::any_of(
                        lineRenders.begin(),
                        lineRenders.end(),
                        [i, &fromPositions, &toPositions, cameraLeft, cameraTop](const LineRender& entry)
                        {
                            auto expectedFromPosition = fromPositions[i];
                            expectedFromPosition.x -= cameraLeft;
                            expectedFromPosition.y -= cameraTop;

                            auto expectedToPosition = toPositions[i];
                            expectedToPosition.x -= cameraLeft;
                            expectedToPosition.y -= cameraTop;

                            return entry.from == expectedFromPosition && entry.to == expectedToPosition;
                        }));
                }
            }
        }
    }
}

SCENARIO_METHOD(RenderTestsFixture, "rendering culled material views")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        field.Reliquary().Do<ResizeCamera>(ScreenSize(100, 100));

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating static material views and starting execution")
        {
            static constexpr auto gridCellSize = Grid::CellSize<Position3D::Value>;
            static constexpr auto halfGridCellSize = gridCellSize / 2;

            auto materialView1 = field.Reliquary().Do<Arca::Create<StaticMaterialView>>();
            auto materialView2 = field.Reliquary().Do<Arca::Create<StaticMaterialView>>(
                Position3D{ gridCellSize * -16 + halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Size2D{ gridCellSize, gridCellSize });
            auto materialView3 = field.Reliquary().Do<Arca::Create<StaticMaterialView>>(
                Position3D{ gridCellSize + halfGridCellSize, gridCellSize * 4 + halfGridCellSize, halfGridCellSize },
                Size2D{ gridCellSize, gridCellSize * 16 });

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("only material views inside the camera are rendered")
            {
                auto& materialRenders = engine.mockGraphicsManager->renderer.materialRenders;
                REQUIRE(materialRenders.size() == 2);

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&materialView1, cameraLeft, cameraTop](const MaterialRender& entry)
                    {
                        auto expectedPosition = materialView1->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(!std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&materialView2, cameraLeft, cameraTop](const MaterialRender& entry)
                    {
                        auto expectedPosition = materialView2->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&materialView3, cameraLeft, cameraTop](const MaterialRender& entry)
                    {
                        auto expectedPosition = materialView3->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));
            }
        }
    }
}