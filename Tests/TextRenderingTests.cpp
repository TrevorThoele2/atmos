#include <catch.hpp>

#include "TextRenderingTests.h"

#include <Atmos/DynamicText.h>
#include <Atmos/MaterialAsset.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>
#include <Arca/Create.h>
#include <Arca/Destroy.h>

#include "DerivedEngine.h"
#include "MockSurfaceResource.h"
#include "MockFontAssetResource.h"

using namespace Atmos;

SCENARIO_METHOD(TextRenderingTestsFixture, "rendering texts", "[render]")
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
            *engine.mockTextManager,
            *engine.mockScriptManager,
            *engine.worldManager,
            Spatial::Size2D{
                std::numeric_limits<Spatial::Size2D::Value>::max(),
                std::numeric_limits<Spatial::Size2D::Value>::max() },
                *engine.mockWindow,
                engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        const auto mainSurface = fieldReliquary.Find<MainSurface>();
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{} });

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        std::unique_ptr<Asset::Resource::Font> fontResource = std::make_unique<MockFontAssetResource>();
        auto fontAsset = fieldReliquary.Do(Arca::Create<Asset::Font> {
            String{}, std::move(fontResource)});

        auto string = "abcdef";
        auto wrapWidth = std::numeric_limits<float>::max();
        auto bold = dataGeneration.Random<bool>();
        auto italics = dataGeneration.Random<bool>();
        auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();
        auto scalers = Spatial::Scalers2D{ 1, 1 };
        auto rotation = 0.0f;

        WHEN("creating dynamic texts")
        {
            auto positions = std::vector<Spatial::Point3D>
            {
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                },
                Spatial::Point3D
                {
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000)),
                    dataGeneration.Random<Spatial::Point2D::Value>(
                        TestFramework::Range<Spatial::Point2D::Value>(-1000, 1000))
                }
            };

            auto text1 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, materialAsset, wrapWidth, bold, italics, color, positions[0], scalers, rotation});
            auto text2 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, materialAsset, wrapWidth, bold, italics, color, positions[1], scalers, rotation});
            auto text3 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, materialAsset, wrapWidth, bold, italics, color, positions[2], scalers, rotation});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("all texts rendered in graphics manager")
                {
                    auto& textRenders = engine.mockGraphicsManager->textRenders;
                    REQUIRE(textRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            textRenders.begin(),
                            textRenders.end(),
                            [i, &positions, scalers, cameraLeft, cameraTop](const RenderText& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
                            }));
                    }
                }
            }

            WHEN("starting engine execution, destroying lines, then starting execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<DynamicText>{ text1.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicText>{ text2.ID() });
                fieldReliquary.Do(Arca::Destroy<DynamicText>{ text3.ID() });

                THEN("texts were rendered only once")
                {
                    auto& textRenders = engine.mockGraphicsManager->textRenders;
                    REQUIRE(textRenders.size() == 3);

                    for (auto i = 0; i < 3; ++i)
                    {
                        REQUIRE(std::any_of(
                            textRenders.begin(),
                            textRenders.end(),
                            [i, &positions, scalers, cameraLeft, cameraTop](const RenderText& entry)
                            {
                                auto expectedPosition = positions[i];
                                expectedPosition.x -= cameraLeft;
                                expectedPosition.y -= cameraTop;

                                return entry.position == expectedPosition;
                            }));
                    }
                }
            }
        }

        WHEN("creating texts without materials")
        {
            auto positions = std::vector<Spatial::Point2D>
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

            auto text1 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, Arca::Index<Asset::Material>(), wrapWidth, bold, italics, color, positions[0], scalers, rotation});
            auto text2 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, Arca::Index<Asset::Material>(), wrapWidth, bold, italics, color, positions[1], scalers, rotation});
            auto text3 = fieldReliquary.Do(Arca::Create<DynamicText>{
                string, fontAsset, Arca::Index<Asset::Material>(), wrapWidth, bold, italics, color, positions[2], scalers, rotation});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no lines rendered in graphics manager")
                {
                    auto& lineRenders = engine.mockGraphicsManager->lineRenders;
                    REQUIRE(lineRenders.empty());
                }
            }
        }
    }
}