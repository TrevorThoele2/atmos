#include <catch.hpp>

#include "GridRegionTests.h"

#include <Atmos/GridRegion.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/StringUtility.h>
#include <Atmos/FindGridRegionsByBox.h>
#include <Arca/Create.h>

#include "DerivedEngine.h"
#include "MockSurfaceResource.h"
#include <Atmos/SpatialAlgorithms.h>

using namespace Atmos;
using namespace Spatial;

SCENARIO_METHOD(GridRegionTestsFixture, "grid regions", "[render]")
{
    GIVEN("setup engine with field")
    {
        Logging::Logger logger(Logging::Severity::Verbose);
        DerivedEngine engine(logger);

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(
            fieldOrigin,
            *engine.mockAssetResourceManager,
            *engine.mockAudioManager,
            *engine.mockInputManager,
            *engine.mockGraphicsManager,
            *engine.mockTextManager,
            *engine.mockScriptManager,
            *engine.worldManager,
            Size2D{
                std::numeric_limits<Size2D::Value>::max(),
                std::numeric_limits<Size2D::Value>::max() },
                *engine.mockWindow,
                engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{} });

        WHEN("creating grid region")
        {
            auto positions = std::vector<Grid::Point>
            {
                Grid::Point { 0, 0 },
                Grid::Point { 1, 0 },
                Grid::Point { 0, 1 },
                Grid::Point { 1, 1 },
            };
            auto gridRegion = fieldReliquary.Do(Arca::Create<GridRegion>{
                std::unordered_set<Grid::Point>(positions.begin(), positions.end()), 0, materialAsset});

            std::vector<Arca::RelicID> gridRegionIDs = { gridRegion.ID() };

            WHEN("querying for all gridRegions with box 2D")
            {
                auto box = AxisAlignedBox2D(
                    Point2D{},
                    Size2D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedGridRegionIDs = fieldReliquary.Do(FindGridRegionsByBox(box));

                THEN("all gridRegions returned")
                {
                    for (auto i = 0; i < 1; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedGridRegionIDs.begin(),
                            returnedGridRegionIDs.end(),
                            [i, &gridRegionIDs](const Arca::RelicID& id)
                            {
                                return gridRegionIDs[i] == id;
                            }));
                    }
                }
            }

            WHEN("querying for all gridRegions with box 3D")
            {
                auto box = AxisAlignedBox3D(
                    Point3D{},
                    Size3D
                    {
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1,
                        std::numeric_limits<AxisAlignedBox2D::Coordinate>::max() / 2 - 1
                    });

                auto returnedGridRegionIDs = fieldReliquary.Do(FindGridRegionsByBox(box));

                THEN("all gridRegions returned")
                {
                    for (auto i = 0; i < 1; ++i)
                    {
                        REQUIRE(std::any_of(
                            returnedGridRegionIDs.begin(),
                            returnedGridRegionIDs.end(),
                            [i, &gridRegionIDs](const Arca::RelicID& id)
                            {
                                return gridRegionIDs[i] == id;
                            }));
                    }
                }
            }
        }
    }
}