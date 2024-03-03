#include "catch.hpp"

#include "SpatialAlgorithmsTests.h"

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "spatial algorithms", "[spatial]")
{
    GIVEN("2D axis aligned boxes")
    {
        const auto boxes = std::vector
        {
            AxisAlignedBox2D{Point2D{-15, -15}, Size2D{5, 6}},
            AxisAlignedBox2D{Point2D{20, 20}, Size2D{3, 4}}
        };

        WHEN("finding envelope around boxes")
        {
            const auto envelope = Envelope(boxes);

            THEN("returns correct envelope")
            {
                REQUIRE(envelope.Left() == Approx(-17.5));
                REQUIRE(envelope.Top() == Approx(-18));
                REQUIRE(envelope.Right() == Approx(21.5));
                REQUIRE(envelope.Bottom() == Approx(22));
            }
        }
    }

    GIVEN("3D axis aligned boxes")
    {
        const auto boxes = std::vector
        {
            AxisAlignedBox3D{Point3D{-15, -15, -15}, Size3D{5, 6, 7}},
            AxisAlignedBox3D{Point3D{20, 20, 20}, Size3D{2, 3, 4}}
        };

        WHEN("finding envelope around boxes")
        {
            const auto envelope = Envelope(boxes);

            THEN("returns correct envelope")
            {
                REQUIRE(envelope.Left() == Approx(-17.5));
                REQUIRE(envelope.Top() == Approx(-18));
                REQUIRE(envelope.FarZ() == Approx(-18.5));
                REQUIRE(envelope.Right() == Approx(21));
                REQUIRE(envelope.Bottom() == Approx(21.5));
                REQUIRE(envelope.NearZ() == Approx(22));
            }
        }
    }

    GIVEN("grid position at origin")
    {
        const auto origin = Point{ 0, 0 };

        WHEN("finding distance to position in bottom right")
        {
            const auto next = Point{ 3, 3 };

            const auto distance = Distance(origin, next);

            THEN("returns correct values")
            {
                REQUIRE(distance == 6);
            }
        }

        WHEN("finding distance to position in top left")
        {
            const auto next = Point{ -3, -3 };

            const auto distance = Distance(origin, next);

            THEN("returns correct values")
            {
                REQUIRE(distance == 6);
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "clamp algorithms", "[spatial]")
{
    GIVEN("2D axis aligned boxes")
    {
        GIVEN("overlapping box")
        {
            const auto box = ToAxisAlignedBox2D(5, 5, 15, 15);
            const auto to = ToAxisAlignedBox2D(0, 0, 10, 10);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(5));
                    REQUIRE(clamped.Top() == Approx(5));
                    REQUIRE(clamped.Right() == Approx(10));
                    REQUIRE(clamped.Bottom() == Approx(10));
                }
            }
        }

        GIVEN("box to the left")
        {
            const auto box = ToAxisAlignedBox2D(-10, -10, -5, -5);
            const auto to = ToAxisAlignedBox2D(10, -10, 15, -5);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.Right() == Approx(10));
                    REQUIRE(clamped.Bottom() == Approx(-5));
                }
            }
        }

        GIVEN("box to the top")
        {
            const auto box = ToAxisAlignedBox2D(10, -20, 15, -15);
            const auto to = ToAxisAlignedBox2D(10, -10, 15, -5);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-10));
                }
            }
        }

        GIVEN("box to the right")
        {
            const auto box = ToAxisAlignedBox2D(20, -10, 25, -5);
            const auto to = ToAxisAlignedBox2D(10, -10, 15, -5);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(15));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-5));
                }
            }
        }

        GIVEN("box to the bottom")
        {
            const auto box = ToAxisAlignedBox2D(10, 20, 15, 25);
            const auto to = ToAxisAlignedBox2D(10, 10, 15, 15);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(15));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(15));
                }
            }
        }
    }

    GIVEN("3D axis aligned boxes")
    {
        GIVEN("overlapping box")
        {
            const auto box = ToAxisAlignedBox3D(5, 5, 5, 15, 15, 15);
            const auto to = ToAxisAlignedBox3D(0, 0, 0, 10, 10, 10);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(5));
                    REQUIRE(clamped.Top() == Approx(5));
                    REQUIRE(clamped.FarZ() == Approx(5));
                    REQUIRE(clamped.Right() == Approx(10));
                    REQUIRE(clamped.Bottom() == Approx(10));
                    REQUIRE(clamped.NearZ() == Approx(10));
                }
            }
        }

        GIVEN("box to the left")
        {
            const auto box = ToAxisAlignedBox3D(-10, -10, -30, -5, -5, -25);
            const auto to = ToAxisAlignedBox3D(10, -10, -30, 15, -5, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.FarZ() == Approx(-30));
                    REQUIRE(clamped.Right() == Approx(10));
                    REQUIRE(clamped.Bottom() == Approx(-5));
                    REQUIRE(clamped.NearZ() == Approx(-25));
                }
            }
        }

        GIVEN("box to the top")
        {
            const auto box = ToAxisAlignedBox3D(10, -20, -30, 15, -15, -25);
            const auto to = ToAxisAlignedBox3D(10, -10, -30, 15, -5, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.FarZ() == Approx(-30));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-10));
                    REQUIRE(clamped.NearZ() == Approx(-25));
                }
            }
        }

        GIVEN("box to the far z")
        {
            const auto box = ToAxisAlignedBox3D(10, -20, -50, 15, -15, -45);
            const auto to = ToAxisAlignedBox3D(10, -10, -30, 15, -5, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.FarZ() == Approx(-30));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-10));
                    REQUIRE(clamped.NearZ() == Approx(-30));
                }
            }
        }

        GIVEN("box to the right")
        {
            const auto box = ToAxisAlignedBox3D(20, -10, -30, 25, -5, -25);
            const auto to = ToAxisAlignedBox3D(10, -10, -30, 15, -5, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(15));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.FarZ() == Approx(-30));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-5));
                    REQUIRE(clamped.NearZ() == Approx(-25));
                }
            }
        }

        GIVEN("box to the bottom")
        {
            const auto box = ToAxisAlignedBox3D(10, 20, -30, 15, 25, -25);
            const auto to = ToAxisAlignedBox3D(10, 10, -30, 15, 15, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(15));
                    REQUIRE(clamped.FarZ() == Approx(-30));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(15));
                    REQUIRE(clamped.NearZ() == Approx(-25));
                }
            }
        }

        GIVEN("box to the near z")
        {
            const auto box = ToAxisAlignedBox3D(10, -20, 50, 15, -15, 55);
            const auto to = ToAxisAlignedBox3D(10, -10, -30, 15, -5, -25);

            WHEN("clamping")
            {
                const auto clamped = Clamp(box, to);

                THEN("returns correct clamp")
                {
                    REQUIRE(clamped.Left() == Approx(10));
                    REQUIRE(clamped.Top() == Approx(-10));
                    REQUIRE(clamped.FarZ() == Approx(-25));
                    REQUIRE(clamped.Right() == Approx(15));
                    REQUIRE(clamped.Bottom() == Approx(-10));
                    REQUIRE(clamped.NearZ() == Approx(-25));
                }
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "ScaleOf", "[spatial]")
{
    GIVEN("2D axis aligned boxes")
    {
        GIVEN("overlapping box")
        {
            const auto box = ToAxisAlignedBox2D(5, 5, 15, 15);
            const auto against = ToAxisAlignedBox2D(0, 0, 10, 10);

            WHEN("retrieve ScaleOf")
            {
                const auto result = ScaleOf(box, against);

                THEN("returns correct result")
                {
                    REQUIRE(result.Left() == Approx(0.5));
                    REQUIRE(result.Top() == Approx(0.5));
                    REQUIRE(result.Right() == Approx(1.5));
                    REQUIRE(result.Bottom() == Approx(1.5));
                }
            }
        }
    }

    GIVEN("3D axis aligned boxes")
    {
        GIVEN("overlapping box")
        {
            const auto box = ToAxisAlignedBox3D(5, 5, 5, 15, 15, 15);
            const auto against = ToAxisAlignedBox3D(0, 0, 0, 10, 10, 10);

            WHEN("retrieve ScaleOf")
            {
                const auto result = ScaleOf(box, against);

                THEN("returns correct result")
                {
                    REQUIRE(result.Left() == Approx(0.5));
                    REQUIRE(result.Top() == Approx(0.5));
                    REQUIRE(result.FarZ() == Approx(0.5));
                    REQUIRE(result.Right() == Approx(1.5));
                    REQUIRE(result.Bottom() == Approx(1.5));
                    REQUIRE(result.NearZ() == Approx(1.5));
                }
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "SideOf", "[spatial]")
{
    GIVEN("vertical line up")
    {
        const auto line = Line2D{ {0, 0}, {0, 5} };

        WHEN("point is to the left")
        {
            const auto point = Point2D{ -5, 2 };

            THEN("returns correct result")
            {
                const auto result = SideOf(line, point);
                REQUIRE(result == Side::Left);
            }
        }

        WHEN("point is to the right")
        {
            const auto point = Point2D{ 5, 2 };

            THEN("returns correct result")
            {
                const auto result = SideOf(line, point);
                REQUIRE(result == Side::Right);
            }
        }
    }

    GIVEN("vertical line down")
    {
        const auto line = Line2D{ {0, 5}, {0, 0} };

        WHEN("point is to the left")
        {
            const auto point = Point2D{ 5, 2 };

            THEN("returns correct result")
            {
                const auto result = SideOf(line, point);
                REQUIRE(result == Side::Left);
            }
        }

        WHEN("point is to the right")
        {
            const auto point = Point2D{ -5, 2 };

            THEN("returns correct result")
            {
                const auto result = SideOf(line, point);
                REQUIRE(result == Side::Right);
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "Intersection", "[spatial]")
{
    GIVEN("vertical line up")
    {
        const auto verticalLine = Line2D{ {0, 0}, {0, 5} };

        WHEN("finding intersection of horizontal line")
        {
            const auto horizontalLine = Line2D{ {-2.5, 2.5}, { 2.5, 2.5 } };

            THEN("returns correct result")
            {
                const auto result = Intersection(verticalLine, horizontalLine);
                REQUIRE(result.has_value());
                REQUIRE(result->x == Approx(0));
                REQUIRE(result->y == Approx(2.5));
            }
        }

        WHEN("finding intersection of disjoint horizontal line")
        {
            const auto horizontalLine = Line2D{ {0.5, 0}, { 5, 0 } };

            THEN("returns correct result")
            {
                const auto result = Intersection(verticalLine, horizontalLine);
                REQUIRE(result.has_value());
                REQUIRE(result->x == Approx(0));
                REQUIRE(result->y == Approx(0));
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "Clip", "[spatial]")
{
    GIVEN("points in rectangle")
    {
        const auto points = std::vector<Point2D>
        {
            { 0.0, 0.0 },
            { 5.0, 0.0 },
            { 5.0, 5.0 },
            { 0.0, 5.0 }
        };

        WHEN("finding clip of rectangle")
        {
            const auto clip = std::vector<Point2D>
            {
                { 2.5, 2.5 },
                { 7.5, 2.5 },
                { 7.5, 7.5 },
                { 2.5, 7.5 }
            };

            THEN("returns correct result")
            {
                const auto result = Clip(points, clip);
                REQUIRE(result.size() == 4);
                REQUIRE(result[0].x == Approx(2.5));
                REQUIRE(result[0].y == Approx(2.5));
                REQUIRE(result[1].x == Approx(5.0));
                REQUIRE(result[1].y == Approx(2.5));
                REQUIRE(result[2].x == Approx(5.0));
                REQUIRE(result[2].y == Approx(5.0));
                REQUIRE(result[3].x == Approx(2.5));
                REQUIRE(result[3].y == Approx(5.0));
            }
        }

        WHEN("finding clip of same rectangle")
        {
            constexpr auto epsilon = std::numeric_limits<float>::epsilon();
            const auto clip = points;

            THEN("returns points")
            {
                const auto result = Clip(points, clip);
                REQUIRE(result.size() == 4);
                REQUIRE(result[0].x == Approx(0.0));
                REQUIRE(result[0].y == Approx(5.0));
                REQUIRE(result[1].x == Approx(0.0));
                REQUIRE(result[1].y == Approx(0.0));
                REQUIRE(result[2].x == Approx(5.0));
                REQUIRE(result[2].y == Approx(0.0));
                REQUIRE(result[3].x == Approx(5.0));
                REQUIRE(result[3].y == Approx(5.0));
            }
        }
    }

    GIVEN("AxisAlignedBox2D")
    {
        const auto box = ToAxisAlignedBox2D(0.0, 0.0, 5.0, 5.0);

        WHEN("finding clip of rectangle")
        {
            const auto clip = ToAxisAlignedBox2D(2.5, 2.5, 7.5, 7.5);

            THEN("returns correct result")
            {
                const auto result = Clip(box, clip);
                REQUIRE(result.Left() == Approx(2.5));
                REQUIRE(result.Top() == Approx(2.5));
                REQUIRE(result.Right() == Approx(5.0));
                REQUIRE(result.Bottom() == Approx(5.0));
            }
        }
    }
}

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "ToPoints", "[spatial]")
{
    GIVEN("AxisAlignedBox2D and rotation")
    {
        const auto box = AxisAlignedBox2D
        {
            Point2D { 10, 20 },
            Size2D { 30, 40 }
        };

        const auto rotation = 95.0f;
        const auto rotationCenter = Point2D{ 50, 60 };

        WHEN("finding points")
        {
            THEN("returns correct result")
            {
                const auto result = ToPoints(box, rotation, rotationCenter);
                REQUIRE(result.size() == 4);
                REQUIRE(result[0].x == Approx(50.8361549));
                REQUIRE(result[0].y == Approx(-21.3898087));
                REQUIRE(result[1].x == Approx(72.7413635));
                REQUIRE(result[1].y == Approx(-0.891960144));
                REQUIRE(result[2].x == Approx(45.4108963));
                REQUIRE(result[2].y == Approx(28.3149853));
                REQUIRE(result[3].x == Approx(23.5056877));
                REQUIRE(result[3].y == Approx(7.81713867));
            }
        }
    }
}