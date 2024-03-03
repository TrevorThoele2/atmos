#include <catch.hpp>

#include "AngelScriptLocalIndexTests.h"

TEMPLATE_TEST_CASE_METHOD(
    AngelScriptRelicTestsFixture,
    "running local index AngelScript scripts",
    "[script][angelscript][local][index]",
    Asset::Audio,
    Asset::Image,
    Asset::Font,
    Asset::Shader,
    Asset::Material,
    Asset::Script,
    Render::DynamicImage,
    Render::Line,
    Render::GridRegion,
    UI::Image,
    Entity::Entity,
    Render::ImageCore,
    Spatial::Bounds)
{
    GIVEN("type")
    {
        auto [index, angelScriptName] = this->template CreateObject<TestType>(*this->fieldReliquary);

        GIVEN("script that returns id")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "(id);\n" \
                "    return index.ID();\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns ID")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(this->finishes[0].result)) == index.ID());
                }
            }
        }

        GIVEN("script that returns equality from two indices")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "bool main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto index1 = " + angelScriptName + "(id);\n" \
                "    auto index2 = " + angelScriptName + "(id);\n" \
                "    return index1 == index2;\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns true")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == true);
                }
            }
        }

        GIVEN("script that returns not equality with default")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "bool main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto index1 = " + angelScriptName + "(id);\n" \
                "    auto index2 = " + angelScriptName + "();\n" \
                "    return index1 == index2;\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == false);
                }
            }
        }

        GIVEN("script that returns implicit cast to handle")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "string main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "(id);\n" \
                "    auto handle = Arca::Handle(index);\n"\
                "    return Atmos::ToString(handle.id) +\n" \
                "        \" \" +\n" \
                "        handle.type.name +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(handle.type.isConst);\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns correct string")
                {
                    REQUIRE(this->finishes.size() == 1);

                    Arca::Handle handle = index;
                    const auto expectedResult =
                        ToString(handle.id) +
                        " " +
                        handle.type.name +
                        " " +
                        ToString(handle.type.isConst);

                    REQUIRE(std::get<String>(std::get<Variant>(this->finishes[0].result)) == expectedResult);
                }
            }
        }

        GIVEN("script that returns implicit cast to boolean with occupied")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "bool main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "(id);\n" \
                "    return bool(index);\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns true")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == true);
                }
            }
        }

        GIVEN("script that returns implicit cast to boolean with not occupied")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    return bool(index);\n" \
                "}",
                {},
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == false);
                }
            }
        }

        GIVEN("script that returns traits type name")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "string main()\n" \
                "{\n" \
                "    return Arca::Traits<" + angelScriptName + ">().Type().name;\n" \
                "}",
                {},
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<String>(std::get<Variant>(this->finishes[0].result)) == Arca::TypeFor<TestType>().name);
                }
            }
        }

        GIVEN("script that returns traits type isConst")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    return Arca::Traits<" + angelScriptName + ">().Type().isConst;\n" \
                "}",
                {},
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == Arca::TypeFor<TestType>().isConst);
                }
            }
        }

        GIVEN("script that returns ID from batch")
        {
            this->CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main(Arca::RelicID id)\n" \
                "{\n" \
                "    auto values = Arca::Batch<" + angelScriptName + ">().Values();\n" \
                "    for(uint i = 0; i < values.length(); ++i)\n" \
                "        if (values[i].ID() == id)\n" \
                "            return values[i].ID();\n" \
                "    return 0;\n" \
                "}",
                { index.ID() },
                *this->fieldReliquary);

            WHEN("working reliquary")
            {
                this->fieldReliquary->Do(Work{});

                THEN("returns false")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<Arca::RelicID>(std::get<Variant>(this->finishes[0].result)) == index.ID());
                }
            }
        }
    }
}