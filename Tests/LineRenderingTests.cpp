#include <catch.hpp>

#include "LineRenderingTests.h"

#include <Atmos/Line.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>

#include "DerivedEngine.h"
#include "MockSurfaceResource.h"

using namespace Atmos;

SCENARIO_METHOD(LineRenderingTestsFixture, "rendering lines", "[render]")
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
            *engine.mockScriptManager,
            *engine.worldManager,
            Spatial::ScreenSize{
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
                std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        const auto mainSurface = Arca::Index<MainSurface>(fieldReliquary);
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::LineMaterial> {
            String{}, std::vector<Asset::LineMaterial::Pass>{} });

        const auto camera = Arca::Index<Camera>(fieldReliquary);

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        WHEN("creating lines")
        {
            auto fromPositions = std::vector<Spatial::Point2D>
            {
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                }
            };

            auto toPositions = std::vector<Spatial::Point2D>
            {
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                }
            };
            auto line1 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[0], toPositions[0] }, materialAsset});
            auto line2 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[1], toPositions[1] }, materialAsset});
            auto line3 = fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[2], toPositions[2] }, materialAsset});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all lines rendered in graphics manager")
                {
                    auto& lineRenders = mainSurfaceImplementation->lineRenders;
                    REQUIRE(lineRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            lineRenders.begin(),
                            lineRenders.end(),
                            [i, &fromPositions, &toPositions, cameraLeft, cameraTop](const LineRender& entry)
                            {
                                auto expectedFromPosition = fromPositions[i];
                                expectedFromPosition.x -= cameraLeft;
                                expectedFromPosition.y -= cameraTop;

                                auto expectedToPosition = toPositions[i];
                                expectedToPosition.x -= cameraLeft;
                                expectedToPosition.y -= cameraTop;

                                return entry.points[0] == expectedFromPosition && entry.points[1] == expectedToPosition;
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying lines, then starting execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<Line>{ line1.ID() });
                fieldReliquary.Do(Arca::Destroy<Line>{ line2.ID() });
                fieldReliquary.Do(Arca::Destroy<Line>{ line3.ID() });

                THEN("lines were rendered only once")
                {
                    auto& lineRenders = mainSurfaceImplementation->lineRenders;
                    REQUIRE(lineRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            lineRenders.begin(),
                            lineRenders.end(),
                            [i, &fromPositions, &toPositions, cameraLeft, cameraTop](const LineRender& entry)
                            {
                                auto expectedFromPosition = fromPositions[i];
                                expectedFromPosition.x -= cameraLeft;
                                expectedFromPosition.y -= cameraTop;

                                auto expectedToPosition = toPositions[i];
                                expectedToPosition.x -= cameraLeft;
                                expectedToPosition.y -= cameraTop;

                                return entry.points[0] == expectedFromPosition && entry.points[1] == expectedToPosition;
                            }));
                    }
                }
            }
        }

        WHEN("creating lines without materials")
        {
            auto fromPositions = std::vector<Spatial::Point2D>
            {
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                }
            };

            auto toPositions = std::vector<Spatial::Point2D>
            {
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point2D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                }
            };
            fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[0], toPositions[0] }, Arca::Index<Asset::LineMaterial>{}});
            fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[1], toPositions[1] }, Arca::Index<Asset::LineMaterial>{}});
            fieldReliquary.Do(Arca::Create<Line>{
                std::vector<Spatial::Point2D>{ fromPositions[2], toPositions[2] }, Arca::Index<Asset::LineMaterial>{}});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no lines rendered in graphics manager")
                {
                    auto& lineRenders = mainSurfaceImplementation->lineRenders;
                    REQUIRE(lineRenders.empty());
                }
            }
        }
    }
}