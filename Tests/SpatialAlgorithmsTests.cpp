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