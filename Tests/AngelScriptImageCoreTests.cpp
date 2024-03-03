#include <catch.hpp>

#include "AngelScriptImageCoreTests.h"

#include "ScriptEngine.h"

#include <Atmos/AngelScriptImageCore.h>

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Arca/OpenRelic.h>

SCENARIO_METHOD(AngelScriptImageCoreTestsFixture, "running image core AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    logger.Add<Logging::FileSink>();
    ScriptEngine engine(logger);

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    auto imageAssetName = dataGeneration.Random<std::string>();
    auto imageAssetWidth = dataGeneration.Random<Spatial::Size2D::Value>(
        TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
    auto imageAssetHeight = dataGeneration.Random<Spatial::Size2D::Value>(
        TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
    auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
    auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

    auto imageAssetResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
        Buffer{}, imageAssetName, Spatial::Size2D{ imageAssetWidth, imageAssetHeight }});
    auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{
        imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

    auto vertexShaderName = dataGeneration.Random<std::string>();
    auto vertexResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, vertexShaderName});
    auto vertexShaderAsset = fieldReliquary.Do(Arca::Create<Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

    auto fragmentShaderName = dataGeneration.Random<std::string>();
    auto fragmentResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, fragmentShaderName});
    auto fragmentShaderAsset = fieldReliquary.Do(Arca::Create<Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

    auto materialAssetName = dataGeneration.Random<std::string>();
    auto materialAssetPasses = std::vector<Asset::Material::Pass>
    {
        { vertexShaderAsset, fragmentShaderAsset }
    };
    auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material>{ materialAssetName, materialAssetPasses });

    auto assetIndex = dataGeneration.Random<ImageCore::Index>();
    auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

    auto openRelic = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());

    auto imageCore = fieldReliquary.Do(Arca::Create<Render::ImageCore>{ openRelic, imageAsset, assetIndex });

    GIVEN("script that returns asset ID")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto imageCore = Atmos::Render::ImageCore(id);\n" \
            "    return imageCore.Asset().ID();\n" \
            "}",
            { imageCore.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == imageAsset.ID());
            }
        }
    }

    GIVEN("script that returns asset index")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "int main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto imageCore = Atmos::Render::ImageCore(id);\n" \
            "    return imageCore.AssetIndex();\n" \
            "}",
            { imageCore.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<ImageCore::Index>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == assetIndex);
            }
        }
    }
}