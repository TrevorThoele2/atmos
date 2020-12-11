#include <catch.hpp>

#include "ImageRenderingTests.h"

#include <Arca/LocalRelic.h>
#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/RelativeImage.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/GridCellSize.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>

#include "DerivedEngine.h"
#include "MockImageAssetResource.h"
#include "MockSurfaceResource.h"

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(ImageRenderingTestsFixture, "rendering images", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        DerivedEngine engine(logger);
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterArcaTypes(fieldOrigin);
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockImageAssetManager,
            *engine.nullAudioManager,
            *engine.nullInputManager,
            *engine.mockGraphicsManager,
            *engine.mockScriptManager,
            Spatial::ScreenSize{
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize();

        auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        const auto camera = Arca::Index<Camera>(fieldReliquary);

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{}});

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::ImageMaterial> {
            String{}, std::vector<Asset::ImageMaterial::Pass>{}});

        auto positions = std::vector<Point3D>
        {
            Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
            },
            Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
            },
            Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
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
            auto image1 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
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
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<StaticImage> { image1.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage> { image2.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage> { image3.ID() });

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

        WHEN("creating static images without asset")
        {
            fieldReliquary.Do(Arca::Create<StaticImage>{
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating static images without material")
        {
            fieldReliquary.Do(Arca::Create<StaticImage>{
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating dynamic images")
        {
            auto image1 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
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
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image1.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image2.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image3.ID() });

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

        WHEN("creating dynamic images without asset")
        {
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating dynamic images without material")
        {
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating relative images")
        {
            auto parentPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
            };

            auto parent = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());
            fieldReliquary.Do(
                Arca::Create<Bounds>(parent, parentPosition, Size2D{1, 1}, Scalers2D{1, 1}, Angle2D{}));

            auto image1 = fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
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
                            [i, &positions, &scalers, cameraLeft, cameraTop, parentPosition](const ImageRender& entry)
                            {
                                const auto expectedPosition = Point3D
                                {
                                    parentPosition.x + positions[i].x - cameraLeft,
                                    parentPosition.y + positions[i].y - cameraTop,
                                    parentPosition.z + positions[i].z
                                };

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying images, then starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<StaticImage>{ image1.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage>{ image2.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage>{ image3.ID() });

                THEN("images were rendered only once")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop, parentPosition](const ImageRender& entry)
                            {
                                const auto expectedPosition = Point3D
                                {
                                    parentPosition.x + positions[i].x - cameraLeft,
                                    parentPosition.y + positions[i].y - cameraTop,
                                    parentPosition.z + positions[i].z
                                };

                                return entry.position == expectedPosition
                                    && entry.size == Size2D{ scalers[i].x, scalers[i].y };
                            }));
                    }
                }
            }
        }

        WHEN("creating relative images without asset")
        {
            auto parentPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
            };

            auto parent = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());
            fieldReliquary.Do(
                Arca::Create<Bounds>(parent, parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating relative images without material")
        {
            auto parentPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
            };

            auto parent = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());
            fieldReliquary.Do(
                Arca::Create<Bounds>(parent, parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::CreateChild<RelativeImage> {
                parent,
                imageAsset,
                0,
                Arca::Index<Asset::ImageMaterial>{},
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = mainSurfaceImplementation->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }
    }
}

SCENARIO_METHOD(ImageRenderingTestsFixture, "rendering culled images", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        DerivedEngine engine(logger);
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockImageAssetManager,
            *engine.nullAudioManager,
            *engine.nullInputManager,
            *engine.mockGraphicsManager,
            *engine.mockScriptManager,
            Spatial::ScreenSize{
                100,
                100 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize();

        auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        const auto camera = Arca::Index<Camera>(fieldReliquary);

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{} });

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::ImageMaterial> {
            String{}, std::vector<Asset::ImageMaterial::Pass>{} });

        WHEN("creating static images and starting execution")
        {
            static constexpr auto gridCellSize = Grid::CellSize<Point3D::Value>;
            static constexpr auto halfGridCellSize = gridCellSize / 2;

            auto image1 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                Point3D{},
                Scalers2D{},
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                Point3D{ gridCellSize * -16 + halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Scalers2D{ gridCellSize, gridCellSize },
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                Point3D{ gridCellSize + halfGridCellSize, gridCellSize * 4 + halfGridCellSize, halfGridCellSize },
                Scalers2D{ gridCellSize, gridCellSize * 16 },
                Angle2D{} });

            engine.UseField(std::move(field), std::filesystem::current_path() / "Assets.dat");
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