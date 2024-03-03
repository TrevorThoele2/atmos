#include "catch.hpp"
#include "AssetTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>

SCENARIO_METHOD(AssetTestsFixture, "assets")
{
    GIVEN("registered reliquary")
    {
        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterTypes(reliquaryOrigin);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating audio asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<AudioAsset>{name, std::unique_ptr<AudioAssetData>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }

        WHEN("creating image asset with null data")
        {
            auto name = dataGeneration.Random<String>();
            auto columns = dataGeneration.Random<ImageAsset::GridDimension>();
            auto rows = dataGeneration.Random<ImageAsset::GridDimension>();

            auto asset = reliquary->Do(Arca::Create<ImageAsset>{
                name,
                std::unique_ptr<ImageAssetData>{},
                columns,
                rows});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("asset has width 0")
            {
                REQUIRE(asset->Width() == 0);
            }

            THEN("asset has height 0")
            {
                REQUIRE(asset->Height() == 0);
            }

            THEN("asset has columns")
            {
                REQUIRE(asset->Columns() == columns);
            }

            THEN("asset has rows")
            {
                REQUIRE(asset->Rows() == rows);
            }
        }

        WHEN("creating material asset with null shaders")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<MaterialAsset>{
                name,
                Arca::Index<ShaderAsset>{},
                Arca::Index<ShaderAsset>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("asset has empty vertex shader")
            {
                REQUIRE(asset->VertexShader().Get() == nullptr);
            }

            THEN("asset has empty fragment shader")
            {
                REQUIRE(asset->FragmentShader().Get() == nullptr);
            }
        }

        WHEN("creating script asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<ScriptAsset>{name, std::unique_ptr<ScriptAssetData>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }

        WHEN("creating shader asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<ShaderAsset>{name, std::unique_ptr<ShaderAssetData>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }
    }
}