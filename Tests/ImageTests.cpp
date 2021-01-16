#include <catch.hpp>

#include "ImageTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/RelativeImage.h>
#include <Atmos/FindImagesByBox.h>
#include <Atmos/TypeRegistration.h>
#include "DerivedEngine.h"
#include "MockImageAssetResource.h"
#include "MockSurfaceResource.h"
#include <Atmos/SpatialAlgorithms.h>

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(ImageTestsFixture, "images", "[render]")
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
            *engine.mockScriptManager,
            *engine.worldManager,
            Spatial::ScreenSize{
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
                *engine.mockWindow,
                engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

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

            std::vector<Arca::RelicID> imageIDs = { image1.ID(), image2.ID(), image3.ID() };

            WHEN("querying for all images with box 2D")
            {
                auto box = AxisAlignedBox2D(
                    Point2D{},
                    Size2D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box));

                THEN("all images returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedImageIDs.begin(),
                            returnedImageIDs.end(),
                            [i, &imageIDs](const Arca::RelicID& id)
                            {
                                return imageIDs[i] == id;
                            }));
                    }
                }
            }

            WHEN("querying for all images with box 3D")
            {
                auto box = AxisAlignedBox3D(
                    Point3D{},
                    Size3D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box));

                THEN("all images returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedImageIDs.begin(),
                            returnedImageIDs.end(),
                            [i, &imageIDs](const Arca::RelicID& id)
                            {
                                return imageIDs[i] == id;
                            }));
                    }
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

            std::vector<Arca::RelicID> imageIDs = { image1.ID(), image2.ID(), image3.ID() };

            WHEN("querying for all images with box 2D")
            {
                auto box = AxisAlignedBox2D(
                    Point2D{},
                    Size2D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box));

                THEN("all images returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedImageIDs.begin(),
                            returnedImageIDs.end(),
                            [i, &imageIDs](const Arca::RelicID& id)
                            {
                                return imageIDs[i] == id;
                            }));
                    }
                }
            }

            WHEN("querying for all images with box 3D")
            {
                auto box = AxisAlignedBox3D(
                    Point3D{},
                    Size3D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box));

                THEN("all images returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedImageIDs.begin(),
                            returnedImageIDs.end(),
                            [i, &imageIDs](const Arca::RelicID& id)
                            {
                                return imageIDs[i] == id;
                            }));
                    }
                }
            }
        }
    }
}