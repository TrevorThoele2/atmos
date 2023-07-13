#include <catch.hpp>

#include "WorldSerializationTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/TypeRegistration.h>

#include <Atmos/OutputWorldFile.h>
#include <Atmos/OutputAssetsFile.h>
#include <Atmos/WorldFileExtension.h>
#include <Atmos/AssetsFileExtension.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Arca/Create.h>

#include "RealEngine.h"
#include "MockImageAssetResource.h"
#include "PixelBuffer.h"

SCENARIO_METHOD(WorldSerializationTestsFixture, "rendering after world serialization", "[world]")
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
            Spatial::Size2D {
                std::numeric_limits<Spatial::Size2D::Value>::max(),
                std::numeric_limits<Spatial::Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource =
            std::make_unique<MockImageAssetResource>();
        const Name imageAssetName = "Pixel";
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            imageAssetName, std::move(imageResource), Asset::ImageGridSize{} });

        auto materialScriptAsset = CompileAndCreateBasicMaterialScript(fieldReliquary);

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, materialScriptAsset, "main", Scripting::Parameters{} });

        WHEN("creating static images and loading through world file then starting execution")
        {
            std::vector<World::Field> fields;
            fields.push_back(std::move(field));

            auto positions = std::vector
            {
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000))
                },
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000))
                },
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point3D::Value>(
                        TestFramework::Range<Spatial::Point3D::Value>(-1000, 1000))
                }
            };
            auto scalers = std::vector
            {
                Spatial::Scalers2D
                {
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000)),
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000))
                },
                Spatial::Scalers2D
                {
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000)),
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000))
                },
                Spatial::Scalers2D
                {
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000)),
                    dataGeneration.Random<Spatial::Scalers2D::Value>(
                        TestFramework::Range<Spatial::Scalers2D::Value>(1, 1000))
                }
            };
            fields[0].Reliquary().Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[0],
                scalers[0],
                Spatial::Angle2D{} });
            fields[0].Reliquary().Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[1],
                scalers[1],
                Spatial::Angle2D{} });
            fields[0].Reliquary().Do(Arca::Create<StaticImage> {
                imageAsset,
                0,
                materialAsset,
                Color{},
                positions[2],
                scalers[2],
                Spatial::Angle2D{} });

            auto filePath = "Test." + World::Serialization::worldFileExtension;
            auto assetsFilePath = "Test." + World::Serialization::assetsFileExtension;

            {
                auto outputFile = World::Serialization::OutputWorldFile(filePath);
                outputFile.Save(fields, {});
            }

            {
                auto outputFile = World::Serialization::OutputAssetsFile(assetsFilePath);

                World::Serialization::OutputAssetsFile::Saves assetSaves;
                assetSaves.images.emplace_back(imageAssetName, Buffer(pixelBuffer));
                assetSaves.scripts.emplace_back("basic_script", BasicMaterialScriptBytes(fieldReliquary));
                outputFile.Save(assetSaves);
            }

            engine.LoadWorld(filePath, assetsFilePath);
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

                            auto expectedPosition = positions[i];
                            expectedPosition.x -= cameraLeft;
                            expectedPosition.y -= cameraTop;

                            return drawCommand.position == ToPoint2D(expectedPosition);
                        }));
                }
            }
        }
    }
}