#include <catch.hpp>

#include "RenderAlgorithmTests.h"
#include <Atmos/RenderAlgorithms.h>

using namespace Atmos;
using namespace Spatial;
using namespace Render;

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "simplification", "[render]")
{
    std::vector<Point2D> points;

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
        points.push_back(Point2D{ 0.0f, 0.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 1.0f, 1.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 1.0f, 1.0f });
        points.push_back(Point2D{ 2.0f, 2.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 1.0f, 0.0f });
        points.push_back(Point2D{ 2.0f, 0.0f });
        points.push_back(Point2D{ 4.0f, 0.0f });
        points.push_back(Point2D{ 5.0f, 0.0f });
        points.push_back(Point2D{ 5.0f, 1.0f });
        points.push_back(Point2D{ 5.0f, 2.0f });
        points.push_back(Point2D{ 5.0f, 3.0f });
        points.push_back(Point2D{ 5.0f, 4.0f });
        points.push_back(Point2D{ 5.0f, 5.0f });
        points.push_back(Point2D{ 4.0f, 5.0f });
        points.push_back(Point2D{ 3.0f, 5.0f });
        points.push_back(Point2D{ 2.0f, 5.0f });
        points.push_back(Point2D{ 1.0f, 5.0f });
        points.push_back(Point2D{ 0.0f, 5.0f });
        points.push_back(Point2D{ 0.0f, 4.0f });
        points.push_back(Point2D{ 0.0f, 3.0f });
        points.push_back(Point2D{ 0.0f, 2.0f });
        points.push_back(Point2D{ 0.0f, 1.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 2.0f, 0.0f });
        points.push_back(Point2D{ 0.0f, 2.0f });

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

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "triangulation of Point2D", "[render]")
{
    std::vector<Point2D> points;

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
        points.push_back(Point2D{ 0.0f, 0.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 1.0f, 1.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 1.0f, 1.0f });
        points.push_back(Point2D{ 2.0f, 2.0f });

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
        points.push_back(Point2D{ 0.0f, 0.0f });
        points.push_back(Point2D{ 2.0f, 0.0f });
        points.push_back(Point2D{ 0.0f, 2.0f });

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

        points.push_back(Point2D{ -boxDistance, -boxDistance });
        points.push_back(Point2D{ boxDistance, -boxDistance });
        points.push_back(Point2D{ boxDistance, boxDistance });
        points.push_back(Point2D{ -boxDistance, boxDistance });

        WHEN("triangulating")
        {
            auto triangulation = Triangulate(points);

            THEN("converts to 6 vertices")
            {
                REQUIRE(triangulation.size() == 6);
                REQUIRE(triangulation[0].x == Approx(-10));
                REQUIRE(triangulation[0].y == Approx(-10));
                REQUIRE(triangulation[1].x == Approx(10));
                REQUIRE(triangulation[1].y == Approx(-10));
                REQUIRE(triangulation[2].x == Approx(10));
                REQUIRE(triangulation[2].y == Approx(10));
                REQUIRE(triangulation[3].x == Approx(-10));
                REQUIRE(triangulation[3].y == Approx(-10));
                REQUIRE(triangulation[4].x == Approx(10));
                REQUIRE(triangulation[4].y == Approx(10));
                REQUIRE(triangulation[5].x == Approx(-10));
                REQUIRE(triangulation[5].y == Approx(10));
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

        points.push_back(Point2D{ 0.0f, -pointDistance });
        points.push_back(Point2D{ boxDistance, -boxDistance });
        points.push_back(Point2D{ pointDistance, 0.0f });
        points.push_back(Point2D{ boxDistance, boxDistance });
        points.push_back(Point2D{ 0.0f, pointDistance });
        points.push_back(Point2D{ -boxDistance, boxDistance });
        points.push_back(Point2D{ -pointDistance, 0.0f });
        points.push_back(Point2D{ -boxDistance, -boxDistance });

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

        points.push_back(Point2D{ xOffset, yOffset });
        points.push_back(Point2D{ xOffset + width, yOffset });
        points.push_back(Point2D{ xOffset + width, yOffset + thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 2 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 2 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 3 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 3 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 4 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 4 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 5 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 5 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 6 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 6 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 7 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 7 * thickness });
        points.push_back(Point2D{ xOffset + thickness, yOffset + 8 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 8 * thickness });
        points.push_back(Point2D{ xOffset + width, yOffset + 9 * thickness });
        points.push_back(Point2D{ xOffset, yOffset + 9 * thickness });

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

SCENARIO_METHOD(RenderAlgorithmTestsFixture, "triangulation of GridPoint", "[render]")
{
    std::vector<Grid::Point> points;

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
        points.push_back(Grid::Point{ 0, 0 });

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
        points.push_back(Grid::Point{ 0, 0 });
        points.push_back(Grid::Point{ 1, 1 });

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
        points.push_back(Grid::Point{ 0, 0 });
        points.push_back(Grid::Point{ 1, 0 });
        points.push_back(Grid::Point{ 2, 0 });

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
        points.push_back(Grid::Point{ 0, 0 });
        points.push_back(Grid::Point{ 2, 0 });
        points.push_back(Grid::Point{ 0, 2 });

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