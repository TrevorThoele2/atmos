#include "catch.hpp"

#include "SpatialAlgorithmsTests.h"

SCENARIO_METHOD(SpatialAlgorithmsTestsFixture, "spatial algorithms", "[spatial]")
{
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