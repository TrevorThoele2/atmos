#include <catch.hpp>

#include "RenderAlgorithmTests.h"
#include <Atmos/RenderAlgorithms.h>
#include <Atmos/GridCellSize.h>

using namespace Atmos;
using namespace Render;

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "simplification")
{
    std::vector<Position2D> points;

    GIVEN("zero points")
    {
        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to empty vertices")
            {
                REQUIRE(simplification.empty());
            }
        }
    }

    GIVEN("one point")
    {
        points.emplace_back(0.0f, 0.0f);

        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to one vertices")
            {
                REQUIRE(simplification.size() == 1);
                REQUIRE(simplification[0] == points[0]);
            }
        }
    }

    GIVEN("two points")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(1.0f, 1.0f);

        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to two vertices")
            {
                REQUIRE(simplification.size() == 2);
                REQUIRE(simplification[0] == points[0]);
                REQUIRE(simplification[1] == points[1]);
            }
        }
    }

    GIVEN("three points in a line")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(1.0f, 1.0f);
        points.emplace_back(2.0f, 2.0f);

        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to two vertices")
            {
                REQUIRE(simplification.size() == 2);
                REQUIRE(simplification[0] == points[0]);
                REQUIRE(simplification[1] == points[2]);
            }
        }
    }

    GIVEN("square with many redundant points")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(1.0f, 0.0f);
        points.emplace_back(2.0f, 0.0f);
        points.emplace_back(4.0f, 0.0f);
        points.emplace_back(5.0f, 0.0f);
        points.emplace_back(5.0f, 1.0f);
        points.emplace_back(5.0f, 2.0f);
        points.emplace_back(5.0f, 3.0f);
        points.emplace_back(5.0f, 4.0f);
        points.emplace_back(5.0f, 5.0f);
        points.emplace_back(4.0f, 5.0f);
        points.emplace_back(3.0f, 5.0f);
        points.emplace_back(2.0f, 5.0f);
        points.emplace_back(1.0f, 5.0f);
        points.emplace_back(0.0f, 5.0f);
        points.emplace_back(0.0f, 4.0f);
        points.emplace_back(0.0f, 3.0f);
        points.emplace_back(0.0f, 2.0f);
        points.emplace_back(0.0f, 1.0f);

        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to four vertices")
            {
                REQUIRE(simplification.size() == 4);
                REQUIRE(simplification[0] == points[0]);
                REQUIRE(simplification[1] == points[4]);
                REQUIRE(simplification[2] == points[9]);
                REQUIRE(simplification[3] == points[14]);
            }
        }
    }

    GIVEN("three points not in a line")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(2.0f, 0.0f);
        points.emplace_back(0.0f, 2.0f);

        WHEN("simplifying")
        {
            const auto simplification = Simplify(points);

            THEN("converts to three vertices")
            {
                REQUIRE(simplification.size() == 3);
            }
        }
    }
}

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "triangulation of Position2D")
{
    std::vector<Position2D> points;

    GIVEN("zero points")
    {
        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to empty vertices")
            {
                REQUIRE(triangulation.empty());
            }
        }
    }

    GIVEN("one point")
    {
        points.emplace_back(0.0f, 0.0f);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to empty vertices")
            {
                REQUIRE(triangulation.empty());
            }
        }
    }

    GIVEN("two points")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(1.0f, 1.0f);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to empty vertices")
            {
                REQUIRE(triangulation.empty());
            }
        }
    }

    GIVEN("three points in a line")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(1.0f, 1.0f);
        points.emplace_back(2.0f, 2.0f);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to empty vertices and indices")
            {
                REQUIRE(triangulation.empty());
            }
        }
    }

    GIVEN("three points not in a line")
    {
        points.emplace_back(0.0f, 0.0f);
        points.emplace_back(2.0f, 0.0f);
        points.emplace_back(0.0f, 2.0f);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to three vertices")
            {
                REQUIRE(triangulation.size() == 3);
                REQUIRE(triangulation[0] == points[0]);
                REQUIRE(triangulation[1] == points[1]);
                REQUIRE(triangulation[2] == points[2]);
            }
        }
    }

    GIVEN("quad")
    {
        const auto boxDistance = 10.0f;

        points.emplace_back(-boxDistance, -boxDistance);
        points.emplace_back(boxDistance, -boxDistance);
        points.emplace_back(boxDistance, boxDistance);
        points.emplace_back(-boxDistance, boxDistance);

        WHEN("triangulating")
        {
            auto triangulation = Triangulate(points);

            THEN("converts to 6 vertices")
            {
                REQUIRE(triangulation.size() == 6);
            }

            WHEN("converting to mesh")
            {
                auto mesh = ConvertToMesh(triangulation);

                THEN("converts to vertex count as input count and index count as triangulated vertex count")
                {
                    REQUIRE(mesh.vertices.size() == points.size());
                    REQUIRE(mesh.indices.size() == triangulation.size());
                }
            }
        }
    }

    GIVEN("four point star")
    {
        const auto boxDistance = 10.0f;
        const auto pointDistance = 30.0f;

        points.emplace_back(0.0f, -pointDistance);
        points.emplace_back(boxDistance, -boxDistance);
        points.emplace_back(pointDistance, 0.0f);
        points.emplace_back(boxDistance, boxDistance);
        points.emplace_back(0.0f, pointDistance);
        points.emplace_back(-boxDistance, boxDistance);
        points.emplace_back(-pointDistance, 0.0f);
        points.emplace_back(-boxDistance, -boxDistance);

        WHEN("triangulating")
        {
            auto triangulation = Triangulate(points);

            THEN("converts to 18 vertices")
            {
                REQUIRE(triangulation.size() == 18);
            }

            WHEN("converting to mesh")
            {
                auto mesh = ConvertToMesh(triangulation);

                THEN("converts to vertex count as input count and index count as triangulated vertex count")
                {
                    REQUIRE(mesh.vertices.size() == points.size());
                    REQUIRE(mesh.indices.size() == triangulation.size());
                }
            }
        }
    }

    GIVEN("strange E shape positively offset from center")
    {
        const auto width = 15.0f;
        const auto thickness = 5.0f;
        const auto xOffset = 100.0f;
        const auto yOffset = 100.0f;

        points.emplace_back(xOffset, yOffset);
        points.emplace_back(xOffset + width, yOffset);
        points.emplace_back(xOffset + width, yOffset + thickness);
        points.emplace_back(xOffset + thickness, yOffset + thickness);
        points.emplace_back(xOffset + thickness, yOffset + 2 * thickness);
        points.emplace_back(xOffset + width, yOffset + 2 * thickness);
        points.emplace_back(xOffset + width, yOffset + 3 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 3 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 4 * thickness);
        points.emplace_back(xOffset + width, yOffset + 4 * thickness);
        points.emplace_back(xOffset + width, yOffset + 5 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 5 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 6 * thickness);
        points.emplace_back(xOffset + width, yOffset + 6 * thickness);
        points.emplace_back(xOffset + width, yOffset + 7 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 7 * thickness);
        points.emplace_back(xOffset + thickness, yOffset + 8 * thickness);
        points.emplace_back(xOffset + width, yOffset + 8 * thickness);
        points.emplace_back(xOffset + width, yOffset + 9 * thickness);
        points.emplace_back(xOffset, yOffset + 9 * thickness);

        WHEN("triangulating")
        {
            auto triangulation = Triangulate(points);

            THEN("converts to 39 vertices")
            {
                REQUIRE(triangulation.size() == 39);
            }

            WHEN("converting to mesh")
            {
                auto mesh = ConvertToMesh(triangulation);

                THEN("converts to vertex count as input count and index count as triangulated vertex count")
                {
                    REQUIRE(mesh.vertices.size() == points.size());
                    REQUIRE(mesh.indices.size() == triangulation.size());
                }
            }
        }
    }
}

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "triangulation of Grid::Position")
{
    std::vector<Grid::Position> points;

    GIVEN("zero points")
    {
        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to empty vertices")
            {
                REQUIRE(triangulation.empty());
            }
        }
    }

    GIVEN("one point")
    {
        points.emplace_back(0, 0);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to six vertices")
            {
                REQUIRE(triangulation.size() == 6);
            }
        }
    }

    GIVEN("two disconnected points")
    {
        points.emplace_back(0, 0);
        points.emplace_back(1, 1);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to twelve vertices")
            {
                REQUIRE(triangulation.size() == 12);
            }
        }
    }

    GIVEN("three points in a line")
    {
        points.emplace_back(0, 0);
        points.emplace_back(1, 0);
        points.emplace_back(2, 0);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to six vertices")
            {
                REQUIRE(triangulation.size() == 6);
            }
        }
    }

    GIVEN("three points not in a line")
    {
        points.emplace_back(0, 0);
        points.emplace_back(2, 0);
        points.emplace_back(0, 2);

        WHEN("triangulating")
        {
            const auto triangulation = Triangulate(points);

            THEN("converts to eighteen vertices")
            {
                REQUIRE(triangulation.size() == 18);
            }
        }
    }
}