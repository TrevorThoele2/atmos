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
#include <Atmos/CreateImageAssetResource.h>

#include "DerivedEngine.h"
#include "MockImageAssetResource.h"
#include "MockSurfaceResource.h"

using namespace Atmos;

SCENARIO_METHOD(RenderTestsFixture, "rendering images")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.nullAudioManager, *engine.nullInputManager, *engine.mockGraphicsManager);
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize(fieldReliquary, nullptr);

        auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResourceImplementation>();

        fieldReliquary.Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        const auto camera = Arca::Index<Camera>(fieldReliquary);
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<ImageAssetResourceImplementation>();
        auto imageAsset = fieldReliquary.Do<Arca::Create<Asset::Image>>(
            String{}, std::move(imageResource), Asset::ImageSize{1, 1}, Asset::ImageGridSize{});

        auto materialAsset = fieldReliquary.Do<Arca::Create<Asset::Material>>(
            String{}, Asset::MaterialType::Image, Arca::Index<Asset::Shader>{}, Arca::Index<Asset::Shader>{});

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
        auto scalers = std::vector<Scalers2D>
        {
            Scalers2D
            {
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000)),
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000))
            },
            Scalers2D
            {
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000)),
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000))
            },
            Scalers2D
            {
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000)),
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range<Scalers2D::Value>(1, 1000))
            }
        };

        WHEN("creating static images")
        {
            auto image1 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0]);
            auto image2 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1]);
            auto image3 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2]);
            WHEN("starting engine execution")
            {
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
                            [i, &positions, &scalers, cameraLeft, cameraTop](const ImageRender& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying images, then starting engine execution")
            {
                engine.UseField(std::move(field));
                engine.StartExecution();

                fieldReliquary.Do<Arca::Destroy<StaticImage>>(image1.ID());
                fieldReliquary.Do<Arca::Destroy<StaticImage>>(image2.ID());
                fieldReliquary.Do<Arca::Destroy<StaticImage>>(image3.ID());

                THEN("images were rendered only once")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const ImageRender& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
                }
            }
        }

        WHEN("creating dynamic images")
        {
            auto image1 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0]);
            auto image2 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1]);
            auto image3 = fieldReliquary.Do<Arca::Create<DynamicImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2]);

            WHEN("starting engine execution")
            {
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
                            [i, &positions, &scalers, cameraLeft, cameraTop](const ImageRender& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying static images, then starting execution")
            {
                engine.UseField(std::move(field));
                engine.StartExecution();

                fieldReliquary.Do<Arca::Destroy<DynamicImage>>(image1.ID());
                fieldReliquary.Do<Arca::Destroy<DynamicImage>>(image2.ID());
                fieldReliquary.Do<Arca::Destroy<DynamicImage>>(image3.ID());

                THEN("images were rendered only once")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const ImageRender& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
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
        RegisterFieldTypes(fieldOrigin, *engine.nullAudioManager, *engine.nullInputManager, *engine.mockGraphicsManager);
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize(fieldReliquary, nullptr);

        auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResourceImplementation>();

        fieldReliquary.Do<ResizeCamera>(ScreenSize(100, 100));

        const auto camera = Arca::Index<Camera>(fieldReliquary);
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<ImageAssetResourceImplementation>();
        auto imageAsset = fieldReliquary.Do<Arca::Create<Asset::Image>>(
            String{}, std::move(imageResource), Asset::ImageSize{1, 1}, Asset::ImageGridSize{});

        auto materialAsset = fieldReliquary.Do<Arca::Create<Asset::Material>>(
            String{}, Asset::MaterialType::Image, Arca::Index<Asset::Shader>{}, Arca::Index<Asset::Shader>{});

        WHEN("creating static images and starting execution")
        {
            static constexpr auto gridCellSize = Grid::CellSize<Position3D::Value>;
            static constexpr auto halfGridCellSize = gridCellSize / 2;

            auto image1 = fieldReliquary.Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                Position3D{},
                Scalers2D{});
            auto image2 = fieldReliquary.Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                Position3D{ gridCellSize * -16 + halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Scalers2D{ gridCellSize, gridCellSize });
            auto image3 = fieldReliquary.Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                materialAsset,
                Color{},
                Position3D{ gridCellSize + halfGridCellSize, gridCellSize * 4 + halfGridCellSize, halfGridCellSize },
                Scalers2D{ gridCellSize, gridCellSize * 16 });

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

SCENARIO_METHOD(RenderTestsFixture, "rendering lines")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.nullAudioManager, *engine.nullInputManager, *engine.mockGraphicsManager);
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize(fieldReliquary, nullptr);

        const auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResourceImplementation>();

        auto materialAsset = fieldReliquary.Do<Arca::Create<Asset::Material>>(
            String{}, Asset::MaterialType::Image, Arca::Index<Asset::Shader>{}, Arca::Index<Asset::Shader>{});

        fieldReliquary.Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        const auto camera = Arca::Index<Camera>(fieldReliquary);
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        WHEN("creating lines")
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
            auto line1 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Position2D>{ fromPositions[0], toPositions[0] }, materialAsset});
            auto line2 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Position2D>{ fromPositions[1], toPositions[1] }, materialAsset});
            auto line3 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Position2D>{ fromPositions[2], toPositions[2] }, materialAsset});

            WHEN("starting engine execution")
            {
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

            WHEN("starting engine execution, destroying static images, then starting execution")
            {
                engine.UseField(std::move(field));
                engine.StartExecution();

                fieldReliquary.Do<Arca::Destroy<Line>>(line1.ID());
                fieldReliquary.Do<Arca::Destroy<Line>>(line2.ID());
                fieldReliquary.Do<Arca::Destroy<Line>>(line3.ID());

                THEN("lines were rendered only once")
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
}