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

            auto asset = reliquary->Do(Arca::Create<Atmos::Asset::Audio>{name, std::unique_ptr<Resource::Audio>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }

        WHEN("creating image asset with null data")
        {
            auto name = dataGeneration.Random<String>();
            auto columns = dataGeneration.Random<ImageGridSize::Dimension>();
            auto rows = dataGeneration.Random<ImageGridSize::Dimension>();

            auto asset = reliquary->Do(Arca::Create<Image>{
                name,
                std::unique_ptr<Resource::Image>{},
                ImageSize{},
                ImageGridSize{ columns, rows }});

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

            auto asset = reliquary->Do(Arca::Create<Material>{
                name,
                MaterialType::Image,
                Arca::Index<Shader>{},
                Arca::Index<Shader>{}});

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

            auto asset = reliquary->Do(Arca::Create<Script>{name, std::unique_ptr<Resource::Script>{}});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }

        WHEN("creating shader asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<Shader>{name, std::unique_ptr<Resource::Shader>{}, ""});

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }
        }
    }
}