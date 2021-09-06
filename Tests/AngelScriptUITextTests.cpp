#include <catch.hpp>

#include "AngelScriptUITextTests.h"

#include <Atmos/UIText.h>
#include <Atmos/MoveLine.h>
#include <Atmos/AxisAlignedBox3D.h>

Arca::Index<Atmos::Asset::Font> AngelScriptUITextTestsFixture::CreateFontAsset(Arca::Reliquary& reliquary)
{
    const auto fontAssetName = dataGeneration.Random<std::string>();

    auto resource = reliquary.Do(Asset::Resource::Create<Asset::Resource::Font>{Buffer{}, fontAssetName});
    return reliquary.Do(Arca::Create<Asset::Font>{ fontAssetName, std::move(resource), });
}

SCENARIO_METHOD(AngelScriptUITextTestsFixture, "running UI text AngelScript scripts", "[script][angelscript]")
{
    GIVEN("Text")
    {
        auto createCommand = Arca::Create<UI::Text>{
            "",
            CreateFontAsset(*fieldReliquary),
            Arca::Index<Asset::Material>{},
            0.0f,
            false,
            false,
            Color{},
            Spatial::Point3D{},
            Spatial::Scalers2D{},
            Spatial::Angle2D{} };
        auto text = fieldReliquary->Do(createCommand);

        GIVEN("string")
        {
            auto string = dataGeneration.Random<String>();

            GIVEN("script that sets string and returns string")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "string main(Arca::RelicID id, string to)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeTextString(id, to));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.String();\n" \
                    "}",
                    { text.ID(), string },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<String>(std::get<Variant>(finishes[0].result)) == string);
                    }
                }
            }
        }

        GIVEN("font asset")
        {
            auto fontAssetName = dataGeneration.Random<std::string>();

            auto fontAssetResource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Font>{
                Buffer{}, fontAssetName});
            auto fontAsset = fieldReliquary->Do(Arca::Create<Asset::Font>{
                fontAssetName, std::move(fontAssetResource) });

            GIVEN("script that sets asset and returns asset ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID fontAssetID)\n" \
                    "{\n" \
                    "    auto fontAsset = Atmos::Asset::Font(fontAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeFontAsset(id, fontAsset));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.Font().ID();\n" \
                    "}",
                    { text.ID(), fontAsset.ID() },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(finishes[0].result)) == fontAsset.ID());
                    }
                }
            }
        }

        GIVEN("wrap width")
        {
            auto wrapWidth = dataGeneration.Random<float>();

            GIVEN("script that sets wrap width and returns wrap width")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "float main(Arca::RelicID id, float wrapWidth)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeWrapWidth(id, wrapWidth));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.WrapWidth();\n" \
                    "}",
                    { text.ID(), wrapWidth },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<float>(std::get<Variant>(finishes[0].result)) == wrapWidth);
                    }
                }
            }
        }

        GIVEN("bold")
        {
            GIVEN("script that sets bold and returns bold")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "bool main(Arca::RelicID id, bool bold)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeBold(id, bold));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.Bold();\n" \
                    "}",
                    { text.ID(), true },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
                    }
                }
            }
        }

        GIVEN("italics")
        {
            GIVEN("script that sets italics and returns italics")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "bool main(Arca::RelicID id, bool italics)\n" \
                    "{\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeItalics(id, italics));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.Italics();\n" \
                    "}",
                    { text.ID(), true },
                    *fieldReliquary);

                WHEN("working reliquary")
                {
                    fieldReliquary->Do(Work{});

                    THEN("has correct properties")
                    {
                        REQUIRE(finishes.size() == 1);
                        REQUIRE(std::get<bool>(std::get<Variant>(finishes[0].result)) == true);
                    }
                }
            }
        }

        GIVEN("material asset")
        {
            auto vertexShaderName = dataGeneration.Random<std::string>();
            auto vertexResource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, vertexShaderName});
            auto vertexShaderAsset = fieldReliquary->Do(Arca::Create<Asset::Shader>{ vertexShaderName, std::move(vertexResource) });

            auto fragmentShaderName = dataGeneration.Random<std::string>();
            auto fragmentResource = fieldReliquary->Do(Asset::Resource::Create<Asset::Resource::Shader>{Buffer{}, fragmentShaderName});
            auto fragmentShaderAsset = fieldReliquary->Do(Arca::Create<Asset::Shader>{ fragmentShaderName, std::move(fragmentResource) });

            auto materialAssetName = dataGeneration.Random<std::string>();
            auto materialAssetPasses = std::vector<Asset::Material::Pass>
            {
                { vertexShaderAsset, fragmentShaderAsset }
            };
            auto materialAsset = fieldReliquary->Do(Arca::Create<Asset::Material>{ materialAssetName, materialAssetPasses });

            GIVEN("script that sets material asset and returns material ID")
            {
                CompileAndCreateScript(
                    "basic_script.as",
                    "Arca::RelicID main(Arca::RelicID id, Arca::RelicID materialAssetID)\n" \
                    "{\n" \
                    "    auto materialAsset = Atmos::Asset::Material(materialAssetID);\n" \
                    "    Arca::Reliquary::Do(Atmos::Render::ChangeMaterialAsset(id, materialAsset));\n" \
                    "\n" \
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return text.Material().ID();\n" \
                    "}",
                    { text.ID(), materialAsset.ID() },
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
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return Atmos::ToString(text.Color().alpha) + \" \" + Atmos::ToString(text.Color().red) + \" \" +\n"
                    "        Atmos::ToString(text.Color().green) + \" \" + Atmos::ToString(text.Color().blue);\n"
                    "}",
                    { text.ID(), color.alpha, color.red, color.green, color.blue },
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
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return Atmos::ToString(text.Position().x) + \" \" + Atmos::ToString(text.Position().y) + \" \" +\n"
                    "        Atmos::ToString(text.Position().z);\n"
                    "}",
                    { text.ID(), position.x, position.y, position.z },
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
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return Atmos::ToString(text.Rotation());\n"
                    "}",
                    { text.ID(), rotation },
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
                    "    auto text = Atmos::UI::Text(id);\n" \
                    "    return Atmos::ToString(text.Size().width) + \" \" + Atmos::ToString(text.Size().height);\n"
                    "}",
                    { text.ID(), scalers.x, scalers.y },
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
}