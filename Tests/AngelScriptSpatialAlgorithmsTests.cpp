#include <catch.hpp>

#include "AngelScriptSpatialAlgorithmsTests.h"

#include <Atmos/Size3D.h>
#include <Atmos/AxisAlignedBox3D.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/ToWorldPoint2D.h>
#include <Atmos/ToWorldPoint3D.h>

SCENARIO_METHOD(AngelScriptSpatialAlgorithmsTestsFixture, "running spatial algorithm AngelScript scripts", "[script][angelscript]")
{
    GIVEN("script that returns Distance from Point2D")
    {
        auto starting = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto destination = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "float main(float x0, float y0, float x1, float y1)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D starting(x0, y0);\n" \
            "    Atmos::Spatial::Point2D destination(x1, y1);\n" \
            "    return Atmos::Spatial::Distance(starting, destination);\n" \
            "}",
            { starting.x, starting.y, destination.x, destination.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<float>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Distance(starting, destination));
            }
        }
    }

    GIVEN("script that returns Distance from Point3D")
    {
        auto starting = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto destination = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "float main(float x0, float y0, float z0, float x1, float y1, float z1)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D starting(x0, y0, z0);\n" \
            "    Atmos::Spatial::Point3D destination(x1, y1, z1);\n" \
            "    return Atmos::Spatial::Distance(starting, destination);\n" \
            "}",
            { starting.x, starting.y, starting.z, destination.x, destination.y, destination.z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<float>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Distance(starting, destination));
            }
        }
    }

    GIVEN("script that returns Distance from Point3D")
    {
        auto starting = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto destination = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "int main(int x0, int y0, int x1, int y1)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point starting(x0, y0);\n" \
            "    Atmos::Spatial::Grid::Point destination(x1, y1);\n" \
            "    return Atmos::Spatial::Grid::Distance(starting, destination);\n" \
            "}",
            { starting.x, starting.y, destination.x, destination.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<std::int32_t>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Grid::Distance(starting, destination));
            }
        }
    }

    GIVEN("script that returns IsBetween from Point2D")
    {
        auto one = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto check = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto two = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float xOne, float yOne, float xCheck, float yCheck, float xTwo, float yTwo)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D one(xOne, yOne);\n" \
            "    Atmos::Spatial::Point2D check(xCheck, yCheck);\n" \
            "    Atmos::Spatial::Point2D two(xTwo, yTwo);\n" \
            "    return Atmos::Spatial::IsBetween(one, check, two);\n" \
            "}",
            { one.x, one.y, check.x, check.y, two.x, two.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::IsBetween(one, check, two));
            }
        }
    }

    GIVEN("script that returns AngleOf from Point2D")
    {
        auto side1 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto middle = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto side2 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "float main(float xOne, float yOne, float xCheck, float yCheck, float xTwo, float yTwo)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D side1(xOne, yOne);\n" \
            "    Atmos::Spatial::Point2D middle(xCheck, yCheck);\n" \
            "    Atmos::Spatial::Point2D side2(xTwo, yTwo);\n" \
            "    return Atmos::Spatial::AngleOf(side1, middle, side2);\n" \
            "}",
            { side1.x, side1.y, middle.x, middle.y, side2.x, side2.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<float>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::AngleOf(side1, middle, side2));
            }
        }
    }

    GIVEN("script that returns Contains from AxisAlignedBox2D and Point2D")
    {
        auto center = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto point = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float centerX, float centerY, float sizeWidth, float sizeHeight, float pointX, float pointY)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center(centerX, centerY);\n" \
            "    Atmos::Spatial::Size2D size(sizeWidth, sizeHeight);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box(center, size);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D point(pointX, pointY);\n" \
            "    return Atmos::Spatial::Contains(box, point);\n" \
            "}",
            { center.x, center.y, size.width, size.height, point.x, point.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box = Spatial::AxisAlignedBox2D(center, size);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Contains(box, point));
            }
        }
    }

    GIVEN("script that returns Contains from AxisAlignedBox2D and AxisAlignedBox2D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float center0X, " \
            "float center0Y, " \
            "float size0Width, " \
            "float size0Height, " \
            "float center1X, " \
            "float center1Y, " \
            "float size1Width, " \
            "float size1Height)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center0(center0X, center0Y);\n" \
            "    Atmos::Spatial::Size2D size0(size0Width, size0Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D center1(center1X, center1Y);\n" \
            "    Atmos::Spatial::Size2D size1(size1Width, size1Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Contains(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                size0.width,
                size0.height,
                center1.x,
                center1.y,
                size1.width,
                size1.height },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox2D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox2D(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Contains(box0, box1));
            }
        }
    }

    GIVEN("script that returns Contains from AxisAlignedBox3D and Point3D")
    {
        auto center = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto point = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float centerX, float centerY, float centerZ, float sizeWidth, float sizeHeight, float sizeDepth, float pointX, float pointY, float pointZ)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center(centerX, centerY, centerZ);\n" \
            "    Atmos::Spatial::Size3D size(sizeWidth, sizeHeight, sizeDepth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box(center, size);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D point(pointX, pointY, pointZ);\n" \
            "    return Atmos::Spatial::Contains(box, point);\n" \
            "}",
            { center.x, center.y, center.z, size.width, size.height, size.depth, point.x, point.y, point.z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box = Spatial::AxisAlignedBox3D(center, size);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Contains(box, point));
            }
        }
    }

    GIVEN("script that returns Contains from AxisAlignedBox3D and AxisAlignedBox3D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float center0X, " \
            "float center0Y, " \
            "float center0Z, " \
            "float size0Width, " \
            "float size0Height, " \
            "float size0Depth, " \
            "float center1X, " \
            "float center1Y, " \
            "float center1Z, " \
            "float size1Width, " \
            "float size1Height, " \
            "float size1Depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center0(center0X, center0Y, center0Z);\n" \
            "    Atmos::Spatial::Size3D size0(size0Width, size0Height, size0Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D center1(center1X, center1Y, center1Z);\n" \
            "    Atmos::Spatial::Size3D size1(size1Width, size1Height, size1Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Contains(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                center0.z,
                size0.width,
                size0.height,
                size0.depth,
                center1.x,
                center1.y,
                center1.z,
                size1.width,
                size1.height,
                size1.depth },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox3D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox3D(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Contains(box0, box1));
            }
        }
    }

    GIVEN("script that returns Intersects from AxisAlignedBox2D and AxisAlignedBox2D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float center0X, " \
            "float center0Y, " \
            "float size0Width, " \
            "float size0Height, " \
            "float center1X, " \
            "float center1Y, " \
            "float size1Width, " \
            "float size1Height)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center0(center0X, center0Y);\n" \
            "    Atmos::Spatial::Size2D size0(size0Width, size0Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D center1(center1X, center1Y);\n" \
            "    Atmos::Spatial::Size2D size1(size1Width, size1Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Intersects(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                size0.width,
                size0.height,
                center1.x,
                center1.y,
                size1.width,
                size1.height },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox2D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox2D(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Intersects(box0, box1));
            }
        }
    }

    GIVEN("script that returns Intersects from AxisAlignedBox3D and AxisAlignedBox3D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(float center0X, " \
            "float center0Y, " \
            "float center0Z, " \
            "float size0Width, " \
            "float size0Height, " \
            "float size0Depth, " \
            "float center1X, " \
            "float center1Y, " \
            "float center1Z, " \
            "float size1Width, " \
            "float size1Height, " \
            "float size1Depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center0(center0X, center0Y, center0Z);\n" \
            "    Atmos::Spatial::Size3D size0(size0Width, size0Height, size0Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D center1(center1X, center1Y, center1Z);\n" \
            "    Atmos::Spatial::Size3D size1(size1Width, size1Height, size1Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Intersects(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                center0.z,
                size0.width,
                size0.height,
                size0.depth,
                center1.x,
                center1.y,
                center1.z,
                size1.width,
                size1.height,
                size1.depth },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox3D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox3D(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Intersects(box0, box1));
            }
        }
    }

    GIVEN("script that returns Envelope of AxisAlignedBox2D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size2D, Spatial::Size2D::Value, Spatial::Size2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float center0X, " \
            "float center0Y, " \
            "float size0Width, " \
            "float size0Height, " \
            "float center1X, " \
            "float center1Y, " \
            "float size1Width, " \
            "float size1Height)\n" \
            "{\n" \
            "    Atmos::Spatial::Point2D center0(center0X, center0Y);\n" \
            "    Atmos::Spatial::Size2D size0(size0Width, size0Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point2D center1(center1X, center1Y);\n" \
            "    Atmos::Spatial::Size2D size1(size1Width, size1Height);\n" \
            "    Atmos::Spatial::AxisAlignedBox2D box1(center1, size1);\n" \
            "\n" \
            "    const array<Atmos::Spatial::AxisAlignedBox2D> boxes = { box0, box1 };\n" \
            "    const auto envelope = Atmos::Spatial::Envelope(boxes);\n" \
            "    return Atmos::ToString(envelope.Left())\n" \
            "        + \" \" + Atmos::ToString(envelope.Top())\n" \
            "        + \" \" + Atmos::ToString(envelope.Right())\n" \
            "        + \" \" + Atmos::ToString(envelope.Bottom());\n" \
            "}",
            {
                center0.x,
                center0.y,
                size0.width,
                size0.height,
                center1.x,
                center1.y,
                size1.width,
                size1.height },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox2D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox2D(center1, size1);
                const auto envelope = Spatial::Envelope({ box0, box1 });

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                const auto expectedResult = ToString(envelope.Left())
                    + " " + ToString(envelope.Top())
                    + " " + ToString(envelope.Right())
                    + " " + ToString(envelope.Bottom());
                REQUIRE(result == expectedResult);
            }
        }
    }

    GIVEN("script that returns Envelope of AxisAlignedBox3D")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Size3D, Spatial::Size3D::Value, Spatial::Size3D::Value, Spatial::Size3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float center0X, " \
            "float center0Y, " \
            "float center0Z, " \
            "float size0Width, " \
            "float size0Height, " \
            "float size0Depth, " \
            "float center1X, " \
            "float center1Y, " \
            "float center1Z, " \
            "float size1Width, " \
            "float size1Height, " \
            "float size1Depth)\n" \
            "{\n" \
            "    Atmos::Spatial::Point3D center0(center0X, center0Y, center0Z);\n" \
            "    Atmos::Spatial::Size3D size0(size0Width, size0Height, size0Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Point3D center1(center1X, center1Y, center1Z);\n" \
            "    Atmos::Spatial::Size3D size1(size1Width, size1Height, size1Depth);\n" \
            "    Atmos::Spatial::AxisAlignedBox3D box1(center1, size1);\n" \
            "\n" \
            "    const array<Atmos::Spatial::AxisAlignedBox3D> boxes = { box0, box1 };\n" \
            "    const auto envelope = Atmos::Spatial::Envelope(boxes);\n" \
            "    return Atmos::ToString(envelope.Left())\n" \
            "        + \" \" + Atmos::ToString(envelope.Top())\n" \
            "        + \" \" + Atmos::ToString(envelope.FarZ())\n" \
            "        + \" \" + Atmos::ToString(envelope.Right())\n" \
            "        + \" \" + Atmos::ToString(envelope.Bottom())\n" \
            "        + \" \" + Atmos::ToString(envelope.NearZ());\n" \
            "}",
            {
                center0.x,
                center0.y,
                center0.z,
                size0.width,
                size0.height,
                size0.depth,
                center1.x,
                center1.y,
                center1.z,
                size1.width,
                size1.height,
                size1.depth },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::AxisAlignedBox3D(center0, size0);
                const auto box1 = Spatial::AxisAlignedBox3D(center1, size1);
                const auto envelope = Spatial::Envelope({ box0, box1 });

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                const auto expectedResult = ToString(envelope.Left())
                    + " " + ToString(envelope.Top())
                    + " " + ToString(envelope.FarZ())
                    + " " + ToString(envelope.Right())
                    + " " + ToString(envelope.Bottom())
                    + " " + ToString(envelope.NearZ());
                REQUIRE(result == expectedResult);
            }
        }
    }

    GIVEN("script that returns Contains from GridAxisAlignedBox and GridPoint")
    {
        auto center = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto size = dataGeneration.RandomStack<
            Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>();
        auto point = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(int centerX, int centerY, int sizeWidth, int sizeHeight, int pointX, int pointY)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point center(centerX, centerY);\n" \
            "    Atmos::Spatial::Grid::Size size(sizeWidth, sizeHeight);\n" \
            "    Atmos::Spatial::Grid::AxisAlignedBox box(center, size);\n" \
            "\n" \
            "    Atmos::Spatial::Grid::Point point(pointX, pointY);\n" \
            "    return Atmos::Spatial::Grid::Contains(box, point);\n" \
            "}",
            { center.x, center.y, size.width, size.height, point.x, point.y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box = Spatial::Grid::AxisAlignedBox(center, size);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Grid::Contains(box, point));
            }
        }
    }

    GIVEN("script that returns Contains from GridAxisAlignedBox and GridAxisAlignedBox")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(int center0X, " \
            "int center0Y, " \
            "int size0Width, " \
            "int size0Height, " \
            "int center1X, " \
            "int center1Y, " \
            "int size1Width, " \
            "int size1Height)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point center0(center0X, center0Y);\n" \
            "    Atmos::Spatial::Grid::Size size0(size0Width, size0Height);\n" \
            "    Atmos::Spatial::Grid::AxisAlignedBox box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Grid::Point center1(center1X, center1Y);\n" \
            "    Atmos::Spatial::Grid::Size size1(size1Width, size1Height);\n" \
            "    Atmos::Spatial::Grid::AxisAlignedBox box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Grid::Contains(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                size0.width,
                size0.height,
                center1.x,
                center1.y,
                size1.width,
                size1.height },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::Grid::AxisAlignedBox(center0, size0);
                const auto box1 = Spatial::Grid::AxisAlignedBox(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Grid::Contains(box0, box1));
            }
        }
    }

    GIVEN("script that returns Intersects from GridAxisAlignedBox and GridAxisAlignedBox")
    {
        auto center0 = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto size0 = dataGeneration.RandomStack<
            Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>();
        auto center1 = dataGeneration.RandomStack<
            Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto size1 = dataGeneration.RandomStack<
            Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "bool main(int center0X, " \
            "int center0Y, " \
            "int size0Width, " \
            "int size0Height, " \
            "int center1X, " \
            "int center1Y, " \
            "int size1Width, " \
            "int size1Height)\n" \
            "{\n" \
            "    Atmos::Spatial::Grid::Point center0(center0X, center0Y);\n" \
            "    Atmos::Spatial::Grid::Size size0(size0Width, size0Height);\n" \
            "    Atmos::Spatial::Grid::AxisAlignedBox box0(center0, size0);\n" \
            "\n" \
            "    Atmos::Spatial::Grid::Point center1(center1X, center1Y);\n" \
            "    Atmos::Spatial::Grid::Size size1(size1Width, size1Height);\n" \
            "    Atmos::Spatial::Grid::AxisAlignedBox box1(center1, size1);\n" \
            "\n" \
            "    return Atmos::Spatial::Grid::Intersects(box0, box1);\n" \
            "}",
            {
                center0.x,
                center0.y,
                size0.width,
                size0.height,
                center1.x,
                center1.y,
                size1.width,
                size1.height },
                *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto box0 = Spatial::Grid::AxisAlignedBox(center0, size0);
                const auto box1 = Spatial::Grid::AxisAlignedBox(center1, size1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Spatial::Grid::Intersects(box0, box1));
            }
        }
    }

    GIVEN("script that returns ToPoint2D from Point3D")
    {
        auto x = dataGeneration.Random<Spatial::Point3D::Value>();
        auto y = dataGeneration.Random<Spatial::Point3D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint2D(Atmos::Spatial::Point3D(x, y, z));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y, z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(x) + " " + Atmos::ToString(y));
            }
        }
    }

    GIVEN("script that returns ToPoint2D from GridPoint")
    {
        auto x = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto y = dataGeneration.Random<Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(int x, int y)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint2D(Atmos::Spatial::Grid::Point(x, y));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToPoint2D(Spatial::Grid::Point{ x, y });

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that returns ToPoint2D from angle")
    {
        auto angle = dataGeneration.Random<Spatial::Angle2D>();
        auto distance = dataGeneration.Random<Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float angle, float distance)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint2D(angle, distance);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { angle, distance },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToPoint2D(angle, distance);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that returns ToPoint3D from Point2D")
    {
        auto x = dataGeneration.Random<Spatial::Point2D::Value>();
        auto y = dataGeneration.Random<Spatial::Point2D::Value>();
        auto z = dataGeneration.Random<Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint3D(Atmos::Spatial::Point2D(x, y), z);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y) + \" \" + Atmos::ToString(point.z);\n" \
            "}",
            { x, y, z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToPoint3D(Spatial::Point2D{ x, y }, z);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y) + " " + Atmos::ToString(expectedResult.z));
            }
        }
    }

    GIVEN("script that returns ToPoint3D from GridPoint")
    {
        auto x = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto y = dataGeneration.Random<Spatial::Grid::Point::Value>();
        auto z = dataGeneration.Random<Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(int x, int y, float z)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint3D(Atmos::Spatial::Grid::Point(x, y), z);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y) + \" \" + Atmos::ToString(point.z);\n" \
            "}",
            { x, y, z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToPoint3D(Spatial::Grid::Point{ x, y }, z);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y) + " " + Atmos::ToString(expectedResult.z));
            }
        }
    }

    GIVEN("script that returns ToPoint3D from angle")
    {
        auto yaw = dataGeneration.Random<Spatial::Angle3D::Value>(TestFramework::Range<Spatial::Angle3D::Value>{ -1000, 1000 });
        auto pitch = dataGeneration.Random<Spatial::Angle3D::Value>(TestFramework::Range<Spatial::Angle3D::Value>{ -1000, 1000 });
        auto distance = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>{ -1000, 1000 });

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float yaw, float pitch, float distance)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::ToPoint3D(Atmos::Spatial::Angle3D(yaw, pitch), distance);\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y) + \" \" + Atmos::ToString(point.z);\n" \
            "}",
            { yaw, pitch, distance },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToPoint3D(Spatial::Angle3D{ yaw, pitch }, distance);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y) + " " + Atmos::ToString(expectedResult.z));
            }
        }
    }

    GIVEN("script that returns ToGridPoint from Point2D")
    {
        auto x = dataGeneration.Random<Spatial::Point2D::Value>();
        auto y = dataGeneration.Random<Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::Grid::ToPoint(Atmos::Spatial::Point2D(x, y));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::Grid::ToPoint(Spatial::Point2D{ x, y });

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that returns ToGridPoint from Point3D")
    {
        auto x = dataGeneration.Random<Spatial::Point3D::Value>();
        auto y = dataGeneration.Random<Spatial::Point3D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float x, float y, float z)\n" \
            "{\n" \
            "    auto point = Atmos::Spatial::Grid::ToPoint(Atmos::Spatial::Point3D(x, y, z));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::Grid::ToPoint(Spatial::Point3D{ x, y, z });

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == Atmos::ToString(expectedResult.x) + " " + Atmos::ToString(expectedResult.y));
            }
        }
    }

    GIVEN("script that returns ToAxisAlignedBox2D")
    {
        auto left = dataGeneration.Random<Spatial::AxisAlignedBox2D::Coordinate>();
        auto top = dataGeneration.Random<Spatial::AxisAlignedBox2D::Coordinate>();
        auto right = dataGeneration.Random<Spatial::AxisAlignedBox2D::Coordinate>();
        auto bottom = dataGeneration.Random<Spatial::AxisAlignedBox2D::Coordinate>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float left, float top, float right, float bottom)\n" \
            "{\n" \
            "    auto box = Atmos::Spatial::ToAxisAlignedBox2D(left, top, right, bottom);\n" \
            "    return Atmos::ToString(box.center.x) + \" \" + " \
            "Atmos::ToString(box.center.y) + \" \" + " \
            "Atmos::ToString(box.size.width) + \" \" + " \
            "Atmos::ToString(box.size.height);\n" \
            "}",
            { left, top, right, bottom },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToAxisAlignedBox2D(left, top, right, bottom);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(expectedResult.center.x) + " " +
                    Atmos::ToString(expectedResult.center.y) + " " +
                    Atmos::ToString(expectedResult.size.width) + " " +
                    Atmos::ToString(expectedResult.size.height));
            }
        }
    }

    GIVEN("script that returns ToAxisAlignedBox3D")
    {
        auto left = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();
        auto top = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();
        auto farZ = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();
        auto right = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();
        auto bottom = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();
        auto nearZ = dataGeneration.Random<Spatial::AxisAlignedBox3D::Coordinate>();

        CompileAndCreateScript(
            "basic_script.as",
            "string main(float left, float top, float farZ, float right, float bottom, float nearZ)\n" \
            "{\n" \
            "    auto box = Atmos::Spatial::ToAxisAlignedBox3D(left, top, farZ, right, bottom, nearZ);\n" \
            "    return Atmos::ToString(box.center.x) + \" \" + " \
            "Atmos::ToString(box.center.y) + \" \" + " \
            "Atmos::ToString(box.center.z) + \" \" + " \
            "Atmos::ToString(box.size.width) + \" \" + " \
            "Atmos::ToString(box.size.height) + \" \" + " \
            "Atmos::ToString(box.size.depth);\n" \
            "}",
            { left, top, farZ, right, bottom, nearZ },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedResult = Spatial::ToAxisAlignedBox3D(left, top, farZ, right, bottom, nearZ);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result ==
                    Atmos::ToString(expectedResult.center.x) + " " +
                    Atmos::ToString(expectedResult.center.y) + " " +
                    Atmos::ToString(expectedResult.center.z) + " " +
                    Atmos::ToString(expectedResult.size.width) + " " +
                    Atmos::ToString(expectedResult.size.height) + " " +
                    Atmos::ToString(expectedResult.size.depth));
            }
        }
    }

    GIVEN("script that returns Point2D ToWorldPoint2D")
    {
        auto x = 100.0f;
        auto y = 200.0f;

        CompileAndCreateScript(
            "basic_script.as",
            "string main(int x, int y)\n" \
            "{\n" \
            "    auto screenPoint = Atmos::Spatial::Point2D(x, y);\n" \
            "    auto point = Arca::Reliquary::Do(Atmos::Spatial::ToWorldPoint2D(screenPoint));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y);\n" \
            "}",
            { x, y },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedPoint = fieldReliquary->Do(Spatial::ToWorldPoint2D(Spatial::Point2D{ x, y }));
                const auto expectedResult = ToString(expectedPoint.x) + " " + ToString(expectedPoint.y);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == expectedResult);
            }
        }
    }

    GIVEN("script that returns Point2D ToWorldPoint3D")
    {
        auto x = 100.0f;
        auto y = 200.0f;
        auto z = 300.0f;

        CompileAndCreateScript(
            "basic_script.as",
            "string main(int x, int y, float z)\n" \
            "{\n" \
            "    auto screenPoint = Atmos::Spatial::Point2D(x, y);\n" \
            "    auto point = Arca::Reliquary::Do(Atmos::Spatial::ToWorldPoint3D(screenPoint, z));\n" \
            "    return Atmos::ToString(point.x) + \" \" + Atmos::ToString(point.y) + \" \" + Atmos::ToString(point.z);\n" \
            "}",
            { x, y, z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("has correct properties")
            {
                REQUIRE(finishes.size() == 1);

                const auto expectedPoint = fieldReliquary->Do(Spatial::ToWorldPoint3D(Spatial::Point2D{ x, y }, z));
                const auto expectedResult = ToString(expectedPoint.x) + " " + ToString(expectedPoint.y) + " " + Atmos::ToString(expectedPoint.z);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));
                REQUIRE(result == expectedResult);
            }
        }
    }
}