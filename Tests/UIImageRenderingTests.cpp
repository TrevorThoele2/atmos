#include <catch.hpp>

#include "UIImageRenderingTests.h"

#include <Atmos/UIImage.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Arca/Create.h>
#include <Arca/Destroy.h>

#include "RealEngine.h"
#include "MockImageAssetResource.h"

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(UIImageRenderingTestsFixture, "rendering UI images", "[ui]")
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
            auto image1 = fieldReliquary.Do(Arca::Create<UI::Image> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            auto image2 = fieldReliquary.Do(Arca::Create<UI::Image> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            auto image3 = fieldReliquary.Do(Arca::Create<UI::Image> {
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
                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                const auto expectedPosition = positions[i];
                                
                                return drawCommand.position == ToPoint2D(expectedPosition);
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying images, then starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<UI::Image> { image1.ID() });
                fieldReliquary.Do(Arca::Destroy<UI::Image> { image2.ID() });
                fieldReliquary.Do(Arca::Destroy<UI::Image> { image3.ID() });

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
                                auto drawCommand = std::get<Raster::DrawImage>(command);

                                const auto expectedPosition = positions[i];
                                
                                return drawCommand.position == ToPoint2D(expectedPosition);
                            }));
                    }
                }
            }
        }

        WHEN("creating static images without asset")
        {
            fieldReliquary.Do(Arca::Create<UI::Image>{
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<UI::Image> {
                Arca::Index<Asset::Image>{},
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<UI::Image> {
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
            fieldReliquary.Do(Arca::Create<UI::Image>{
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[0],
                scalers[0],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<UI::Image> {
                imageAsset,
                0,
                Arca::Index<Asset::Material>{},
                Color{},
                positions[1],
                scalers[1],
                Angle2D{} });
            fieldReliquary.Do(Arca::Create<UI::Image> {
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