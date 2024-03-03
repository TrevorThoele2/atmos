#include <catch.hpp>

#include "AngelScriptAssetTests.h"

#include "ScriptEngine.h"

#include <Atmos/ActionAsset.h>
#include <Atmos/BindAction.h>
#include <Atmos/ImageMaterialAsset.h>
#include <Atmos/LineMaterialAsset.h>
#include <Atmos/RegionMaterialAsset.h>
#include <Atmos/CreateImageAssetResource.h>
#include <Atmos/CreateAudioAssetResource.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/LocalRelic.h>

#include "AudioBuffer.h"

SCENARIO_METHOD(AngelScriptAssetTestsFixture, "running asset AngelScript scripts", "[script][angelscript]")
{
    Logging::Logger logger(Logging::Severity::Verbose);
    ScriptEngine engine(logger);
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.nullAudioManager,
        *engine.mockInputManager,
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

    GIVEN("ActionAsset")
    {
        auto name = dataGeneration.Random<std::string>();
        const auto boundKey = Input::Key::Z;
        const auto boundModifier = Input::Key::LeftShift;
        Asset::Action::Modifiers boundModifiers;
        boundModifiers.emplace(boundModifier);

        auto actionAsset = fieldReliquary.Do(Arca::Create<Asset::Action>{ name, boundKey, boundModifiers });

        GIVEN("script that returns name")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Action> command(name);\n" \
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

        GIVEN("script that returns bound key")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Action> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.BoundKey());\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(static_cast<std::underlying_type_t<Input::Key>>(boundKey));
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                }
            }
        }

        GIVEN("script that returns bound modifier")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Action> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return Atmos::ToString(asset.BoundModifiers()[0]);\n" \
                "}",
                { name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(static_cast<std::underlying_type_t<Input::Key>>(boundModifier));
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                }
            }
        }

        GIVEN("script that returns bound modifier after binding action")
        {
            auto toBound = Input::Key::A;

            CompileAndCreateScript(
                "basic_script.as",
                "string main(int key, int toBound, string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<Atmos::Asset::Action> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "\n" \
                "    auto boundKey = Atmos::Input::Key(key);\n" \
                "    Atmos::Input::Key[] boundModifiers = { Atmos::Input::Key(toBound) };\n" \
                "    Arca::Reliquary::Do(Atmos::Input::BindAction(asset, boundKey, boundModifiers));\n" \
                "\n" \
                "    return Atmos::ToString(asset.BoundModifiers()[0]);\n" \
                "}",
                { static_cast<std::underlying_type_t<Input::Key>>(boundKey), static_cast<std::underlying_type_t<Input::Key>>(toBound), name },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto expectedResult = ToString(static_cast<std::underlying_type_t<Input::Key>>(toBound));
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                }
            }
        }
    }

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

        auto resource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{Buffer{}, name, Asset::ImageSize{ width, height }});
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

        auto resource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, name});
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

TEMPLATE_TEST_CASE_METHOD(
    AngelScriptMaterialAssetTestsFixture,
    "running material asset AngelScript scripts",
    "[script][angelscript][asset]",
    Asset::ImageMaterial,
    Asset::LineMaterial,
    Asset::RegionMaterial)
{
    Logging::Logger logger(Logging::Severity::Verbose);
    ScriptEngine engine(logger);
    engine.Setup();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockImageAssetManager,
        *engine.nullAudioManager,
        *engine.mockInputManager,
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

    GIVEN("type")
    {
        auto [index, angelScriptName] = this->template CreateObject<TestType>(fieldReliquary);

        GIVEN("script that returns name")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<" + angelScriptName + "> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    return asset.Name();\n" \
                "}",
                { index->Name() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == index->Name());
                }
            }
        }

        GIVEN("script that returns passes")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "string main(string name)\n" \
                "{\n" \
                "    Atmos::Asset::FindByName<" + angelScriptName + "> command(name);\n" \
                "    auto asset = Arca::Reliquary::Do(command);\n" \
                "    auto pass = asset.Passes()[0];\n" \
                "    return pass.VertexShader().Name() + \" \" + pass.FragmentShader().Name();\n" \
                "}",
                { index->Name() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    auto pass = index->Passes()[0];
                    auto expectedFinish = pass.VertexShader()->Name() + " " + pass.FragmentShader()->Name();
                    REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedFinish);
                }
            }
        }
    }
}