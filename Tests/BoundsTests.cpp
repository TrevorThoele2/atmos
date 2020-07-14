#include "catch.hpp"
#include "BoundsTests.h"

#include "SignalListener.h"

#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/ProcessedLog.h>

SCENARIO_METHOD(BoundsTestsFixture, "bounds")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        auto created = reliquary->Do(Arca::Create<Arca::OpenRelic>());
        auto bounds = created->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});

        WHEN("moving bounds to position")
        {
            const auto position = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            reliquary->Do(MoveBoundsTo(bounds.ID(), position));

            THEN("bounds are moved")
            {
                REQUIRE(bounds->Position() != Point3D());
                REQUIRE(bounds->Position() == position);
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

            THEN("bounds are moved")
            {
                REQUIRE(bounds->Position() != initialPosition);
                REQUIRE(bounds->Position() == Point3D
                    {
                        initialPosition.x + delta.x,
                        initialPosition.y + delta.y,
                        initialPosition.z + delta.z
                    });
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
            const auto direction = Direction(Direction::Value(
                dataGeneration.Random<std::underlying_type_t<Direction::Value>>(
                    TestFramework::Range<std::underlying_type_t<Direction::Value>>(
                        Direction::Left, Direction::ZNearer))));
            const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

            reliquary->Do(MoveBoundsTo(bounds.ID(), initialPosition));
            reliquary->Do(MoveBoundsDirection(bounds.ID(), direction, amount));

            THEN("bounds are moved")
            {
                auto position = initialPosition;

                REQUIRE(bounds->Position() != initialPosition);
                switch(direction.Get())
                {
                case Direction::Value::Left:
                    position.x -= amount;
                    break;
                case Direction::Value::Up:
                    position.y -= amount;
                    break;
                case Direction::Value::ZFarther:
                    position.z -= amount;
                    break;
                case Direction::Value::Right:
                    position.x += amount;
                    break;
                case Direction::Value::Down:
                    position.y += amount;
                    break;
                case Direction::Value::ZNearer:
                    position.z += amount;
                    break;
                default:
                    FAIL_CHECK("unknown direction");
                }
                REQUIRE(bounds->Position() == position);
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
            const auto angle = dataGeneration.Random<Angle>(TestFramework::Range(-1000.0f, 1000.0f));

            reliquary->Do(RotateBounds(bounds.ID(), angle));

            THEN("bounds are rotated")
            {
                REQUIRE(bounds->Rotation() != Angle{});
                REQUIRE(bounds->Rotation() == angle);
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

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
            parent->Create<Bounds>(parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = child->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});
            auto relativeBounds = child->Create<RelativeBounds>(relativePosition);

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
                    REQUIRE(newChildPosition.x == checkPosition.x);
                    REQUIRE(newChildPosition.y == checkPosition.y);
                    REQUIRE(newChildPosition.z == checkPosition.z);
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + newRelativePosition.x,
                        parentPosition.y + newRelativePosition.y,
                        parentPosition.z + newRelativePosition.z
                    };

                    REQUIRE(newChildPosition.x == checkPosition.x);
                    REQUIRE(newChildPosition.y == checkPosition.y);
                    REQUIRE(newChildPosition.z == checkPosition.z);
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
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + relativePosition.x + childDelta.x,
                        parentPosition.y + relativePosition.y + childDelta.y,
                        parentPosition.z + relativePosition.z + childDelta.z,
                    };

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == checkPosition.x);
                    REQUIRE(childPosition.y == checkPosition.y);
                    REQUIRE(childPosition.z == checkPosition.z);
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + newRelativePosition.x,
                        parentPosition.y + newRelativePosition.y,
                        parentPosition.z + newRelativePosition.z
                    };

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == checkPosition.x);
                    REQUIRE(childPosition.y == checkPosition.y);
                    REQUIRE(childPosition.z == checkPosition.z);
                }
            }

            WHEN("moving child bounds in direction")
            {
                const auto direction = Direction(Direction::Value(
                    dataGeneration.Random<std::underlying_type_t<Direction::Value>>(
                        TestFramework::Range<std::underlying_type_t<Direction::Value>>(
                            Direction::Left, Direction::ZNearer))));
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(childBounds.ID(), direction, amount));
                const auto newRelativePosition = relativeBounds->delta;
                const auto childPosition = childBounds->Position();

                THEN("is at correct position")
                {
                    auto checkPosition = Point3D
                    {
                        parentPosition.x + relativePosition.x,
                        parentPosition.y + relativePosition.y,
                        parentPosition.z + relativePosition.z,
                    };

                    switch (direction.Get())
                    {
                    case Direction::Value::Left:
                        checkPosition.x -= amount;
                        break;
                    case Direction::Value::Up:
                        checkPosition.y -= amount;
                        break;
                    case Direction::Value::ZFarther:
                        checkPosition.z -= amount;
                        break;
                    case Direction::Value::Right:
                        checkPosition.x += amount;
                        break;
                    case Direction::Value::Down:
                        checkPosition.y += amount;
                        break;
                    case Direction::Value::ZNearer:
                        checkPosition.z += amount;
                        break;
                    default:
                        FAIL_CHECK("unknown direction");
                    }

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == checkPosition.x);
                    REQUIRE(childPosition.y == checkPosition.y);
                    REQUIRE(childPosition.z == checkPosition.z);
                }

                THEN("parent position and new relative position adds up to child position")
                {
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + newRelativePosition.x,
                        parentPosition.y + newRelativePosition.y,
                        parentPosition.z + newRelativePosition.z
                    };

                    REQUIRE(childPosition != Point3D());
                    REQUIRE(childPosition.x == checkPosition.x);
                    REQUIRE(childPosition.y == checkPosition.y);
                    REQUIRE(childPosition.z == checkPosition.z);
                }
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds initialization")
{
    GIVEN("registered reliquary with parent")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        Spatial::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        const auto parentPosition = Point3D
        {
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
            dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
        };

        auto parent = reliquary->Do(Arca::Create<Arca::OpenRelic>());
        parent->Create<Bounds>(parentPosition, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});

        WHEN("creating child relic with relative bounds last")
        {
            const auto relativePosition = Point3D
            {
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
            };

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = child->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});
            child->Create<RelativeBounds>(relativePosition);
            const auto childPosition = childBounds->Position();

            THEN("is at correct position")
            {
                const auto checkPosition = Point3D
                {
                    parentPosition.x + relativePosition.x,
                    parentPosition.y + relativePosition.y,
                    parentPosition.z + relativePosition.z,
                };

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == checkPosition.x);
                REQUIRE(childPosition.y == checkPosition.y);
                REQUIRE(childPosition.z == checkPosition.z);
            }

            THEN("parent position and relative position adds up to child position")
            {
                const auto checkPosition = Point3D
                {
                    parentPosition.x + relativePosition.x,
                    parentPosition.y + relativePosition.y,
                    parentPosition.z + relativePosition.z
                };

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == checkPosition.x);
                REQUIRE(childPosition.y == checkPosition.y);
                REQUIRE(childPosition.z == checkPosition.z);
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
            child->Create<RelativeBounds>(relativePosition);
            auto childBounds = child->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});
            const auto childPosition = childBounds->Position();

            THEN("is at correct position")
            {
                const auto checkPosition = Point3D
                {
                    parentPosition.x + relativePosition.x,
                    parentPosition.y + relativePosition.y,
                    parentPosition.z + relativePosition.z,
                };

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == checkPosition.x);
                REQUIRE(childPosition.y == checkPosition.y);
                REQUIRE(childPosition.z == checkPosition.z);
            }

            THEN("parent position and relative position adds up to child position")
            {
                const auto checkPosition = Point3D
                {
                    parentPosition.x + relativePosition.x,
                    parentPosition.y + relativePosition.y,
                    parentPosition.z + relativePosition.z
                };

                REQUIRE(childPosition != Point3D());
                REQUIRE(childPosition.x == checkPosition.x);
                REQUIRE(childPosition.y == checkPosition.y);
                REQUIRE(childPosition.z == checkPosition.z);
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds children")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

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
            auto parentBounds = parent->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});

            auto child = reliquary->Do(Arca::CreateChild<Arca::OpenRelic>(parent));
            auto childBounds = child->Create<Bounds>(Point3D{}, Size2D{ 1, 1 }, Scalers2D{ 1, 1 }, Angle{});
            child->Create<RelativeBounds>(relativePosition);

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
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + relativePosition.x,
                        parentPosition.y + relativePosition.y,
                        parentPosition.z + relativePosition.z
                    };

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

                THEN("child bounds are moved")
                {
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + relativePosition.x,
                        parentPosition.y + relativePosition.y,
                        parentPosition.z + relativePosition.z
                    };

                    REQUIRE(childBounds->Position() != Point3D());
                    REQUIRE(childBounds->Position() == checkPosition);
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
                const auto direction = Direction(Direction::Value(
                    dataGeneration.Random<std::underlying_type_t<Direction::Value>>(
                        TestFramework::Range<std::underlying_type_t<Direction::Value>>(
                            Direction::Left, Direction::ZNearer))));
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsTo(parentBounds.ID(), initialPosition));
                reliquary->Do(MoveBoundsDirection(parentBounds.ID(), direction, amount));
                const auto parentPosition = parentBounds->Position();

                THEN("child bounds are moved")
                {
                    const auto checkPosition = Point3D
                    {
                        parentPosition.x + relativePosition.x,
                        parentPosition.y + relativePosition.y,
                        parentPosition.z + relativePosition.z
                    };

                    REQUIRE(childBounds->Position() != Point3D());
                    REQUIRE(childBounds->Position() == checkPosition);
                }
            }
        }
    }
}

SCENARIO_METHOD(BoundsTestsFixture, "relative bounds errors")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        Spatial::RegisterTypes(reliquaryOrigin);
        Logging::RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        auto logs = SignalListener<Atmos::Logging::ProcessedLog>(*reliquary);

        WHEN("creating relic with relative bounds without parent")
        {
            auto relic = reliquary->Do(Arca::Create<Arca::OpenRelic>());
            auto bounds = relic->Create<Bounds>();
            relic->Create<RelativeBounds>();

            WHEN("moving relic to position")
            {
                const auto position = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(bounds.ID(), position));

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relics with " + Arca::TypeFor<RelativeBounds>().name + " must be parented.");
                }

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

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relics with " + Arca::TypeFor<RelativeBounds>().name + " must be parented.");
                }

                THEN("does not move")
                {
                    REQUIRE(bounds->Position() == Point3D{});
                }
            }

            WHEN("moving relic in direction")
            {
                const auto direction = Direction(Direction::Value(
                    dataGeneration.Random<std::underlying_type_t<Direction::Value>>(
                        TestFramework::Range<std::underlying_type_t<Direction::Value>>(
                            Direction::Left, Direction::ZNearer))));
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(bounds.ID(), direction, amount));

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relics with " + Arca::TypeFor<RelativeBounds>().name + " must be parented.");
                }

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
            auto childBounds = child->Create<Bounds>();
            child->Create<RelativeBounds>();

            WHEN("moving child to position")
            {
                const auto position = Point3D
                {
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f)),
                    dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f))
                };

                reliquary->Do(MoveBoundsTo(childBounds.ID(), position));

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relic parents of " + Arca::TypeFor<RelativeBounds>().name + " must have a Bounds.");
                }

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

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relic parents of " + Arca::TypeFor<RelativeBounds>().name + " must have a Bounds.");
                }

                THEN("does not move child")
                {
                    REQUIRE(childBounds->Position() == Point3D{});
                }
            }

            WHEN("moving child in direction")
            {
                const auto direction = Direction(Direction::Value(
                    dataGeneration.Random<std::underlying_type_t<Direction::Value>>(
                        TestFramework::Range<std::underlying_type_t<Direction::Value>>(
                            Direction::Left, Direction::ZNearer))));
                const auto amount = dataGeneration.Random<Point3D::Value>(TestFramework::Range(-1000.0f, 1000.0f));

                reliquary->Do(MoveBoundsDirection(childBounds.ID(), direction, amount));

                THEN("logs error")
                {
                    REQUIRE(!logs.Executions().empty());
                    REQUIRE(logs.Executions()[0].originalMessage
                        == "Relic parents of " + Arca::TypeFor<RelativeBounds>().name + " must have a Bounds.");
                }

                THEN("does not move child")
                {
                    REQUIRE(childBounds->Position() == Point3D{});
                }
            }
        }
    }
}