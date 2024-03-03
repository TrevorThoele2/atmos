#include <catch.hpp>

#include "AngelScriptAssetTests.h"

#include "ScriptEngine.h"

#include <Atmos/CreateImageAssetResource.h>
#include <Atmos/CreateAudioAssetResource.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>

#include "AudioBuffer.h"

SCENARIO_METHOD(AngelScriptAssetTestsFixture, "running asset AngelScript scripts", "[script][angelscript]")
{
    ScriptEngine engine;
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.nullAudioManager,
        *engine.nullInputManager,
        *engine.mockGraphicsManager,
        *engine.scriptManager,
        Spatial::ScreenSize{
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max(),
            std::numeric_limits<Spatial::ScreenSize::Dimension>::max() },
            *engine.mockWindow,
            engine.Logger());
    fieldOrigin.CuratorCommandPipeline<Work>(Arca::Pipeline{ Scripting::Stage() });
    World::Field field(0, fieldOrigin.Actualize());

    auto& fieldReliquary = field.Reliquary();

    engine.mockGraphicsManager->Initialize();

    std::vector<Scripting::Finished> finishes;
    fieldReliquary.On<Scripting::Finished>([&finishes](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });

    GIVEN("AudioAsset")
    {
        auto name = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Audio>{ audioBuffer, name });
        auto audioAsset = fieldReliquary.Do(Arca::Create<Asset::Audio>{ name, std::move(resource) });

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Audio> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
                }
            }
        }
    }

    GIVEN("ImageAsset")
    {
        auto name = dataGeneration.Random<std::string>();
        auto width = dataGeneration.Random<Asset::ImageSize::Dimension>(
            TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
        auto height = dataGeneration.Random<Asset::ImageSize::Dimension>(
            TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
        auto columns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
        auto rows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

        auto resource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{DataBuffer{}, name, Asset::ImageSize{ width, height }});
        auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{ name, std::move(resource), Asset::ImageGridSize{ columns, rows } });

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
                }
            }
        }

        GIVEN("script that returns width")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Width();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Asset::ImageSize::Dimension>(std::get<Variant>(finishes[0].result)) == width);
                }
            }
        }

        GIVEN("script that returns height")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Height();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Asset::ImageSize::Dimension>(std::get<Variant>(finishes[0].result)) == height);
                }
            }
        }

        GIVEN("script that returns size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.Size().width) + \" \" + Atmos::ToString(asset.Size().height);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Atmos::ToString(width) + " " + Atmos::ToString(height));
                }
            }
        }

        GIVEN("script that returns columns")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Columns();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Asset::ImageSize::Dimension>(std::get<Variant>(finishes[0].result)) == columns);
                }
            }
        }

        GIVEN("script that returns rows")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Rows();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<Asset::ImageSize::Dimension>(std::get<Variant>(finishes[0].result)) == rows);
                }
            }
        }

        GIVEN("script that returns grid size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.GridSize().columns) + \" \" + Atmos::ToString(asset.GridSize().rows);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Atmos::ToString(columns) + " " + Atmos::ToString(rows));
                }
            }
        }

        GIVEN("script that returns grid size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.GridSize().columns) + \" \" + Atmos::ToString(asset.GridSize().rows);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Atmos::ToString(columns) + " " + Atmos::ToString(rows));
                }
            }
        }

        GIVEN("script that returns slice")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.Slice(1).size.width) + \" \" + Atmos::ToString(asset.Slice(1).size.height);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto slice = imageAsset->Slice(1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Atmos::ToString(slice.size.width) + " " + Atmos::ToString(slice.size.height));
                }
            }
        }

        GIVEN("script that returns slice size")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Image> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.SliceSize().width) + \" \" + Atmos::ToString(asset.SliceSize().height);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto sliceSize = imageAsset->SliceSize();
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == Atmos::ToString(sliceSize.width) + " " + Atmos::ToString(sliceSize.height));
                }
            }
        }
    }

    GIVEN("ShaderAsset")
    {
        auto name = dataGeneration.Random<std::string>();

        auto resource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{DataBuffer{}, name});
        auto shaderAsset = fieldReliquary.Do(Arca::Create<Asset::Shader>{ name, std::move(resource) });

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Shader> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
                }
            }
        }
    }

    GIVEN("MaterialAsset")
    {
        auto vertexShaderName = dataGeneration.Random<std::string>();
        auto vertexResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{DataBuffer{}, vertexShaderName});
        auto vertexShaderAsset = fieldReliquary.Do(Arca::Create<Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

        auto fragmentShaderName = dataGeneration.Random<std::string>();
        auto fragmentResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{DataBuffer{}, fragmentShaderName});
        auto fragmentShaderAsset = fieldReliquary.Do(Arca::Create<Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

        auto materialAssetName = dataGeneration.Random<std::string>();
        auto materialAssetType = Asset::MaterialType::Region;
        auto materialAssetPasses = std::vector<Asset::Material::Pass>
        {
            { vertexShaderAsset, fragmentShaderAsset }
        };
        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material>{ materialAssetName, materialAssetType, materialAssetPasses });

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Material> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { materialAssetName },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == materialAssetName);
                }
            }
        }

        GIVEN("script that returns type")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "int main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Material> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Type();\n" \
                "}",
                { materialAssetName },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    auto finish = static_cast<Asset::MaterialType>(
                        std::get<std::underlying_type_t<Asset::MaterialType>>(std::get<Variant>(finishes[0].result)));
                    REQUIRE(finish == materialAssetType);
                }
            }
        }

        GIVEN("script that returns passes")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Material> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    auto pass = asset.Passes()[0];\n" \
                "    return pass.VertexShader().Name() + \" \" + pass.FragmentShader().Name();\n" \
                "}",
                { materialAssetName },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    auto pass = materialAsset->Passes()[0];
                    auto expectedFinish = pass.VertexShader()->Name() + " " + pass.FragmentShader()->Name();
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedFinish);
                }
            }
        }
    }

    GIVEN("ScriptAsset")
    {
        auto name = dataGeneration.Random<std::string>();

        auto scriptAsset = CompileAndCreateScriptAsset(name, "void main(){}", fieldReliquary);

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Script> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == name);
                }
            }
        }
    }
}