#include "catch.hpp"
#include "BoundsTests.h"

#include "SignalListener.h"

#include <Arca/ReliquaryOrigin.h>
#include <Arca/LocalRelic.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Atmos/MathUtility.h>

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
            Arca::Create<Bounds>(created, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

        WHEN("moving bounds to position")
        {
            const auto position = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            reliquary->Do(MoveBoundsTo(bounds.ID(), position));

            const auto newPosition = bounds->Position();

            THEN("bounds are moved")
            {
                REQUIRE(newPosition.x == Approx(position.x));
                REQUIRE(newPosition.y == Approx(position.y));
                REQUIRE(newPosition.z == Approx(position.z));
            }
        }

        WHEN("moving bounds by position")
        {
            const auto initialPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };
            const auto delta =
                dataGeneration.RandomStack<Point3D, Point3D::Value, Point3D::Value, Point3D::Value>();

            reliquary->Do(MoveBoundsTo(bounds.ID(), initialPosition));
            reliquary->Do(MoveBoundsBy(bounds.ID(), delta));

            const auto newPosition = bounds->Position();

            THEN("bounds are moved")
            {
                const auto checkPosition = initialPosition + delta;

                REQUIRE(newPosition.x == Approx(checkPosition.x));
                REQUIRE(newPosition.y == Approx(checkPosition.y));
                REQUIRE(newPosition.z == Approx(checkPosition.z));
            }
        }

        WHEN("moving bounds in direction")
        {
            const auto initialPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };
            const auto direction = Angle3D
            {
                dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>)),
                dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>))
            };
            const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

            reliquary->Do(MoveBoundsTo(bounds.ID(), initialPosition));
            reliquary->Do(MoveBoundsDirection(bounds.ID(), direction, amount));

            const auto newPosition = bounds->Position();

            THEN("bounds are moved")
            {
                const auto checkPosition = initialPosition + ToPoint3D(direction, amount);

                REQUIRE(newPosition.x == Approx(checkPosition.x));
                REQUIRE(newPosition.y == Approx(checkPosition.y));
                REQUIRE(newPosition.z == Approx(checkPosition.z));
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

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds", "[spatial]")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterArcaTypes(reliquaryOrigin);
        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating child relic with relative bounds")
        {
            const auto parentPosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            const auto relativePosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            auto parent = reliquary->Do(Arca::Create<Arca::OpenRelic>());
            reliquary->Do(
                Arca::Create<Bounds>(parent, parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = reliquary->Do(
                Arca::Create<Bounds>(child, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));
            auto relativeBounds = reliquary->Do(
                Arca::Create<RelativeBounds>(child, relativePosition));

            WHEN("moving child bounds to position")
            {
                const auto childPosition = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(childBounds.ID(), childPosition));
                const auto newRelativePosition = relativeBounds->delta;
                const auto newChildPosition = childBounds->Position();

                THEN("is at correct position")
                {
                    const auto checkPosition = childPosition;

                    REQUIRE(newChildPosition != Point3D());
                    REQUIRE(newChildPosition.x == Approx(checkPosition.x));
                    REQUIRE(newChildPosition.y == Approx(checkPosition.y));
                    REQUIRE(newChildPosition.z == Approx(checkPosition.z));
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = parentPosition + newRelativePosition;

                    REQUIRE(newChildPosition.x == Approx(checkPosition.x));
                    REQUIRE(newChildPosition.y == Approx(checkPosition.y));
                    REQUIRE(newChildPosition.z == Approx(checkPosition.z));
                }
            }

            WHEN("moving child bounds by position")
            {
                const auto childDelta =
                    dataGeneration.RandomStack<Point3D, Point3D::Value, Point3D::Value, Point3D::Value>();

                reliquary->Do(MoveBoundsBy(childBounds.ID(), childDelta));
                const auto newRelativePosition = relativeBounds->delta;
                const auto childPosition = childBounds->Position();

                THEN("is at correct position")
                {
                    const auto checkPosition = parentPosition + relativePosition + childDelta;

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = parentPosition + newRelativePosition;

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }
            }

            WHEN("moving child bounds in direction")
            {
                const auto direction = Angle3D
                {
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>)),
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>))
                };
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(childBounds.ID(), direction, amount));
                const auto newRelativePosition = relativeBounds->delta;
                const auto childPosition = childBounds->Position();

                THEN("is at correct position")
                {
                    const auto checkPosition = parentPosition + relativePosition + ToPoint3D(direction, amount);

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = parentPosition + newRelativePosition;

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds initialization", "[spatial]")
{
    GIVEN("registered reliquary with parent")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterArcaTypes(reliquaryOrigin);
        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        const auto parentPosition = Point3D
        {
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
        };

        auto parent = reliquary->Do(Arca::Create<Arca::OpenRelic>());
        reliquary->Do(Arca::Create<Bounds>(parent, parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

        WHEN("creating child relic with relative bounds last")
        {
            const auto relativePosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = reliquary->Do(
                Arca::Create<Bounds>(child, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));
            reliquary->Do(
                Arca::Create<RelativeBounds>(child, relativePosition));
            const auto childPosition = childBounds->Position();

            THEN("is at correct position")
            {
                const auto checkPosition = parentPosition + relativePosition;

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == Approx(checkPosition.x));
                REQUIRE(childPosition.y == Approx(checkPosition.y));
                REQUIRE(childPosition.z == Approx(checkPosition.z));
            }

            THEN("parent position and relative position adds up to child position")
            {
                const auto checkPosition = parentPosition + relativePosition;

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == Approx(checkPosition.x));
                REQUIRE(childPosition.y == Approx(checkPosition.y));
                REQUIRE(childPosition.z == Approx(checkPosition.z));
            }
        }

        WHEN("creating child relic with bounds last")
        {
            const auto relativePosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            reliquary->Do(Arca::Create<RelativeBounds>(child, relativePosition));
            auto childBounds = reliquary->Do(
                Arca::Create<Bounds>(child, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));
            const auto childPosition = childBounds->Position();

            THEN("is at correct position")
            {
                const auto checkPosition = parentPosition + relativePosition;

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == Approx(checkPosition.x));
                REQUIRE(childPosition.y == Approx(checkPosition.y));
                REQUIRE(childPosition.z == Approx(checkPosition.z));
            }

            THEN("parent position and relative position adds up to child position")
            {
                const auto checkPosition = parentPosition + relativePosition;

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == Approx(checkPosition.x));
                REQUIRE(childPosition.y == Approx(checkPosition.y));
                REQUIRE(childPosition.z == Approx(checkPosition.z));
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds children", "[spatial]")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterArcaTypes(reliquaryOrigin);
        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating child relic with relative bounds")
        {
            const auto relativePosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            auto parent = reliquary->Do(Arca::Create<Arca::OpenRelic>());
            auto parentBounds = reliquary->Do(
                Arca::Create<Bounds>(parent, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = reliquary->Do(
                Arca::Create<Bounds>(child, Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle2D{}));
            reliquary->Do(
                Arca::Create<RelativeBounds>(child, relativePosition));

            WHEN("moving parent bounds to position")
            {
                const auto parentPosition = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(parentBounds.ID(), parentPosition));

                THEN("child bounds are moved")
                {
                    const auto checkPosition = parentPosition + relativePosition;

                    REQUIRE(childBounds->Position() != Point3D());
                    REQUIRE(childBounds->Position() == checkPosition);
                }
            }

            WHEN("moving parent bounds by position")
            {
                const auto initialParentPosition = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };
                const auto parentDelta =
                    dataGeneration.RandomStack<Point3D, Point3D::Value, Point3D::Value, Point3D::Value>();

                reliquary->Do(MoveBoundsTo(parentBounds.ID(), initialParentPosition));
                reliquary->Do(MoveBoundsBy(parentBounds.ID(), parentDelta));

                const auto parentPosition = parentBounds->Position();
                const auto childPosition = childBounds->Position();

                THEN("child bounds are moved")
                {
                    const auto checkPosition = parentPosition + relativePosition;

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }
            }

            WHEN("moving parent bounds in direction")
            {
                const auto initialPosition = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };
                const auto direction = Angle3D
                {
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>)),
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>))
                };
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsTo(parentBounds.ID(), initialPosition));
                reliquary->Do(MoveBoundsDirection(parentBounds.ID(), direction, amount));

                const auto parentPosition = parentBounds->Position();
                const auto childPosition = childBounds->Position();

                THEN("child bounds are moved")
                {
                    const auto checkPosition = parentPosition + relativePosition;

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == Approx(checkPosition.x));
                    REQUIRE(childPosition.y == Approx(checkPosition.y));
                    REQUIRE(childPosition.z == Approx(checkPosition.z));
                }
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds errors", "[spatial]")
{
    GIVEN("registered reliquary")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        logger.Add<Logging::FileSink>();

        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterArcaTypes(reliquaryOrigin);
        Spatial::RegisterTypes(reliquaryOrigin);
        Logging::RegisterTypes(reliquaryOrigin, logger);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating relic with relative bounds without parent")
        {
            auto relic = reliquary->Do(Arca::Create<Arca::OpenRelic>());
            auto bounds = reliquary->Do(Arca::Create<Bounds>(relic));
            reliquary->Do(Arca::Create<RelativeBounds>(relic));

            WHEN("moving relic to position")
            {
                const auto position = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(bounds.ID(), position));

                THEN("does not move")
                {
                    REQUIRE(bounds->Position() == Point3D{});
                }
            }

            WHEN("moving relic by position")
            {
                const auto delta =
                    dataGeneration.RandomStack<Point3D, Point3D::Value, Point3D::Value, Point3D::Value>();

                reliquary->Do(MoveBoundsBy(bounds.ID(), delta));

                THEN("does not move")
                {
                    REQUIRE(bounds->Position() == Point3D{});
                }
            }

            WHEN("moving relic in direction")
            {
                const auto direction = Angle3D
                {
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>)),
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>))
                };
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(bounds.ID(), direction, amount));

                THEN("does not move")
                {
                    REQUIRE(bounds->Position() == Point3D{});
                }
            }
        }

        WHEN("creating relic with relative bounds with parent without bounds")
        {
            auto parent = reliquary->Do(Arca::Create<Arca::OpenRelic>());

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = reliquary->Do(Arca::Create<Bounds>(child));
            reliquary->Do(Arca::Create<RelativeBounds>(child));

            WHEN("moving child to position")
            {
                const auto position = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(childBounds.ID(), position));

                THEN("does not move child")
                {
                    REQUIRE(childBounds->Position() == Point3D{});
                }
            }

            WHEN("moving child by position")
            {
                const auto delta =
                    dataGeneration.RandomStack<Point3D, Point3D::Value, Point3D::Value, Point3D::Value>();

                reliquary->Do(MoveBoundsBy(childBounds.ID(), delta));

                THEN("does not move child")
                {
                    REQUIRE(childBounds->Position() == Point3D{});
                }
            }

            WHEN("moving child in direction")
            {
                const auto direction = Angle3D
                {
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>)),
                    dataGeneration.Random<Angle3D::Value>(TestFramework::Range(0.0f, pi<Angle3D::Value>))
                };
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(childBounds.ID(), direction, amount));

                THEN("does not move child")
                {
                    REQUIRE(childBounds->Position() == Point3D{});
                }
            }
        }
    }
}