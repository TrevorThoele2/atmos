#include <catch.hpp>

#include "JavaScriptRenderTests.h"

#include <Atmos/JavaScriptDynamicText.h>
#include <Atmos/JavaScriptGridRegion.h>
#include <Atmos/JavaScriptLine.h>
#include <Atmos/JavaScriptImageCore.h>
#include <Atmos/JavaScriptViewSlice.h>

#include <Arca/OpenRelic.h>
#include <Arca/Create.h>
#include <Inscription/Json.h>

SCENARIO_METHOD(JavaScriptRenderTestsFixture, "running render JavaScript scripts", "[script][javascript][logging]")
{
    GIVEN("script that changes color")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Arca::OpenRelic>());

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Render::RenderCore>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.ChangeColor.typeName, { id, to: { alpha: 127, red: 127, green: 127, blue: 127 }});
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(shard->color.alpha == 127);
                REQUIRE(shard->color.red == 127);
                REQUIRE(shard->color.green == 127);
                REQUIRE(shard->color.blue == 127);
            }
        }
    }

    GIVEN("script that changes image core")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Arca::OpenRelic>());

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Render::ImageCore>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId, assetIndex: number): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.ChangeImageCore.typeName, { id, asset: null, assetIndex });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID(), 123 },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(shard->assetIndex == 123);
            }
        }
    }

    GIVEN("script that moves grid region")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Atmos::Render::GridRegion>());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId, z: number): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.MoveGridRegion.typeName, { id, points: null, z });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID(), 123 },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(relic->z == 123);
            }
        }
    }

    GIVEN("script that moves line")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Atmos::Render::Line>());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId, z: number): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.MoveLine.typeName, { id, points: null, z });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID(), 123.0f },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(relic->z == 123.0f);
            }
        }
    }

    GIVEN("script that changes material asset")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Atmos::Render::GridRegion>());

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.ChangeMaterialAsset.typeName, { id, to: null });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(!relic->renderCore->material);
            }
        }
    }

    GIVEN("script that changes text core")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Atmos::Render::DynamicText>(
            String(""),
            Arca::Index<Atmos::Asset::Font>(),
            Arca::Index<Atmos::Asset::Material>(),
            0.0f,
            false,
            false,
            Atmos::Render::Color(),
            Atmos::Spatial::Point3D(),
            Atmos::Spatial::Scalers2D(),
            0.0f));

        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId, string: string): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.ChangeTextCore.typeName, { id, string, font: null, wrapWidth: null, bold: null, italics: null });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID(), String("abc") },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                REQUIRE(relic->String() == "abc");
            }
        }
    }

    GIVEN("script that changes view slice")
    {
        const auto relic = fieldReliquary->Do(Arca::Create<Atmos::Render::DynamicText>(
            String(""),
            Arca::Index<Atmos::Asset::Font>(),
            Arca::Index<Atmos::Asset::Material>(),
            0.0f,
            false,
            false,
            Atmos::Render::Color(),
            Atmos::Spatial::Point3D(),
            Atmos::Spatial::Scalers2D(),
            0.0f));

        const auto shard = fieldReliquary->Do(Arca::Create<Atmos::Render::ViewSlice>(relic.ID()));

        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    Atmos.Reliquary.send(Atmos.Traits.Render.ChangeViewSlice.typeName, { id, to: {center:{x:0, y:0},size:{width:0,height:0}} });
    return {
        done: true
    };
};)V0G0N",
            { relic.ID() },
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);
            }
        }
    }

    GIVEN("script that finds images by box")
    {
        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    const found = Atmos.Reliquary.send(Atmos.Traits.Render.FindImagesByBox.typeName, { box: {center:{x:0,y:0,z:0},size:{width:0,height:0,depth:0}}, space: Atmos.Spatial.Space.World });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                std::vector<Arca::RelicID> resultFound;
                Inscription::Json::FromString(resultFound, result);

                REQUIRE(resultFound.empty());
            }
        }
    }

    GIVEN("script that finds lines by box")
    {
        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    const found = Atmos.Reliquary.send(Atmos.Traits.Render.FindLinesByBox.typeName, { box: {center:{x:0,y:0,z:0},size:{width:0,height:0,depth:0}} });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                std::vector<Arca::RelicID> resultFound;
                Inscription::Json::FromString(resultFound, result);

                REQUIRE(resultFound.empty());
            }
        }
    }

    GIVEN("script that finds grid regions by box")
    {
        CompileAndCreateScript(
            "basic_script.ts",
            R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    const found = Atmos.Reliquary.send(Atmos.Traits.Render.FindGridRegionsByBox.typeName, { box: {center:{x:0,y:0,z:0},size:{width:0,height:0,depth:0}} });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);

                const auto result = std::get<String>(std::get<Variant>(this->finishes[0].result));

                std::vector<Arca::RelicID> resultFound;
                Inscription::Json::FromString(resultFound, result);

                REQUIRE(resultFound.empty());
            }
        }
    }

    GIVEN("script that finds text base size")
    {
        CompileAndCreateScript(
            "basic_script.ts",
R"V0G0N(import { Atmos } from "./atmos";

export const main = (id: Atmos.RelicId): Atmos.Result => {
    const found = Atmos.Reliquary.send(Atmos.Traits.Render.TextBaseSize.typeName, { string: "", font: 1, wrapWidth: 0, bold: false, italics: false });
    return {
        done: true,
        value: JSON.stringify(found, (key, value) => typeof value === 'bigint' ? value.toString() : value)
    };
};)V0G0N",
            {},
            *fieldReliquary);

        WHEN("working reliquary")
        {
            fieldReliquary->Do(Work{});

            THEN("script has executed")
            {
                REQUIRE(this->finishes.size() == 1);
            }
        }
    }
}