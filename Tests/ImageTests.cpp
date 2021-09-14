#include <catch.hpp>

#include "ImageTests.h"

#include <Atmos/StaticImage.h>
#include <Atmos/DynamicImage.h>
#include <Atmos/FindImagesByBox.h>
#include <Atmos/TypeRegistration.h>
#include "DerivedEngine.h"
#include "MockImageAssetResource.h"
#include "MockSurfaceResource.h"
#include <Atmos/SpatialAlgorithms.h>

using namespace Atmos;
using namespace Spatial;

ImageTestsFixture::ImageTestsFixture() : logger(Logging::Severity::Verbose)
{
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
        Size2D{
            std::numeric_limits<Size2D::Value>::max(),
            std::numeric_limits<Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
    field = World::Field(0, fieldOrigin.Actualize());

    fieldReliquary = &field.Reliquary();

    materialAsset = fieldReliquary->Do(Arca::Create<Asset::Material> {
        String{}, std::vector<Asset::Material::Pass>{}});
}

SCENARIO_METHOD(ImageTestsFixture, "images", "[render]")
{
    GIVEN("image and material assets")
    {
        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>();
        auto imageAsset = fieldReliquary->Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{}});

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
            auto image1 = fieldReliquary->Do(Arca::Create<StaticImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[0],
                    scalers[0],
                    Angle2D{} });
            auto image2 = fieldReliquary->Do(Arca::Create<StaticImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[1],
                    scalers[1],
                    Angle2D{} });
            auto image3 = fieldReliquary->Do(Arca::Create<StaticImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[2],
                    scalers[2],
                    Angle2D{} });

            std::vector imageIDs = { image1.ID(), image2.ID(), image3.ID() };

            WHEN("querying for all world images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::World));

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

            WHEN("querying for all screen images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::Screen));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }

            WHEN("querying for all world images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::World));

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

            WHEN("querying for all screen images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::Screen));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }
        }

        WHEN("creating dynamic images")
        {
            auto image1 = fieldReliquary->Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[0],
                    scalers[0],
                    Angle2D{} });
            auto image2 = fieldReliquary->Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[1],
                    scalers[1],
                    Angle2D{} });
            auto image3 = fieldReliquary->Do(Arca::Create<DynamicImage> {
                imageAsset,
                    0,
                    materialAsset,
                    Color{},
                    positions[2],
                    scalers[2],
                    Angle2D{} });

            std::vector imageIDs = { image1.ID(), image2.ID(), image3.ID() };

            WHEN("querying for all world images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::World));

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

            WHEN("querying for all screen images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::Screen));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }

            WHEN("querying for all world images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::World));

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

            WHEN("querying for all screen images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary->Do(FindImagesByBox(box, Space::Screen));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }
        }
    }
}

SCENARIO_METHOD(ImageTestsFixture, "image asset slice", "[render]")
{
    using Data = std::tuple<int, float, float, float, float>;

    GIVEN("image asset with 2 columns and 1 row")
    {
        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>(Size2D{ 80, 40 });
        const auto imageAsset = fieldReliquary->Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{ 2, 1 }});

        WHEN("creating static image")
        {
            const auto [assetIndex, left, top, right, bottom] = GENERATE(
                Data(0, 0.0f, 0.0f, 40.0f, 40.0f),
                Data(1, 40.0f, 0.0f, 80.0f, 40.0f));

            const auto image1 = fieldReliquary->Do(Arca::Create<StaticImage> {
                imageAsset,
                    assetIndex,
                    materialAsset,
                    Color{},
                    Point3D{},
                    Scalers2D{},
                    Angle2D{} });

            WHEN("querying asset slice")
            {
                const auto assetSlice = image1->AssetSlice();

                THEN("asset slice is correct")
                {
                    REQUIRE(assetSlice.Left() == Approx(left));
                    REQUIRE(assetSlice.Top() == Approx(top));
                    REQUIRE(assetSlice.Right() == Approx(right));
                    REQUIRE(assetSlice.Bottom() == Approx(bottom));
                }
            }
        }
    }

    GIVEN("image asset with 2 columns and 2 rows")
    {
        std::unique_ptr<Asset::Resource::Image> imageResource = std::make_unique<MockImageAssetResource>(Size2D{ 80, 80 });
        const auto imageAsset = fieldReliquary->Do(Arca::Create<Asset::Image> {
            String{}, std::move(imageResource), Asset::ImageGridSize{ 2, 2 }});

        WHEN("creating static image")
        {
            const auto [assetIndex, left, top, right, bottom] = GENERATE(
                Data(0, 0.0f, 0.0f, 40.0f, 40.0f),
                Data(1, 40.0f, 0.0f, 80.0f, 40.0f),
                Data(2, 0.0f, 40.0f, 40.0f, 80.0f),
                Data(3, 40.0f, 40.0f, 80.0f, 80.0f));

            const auto image1 = fieldReliquary->Do(Arca::Create<StaticImage> {
                imageAsset,
                    assetIndex,
                    materialAsset,
                    Color{},
                    Point3D{},
                    Scalers2D{},
                    Angle2D{} });

            WHEN("querying asset slice")
            {
                const auto assetSlice = image1->AssetSlice();

                THEN("asset slice is correct")
                {
                    REQUIRE(assetSlice.Left() == Approx(left));
                    REQUIRE(assetSlice.Top() == Approx(top));
                    REQUIRE(assetSlice.Right() == Approx(right));
                    REQUIRE(assetSlice.Bottom() == Approx(bottom));
                }
            }
        }
    }
}