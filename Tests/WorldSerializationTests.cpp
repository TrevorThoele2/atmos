#include <catch.hpp>

#include "WorldSerializationTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/TypeRegistration.h>

#include <Atmos/OutputWorldArchiveInterface.h>
#include <Atmos/OutputAssetsArchiveInterface.h>
#include <Atmos/WorldFileExtension.h>
#include <Atmos/AssetsFileExtension.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>

#include "DerivedEngine.h"
#include "MockSurfaceResource.h"
#include "MockImageAssetResource.h"
#include "PixelBuffer.h"

SCENARIO_METHOD(WorldSerializationTestsFixture, "rendering after world serialization")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.nullAudioManager,
            *engine.nullInputManager,
            *engine.mockGraphicsManager,
            Spatial::ScreenSize {
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            nullptr);
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        engine.mockGraphicsManager->Initialize();

        const auto camera = Arca::Index<Camera>(fieldReliquary);

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Image> imageResource =
            std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image> {
            "Pixel.png", std::move(imageResource), Asset::ImageGridSize{} });

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, Asset::MaterialType::Image, std::vector<Asset::Material::Pass>{} });

        WHEN("creating static images and loading through world file then starting execution")
        {
            std::vector<World::Field> fields;
            fields.push_back(std::move(field));

            auto positions = std::vector<Spatial::Point3D>
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
            auto scalers = std::vector<Spatial::Scalers2D>
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
                auto outputArchive = World::Serialization::OutputWorldArchiveInterface(filePath);
                outputArchive.Save(fields);
            }

            {
                auto outputArchive = World::Serialization::OutputAssetsArchiveInterface(assetsFilePath);

                World::Serialization::OutputAssetsArchiveInterface::Saves assetSaves;
                assetSaves.images.emplace_back(Name("Pixel.png"), DataBuffer(pixelBuffer));
                outputArchive.Save(assetSaves);
            }

            engine.LoadWorld(filePath, assetsFilePath);

            auto& loadedFieldReliquary = engine.CurrentField()->Reliquary();
            engine.mockGraphicsManager->Initialize();

            engine.StartExecution();

            THEN("all images rendered in graphics manager")
            {
                auto mainSurface = Arca::Index<MainSurface>(loadedFieldReliquary);
                auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

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
                                && entry.size == Spatial::Size2D{ scalers[i].x, scalers[i].y };
                        }));
                }
            }
        }
    }
}