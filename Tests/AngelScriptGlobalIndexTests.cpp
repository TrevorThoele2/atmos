#include <catch.hpp>

#include "AngelScriptGlobalIndexTests.h"

TEMPLATE_TEST_CASE_METHOD(
    AngelScriptGlobalTestsFixture,
    "running global index AngelScript scripts",
    "[script][angelscript][global][index]",
    Render::Camera)
{
    GIVEN("type")
    {
        auto [index, angelScriptName] = this->template CreateObject<TestType>(*this->fieldReliquary);

        GIVEN("script that returns id")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "Arca::RelicID main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    return index.ID();\n" \
                "}",
                {},
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
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "bool main()\n" \
                "{\n" \
                "    auto index1 = " + angelScriptName + "();\n" \
                "    auto index2 = " + angelScriptName + "();\n" \
                "    return index1 == index2;\n" \
                "}",
                {},
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

        GIVEN("script that returns implicit cast to handle")
        {
            auto script = this->CompileAndCreateScript(
                "basic_script.as",
                "string main()\n" \
                "{\n" \
                "    auto index = " + angelScriptName + "();\n" \
                "    auto handle = Arca::Handle(index);\n"\
                "    return Atmos::ToString(handle.id) +\n" \
                "        \" \" +\n" \
                "        handle.type.name +\n" \
                "        \" \" +\n" \
                "        Atmos::ToString(handle.type.isConst);\n" \
                "}",
                {},
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
            auto script = this->CompileAndCreateScript(
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

                THEN("returns true")
                {
                    REQUIRE(this->finishes.size() == 1);
                    REQUIRE(std::get<bool>(std::get<Variant>(this->finishes[0].result)) == true);
                }
            }
        }

        GIVEN("script that returns traits type name")
        {
            auto script = this->CompileAndCreateScript(
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
            auto script = this->CompileAndCreateScript(
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
    }
}