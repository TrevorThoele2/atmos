#include "catch.hpp"
#include "AssetTests.h"

#include <Arca/ReliquaryOrigin.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/MappedAssets.h>
#include <Atmos/NullAssetResourceManager.h>
#include <Arca/Create.h>
#include <Arca/Assign.h>

SCENARIO_METHOD(AssetTestsFixture, "assets", "[asset]")
{
    GIVEN("registered reliquary")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        Resource::NullManager assetResourceManager(logger);

        Arca::ReliquaryOrigin reliquaryOrigin;

        RegisterTypes(reliquaryOrigin, assetResourceManager);

        auto reliquary = reliquaryOrigin.Actualize();

        WHEN("creating audio asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<Atmos::Asset::Audio>{name, std::unique_ptr<Resource::Audio>{}});

            auto mappedAssets = reliquary->Find<Atmos::Asset::Mapped<Atmos::Asset::Audio>>();

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("mapped assets contains entry")
            {
                REQUIRE(mappedAssets->Exists(name));
                REQUIRE(mappedAssets->Find(name) == asset);
            }

            WHEN("reassigning asset")
            {
                auto newName = dataGeneration.Random<String>();

                reliquary->Do(Arca::AssignMove<Atmos::Asset::Audio>{
                    asset.ID(), newName, std::unique_ptr<Resource::Audio>{}});

                THEN("mapped asset is new asset")
                {
                    REQUIRE(mappedAssets->Exists(newName));
                    REQUIRE(mappedAssets->Find(newName) == asset);
                    REQUIRE(!mappedAssets->Exists(name));
                    REQUIRE(mappedAssets->Find(name) == nullptr);
                }
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
                ImageGridSize{ columns, rows }});

            auto mappedAssets = reliquary->Find<Atmos::Asset::Mapped<Atmos::Asset::Image>>();

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("asset has width 0")
            {
                REQUIRE(asset->Width() == Approx(0));
            }

            THEN("asset has height 0")
            {
                REQUIRE(asset->Height() == Approx(0));
            }

            THEN("asset has columns")
            {
                REQUIRE(asset->Columns() == columns);
            }

            THEN("asset has rows")
            {
                REQUIRE(asset->Rows() == rows);
            }

            THEN("mapped assets contains entry")
            {
                REQUIRE(mappedAssets->Exists(name));
                REQUIRE(mappedAssets->Find(name) == asset);
            }

            WHEN("reassigning asset")
            {
                auto newName = dataGeneration.Random<String>();

                reliquary->Do(Arca::AssignMove<Atmos::Asset::Image>{
                    asset.ID(),
                    newName,
                    std::unique_ptr<Resource::Image>{},
                    ImageGridSize{ columns, rows }});

                THEN("mapped asset is new asset")
                {
                    REQUIRE(mappedAssets->Size() == 1);
                    REQUIRE(mappedAssets->Exists(newName));
                    REQUIRE(mappedAssets->Find(newName) == asset);
                    REQUIRE(!mappedAssets->Exists(name));
                    REQUIRE(mappedAssets->Find(name) == nullptr);
                }
            }
        }
        
        WHEN("creating script asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<Atmos::Asset::Script>{name, std::unique_ptr<Resource::Script>{}});

            auto mappedAssets = reliquary->Find<Atmos::Asset::Mapped<Atmos::Asset::Script>>();

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("mapped assets contains entry")
            {
                REQUIRE(mappedAssets->Exists(name));
                REQUIRE(mappedAssets->Find(name) == asset);
            }

            WHEN("reassigning asset")
            {
                auto newName = dataGeneration.Random<String>();

                reliquary->Do(Arca::AssignMove<Atmos::Asset::Script>{
                    asset.ID(),
                    newName,
                    std::unique_ptr<Resource::Script>{}});

                THEN("mapped asset is new asset")
                {
                    REQUIRE(mappedAssets->Size() == 1);
                    REQUIRE(mappedAssets->Exists(newName));
                    REQUIRE(mappedAssets->Find(newName) == asset);
                    REQUIRE(!mappedAssets->Exists(name));
                    REQUIRE(mappedAssets->Find(name) == nullptr);
                }
            }
        }

        WHEN("creating shader asset with null data")
        {
            auto name = dataGeneration.Random<String>();

            auto asset = reliquary->Do(Arca::Create<Shader>{name, std::unique_ptr<Resource::Shader>{}});

            auto mappedAssets = reliquary->Find<Atmos::Asset::Mapped<Atmos::Asset::Shader>>();

            THEN("asset has name")
            {
                REQUIRE(asset->Name() == name);
            }

            THEN("mapped assets contains entry")
            {
                REQUIRE(mappedAssets->Exists(name));
                REQUIRE(mappedAssets->Find(name) == asset);
            }

            WHEN("reassigning asset")
            {
                auto newName = dataGeneration.Random<String>();

                reliquary->Do(Arca::AssignMove<Atmos::Asset::Shader>{
                    asset.ID(),
                    newName,
                    std::unique_ptr<Resource::Shader>{}});

                THEN("mapped asset is new asset")
                {
                    REQUIRE(mappedAssets->Size() == 1);
                    REQUIRE(mappedAssets->Exists(newName));
                    REQUIRE(mappedAssets->Find(newName) == asset);
                    REQUIRE(!mappedAssets->Exists(name));
                    REQUIRE(mappedAssets->Find(name) == nullptr);
                }
            }
        }
    }
}