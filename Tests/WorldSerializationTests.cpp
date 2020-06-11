#include <catch.hpp>

#include "WorldSerializationTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/TypeRegistration.h>

#include <Atmos/OutputWorldArchiveInterface.h>
#include <Atmos/WorldFileExtension.h>
#include <Atmos/ResizeCamera.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>

#include "DerivedEngine.h"
#include "MockSurfaceData.h"
#include "MockImageAssetData.h"

SCENARIO_METHOD(WorldSerializationTestsFixture, "rendering after world serialization")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        Arca::Postulate<GraphicsManager*>(fieldReliquary)->Initialize(fieldReliquary, nullptr);

        fieldReliquary.Do<ResizeCamera>(ScreenSize(
            std::numeric_limits<ScreenSize::Dimension>::max(),
            std::numeric_limits<ScreenSize::Dimension>::max()));

        const auto camera = Arca::Index<Camera>(fieldReliquary);
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();

        std::unique_ptr<Asset::ImageAssetData> imageData = std::make_unique<ImageAssetDataImplementation>();
        auto imageAsset = engine.TheGlobalReliquary()->Do<Arca::Create<Asset::ImageAsset>>(
            String{}, std::move(imageData), Asset::ImageAssetGridSize{});

        WHEN("creating static images and loading through world file then starting execution")
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
            fields[0].Reliquary().Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                Color{},
                Arca::Index<Asset::MaterialAsset>{},
                positions[0],
                scalers[0]);
            fields[0].Reliquary().Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                Color{},
                Arca::Index<Asset::MaterialAsset>{},
                positions[1],
                scalers[1]);
            fields[0].Reliquary().Do<Arca::Create<StaticImage>>(
                imageAsset,
                0,
                Color{},
                Arca::Index<Asset::MaterialAsset>{},
                positions[2],
                scalers[2]);

            auto filePath = "Test." + World::Serialization::worldFileExtension;

            {
                auto outputArchive = World::Serialization::OutputWorldArchiveInterface(filePath);
                outputArchive.Save(fields);
            }

            engine.LoadWorld(filePath);

            auto& loadedFieldReliquary = engine.CurrentField()->Reliquary();
            Arca::Postulate<GraphicsManager*>(loadedFieldReliquary)->Initialize(loadedFieldReliquary, nullptr);

            engine.StartExecution();

            THEN("all images rendered in graphics manager")
            {
                auto mainSurface = Arca::Index<MainSurface>(loadedFieldReliquary);
                auto mainSurfaceImplementation = mainSurface->Data<MockSurfaceDataImplementation>();

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