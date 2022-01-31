#include <catch.hpp>

#include "JavaScriptSpatialTests.h"

#include <Atmos/Bounds.h>
#include <Atmos/ToWorldPoint2D.h>
#include <Atmos/ToWorldPoint3D.h>
#include <Atmos/SpatialAlgorithms.h>
#include <Inscription/Json.h>
#include <Arca/Create.h>
#include <Arca/OpenRelic.h>

auto JavaScriptSpatialDistanceTestsFixture::Point2D() -> ScenarioT
{
    return CreateScenario(Spatial::Point2D{ 10, 20 }, Spatial::Point2D{ 30, 40 });
}

auto JavaScriptSpatialDistanceTestsFixture::Point3D() -> ScenarioT
{
    return CreateScenario(Spatial::Point3D{ 10, 20, 30 }, Spatial::Point3D{ 40, 50, 60 });
}

auto JavaScriptSpatialContainsTestsFixture::BoxPoint2D() -> ScenarioT
{
    return CreateScenario(Spatial::AxisAlignedBox2D{ Spatial::Point2D{ 10, 20 }, Spatial::Size2D{ 30, 40 } }, Spatial::Point2D{ 50, 60 });
}

auto JavaScriptSpatialContainsTestsFixture::BoxBox2D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{ 10, 20 }, Spatial::Size2D{ 30, 40 } },
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{ 50, 60 }, Spatial::Size2D{ 70, 80 } });
}

auto JavaScriptSpatialContainsTestsFixture::BoxPoint3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{ 10, 20, 30 }, Spatial::Size3D{ 40, 50, 60 } },
        Spatial::Point3D{ 70, 80, 90 });
}

auto JavaScriptSpatialContainsTestsFixture::BoxBox3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{ 10, 20, 30 }, Spatial::Size3D{ 40, 50, 60 } },
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{ 70, 80, 90 }, Spatial::Size3D{ 100, 110, 120 } });
}

auto JavaScriptSpatialIntersectsTestsFixture::Box2D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{10, 20}, Spatial::Size2D{30, 40} },
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{50, 60}, Spatial::Size2D{70, 80} });
}

auto JavaScriptSpatialIntersectsTestsFixture::Box3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{10, 20, 30}, Spatial::Size3D{40, 50, 60} },
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{70, 80, 90}, Spatial::Size3D{100, 110, 120} });
}

auto JavaScriptSpatialEnvelopeTestsFixture::Box2D() -> ScenarioT
{
    return CreateScenario(
        std::vector<Spatial::AxisAlignedBox2D>
        {
            Spatial::AxisAlignedBox2D{ Spatial::Point2D{10, 20}, Spatial::Size2D{30, 40} },
            Spatial::AxisAlignedBox2D{ Spatial::Point2D{50, 60}, Spatial::Size2D{70, 80} },
            Spatial::AxisAlignedBox2D{ Spatial::Point2D{90, 100}, Spatial::Size2D{110, 120} }
        });
}

auto JavaScriptSpatialEnvelopeTestsFixture::Box3D() -> ScenarioT
{
    return CreateScenario(
        std::vector<Spatial::AxisAlignedBox3D>
        {
            Spatial::AxisAlignedBox3D{ Spatial::Point3D{10, 20, 30}, Spatial::Size3D{40, 50, 60} },
            Spatial::AxisAlignedBox3D{ Spatial::Point3D{70, 80, 90}, Spatial::Size3D{100, 110} },
            Spatial::AxisAlignedBox3D{ Spatial::Point3D{120, 130, 140}, Spatial::Size3D{150, 160, 170} }
        });
}

auto JavaScriptSpatialClampTestsFixture::Box2D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{10, 20}, Spatial::Size2D{30, 40} },
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{50, 60}, Spatial::Size2D{70, 80} });
}

auto JavaScriptSpatialClampTestsFixture::Box3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{10, 20, 30}, Spatial::Size3D{40, 50, 60} },
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{70, 80, 90}, Spatial::Size3D{100, 110, 120} });
}

auto JavaScriptSpatialScaleOfTestsFixture::Box2D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{10, 20}, Spatial::Size2D{30, 40} },
        Spatial::AxisAlignedBox2D{ Spatial::Point2D{50, 60}, Spatial::Size2D{70, 80} });
}

auto JavaScriptSpatialScaleByTestsFixture::Size2D() -> ScenarioT
{
    return CreateScenario(
        Spatial::Size2D{ 10, 20 },
        Spatial::Scalers2D{ 30, 40 });
}

auto JavaScriptSpatialScaleByTestsFixture::Size3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::Size3D{ 10, 20, 30 },
        Spatial::Scalers3D{ 40, 50, 60 });
}

auto JavaScriptSpatialScaleOfTestsFixture::Box3D() -> ScenarioT
{
    return CreateScenario(
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{10, 20, 30}, Spatial::Size3D{40, 50, 60} },
        Spatial::AxisAlignedBox3D{ Spatial::Point3D{70, 80, 90}, Spatial::Size3D{100, 110, 120} });
}

auto JavaScriptSpatialAdditionTestsFixture::Point2D() -> ScenarioT
{
    return CreateScenario(Spatial::Point2D{ 10, 20 }, Spatial::Point2D{ 30, 40 });
}

auto JavaScriptSpatialAdditionTestsFixture::Point3D() -> ScenarioT
{
    return CreateScenario(Spatial::Point3D{ 10, 20, 30 }, Spatial::Point3D{ 40, 50, 60 });
}

auto JavaScriptSpatialSubtractionTestsFixture::Point2D() -> ScenarioT
{
    return CreateScenario(Spatial::Point2D{ 10, 20 }, Spatial::Point2D{ 30, 40 });
}

auto JavaScriptSpatialSubtractionTestsFixture::Point3D() -> ScenarioT
{
    return CreateScenario(Spatial::Point3D{ 10, 20, 30 }, Spatial::Point3D{ 40, 50, 60 });
}

SCENARIO_METHOD(JavaScriptSpatialTestsFixture, "running spatial JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("script that moves bounds")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Arca::OpenRelic>());

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Spatial::Bounds>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Spatial.MoveBounds.typeName, { id, to: { x: 123, y: 456, z: 789 }});
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(shard->Position().x == Approx(123));
                REQUIRE(shard->Position().y == Approx(456));
                REQUIRE(shard->Position().z == Approx(789));
            }
        }
    }

    GIVEN("script that rotates bounds")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Arca::OpenRelic>());

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Spatial::Bounds>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Spatial.RotateBounds.typeName, { id, to: 123 });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(shard->Rotation() == Approx(123));
            }
        }
    }

    GIVEN("script that scales bounds")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Arca::OpenRelic>());

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Spatial::Bounds>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Spatial.ScaleBounds.typeName, { id, to: { x: 123, y: 456 }});
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(shard->Scalers().x == Approx(123));
                REQUIRE(shard->Scalers().y == Approx(456));
            }
        }
    }

    GIVEN("script that returns ToWorldPoint2D")
    {
        auto point = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (inputPoint: string): Atmos.Result => {
    const outputPoint = Atmos.Reliquary.send(Atmos.Traits.Spatial.ToWorldPoint2D.typeName, { from: JSON.parse(inputPoint) });
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point2D outputPoint;
                Inscription::Json::FromString(outputPoint, result);

                const auto expectedPoint = fieldReliquary->Do(Atmos::Spatial::ToWorldPoint2D{ point });

                REQUIRE(outputPoint.x == Approx(expectedPoint.x));
                REQUIRE(outputPoint.y == Approx(expectedPoint.y));
            }
        }
    }

    GIVEN("script that returns ToWorldPoint3D")
    {
        auto point = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto z = dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (inputPoint: string, z: number): Atmos.Result => {
    const outputPoint = Atmos.Reliquary.send(Atmos.Traits.Spatial.ToWorldPoint3D.typeName, { from: JSON.parse(inputPoint), z });
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point), z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point3D outputPoint;
                Inscription::Json::FromString(outputPoint, result);

                const auto expectedPoint = fieldReliquary->Do(Atmos::Spatial::ToWorldPoint3D{ point, z });

                REQUIRE(outputPoint.x == Approx(expectedPoint.x));
                REQUIRE(outputPoint.y == Approx(expectedPoint.y));
                REQUIRE(outputPoint.z == Approx(expectedPoint.z));
            }
        }
    }

    GIVEN("script that returns IsBetween of Point2D")
    {
        auto one = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto check = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto two = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (oneJson: string, checkJson: string, twoJson: string): Atmos.Result => {
    const one = JSON.parse(oneJson);
    const check = JSON.parse(checkJson);
    const two = JSON.parse(twoJson);
    const isBetween = Atmos.Spatial.isBetween(one, check, two);
    return {
        done: true,
        value: isBetween
    };
};)V0G0N",
            { Inscription::Json::ToString(one), Inscription::Json::ToString(check), Inscription::Json::ToString(two) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto expectedDistance = Spatial::IsBetween(one, check, two);

                REQUIRE(result == Approx(expectedDistance));
            }
        }
    }

    GIVEN("script that returns AngleOf of Point2D")
    {
        auto side1 = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto middle = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto side2 = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (oneJson: string, checkJson: string, twoJson: string): Atmos.Result => {
    const one = JSON.parse(oneJson);
    const check = JSON.parse(checkJson);
    const two = JSON.parse(twoJson);
    const isBetween = Atmos.Spatial.isBetween(one, check, two);
    return {
        done: true,
        value: isBetween
    };
};)V0G0N",
            { Inscription::Json::ToString(side1), Inscription::Json::ToString(middle), Inscription::Json::ToString(side2) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto isBetween = Spatial::IsBetween(side1, middle, side2);

                REQUIRE(result == Approx(isBetween));
            }
        }
    }

    GIVEN("script that returns IsBetween of Point2D")
    {
        auto side1 = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto middle = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        auto side2 = Spatial::Point2D
        {
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100)),
            dataGeneration.Random<Spatial::Point2D::Value>(TestFramework::Range<Spatial::Point2D::Value>(-100, 100))
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (oneJson: string, checkJson: string, twoJson: string): Atmos.Result => {
    const one = JSON.parse(oneJson);
    const check = JSON.parse(checkJson);
    const two = JSON.parse(twoJson);
    const isBetween = Atmos.Spatial.isBetween(one, check, two);
    return {
        done: true,
        value: isBetween
    };
};)V0G0N",
            { Inscription::Json::ToString(side1), Inscription::Json::ToString(middle), Inscription::Json::ToString(side2) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto isBetween = Spatial::IsBetween(side1, middle, side2);

                REQUIRE(result == Approx(isBetween));
            }
        }
    }

    GIVEN("script that returns Cell of Size2D")
    {
        auto column = 2;
        auto row = 3;
        auto size = Spatial::Size2D{ 10, 20 };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (column: number, row: number, sizeJson: string): Atmos.Result => {
    const size = JSON.parse(sizeJson);
    const cell = Atmos.Spatial.cell(column, row, size);
    return {
        done: true,
        value: JSON.stringify(cell, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { column, row, Inscription::Json::ToString(size) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::AxisAlignedBox2D resultBox;
                Inscription::Json::FromString(resultBox, result);

                const auto expectation = Spatial::Cell(column, row, size);

                REQUIRE(resultBox == expectation);
            }
        }
    }

    GIVEN("script that returns Cell of Size3D")
    {
        auto column = 2;
        auto row = 3;
        auto depth = 4;
        auto size = Spatial::Size3D{ 10, 20, 30 };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (column: number, row: number, depth: number, sizeJson: string): Atmos.Result => {
    const size = JSON.parse(sizeJson);
    const cell = Atmos.Spatial.cell(column, row, depth, size);
    return {
        done: true,
        value: JSON.stringify(cell, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { column, row, depth, Inscription::Json::ToString(size) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::AxisAlignedBox3D resultBox;
                Inscription::Json::FromString(resultBox, result);

                const auto expectation = Spatial::Cell(column, row, depth, size);

                REQUIRE(resultBox == expectation);
            }
        }
    }

    GIVEN("script that returns ToPoint2D of Point3D")
    {
        auto point = dataGeneration.RandomStack<Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string): Atmos.Result => {
    const inputPoint = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.toPoint2D(inputPoint);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point2D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint2D(point);

                REQUIRE(resultPoint.x == Approx(expectation.x));
                REQUIRE(resultPoint.y == Approx(expectation.y));
            }
        }
    }

    GIVEN("script that returns ToPoint2D of grid point")
    {
        auto point = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string): Atmos.Result => {
    const inputPoint = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.Grid.toPoint2D(inputPoint);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point2D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint2D(point);

                REQUIRE(resultPoint == expectation);
            }
        }
    }

    GIVEN("script that returns ToPoint2D of angle and distance")
    {
        auto angle = dataGeneration.Random<Spatial::Angle2D>();
        auto distance = dataGeneration.Random<float>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (angle: number, distance: number): Atmos.Result => {
    const outputPoint = Atmos.Spatial.toPoint2D(angle, distance);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { angle, distance },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point2D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint2D(angle, distance);

                REQUIRE(resultPoint == expectation);
            }
        }
    }

    GIVEN("script that returns ToPoint3D of Point2D")
    {
        auto point = dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string, z: number): Atmos.Result => {
    const inputPoint = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.toPoint3D(inputPoint, z);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point), z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point3D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint3D(point, z);

                REQUIRE(resultPoint.x == Approx(expectation.x));
                REQUIRE(resultPoint.y == Approx(expectation.y));
                REQUIRE(resultPoint.z == Approx(expectation.z));
            }
        }
    }

    GIVEN("script that returns ToPoint3D of grid point")
    {
        auto point = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto z = dataGeneration.Random<Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string, z: number): Atmos.Result => {
    const inputPoint = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.Grid.toPoint3D(inputPoint, z);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point), z },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point3D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint3D(point, z);

                REQUIRE(resultPoint.x == Approx(expectation.x));
                REQUIRE(resultPoint.y == Approx(expectation.y));
                REQUIRE(resultPoint.z == Approx(expectation.z));
            }
        }
    }

    GIVEN("script that returns ToPoint3D of angle and distance")
    {
        auto angle = Spatial::Angle3D{ 10, 20 };
        auto distance = 40.0f;

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (angleJson: string, distance: number): Atmos.Result => {
    const inputAngle = JSON.parse(angleJson);
    const outputPoint = Atmos.Spatial.toPoint3D(inputAngle, distance);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(angle), distance },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Point3D resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::ToPoint3D(angle, distance);

                REQUIRE(resultPoint.x == Approx(expectation.x));
                REQUIRE(resultPoint.y == Approx(expectation.y));
                REQUIRE(resultPoint.z == Approx(expectation.z));
            }
        }
    }

    GIVEN("script that returns ToDegrees of Angle2D")
    {
        auto angle = 50.0f;

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (angle: number): Atmos.Result => {
    const outputAngle = Atmos.Spatial.toDegrees(angle);
    return {
        done: true,
        value: outputAngle
    };
};)V0G0N",
            { angle },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<double>(std::get<Variant>(this->finishes[0].result));

                const auto expectation = Spatial::ToDegrees(angle);

                REQUIRE(result == Approx(expectation));
            }
        }
    }

    GIVEN("script that returns ToAxisAlignedBox2D")
    {
        auto left = dataGeneration.Random<float>();
        auto top = dataGeneration.Random<float>();
        auto right = dataGeneration.Random<float>();
        auto bottom = dataGeneration.Random<float>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (left: number, top: number, right: number, bottom: number): Atmos.Result => {
    const box = Atmos.Spatial.toAxisAlignedBox2D(left, top, right, bottom);
    return {
        done: true,
        value: JSON.stringify(box, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { left, top, right, bottom },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::AxisAlignedBox2D resultBox;
                Inscription::Json::FromString(resultBox, result);

                const auto expectation = Spatial::ToAxisAlignedBox2D(left, top, right, bottom);

                REQUIRE(resultBox == expectation);
            }
        }
    }

    GIVEN("script that returns ToAxisAlignedBox3D")
    {
        auto left = dataGeneration.Random<float>();
        auto top = dataGeneration.Random<float>();
        auto farZ = dataGeneration.Random<float>();
        auto right = dataGeneration.Random<float>();
        auto bottom = dataGeneration.Random<float>();
        auto nearZ = dataGeneration.Random<float>();

        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (left: number, top: number, farZ: number, right: number, bottom: number, nearZ: number): Atmos.Result => {
    const box = Atmos.Spatial.toAxisAlignedBox3D(left, top, farZ, right, bottom, nearZ);
    return {
        done: true,
        value: JSON.stringify(box, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { left, top, farZ, right, bottom, nearZ },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::AxisAlignedBox3D resultBox;
                Inscription::Json::FromString(resultBox, result);

                const auto expectation = Spatial::ToAxisAlignedBox3D(left, top, farZ, right, bottom, nearZ);

                REQUIRE(resultBox == expectation);
            }
        }
    }

    GIVEN("script that returns grid distance")
    {
        auto point1 = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();
        auto point2 = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (point1Json: string, point2Json: string): Atmos.Result => {
    const point1 = JSON.parse(point1Json);
    const point2 = JSON.parse(point2Json);
    const distance = Atmos.Spatial.Grid.distance(point1, point2);
    return {
        done: true,
        value: distance
    };
};)V0G0N",
            { Inscription::Json::ToString(point1), Inscription::Json::ToString(point2) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<Spatial::Grid::Point::Value>(std::get<Variant>(this->finishes[0].result));

                const auto expectation = Spatial::Grid::Distance(point1, point2);

                REQUIRE(result == expectation);
            }
        }
    }

    GIVEN("script that returns grid contains point")
    {
        auto box = Spatial::Grid::AxisAlignedBox
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>(),
            dataGeneration.RandomStack<Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>(),
        };

        auto point = dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (boxJson: string, pointJson: string): Atmos.Result => {
    const box = JSON.parse(boxJson);
    const point = JSON.parse(pointJson);
    const contains = Atmos.Spatial.Grid.contains(box, point);
    return {
        done: true,
        value: contains
    };
};)V0G0N",
            { Inscription::Json::ToString(box), Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto expectation = Spatial::Grid::Contains(box, point);

                REQUIRE(result == expectation);
            }
        }
    }

    GIVEN("script that returns grid contains box")
    {
        auto box1 = Spatial::Grid::AxisAlignedBox
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>(),
            dataGeneration.RandomStack<Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>(),
        };

        auto box2 = Spatial::Grid::AxisAlignedBox
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>(),
            dataGeneration.RandomStack<Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>(),
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (box1Json: string, box2Json: string): Atmos.Result => {
    const box1 = JSON.parse(box1Json);
    const box2 = JSON.parse(box2Json);
    const contains = Atmos.Spatial.Grid.contains(box1, box2);
    return {
        done: true,
        value: contains
    };
};)V0G0N",
            { Inscription::Json::ToString(box1), Inscription::Json::ToString(box2) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto expectation = Spatial::Grid::Contains(box1, box2);

                REQUIRE(result == expectation);
            }
        }
    }

    GIVEN("script that returns grid intersects")
    {
        auto box1 = Spatial::Grid::AxisAlignedBox
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>(),
            dataGeneration.RandomStack<Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>(),
        };

        auto box2 = Spatial::Grid::AxisAlignedBox
        {
            dataGeneration.RandomStack<Spatial::Grid::Point, Spatial::Grid::Point::Value, Spatial::Grid::Point::Value>(),
            dataGeneration.RandomStack<Spatial::Grid::Size, Spatial::Grid::Size::Value, Spatial::Grid::Size::Value>(),
        };

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (box1Json: string, box2Json: string): Atmos.Result => {
    const box1 = JSON.parse(box1Json);
    const box2 = JSON.parse(box2Json);
    const intersects = Atmos.Spatial.Grid.intersects(box1, box2);
    return {
        done: true,
        value: intersects
    };
};)V0G0N",
            { Inscription::Json::ToString(box1), Inscription::Json::ToString(box2) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(this->finishes[0].result));

                const auto expectation = Spatial::Grid::Intersects(box1, box2);

                REQUIRE(result == expectation);
            }
        }
    }

    GIVEN("script that returns ToGridPoint of Point2D")
    {
        auto point = dataGeneration.RandomStack<Spatial::Point2D, Spatial::Point2D::Value, Spatial::Point2D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string): Atmos.Result => {
    const point = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.Grid.toPoint(point);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Grid::Point resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::Grid::ToPoint(point);

                REQUIRE(resultPoint == expectation);
            }
        }
    }

    GIVEN("script that returns ToGridPoint of Point2D")
    {
        auto point = dataGeneration.RandomStack<Spatial::Point3D, Spatial::Point3D::Value, Spatial::Point3D::Value, Spatial::Point3D::Value>();

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (pointJson: string): Atmos.Result => {
    const point = JSON.parse(pointJson);
    const outputPoint = Atmos.Spatial.Grid.toPoint(point);
    return {
        done: true,
        value: JSON.stringify(outputPoint, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            { Inscription::Json::ToString(point) },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                Spatial::Grid::Point resultPoint;
                Inscription::Json::FromString(resultPoint, result);

                const auto expectation = Spatial::Grid::ToPoint(point);

                REQUIRE(resultPoint == expectation);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialDistanceTestsFixture, "running spatial Distance JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("script that returns Distance of Point2D")
    {
        auto [parameters, expectation] = GENERATE(Point2D(), Point3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (point1Json: string, point2Json: string): Atmos.Result => {
    const point1 = JSON.parse(point1Json);
    const point2 = JSON.parse(point2Json);
    const distance = Atmos.Spatial.distance(point1, point2);
    return {
        done: true,
        value: distance
    };
};)V0G0N",
            { parameters },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<double>(std::get<Variant>(this->finishes[0].result));

                REQUIRE(result == Approx(expectation));
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialContainsTestsFixture, "running spatial Contains JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(BoxPoint2D(), BoxBox2D(), BoxPoint3D(), BoxBox3D());

        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const contains = Atmos.Spatial.contains(left, right);
    return {
        done: true,
        value: contains
    };
};)V0G0N",
            parameters,
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));

                REQUIRE(result == expectation);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialIntersectsTestsFixture, "running spatial Intersects JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Box2D(), Box3D());

        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const intersects = Atmos.Spatial.intersects(left, right);
    return {
        done: true,
        value: intersects
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<bool>(std::get<Variant>(finishes[0].result));

                REQUIRE(result == expectation);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialEnvelopeTestsFixture, "running spatial Envelope JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Box2D(), Box3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (boxesJson: string): Atmos.Result => {
    const boxes = JSON.parse(boxesJson);
    const envelope = Atmos.Spatial.envelope(boxes);
    return {
        done: true,
        value: JSON.stringify(envelope, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialClampTestsFixture, "running spatial Clamp JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Box2D(), Box3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const clamp = Atmos.Spatial.clamp(left, right);
    return {
        done: true,
        value: JSON.stringify(clamp, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialScaleByTestsFixture, "running spatial ScaleBy JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Size2D(), Size3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const scaleBy = Atmos.Spatial.scaleBy(left, right);
    return {
        done: true,
        value: JSON.stringify(scaleBy, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialScaleOfTestsFixture, "running spatial ScaleOf JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Box2D(), Box3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const scaleOf = Atmos.Spatial.scaleOf(left, right);
    return {
        done: true,
        value: JSON.stringify(scaleOf, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialAdditionTestsFixture, "running spatial + JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Point2D(), Point3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const addition = Atmos.Spatial.add(left, right);
    return {
        done: true,
        value: JSON.stringify(addition, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}

SCENARIO_METHOD(JavaScriptSpatialSubtractionTestsFixture, "running spatial - JavaScript scripts", "[script][javascript][spatial]")
{
    GIVEN("setup")
    {
        auto [parameters, expectation] = GENERATE(Point2D(), Point3D());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (leftJson: string, rightJson: string): Atmos.Result => {
    const left = JSON.parse(leftJson);
    const right = JSON.parse(rightJson);
    const subtraction = Atmos.Spatial.subtract(left, right);
    return {
        done: true,
        value: JSON.stringify(subtraction, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
parameters,
*fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("returned JSON is correct")
            {
                REQUIRE(finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(finishes[0].result));

                expectation(result);
            }
        }
    }
}