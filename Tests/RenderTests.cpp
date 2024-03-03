#include <catch.hpp>

#include "RenderTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/Line.h>
#include <Atmos/ResizeCamera.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/GridCellSize.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>

#include "DerivedEngine.h"
#include "MockSurfaceData.h"

using namespace Atmos;

SCENARIO_METHOD(RenderTestsFixture, "rendering images")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        auto mainSurface = Arca::Index<MainSurface>(field.Reliquary());
        auto mainSurfaceImplementation = mainSurface->Data<MockSurfaceDataImplementation>();

        field.Reliquary().Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating static images and starting execution")
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
            field.Reliquary().Do<Arca::Create<StaticImage>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<StaticImage>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<StaticImage>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all images rendered in graphics manager")
            {
                auto& imageRenders = mainSurfaceImplementation->imageRenders;
                REQUIRE(imageRenders.size() == 3);

                for (auto i = 0; i < 3; ++i)
                {
                    REQUIRE(std::any_of(
                        imageRenders.begin(),
                        imageRenders.end(),
                        [i, &positions, &sizes, cameraLeft, cameraTop](const ImageRender& entry)
                        {
                            auto expectedPosition = positions[i];
                            expectedPosition.x -= cameraLeft;
                            expectedPosition.y -= cameraTop;

                            return entry.position == expectedPosition && entry.size == sizes[i];
                        }));
                }
            }
        }

        WHEN("creating dynamic images and starting execution")
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
            field.Reliquary().Do<Arca::Create<DynamicImage>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<DynamicImage>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<DynamicImage>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all images rendered in graphics manager")
            {
                auto& imageRenders = mainSurfaceImplementation->imageRenders;
                REQUIRE(imageRenders.size() == 3);

                for (auto i = 0; i < 3; ++i)
                {
                    REQUIRE(std::any_of(
                        imageRenders.begin(),
                        imageRenders.end(),
                        [i, &positions, &sizes, cameraLeft, cameraTop](const ImageRender& entry)
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

        const auto mainSurface = Arca::Index<MainSurface>(field.Reliquary());
        auto mainSurfaceImplementation = mainSurface->Data<MockSurfaceDataImplementation>();

        field.Reliquary().Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating lines and starting execution")
        {
            auto fromPositions = std::vector<Position2D>
            {
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                },
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                },
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                }
            };

            auto toPositions = std::vector<Position2D>
            {
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                },
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                },
                Position2D
                {
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position2D::Value>(TestFramework::Range<Position2D::Value>(-1000, 1000))
                }
            };
            field.Reliquary().Do(Arca::Create<Line>{ std::vector<Position2D>{fromPositions[0], toPositions[0] }});
            field.Reliquary().Do(Arca::Create<Line>{ std::vector<Position2D>{fromPositions[1], toPositions[1] }});
            field.Reliquary().Do(Arca::Create<Line>{ std::vector<Position2D>{fromPositions[2], toPositions[2] }});

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all lines rendered in graphics manager")
            {
                auto& lineRenders = mainSurfaceImplementation->lineRenders;
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

                            return entry.points[0] == expectedFromPosition && entry.points[1] == expectedToPosition;
                        }));
                }
            }
        }
    }
}

SCENARIO_METHOD(RenderTestsFixture, "rendering culled images")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        auto mainSurface = Arca::Index<MainSurface>(field.Reliquary());
        auto mainSurfaceImplementation = mainSurface->Data<MockSurfaceDataImplementation>();

        field.Reliquary().Do<ResizeCamera>(ScreenSize(100, 100));

        const auto camera = Arca::Index<Camera>(field.Reliquary());
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating static images and starting execution")
        {
            static constexpr auto gridCellSize = Grid::CellSize<Position3D::Value>;
            static constexpr auto halfGridCellSize = gridCellSize / 2;

            auto image1 = field.Reliquary().Do<Arca::Create<StaticImage>>();
            auto image2 = field.Reliquary().Do<Arca::Create<StaticImage>>(
                Position3D{ gridCellSize * -16 + halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Size2D{ gridCellSize, gridCellSize });
            auto image3 = field.Reliquary().Do<Arca::Create<StaticImage>>(
                Position3D{ gridCellSize + halfGridCellSize, gridCellSize * 4 + halfGridCellSize, halfGridCellSize },
                Size2D{ gridCellSize, gridCellSize * 16 });

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("only images inside the camera are rendered")
            {
                auto& imageRenders = mainSurfaceImplementation->imageRenders;
                REQUIRE(imageRenders.size() == 2);

                REQUIRE(std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image1, cameraLeft, cameraTop](const ImageRender& entry)
                    {
                        auto expectedPosition = image1->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(!std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image2, cameraLeft, cameraTop](const ImageRender& entry)
                    {
                        auto expectedPosition = image2->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image3, cameraLeft, cameraTop](const ImageRender& entry)
                    {
                        auto expectedPosition = image3->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));
            }
        }
    }
}