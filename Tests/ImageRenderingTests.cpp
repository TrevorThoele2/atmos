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

#include "RealEngine.h"
#include "MockImageAssetResource.h"

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(ImageRenderingTestsFixture, "rendering images", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        RealEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterArcaTypes(fieldOrigin);
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.scriptManager,
            *engine.worldManager,
            Spatial::Size2D{ 10000, 10000 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{}});

        auto materialScriptAsset = CompileAndCreateBasicMaterialScript(fieldReliquary);

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, materialScriptAsset, "main", Scripting::Parameters{} });

        auto positions = std::vector
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
        auto scalers = std::vector
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
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all images rendered in graphics manager")
                {
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            commands.begin(),
                            commands.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const Raster::Command& command)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                return drawCommand.position == ToPoint2D(expectedPosition);
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            commands.begin(),
                            commands.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const Raster::Command& command)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                return drawCommand.position == ToPoint2D(expectedPosition);
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
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.empty());
                }
            }
        }

        WHEN("creating static images without materialAsset")
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.empty());
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
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all images rendered in graphics manager")
                {
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            commands.begin(),
                            commands.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const Raster::Command& command)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                return drawCommand.position == ToPoint2D(expectedPosition);
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            commands.begin(),
                            commands.end(),
                            [i, &positions, &scalers, cameraLeft, cameraTop](const Raster::Command& command)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                return drawCommand.position == ToPoint2D(expectedPosition);
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
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no images rendered in graphics manager")
                {
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.empty());
                }
            }
        }

        WHEN("creating dynamic images without materialAsset")
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.empty());
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
        RealEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.scriptManager,
            *engine.worldManager,
            Size2D{
                100,
                100 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{} });

        auto materialScriptAsset = CompileAndCreateBasicMaterialScript(fieldReliquary);

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, materialScriptAsset, "main", Scripting::Parameters{} });

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
                auto& commands = engine.mockGraphicsManager->commands;
                REQUIRE(commands.size() == 2);

                REQUIRE(std::any_of(
                    commands.begin(),
                    commands.end(),
                    [&image1, cameraLeft, cameraTop](const Raster::Command& command)
                    {
                        auto expectedPosition = image1->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        auto drawCommand = std::get<Raster::DrawImage>(command);

                        return drawCommand.position == ToPoint2D(expectedPosition);
                    }));

                REQUIRE(!std::any_of(
                    commands.begin(),
                    commands.end(),
                    [&image2, cameraLeft, cameraTop](const Raster::Command& command)
                    {
                        auto expectedPosition = image2->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        auto drawCommand = std::get<Raster::DrawImage>(command);

                        return drawCommand.position == ToPoint2D(expectedPosition);
                    }));

                REQUIRE(std::any_of(
                    commands.begin(),
                    commands.end(),
                    [&image3, cameraLeft, cameraTop](const Raster::Command& command)
                    {
                        auto expectedPosition = image3->Position();
                        expectedPosition.x -= cameraLeft;
                        expectedPosition.y -= cameraTop;

                        auto drawCommand = std::get<Raster::DrawImage>(command);

                        return drawCommand.position == ToPoint2D(expectedPosition);
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
                auto& commands = engine.mockGraphicsManager->commands;
                REQUIRE(commands.empty());
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
                auto& commands = engine.mockGraphicsManager->commands;
                REQUIRE(commands.empty());
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
                auto& commands = engine.mockGraphicsManager->commands;
                REQUIRE(commands.empty());
            }
        }
    }
}

SCENARIO_METHOD(ImageRenderingTestsFixture, "rendering view sliced images", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        RealEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterArcaTypes(fieldOrigin);
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.scriptManager,
            *engine.worldManager,
            Size2D{ 10000, 10000 },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto mainSurface = fieldReliquary.Find<MainSurface>();

        const auto camera = fieldReliquary.Find<Camera>();
        
        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>(
            Size2D{ 100 * 2, 100 * 2 });
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{ 2, 2 }});

        auto materialScriptAsset = CompileAndCreateBasicMaterialScript(fieldReliquary);

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, materialScriptAsset, "main", Scripting::Parameters{} });

        const auto position = Point3D
        {
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range<Point3D::Value>(-1000, 1000))
        };
        constexpr auto scalers = Scalers2D{ 10, 10 };

        WHEN("creating static images with view slices and an asset index of 0")
        {
            auto image = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 1);

                    const auto drawCommand = std::get<Raster::DrawImage>(commands[0]);
                    REQUIRE(drawCommand.viewSlice.Left() == Approx(20));
                    REQUIRE(drawCommand.viewSlice.Top() == Approx(30));
                    REQUIRE(drawCommand.viewSlice.Right() == Approx(40));
                    REQUIRE(drawCommand.viewSlice.Bottom() == Approx(50));
                }
            }
        }

        WHEN("creating static images with view slices and an asset index of 3")
        {
            auto image = fieldReliquary.Do(Arca::Create<StaticImage> {
                imageAsset,
                3,
                materialAsset,
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
                    auto& commands = engine.mockGraphicsManager->commands;
                    REQUIRE(commands.size() == 1);

                    const auto drawCommand = std::get<Raster::DrawImage>(commands[0]);
                    REQUIRE(drawCommand.viewSlice.Left() == Approx(20));
                    REQUIRE(drawCommand.viewSlice.Top() == Approx(30));
                    REQUIRE(drawCommand.viewSlice.Right() == Approx(40));
                    REQUIRE(drawCommand.viewSlice.Bottom() == Approx(50));
                }
            }
        }
    }
}