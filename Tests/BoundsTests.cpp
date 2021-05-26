#include "catch.hpp"
#include "BoundsTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Arca/OpenRelic.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/SpatialAlgorithms.h>

using namespace Catch::literals;

SCENARIO_METHOD(BoundsTestsFixture, "bounds", "[spatial]")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterArcaTypes(reliquaryOrigin);
        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        auto created = reliquary->Do(
            Arca::Create<Arca::OpenRelic>());
        auto bounds = reliquary->Do(
            Arca::Create<Bounds>(created, BoundsSpace::World, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

        WHEN("moving bounds to position")
        {
            const auto position = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            reliquary->Do(MoveBounds(bounds.ID(), position));

            const auto newPosition = bounds->Position();

            THEN("bounds are moved")
            {
                REQUIRE(newPosition.x == Approx(position.x));
                REQUIRE(newPosition.y == Approx(position.y));
                REQUIRE(newPosition.z == Approx(position.z));
            }
        }

        
        WHEN("scaling bounds")
        {
            const auto scalers = Scalers2D
            {
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Scalers2D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            reliquary->Do(ScaleBounds(bounds.ID(), scalers));

            THEN("bounds are scaled")
            {
                REQUIRE(bounds->Scalers() != Scalers2D{});
                REQUIRE(bounds->Scalers() == scalers);
            }

            THEN("size is scalers")
            {
                REQUIRE(bounds->Size() == Size2D{ scalers.x, scalers.y });
            }
        }

        WHEN("rotating bounds")
        {
            const auto angle = dataGeneration.Random<Angle2D>(TestFramework::Range(-1000.0f, 1000.0f));

            reliquary->Do(RotateBounds(bounds.ID(), angle));

            THEN("bounds are rotated")
            {
                REQUIRE(bounds->Rotation() != Angle2D{});
                REQUIRE(bounds->Rotation() == angle);
            }
        }
    }
}