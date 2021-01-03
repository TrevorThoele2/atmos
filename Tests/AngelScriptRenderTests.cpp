#include <catch.hpp>

#include "AngelScriptRenderTests.h"

#include "ScriptEngine.h"

#include <Atmos/DynamicImage.h>
#include <Atmos/RelativeImage.h>
#include <Atmos/Line.h>
#include <Atmos/GridRegion.h>
#include <Atmos/Camera.h>
#include <Atmos/ChangeImageCore.h>
#include <Atmos/MoveLine.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/Script.h>
#include <Atmos/ScriptFinished.h>
#include <Atmos/Work.h>
#include <Atmos/StringUtility.h>
#include <Arca/LocalRelic.h>

Arca::Index<Atmos::Asset::Image> AngelScriptRenderTestsFixture::CreateImageAsset(Arca::Reliquary& reliquary)
{
    const auto imageAssetName = dataGeneration.Random<std::string>();

    auto resource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
        Buffer{}, imageAssetName, Asset::ImageSize{ 1, 1 }});
    return reliquary.Do(Arca::Create<Asset::Image>{
        imageAssetName, std::move(resource), Asset::ImageGridSize{ 1, 1 } });
}

SCENARIO_METHOD(AngelScriptRenderTestsFixture, "running render AngelScript scripts", "[script][angelscript]")
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

    GIVEN("color")
    {
        GIVEN("script that constructs Color")
        {
            auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

            CompileAndCreateScript(
                "basic_script.as",
                "string main(uint8 alpha, uint8 red, uint8 green, uint8 blue)\n" \
                "{\n" \
                "    Atmos::Render::Color color(alpha, red, green, blue);\n" \
                "    return Atmos::ToString(color.alpha) + \" \" + " \
                "Atmos::ToString(color.red) + \" \" + " \
                "Atmos::ToString(color.green) + \" \" + " \
                "Atmos::ToString(color.blue);\n"
                "}",
                { color.alpha, color.red, color.green, color.blue },
                fieldReliquary);

            WHEN("working reliquary")
            {
                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                    REQUIRE(result ==
                        Atmos::ToString(color.alpha) + " " +
                        Atmos::ToString(color.red) + " " +
                        Atmos::ToString(color.green) + " " +
                        Atmos::ToString(color.blue));
                }
            }
        }
    }

    GIVEN("DynamicImage")
    {
        auto createCommand = Arca::Create<DynamicImage>{
            CreateImageAsset(fieldReliquary),
            1,
            Arca::Index<Asset::ImageMaterial>{},
            Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            Spatial::Angle2D{} };
        auto dynamicImage = fieldReliquary.Do(createCommand);

        GIVEN("image asset")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetHeight = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Asset::ImageSize{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{
                imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

            GIVEN("script that sets asset and returns asset ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID imageAssetID)\n" \
                    "{\n" \
                    "    auto imageAsset = Atmos::Asset::Image(imageAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeImageAsset(id, imageAsset));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return image.Asset().ID();\n" \
                    "}",
                    { dynamicImage.ID(), imageAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == imageAsset.ID());
                    }
                }
            }
        }

        GIVEN("asset index")
        {
            auto assetIndex = dataGeneration.Random<Render::DynamicImage::Index>();

            GIVEN("script that sets asset index and returns asset index")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "int main(Arca::RelicID id, int assetIndex)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeAssetIndex(id, assetIndex));\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return image.AssetIndex();\n" \
                    "}",
                    { dynamicImage.ID(), assetIndex },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Render::DynamicImage::Index>(std::get<Variant>(finishes[0].result)) == assetIndex);
                    }
                }
            }
        }

        GIVEN("image asset and asset index")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetHeight = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Asset::ImageSize{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{
                imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

            auto assetIndex = dataGeneration.Random<Render::DynamicImage::Index>();

            fieldReliquary.Do(Render::ChangeImageCore{ dynamicImage.ID(), imageAsset, assetIndex });

            GIVEN("script that returns asset slice")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id)\n" \
                    "{\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    auto assetSlice = image.AssetSlice();\n" \
                    "    return Atmos::ToString(assetSlice.center.x) + \" \" + Atmos::ToString(assetSlice.center.y);\n" \
                    "}",
                    { dynamicImage.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(dynamicImage->AssetSlice().center.x) + " " + ToString(dynamicImage->AssetSlice().center.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("material asset")
        {
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
            auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::ImageMaterial>{ materialAssetName, materialAssetPasses });

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::ImageMaterial(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeImageMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return image.Material().ID();\n" \
                    "}",
                    { dynamicImage.ID(), materialAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == materialAsset.ID());
                    }
                }
            }
        }

        GIVEN("color")
        {
            auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

            GIVEN("script that sets color and returns color")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, uint8 alpha, uint8 red, uint8 green, uint8 blue)\n" \
                    "{\n" \
                    "    auto color = Atmos::Render::Color(alpha, red, green, blue);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeColor(id, color));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return Atmos::ToString(image.Color().alpha) + \" \" + Atmos::ToString(image.Color().red) + \" \" +\n"
                    "        Atmos::ToString(image.Color().green) + \" \" + Atmos::ToString(image.Color().blue);\n"
                    "}",
                    { dynamicImage.ID(), color.alpha, color.red, color.green, color.blue },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(color.alpha) +
                            " " +
                            Atmos::ToString(color.red) +
                            " " +
                            Atmos::ToString(color.green) +
                            " " +
                            Atmos::ToString(color.blue);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("position")
        {
            auto position = dataGeneration.RandomStack<
                Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

            GIVEN("script that sets position and returns position")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float x, float y, float z)\n" \
                    "{\n" \
                    "    auto point = Atmos::Spatial::Point3D(x, y, z);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::MoveBoundsTo(id, point));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return Atmos::ToString(image.Position().x) + \" \" + Atmos::ToString(image.Position().y) + \" \" +\n"
                    "        Atmos::ToString(image.Position().z);\n"
                    "}",
                    { dynamicImage.ID(), position.x, position.y, position.z },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(position.x) +
                            " " +
                            Atmos::ToString(position.y) +
                            " " +
                            Atmos::ToString(position.z);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("rotation")
        {
            auto rotation = dataGeneration.Random<Spatial::Angle2D>();

            GIVEN("script that sets rotation and returns rotation")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, Atmos::Spatial::Angle2D angle)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::RotateBounds(id, angle));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return Atmos::ToString(image.Rotation());\n"
                    "}",
                    { dynamicImage.ID(), rotation },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(rotation);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("scalers")
        {
            auto scalers = dataGeneration.RandomStack<
                Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

            GIVEN("script that sets scaling and returns size")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float x, float y)\n" \
                    "{\n" \
                    "    auto scalers = Atmos::Spatial::Scalers2D(x, y);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::ScaleBounds(id, scalers));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::DynamicImage(id);\n" \
                    "    return Atmos::ToString(image.Size().width) + \" \" + Atmos::ToString(image.Size().height);\n"
                    "}",
                    { dynamicImage.ID(), scalers.x, scalers.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(scalers.x) + " " + ToString(scalers.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("script that creates dynamic image")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main()\n" \
                "{\n" \
                "    auto imageAsset = Atmos::Asset::Image();\n" \
                "    auto assetIndex = 1;\n" \
                "    auto materialAsset = Atmos::Asset::ImageMaterial();\n" \
                "    auto color = Atmos::Render::Color();\n" \
                "    auto position = Atmos::Spatial::Point3D();\n" \
                "    auto scalers = Atmos::Spatial::Scalers2D();\n" \
                "    auto rotation = 0;\n" \
                "\n" \
                "    Arca::Create<Atmos::Render::DynamicImage> command(" \
                "imageAsset, " \
                "assetIndex, " \
                "materialAsset, " \
                "color, " \
                "position, " \
                "scalers, " \
                "rotation);\n" \
                "    auto created = Arca::Reliquary::Do(command);\n" \
                "    return created.ID();\n" \
                "}",
                {},
                fieldReliquary);

            WHEN("working reliquary")
            {
                const auto preExecutionCount = fieldReliquary.Batch<DynamicImage>().Size();
                const auto preExecutionAllIDs = fieldReliquary.AllIDs();

                fieldReliquary.Do(Work{});

                THEN("has correct properties")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));

                    REQUIRE(preExecutionCount < fieldReliquary.Batch<DynamicImage>().Size());
                    REQUIRE(std::all_of(
                        preExecutionAllIDs.begin(),
                        preExecutionAllIDs.end(),
                        [result](Arca::RelicID checkID)
                        {
                            return result > checkID;
                        }));
                }
            }
        }

        GIVEN("script that creates dynamic image child")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID parentID)\n" \
                "{\n" \
                "    auto imageAsset = Atmos::Asset::Image();\n" \
                "    auto assetIndex = 1;\n" \
                "    auto materialAsset = Atmos::Asset::ImageMaterial();\n" \
                "    auto color = Atmos::Render::Color();\n" \
                "    auto position = Atmos::Spatial::Point3D();\n" \
                "    auto scalers = Atmos::Spatial::Scalers2D();\n" \
                "    auto rotation = 0;\n" \
                "\n" \
                "    auto parent = Atmos::Render::DynamicImage(parentID);\n" \
                "    Arca::CreateChild<Atmos::Render::DynamicImage> command(" \
                "parent, " \
                "imageAsset, " \
                "assetIndex, " \
                "materialAsset, " \
                "color, " \
                "position, " \
                "scalers, " \
                "rotation);\n" \
                "    auto created = Arca::Reliquary::Do(command);\n" \
                "    return created.ID();\n" \
                "}",
                { dynamicImage.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                const auto preExecutionCount = fieldReliquary.Batch<DynamicImage>().Size();
                const auto preExecutionAllIDs = fieldReliquary.AllIDs();

                fieldReliquary.Do(Work{});

                THEN("has created relic")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));

                    REQUIRE(preExecutionCount < fieldReliquary.Batch<DynamicImage>().Size());
                    REQUIRE(std::all_of(
                        preExecutionAllIDs.begin(),
                        preExecutionAllIDs.end(),
                        [result](Arca::RelicID checkID)
                        {
                            return result > checkID;
                        }));

                    const auto parent = fieldReliquary.ParentOf(result);
                    REQUIRE(parent);
                    REQUIRE(*parent == dynamicImage);
                }
            }
        }

        GIVEN("script that creates identified dynamic image")
        {
            const auto newID = dataGeneration.Random<Arca::RelicID>(TestFramework::Range<Arca::RelicID>(10000, 20000));

            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID setID)\n" \
                "{\n" \
                "    auto imageAsset = Atmos::Asset::Image();\n" \
                "    auto assetIndex = 1;\n" \
                "    auto materialAsset = Atmos::Asset::ImageMaterial();\n" \
                "    auto color = Atmos::Render::Color();\n" \
                "    auto position = Atmos::Spatial::Point3D();\n" \
                "    auto scalers = Atmos::Spatial::Scalers2D();\n" \
                "    auto rotation = 0;\n" \
                "\n" \
                "    Arca::IdentifiedCreate<Atmos::Render::DynamicImage> command(" \
                "setID, " \
                "imageAsset, " \
                "assetIndex, " \
                "materialAsset, " \
                "color, " \
                "position, " \
                "scalers, " \
                "rotation);\n" \
                "    auto created = Arca::Reliquary::Do(command);\n" \
                "    return created.ID();\n" \
                "}",
                { newID },
                fieldReliquary);

            WHEN("working reliquary")
            {
                const auto preExecutionCount = fieldReliquary.Batch<DynamicImage>().Size();
                const auto preExecutionAllIDs = fieldReliquary.AllIDs();

                fieldReliquary.Do(Work{});

                THEN("has created relic")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));

                    REQUIRE(preExecutionCount < fieldReliquary.Batch<DynamicImage>().Size());
                    REQUIRE(std::all_of(
                        preExecutionAllIDs.begin(),
                        preExecutionAllIDs.end(),
                        [result](Arca::RelicID checkID)
                        {
                            return result > checkID;
                        }));

                    REQUIRE(result == newID);
                }
            }
        }

        GIVEN("script that creates identified dynamic image child")
        {
            const auto newID = dataGeneration.Random<Arca::RelicID>(TestFramework::Range<Arca::RelicID>(10000, 20000));

            CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID setID, Arca::RelicID parentID)\n" \
                "{\n" \
                "    auto imageAsset = Atmos::Asset::Image();\n" \
                "    auto assetIndex = 1;\n" \
                "    auto materialAsset = Atmos::Asset::ImageMaterial();\n" \
                "    auto color = Atmos::Render::Color();\n" \
                "    auto position = Atmos::Spatial::Point3D();\n" \
                "    auto scalers = Atmos::Spatial::Scalers2D();\n" \
                "    auto rotation = 0;\n" \
                "\n" \
                "    auto parent = Atmos::Render::DynamicImage(parentID);\n" \
                "    Arca::IdentifiedCreateChild<Atmos::Render::DynamicImage> command(" \
                "setID, " \
                "parent, " \
                "imageAsset, " \
                "assetIndex, " \
                "materialAsset, " \
                "color, " \
                "position, " \
                "scalers, " \
                "rotation);\n" \
                "    auto created = Arca::Reliquary::Do(command);\n" \
                "    return created.ID();\n" \
                "}",
                { newID, dynamicImage.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                const auto preExecutionCount = fieldReliquary.Batch<DynamicImage>().Size();
                const auto preExecutionAllIDs = fieldReliquary.AllIDs();

                fieldReliquary.Do(Work{});

                THEN("has created relic")
                {
                    REQUIRE(finishes.size() == 1);

                    const auto result = std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result));

                    REQUIRE(preExecutionCount < fieldReliquary.Batch<DynamicImage>().Size());
                    REQUIRE(std::all_of(
                        preExecutionAllIDs.begin(),
                        preExecutionAllIDs.end(),
                        [result](Arca::RelicID checkID)
                        {
                            return result > checkID;
                        }));

                    REQUIRE(result == newID);

                    const auto parent = fieldReliquary.ParentOf(result);
                    REQUIRE(parent);
                    REQUIRE(*parent == dynamicImage);
                }
            }
        }

        GIVEN("script that destroys pre-existing dynamic image")
        {
            CompileAndCreateScript(
                "basic_script.as",
                "void main(Arca::RelicID imageID)\n" \
                "{\n" \
                "    auto command = Arca::Destroy<Atmos::Render::DynamicImage>(imageID);\n" \
                "    Arca::Reliquary::Do(command);\n" \
                "}",
                { dynamicImage.ID() },
                fieldReliquary);

            WHEN("working reliquary")
            {
                const auto preExecutionCount = fieldReliquary.Batch<DynamicImage>().Size();
                const auto preExecutionAllIDs = fieldReliquary.AllIDs();

                fieldReliquary.Do(Work{});

                THEN("has created relic")
                {
                    REQUIRE(finishes.size() == 1);

                    REQUIRE(preExecutionCount > fieldReliquary.Batch<DynamicImage>().Size());
                }
            }
        }
    }

    GIVEN("RelativeImage")
    {
        auto parent = fieldReliquary.Do(Arca::Create<Arca::OpenRelic>{});
        fieldReliquary.Do(Arca::Create<Spatial::Bounds>{parent.ID()});

        auto createCommand = Arca::CreateChild<RelativeImage>{
            parent,
            CreateImageAsset(fieldReliquary),
            1,
            Arca::Index<Asset::ImageMaterial>{},
            Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            Spatial::Angle2D{} };
        auto dynamicImage = fieldReliquary.Do(createCommand);

        GIVEN("image asset")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetHeight = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Asset::ImageSize{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{
                imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

            GIVEN("script that sets asset and returns asset ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID imageAssetID)\n" \
                    "{\n" \
                    "    auto imageAsset = Atmos::Asset::Image(imageAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeImageAsset(id, imageAsset));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return image.Asset().ID();\n" \
                    "}",
                    { dynamicImage.ID(), imageAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == imageAsset.ID());
                    }
                }
            }
        }

        GIVEN("asset index")
        {
            auto assetIndex = dataGeneration.Random<Render::RelativeImage::Index>();

            GIVEN("script that sets asset index and returns asset index")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "int main(Arca::RelicID id, int assetIndex)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeAssetIndex(id, assetIndex));\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return image.AssetIndex();\n" \
                    "}",
                    { dynamicImage.ID(), assetIndex },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Render::RelativeImage::Index>(std::get<Variant>(finishes[0].result)) == assetIndex);
                    }
                }
            }
        }

        GIVEN("image asset and asset index")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetHeight = dataGeneration.Random<Asset::ImageSize::Dimension>(
                TestFramework::Range<Asset::ImageSize::Dimension>{1, std::numeric_limits<Asset::ImageSize::Dimension>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Asset::ImageSize{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary.Do(Arca::Create<Asset::Image>{
                imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

            auto assetIndex = dataGeneration.Random<Render::RelativeImage::Index>();

            fieldReliquary.Do(Render::ChangeImageCore{ dynamicImage.ID(), imageAsset, assetIndex });

            GIVEN("script that returns asset slice")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id)\n" \
                    "{\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    auto assetSlice = image.AssetSlice();\n" \
                    "    return Atmos::ToString(assetSlice.center.x) + \" \" + Atmos::ToString(assetSlice.center.y);\n" \
                    "}",
                    { dynamicImage.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(dynamicImage->AssetSlice().center.x) + " " + ToString(dynamicImage->AssetSlice().center.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("material asset")
        {
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
            auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::ImageMaterial>{ materialAssetName, materialAssetPasses });

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::ImageMaterial(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeImageMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return image.Material().ID();\n" \
                    "}",
                    { dynamicImage.ID(), materialAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == materialAsset.ID());
                    }
                }
            }
        }

        GIVEN("color")
        {
            auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

            GIVEN("script that sets color and returns color")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, uint8 alpha, uint8 red, uint8 green, uint8 blue)\n" \
                    "{\n" \
                    "    auto color = Atmos::Render::Color(alpha, red, green, blue);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeColor(id, color));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return Atmos::ToString(image.Color().alpha) + \" \" + Atmos::ToString(image.Color().red) + \" \" +\n"
                    "        Atmos::ToString(image.Color().green) + \" \" + Atmos::ToString(image.Color().blue);\n"
                    "}",
                    { dynamicImage.ID(), color.alpha, color.red, color.green, color.blue },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(color.alpha) +
                            " " +
                            Atmos::ToString(color.red) +
                            " " +
                            Atmos::ToString(color.green) +
                            " " +
                            Atmos::ToString(color.blue);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("position")
        {
            auto position = dataGeneration.RandomStack<
                Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

            GIVEN("script that sets position and returns position")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float x, float y, float z)\n" \
                    "{\n" \
                    "    auto point = Atmos::Spatial::Point3D(x, y, z);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::MoveBoundsTo(id, point));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return Atmos::ToString(image.Position().x) + \" \" + Atmos::ToString(image.Position().y) + \" \" +\n"
                    "        Atmos::ToString(image.Position().z);\n"
                    "}",
                    { dynamicImage.ID(), position.x, position.y, position.z },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(position.x) +
                            " " +
                            Atmos::ToString(position.y) +
                            " " +
                            Atmos::ToString(position.z);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("rotation")
        {
            auto rotation = dataGeneration.Random<Spatial::Angle2D>();

            GIVEN("script that sets rotation and returns rotation")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, Atmos::Spatial::Angle2D angle)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::RotateBounds(id, angle));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return Atmos::ToString(image.Rotation());\n"
                    "}",
                    { dynamicImage.ID(), rotation },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(rotation);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("scalers")
        {
            auto scalers = dataGeneration.RandomStack<
                Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

            GIVEN("script that sets scaling and returns size")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float x, float y)\n" \
                    "{\n" \
                    "    auto scalers = Atmos::Spatial::Scalers2D(x, y);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::ScaleBounds(id, scalers));\n" \
                    "\n" \
                    "    auto image = Atmos::Render::RelativeImage(id);\n" \
                    "    return Atmos::ToString(image.Size().width) + \" \" + Atmos::ToString(image.Size().height);\n"
                    "}",
                    { dynamicImage.ID(), scalers.x, scalers.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(scalers.x) + " " + ToString(scalers.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }
    }

    GIVEN("Line")
    {
        auto createCommand = Arca::Create<Line>{
            std::vector<Spatial::Point2D>{Spatial::Point2D{0, 0}, Spatial::Point2D{1, 1}},
            0.0f,
            Arca::Index<Asset::LineMaterial>{},
            1.0f,
            Color{} };
        auto line = fieldReliquary.Do(createCommand);

        GIVEN("points")
        {
            auto point0 = dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
            auto point1 = dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

            GIVEN("script that sets points and returns points")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, float point0x, float point0y, float point1x, float point1y)\n" \
                    "{\n" \
                    "    const auto point0 = Atmos::Spatial::Point2D(point0x, point0y);\n" \
                    "    const auto point1 = Atmos::Spatial::Point2D(point1x, point1y);\n" \
                    "    Atmos::Spatial::Point2D[] points = { point0, point1 };\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeLinePoints(id, points));\n" \
                    "    auto line = Atmos::Render::Line(id);\n" \
                    "    return \n" \
                    "        Atmos::ToString(line.Points()[0].x) +\n" \
                    "        Atmos::ToString(line.Points()[0].y) +\n" \
                    "        Atmos::ToString(line.Points()[1].x) +\n" \
                    "        Atmos::ToString(line.Points()[1].y);\n" \
                    "}",
                    { line.ID(), point0.x, point0.y, point1.x, point1.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(point0.x) +
                            Atmos::ToString(point0.y) +
                            Atmos::ToString(point1.x) +
                            Atmos::ToString(point1.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("z")
        {
            auto z = dataGeneration.Random<Spatial::Point2D::Value>();

            GIVEN("script that sets z and return z")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "float main(Arca::RelicID id, float z)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeLineZ(id, z));\n" \
                    "    auto line = Atmos::Render::Line(id);\n" \
                    "    return line.Z();\n" \
                    "}",
                    { line.ID(), z },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Spatial::Point2D::Value>(std::get<Variant>(finishes[0].result)) == z);
                    }
                }
            }
        }

        GIVEN("material asset")
        {
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
            auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::LineMaterial>{ materialAssetName, materialAssetPasses });

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::LineMaterial(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeLineMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto line = Atmos::Render::Line(id);\n" \
                    "    return line.Material().ID();\n" \
                    "}",
                    { line.ID(), materialAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == materialAsset.ID());
                    }
                }
            }
        }

        GIVEN("color")
        {
            auto color = dataGeneration.RandomStack<Color, Color::Value, Color::Value, Color::Value, Color::Value>();

            GIVEN("script that sets color and returns color")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, uint8 alpha, uint8 red, uint8 green, uint8 blue)\n" \
                    "{\n" \
                    "    auto color = Atmos::Render::Color(alpha, red, green, blue);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeColor(id, color));\n" \
                    "\n" \
                    "    auto line = Atmos::Render::Line(id);\n" \
                    "    return Atmos::ToString(line.Color().alpha) + \" \" + Atmos::ToString(line.Color().red) + \" \" +\n"
                    "        Atmos::ToString(line.Color().green) + \" \" + Atmos::ToString(line.Color().blue);\n"
                    "}",
                    { line.ID(), color.alpha, color.red, color.green, color.blue },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(color.alpha) +
                            " " +
                            Atmos::ToString(color.red) +
                            " " +
                            Atmos::ToString(color.green) +
                            " " +
                            Atmos::ToString(color.blue);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }
    }

    GIVEN("GridRegion")
    {
        auto createCommand = Arca::Create<GridRegion>{
            std::unordered_set<Spatial::Grid::Point>{Spatial::Grid::Point{0, 0}, Spatial::Grid::Point{1, 1}},
            0,
            Arca::Index<Asset::RegionMaterial>{} };
        auto line = fieldReliquary.Do(createCommand);

        GIVEN("points")
        {
            const auto point0 = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
            const auto point1 = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

            GIVEN("script that sets points and returns points")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, int point0x, int point0y, int point1x, int point1y)\n" \
                    "{\n" \
                    "    const auto point0 = Atmos::Spatial::Grid::Point(point0x, point0y);\n" \
                    "    const auto point1 = Atmos::Spatial::Grid::Point(point1x, point1y);\n" \
                    "    Atmos::Spatial::Grid::Point[] points = { point0, point1 };\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeGridRegionPoints(id, points));\n" \
                    "    auto gridRegion = Atmos::Render::GridRegion(id);\n"
                    "\n" \
                    "    auto retrievedPoints = gridRegion.Points();\n" \
                    "    return \n" \
                    "        Atmos::ToString(retrievedPoints[0].x) + \" \" +\n" \
                    "        Atmos::ToString(retrievedPoints[0].y) + \" \" +\n" \
                    "        Atmos::ToString(retrievedPoints[1].x) + \" \" +\n" \
                    "        Atmos::ToString(retrievedPoints[1].y);\n" \
                    "}",
                    { line.ID(), point0.x, point0.y, point1.x, point1.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            Atmos::ToString(point0.x) + " " +
                            Atmos::ToString(point0.y) + " " +
                            Atmos::ToString(point1.x) + " " +
                            Atmos::ToString(point1.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("z")
        {
            auto z = dataGeneration.Random<Spatial::Grid::Point::Value>();

            GIVEN("script that sets z and return z")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "int main(Arca::RelicID id, int z)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeGridRegionZ(id, z));\n" \
                    "    auto gridRegion = Atmos::Render::GridRegion(id);\n" \
                    "    return gridRegion.Z();\n" \
                    "}",
                    { line.ID(), z },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Spatial::Grid::Point::Value>(std::get<Variant>(finishes[0].result)) == z);
                    }
                }
            }
        }

        GIVEN("material asset")
        {
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
            auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::LineMaterial>{ materialAssetName, materialAssetPasses });

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::RegionMaterial(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeRegionMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto gridRegion = Atmos::Render::GridRegion(id);\n" \
                    "    return gridRegion.Material().ID();\n" \
                    "}",
                    { line.ID(), materialAsset.ID() },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == materialAsset.ID());
                    }
                }
            }
        }
    }

    GIVEN("Camera")
    {
        auto camera = Arca::Index<Camera>(fieldReliquary);

        GIVEN("position")
        {
            auto position = dataGeneration.RandomStack<
                Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

            GIVEN("script that sets position and returns position")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(float x, float y, float z)\n" \
                    "{\n" \
                    "    auto camera = Atmos::Render::Camera();\n" \
                    "    auto point = Atmos::Spatial::Point3D(x, y, z);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::MoveBoundsTo(camera.ID(), point));\n" \
                    "\n" \
                    "    return Atmos::ToString(camera.Position().x) + \" \" + Atmos::ToString(camera.Position().y) + \" \" +\n"
                    "        Atmos::ToString(camera.Position().z);\n"
                    "}",
                    { position.x, position.y, position.z },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult =
                            ToString(position.x) +
                            " " +
                            ToString(position.y) +
                            " " +
                            ToString(position.z);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }

        GIVEN("scalers")
        {
            auto scalers = dataGeneration.RandomStack<
                Spatial::Scalers2D, Spatial::Scalers2D::Value, Spatial::Scalers2D::Value>();

            GIVEN("script that sets scaling and returns size")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(float x, float y)\n" \
                    "{\n" \
                    "    auto camera = Atmos::Render::Camera();\n" \
                    "    auto scalers = Atmos::Spatial::Scalers2D(x, y);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::ScaleBounds(camera.ID(), scalers));\n" \
                    "\n" \
                    "    return Atmos::ToString(camera.Size().width) + \" \" + Atmos::ToString(camera.Size().height);\n"
                    "}",
                    { scalers.x, scalers.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(scalers.x) + " " + ToString(scalers.y);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }

            GIVEN("script that sets scaling and returns sides")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(float x, float y)\n" \
                    "{\n" \
                    "    auto camera = Atmos::Render::Camera();\n" \
                    "    auto scalers = Atmos::Spatial::Scalers2D(x, y);\n" \
                    "    Arca::Reliquary::Do(Atmos::Spatial::ScaleBounds(camera.ID(), scalers));\n" \
                    "\n" \
                    "    return Atmos::ToString(camera.Sides().Left()) +\n" \
                    "        \" \" +\n"
                    "        Atmos::ToString(camera.Sides().Top()) +\n" \
                    "        \" \" +\n"
                    "        Atmos::ToString(camera.Sides().Right()) +\n" \
                    "        \" \" +\n"
                    "        Atmos::ToString(camera.Sides().Bottom());\n"
                    "}",
                    { scalers.x, scalers.y },
                    fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary.Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);

                        const auto expectedResult = ToString(camera->Sides().Left()) +
                            " " +
                            ToString(camera->Sides().Top()) +
                            " " +
                            ToString(camera->Sides().Right()) +
                            " " +
                            ToString(camera->Sides().Bottom());
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == expectedResult);
                    }
                }
            }
        }
    }
}