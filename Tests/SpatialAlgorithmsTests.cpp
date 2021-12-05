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