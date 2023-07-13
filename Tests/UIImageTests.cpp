#include <catch.hpp>

#include "UIImageTests.h"

#include <Atmos/UIImage.h>
#include <Atmos/FindImagesByBox.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Arca/Create.h>

#include "JavaScriptEngine.h"
#include "MockImageAssetResource.h"

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(UIImageTestsFixture, "ui images", "[ui]")
{
    const auto max2D = AxisAlignedBox2D(
        Point2D{},
        Size2D
        {
            std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
        });

    const auto max3D = AxisAlignedBox3D(
        Point3D{},
        Size3D
        {
            std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
            std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
        });

    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        JavaScriptEngine engine(logger);

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
            *engine.mockWorldManager,
            Size2D{
                std::numeric_limits<Size2D::Value>::max(),
                std::numeric_limits<Size2D::Value>::max() },
                *engine.mockWindow,
                engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

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

        WHEN("creating ui images")
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

            std::vector imageIDs = { image1.ID(), image2.ID(), image3.ID() };

            WHEN("retrieving image size")
            {
                THEN("correct image size returned")
                {
                    REQUIRE(image1->Size() == Size2D{ scalers[0].x, scalers[0].y });
                    REQUIRE(image2->Size() == Size2D{ scalers[1].x, scalers[1].y });
                    REQUIRE(image3->Size() == Size2D{ scalers[2].x, scalers[2].y });
                }
            }

            WHEN("querying for all world images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box, Space::World));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }

            WHEN("querying for all screen images with box 2D")
            {
                auto box = max2D;

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box, Space::Screen));
                
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

            WHEN("querying for all world images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box, Space::World));

                THEN("no images returned")
                {
                    REQUIRE(returnedImageIDs.empty());
                }
            }

            WHEN("querying for all screen images with box 3D")
            {
                auto box = max3D;

                auto returnedImageIDs = fieldReliquary.Do(FindImagesByBox(box, Space::Screen));

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