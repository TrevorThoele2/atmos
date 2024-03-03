#include <catch.hpp>

#include "AngelScriptUIImageTests.h"

#include <Atmos/UIImage.h>
#include <Atmos/Camera.h>
#include <Atmos/ChangeImageCore.h>
#include <Atmos/MoveLine.h>
#include <Atmos/AxisAlignedBox3D.h>
#include <Arca/Create.h>

Arca::Index<Atmos::Asset::Image> AngelScriptUIImageTestsFixture::CreateImageAsset(Arca::Reliquary& reliquary)
{
    const auto imageAssetName = dataGeneration.Random<std::string>();

    auto resource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Image>{
        Buffer{}, imageAssetName, Spatial::Size2D{ 1, 1 }});
    return reliquary.Do(Arca::Create<Asset::Image>{
        imageAssetName, std::move(resource), Asset::ImageGridSize{ 1, 1 } });
}

Arca::Index<Atmos::Asset::Material> AngelScriptUIImageTestsFixture::CreateMaterialAsset(Arca::Reliquary& reliquary)
{
    const auto vertexShaderName = dataGeneration.Random<std::string>();
    auto vertexResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, vertexShaderName});
    const auto vertexShaderAsset = reliquary.Do(Arca::Create<Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

    const auto fragmentShaderName = dataGeneration.Random<std::string>();
    auto fragmentResource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, fragmentShaderName});
    const auto fragmentShaderAsset = reliquary.Do(Arca::Create<Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

    const auto materialAssetName = dataGeneration.Random<std::string>();
    const auto materialAssetPasses = std::vector<Asset::Material::Pass>
    {
        { vertexShaderAsset, fragmentShaderAsset }
    };
    return reliquary.Do(Arca::Create<Asset::Material>{ materialAssetName, materialAssetPasses });
}

SCENARIO_METHOD(AngelScriptUIImageTestsFixture, "running UI image AngelScript scripts", "[script][angelscript]")
{
    GIVEN("Image")
    {
        auto createCommand = Arca::Create<UI::Image>{
            CreateImageAsset(*fieldReliquary),
            1,
            Arca::Index<Asset::Material>{},
            Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            Spatial::Angle2D{} };
        auto dynamicImage = fieldReliquary->Do(createCommand);

        GIVEN("image asset")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Spatial::Size2D::Value>(
                TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
            auto imageAssetHeight = dataGeneration.Random<Spatial::Size2D::Value>(
                TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Spatial::Size2D{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary->Do(Arca::Create<Asset::Image>{
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
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return image.Asset().ID();\n" \
                    "}",
                    { dynamicImage.ID(), imageAsset.ID() },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
            auto assetIndex = dataGeneration.Random<UI::Image::Index>();

            GIVEN("script that sets asset index and returns asset index")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "int main(Arca::RelicID id, int assetIndex)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeAssetIndex(id, assetIndex));\n" \
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return image.AssetIndex();\n" \
                    "}",
                    { dynamicImage.ID(), assetIndex },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<UI::Image::Index>(std::get<Variant>(finishes[0].result)) == assetIndex);
                    }
                }
            }
        }

        GIVEN("image asset and asset index")
        {
            auto imageAssetName = dataGeneration.Random<std::string>();
            auto imageAssetWidth = dataGeneration.Random<Spatial::Size2D::Value>(
                TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
            auto imageAssetHeight = dataGeneration.Random<Spatial::Size2D::Value>(
                TestFramework::Range<Spatial::Size2D::Value>{1, std::numeric_limits<Spatial::Size2D::Value>::max()});
            auto imageAssetColumns = dataGeneration.Random<Asset::ImageGridSize::Dimension>();
            auto imageAssetRows = dataGeneration.Random<Asset::ImageGridSize::Dimension>();

            auto imageAssetResource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Image>{
                Buffer{}, imageAssetName, Spatial::Size2D{ imageAssetWidth, imageAssetHeight }});
            auto imageAsset = fieldReliquary->Do(Arca::Create<Asset::Image>{
                imageAssetName, std::move(imageAssetResource), Asset::ImageGridSize{ imageAssetColumns, imageAssetRows } });

            auto assetIndex = dataGeneration.Random<UI::Image::Index>();

            fieldReliquary->Do(Render::ChangeImageCore{ dynamicImage.ID(), imageAsset, assetIndex });

            GIVEN("script that returns asset slice")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id)\n" \
                    "{\n" \
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    auto assetSlice = image.AssetSlice();\n" \
                    "    return Atmos::ToString(assetSlice.center.x) + \" \" + Atmos::ToString(assetSlice.center.y);\n" \
                    "}",
                    { dynamicImage.ID() },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
            auto materialAsset = CreateMaterialAsset(*fieldReliquary);

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::Material(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return image.Material().ID();\n" \
                    "}",
                    { dynamicImage.ID(), materialAsset.ID() },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return Atmos::ToString(image.Color().alpha) + \" \" + Atmos::ToString(image.Color().red) + \" \" +\n"
                    "        Atmos::ToString(image.Color().green) + \" \" + Atmos::ToString(image.Color().blue);\n"
                    "}",
                    { dynamicImage.ID(), color.alpha, color.red, color.green, color.blue },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
                    "    Arca::Reliquary::Do(Atmos::Spatial::MoveBounds(id, point));\n" \
                    "\n" \
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return Atmos::ToString(image.Position().x) + \" \" + Atmos::ToString(image.Position().y) + \" \" +\n"
                    "        Atmos::ToString(image.Position().z);\n"
                    "}",
                    { dynamicImage.ID(), position.x, position.y, position.z },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return Atmos::ToString(image.Rotation());\n"
                    "}",
                    { dynamicImage.ID(), rotation },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
                    "    auto image = Atmos::UI::Image(id);\n" \
                    "    return Atmos::ToString(image.Size().width) + \" \" + Atmos::ToString(image.Size().height);\n"
                    "}",
                    { dynamicImage.ID(), scalers.x, scalers.y },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

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
    
    GIVEN("script that creates image and returns size")
    {
        auto imageAsset = CreateImageAsset(*fieldReliquary);
        auto materialAsset = CreateMaterialAsset(*fieldReliquary);
        
        CompileAndCreateScript(
            "basic_script.as",
            "string main(Arca::RelicID assetID, Arca::RelicID materialID)\n" \
            "{\n" \
            "    auto image = Arca::Reliquary::Do(Arca::Create<Atmos::UI::Image>(\n" \
            "       Atmos::Asset::Image(assetID),\n" \
            "       0,\n" \
            "       Atmos::Asset::Material(assetID),\n" \
            "       Atmos::Render::Color(255, 255, 255, 255),\n" \
            "       Atmos::Spatial::Point3D(),\n"\
            "       Atmos::Spatial::Scalers2D(10, 10),\n" \
            "       0));\n" \
            "    return Atmos::ToString(image.Size().width) + \",\" + Atmos::ToString(image.Size().height);\n" \
            "}",
            { imageAsset.ID(), materialAsset.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);
                REQUIRE(std::get<std::string>(std::get<Variant>(finishes[0].result)) == "10,10");
            }
        }
    }
}