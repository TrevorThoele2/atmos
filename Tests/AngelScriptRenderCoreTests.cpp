#include <catch.hpp>

#include "AngelScriptRenderCoreTests.h"

#include "ScriptEngine.h"

#include <Atmos/AngelScriptRenderCore.h>

#include <Atmos/TypeRegistration.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/OpenRelic.h>

SCENARIO_METHOD(AngelScriptRenderCoreTestsFixture, "running render core AngelScript scripts", "[script][angelscript]")
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
    
    auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

    auto openRelic = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>());

    auto renderCore = fieldReliquary.Do(Arca::Create<RenderCore>{ openRelic, materialAsset, color });
    
    GIVEN("script that returns material asset ID")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "Arca::RelicID main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto renderCore = Atmos::Render::RenderCore(id);\n" \
            "    return renderCore.Material().ID();\n" \
            "}",
            { renderCore.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == materialAsset.ID());
            }
        }
    }

    GIVEN("script that returns color")
    {
        CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID id)\n" \
            "{\n" \
            "    auto renderCore = Atmos::Render::RenderCore(id);\n" \
            "    auto color = renderCore.Color();\n" \
            "    return Atmos::ToString(color.alpha) + \" \" + Atmos::ToString(color.red) + \" \" +\n" \
            "        Atmos::ToString(color.green) + \" \" + Atmos::ToString(color.blue);\n" \
            "}",
            { renderCore.ID() },
            fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary.Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult =
                    ToString(color.alpha) +
                    " " +
                    ToString(color.red) +
                    " " +
                    ToString(color.green) +
                    " " +
                    ToString(color.blue);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == expectedResult);
            }
        }
    }
}