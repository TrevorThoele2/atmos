#include <catch.hpp>

#include "ImageRenderingTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/ViewSlice.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/GridCellSize.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>
#include <Atmos/MoveBounds.h>
#include <Atmos/ScaleBounds.h>
#include <Atmos/RotateBounds.h>
#include <Atmos/MathUtility.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Arca/Create.h>
#include <Arca/Destroy.h>

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

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterArcaTypes(fieldOrigin);
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.mockScriptManager,
            *engine.worldManager,
            Spatial::Size2D{ 10000, 10000 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{}});

        auto material = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{}});

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
                material,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                material,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                material,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const RenderImage& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying images, then starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<StaticImage> { image1.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage> { image2.ID() });
                fieldReliquary.Do(Arca::Destroy<StaticImage> { image3.ID() });

                THEN("images were rendered only once")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const RenderImage& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
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
                material,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                Arca::Index<Asset::Image>{},
                0,
                material,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                Arca::Index<Asset::Image>{},
                0,
                material,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating static images without material")
        {
            fieldReliquary.Do(Arca::Create<StaticImage>{
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating dynamic images")
        {
            auto image1 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                material,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                material,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                material,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const RenderImage& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying static images, then starting execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image1.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image2.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicImage>{ image3.ID() });

                THEN("images were rendered only once")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            imageRenders.begin(),
                            imageRenders.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const RenderImage& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
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
                material,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                Arca::Index<Asset::Image>{},
                0,
                material,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                Arca::Index<Asset::Image>{},
                0,
                material,
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.empty());
                }
            }
        }

        WHEN("creating dynamic images without material")
        {
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[2],
                scalers[2],
                Angle2D{} });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
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

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.mockScriptManager,
            *engine.worldManager,
            Spatial::Size2D{
                100,
                100 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{} });

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{} });

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

            engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
            engine.StartExecution();

            THEN("only images inside the camera are rendered")
            {
                auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                REQUIRE(imageRenders.size() == 2);

                REQUIRE(std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image1, cameraLeft, cameraTop](const RenderImage& entry)
                    {
                        auto expectedPosition = image1->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(!std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image2, cameraLeft, cameraTop](const RenderImage& entry)
                    {
                        auto expectedPosition = image2->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));

                REQUIRE(std::any_of(
                    imageRenders.begin(),
                    imageRenders.end(),
                    [&image3, cameraLeft, cameraTop](const RenderImage& entry)
                    {
                        auto expectedPosition = image3->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        return entry.position == expectedPosition;
                    }));
            }
        }

        WHEN("creating dynamic images in camera, moving them outside of the camera and starting execution")
        {
            auto image = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    Point3D{},
                    Scalers2D{},
                    Angle2D{} });

            fieldReliquary.Do(MoveBounds(image.ID(), Point3D{ 1000, 1000, 1000 }));

            engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
            engine.StartExecution();

            THEN("moving them outside the camera causes culling")
            {
                auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                REQUIRE(imageRenders.empty());
            }
        }

        WHEN("creating dynamic images in camera, scaling them outside of the camera and starting execution")
        {
            auto image = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    Point3D{150, 150, 0},
                    Scalers2D{75, 75},
                    Angle2D{} });

            fieldReliquary.Do(ScaleBounds(image.ID(), Scalers2D{25, 25}));

            engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
            engine.StartExecution();

            THEN("moving them outside the camera causes culling")
            {
                auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                REQUIRE(imageRenders.empty());
            }
        }

        WHEN("creating dynamic images in camera, rotating them outside of the camera and starting execution")
        {
            auto image = fieldReliquary.Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    Point3D{ 150, 150, 0 },
                    Scalers2D{ 1, 75 },
                    Angle2D{ pi<Angle2D> / 2 } });

            fieldReliquary.Do(RotateBounds(image.ID(), 0.0f));

            engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
            engine.StartExecution();

            THEN("moving them outside the camera causes culling")
            {
                auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                REQUIRE(imageRenders.empty());
            }
        }
    }
}

SCENARIO_METHOD(ImageRenderingTestsFixture, "rendering view sliced images", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        DerivedEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterArcaTypes(fieldOrigin);
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.mockScriptManager,
            *engine.worldManager,
            Size2D{ 10000, 10000 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        const auto camera = fieldReliquary.Find<Camera>();
        
        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>(
            Size2D{ 100 * 2, 100 * 2 });
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{ 2, 2 }});

        auto material = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{}});

        const auto position = Point3D
        {
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
        };
        const auto scalers = Scalers2D{ 10, 10 };

        WHEN("creating static images with view slices and an asset index of 0")
        {
            auto image = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                material,
                Color{},
                position,
                scalers,
                Angle2D{} });
            auto viewSlice = fieldReliquary.Do(Arca::Create<ViewSlice> {
                image.ID(), ToAxisAlignedBox2D(20, 30, 40, 50) });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("image rendered with correct view slice")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 1);

                    const auto render = imageRenders[0];
                    REQUIRE(render.slice.Left() == Approx(0.01));
                    REQUIRE(render.slice.Top() == Approx(0.015));
                    REQUIRE(render.slice.Right() == Approx(0.02));
                    REQUIRE(render.slice.Bottom() == Approx(0.025));
                }
            }
        }

        WHEN("creating static images with view slices and an asset index of 3")
        {
            auto image = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                3,
                material,
                Color{},
                position,
                scalers,
                Angle2D{} });
            auto viewSlice = fieldReliquary.Do(Arca::Create<ViewSlice> {
                image.ID(), ToAxisAlignedBox2D(20, 30, 40, 50) });
            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("image rendered with correct view slice")
                {
                    auto& imageRenders = engine.mockGraphicsManager->imageRenders;
                    REQUIRE(imageRenders.size() == 1);

                    const auto render = imageRenders[0];
                    REQUIRE(render.slice.Left() == Approx(0.06));
                    REQUIRE(render.slice.Top() == Approx(0.065));
                    REQUIRE(render.slice.Right() == Approx(0.07));
                    REQUIRE(render.slice.Bottom() == Approx(0.075));
                }
            }
        }
    }
}