#include <catch.hpp>

#include "LineTests.h"

#include <Atmos/Line.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/FindLinesByBox.h>
#include <Arca/Create.h>

#include "JavaScriptEngine.h"
#include <Atmos/SpatialAlgorithms.h>

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(LineTestsFixture, "lines", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        JavaScriptEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
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

        auto materialScriptAsset = CompileAndCreateBasicMaterialScript(fieldReliquary);

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, materialScriptAsset, "main", Scripting::Parameters{} });

        WHEN("creating lines")
        {
            auto fromPositions = std::vector
            {
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                },
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                },
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                }
            };

            auto toPositions = std::vector
            {
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                },
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                },
                Point2D
                {
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Point2D::Value>(
                        TestFramework::Range<Point2D::Value>(-1000, 1000))
                }
            };

            auto line1 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector{ fromPositions[0], toPositions[0] }, materialAsset});
            auto line2 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector{ fromPositions[1], toPositions[1] }, materialAsset});
            auto line3 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector{ fromPositions[2], toPositions[2] }, materialAsset});

            std::vector lineIDs = { line1.ID(), line2.ID(), line3.ID() };

            WHEN("querying for all lines with box 2D")
            {
                auto box = AxisAlignedBox2D(
                    Point2D{},
                    Size2D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedLineIDs = fieldReliquary.Do(FindLinesByBox(box));

                THEN("all lines returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedLineIDs.begin(),
                            returnedLineIDs.end(),
                            [i, &lineIDs](const Arca::RelicID& id)
                            {
                                return lineIDs[i] == id;
                            }));
                    }
                }
            }

            WHEN("querying for all lines with box 3D")
            {
                auto box = AxisAlignedBox3D(
                    Point3D{},
                    Size3D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedLineIDs = fieldReliquary.Do(FindLinesByBox(box));

                THEN("all lines returned")
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedLineIDs.begin(),
                            returnedLineIDs.end(),
                            [i, &lineIDs](const Arca::RelicID& id)
                            {
                                return lineIDs[i] == id;
                            }));
                    }
                }
            }
        }
    }
}