#include <catch.hpp>

#include "GridRegionRenderingTests.h"

#include <Atmos/GridRegion.h>
#include <Atmos/TypeRegistration.h>
#include <Atmos/StringUtility.h>
#include <Atmos/Camera.h>
#include <Atmos/MainSurface.h>
#include <Atmos/GridCellSize.h>

#include "DerivedEngine.h"
#include "MockSurfaceResource.h"

using namespace Atmos;

SCENARIO_METHOD(GridRegionRenderingTestsFixture, "rendering grid regions", "[render]")
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
            Spatial::Size2D {
                std::numeric_limits<Spatial::Size2D::Value>::max(),
                std::numeric_limits<Spatial::Size2D::Value>::max() },
            *engine.mockWindow,
            engine.Logger());
        World::Field field(0, fieldOrigin.Actualize());

        auto& fieldReliquary = field.Reliquary();

        const auto mainSurface = fieldReliquary.Find<MainSurface>();
        auto mainSurfaceImplementation = mainSurface->Resource<MockSurfaceResource>();

        auto materialAsset = fieldReliquary.Do(Arca::Create<Asset::Material> {
            String{}, std::vector<Asset::Material::Pass>{} });

        const auto camera = fieldReliquary.Find<Camera>();

        const auto cameraLeft = camera->Sides().Left();
        const auto cameraTop = camera->Sides().Top();

        WHEN("creating grid region")
        {
            auto positions = std::vector<Spatial::Grid::Point>
            {
                Spatial::Grid::Point { 0, 0 },
                Spatial::Grid::Point { 1, 0 },
                Spatial::Grid::Point { 0, 1 },
                Spatial::Grid::Point { 1, 1 },
            };
            auto gridRegion = fieldReliquary.Do(Arca::Create<GridRegion>{
                std::unordered_set<Spatial::Grid::Point>(positions.begin(), positions.end()), 0, materialAsset});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("region rendered in graphics manager")
                {
                    auto& regionRenders = mainSurfaceImplementation->regionRenders;
                    REQUIRE(regionRenders.size() == 1);

                    REQUIRE(std::any_of(
                        regionRenders.begin(),
                        regionRenders.end(),
                        [&positions, cameraLeft, cameraTop](const RegionRender& entry)
                        {
                            std::vector<Spatial::Point2D> expectedVertices;
                            std::vector<Spatial::Grid::Point> expectedVertexGridPositions =
                            {
                                positions[0],
                                positions[1],
                                positions[2],
                                positions[2],
                                positions[1],
                                positions[3]
                            };
                            for (auto& position : expectedVertexGridPositions)
                            {
                                expectedVertices.push_back(Spatial::Point2D{
                                    position.x * Atmos::Spatial::Grid::CellSize<float> -cameraLeft,
                                    position.y * Atmos::Spatial::Grid::CellSize<float> -cameraTop });
                            }

                            std::vector<std::uint16_t> expectedIndices = {
                                0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5 };

                            return expectedVertices == entry.mesh.vertices && expectedIndices == entry.mesh.indices;
                        }));
                }
            }

            WHEN("starting engine execution, destroying grid region, then starting execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                fieldReliquary.Do(Arca::Destroy<GridRegion>{ gridRegion.ID() });

                THEN("regions were rendered only once")
                {
                    auto& regionRenders = mainSurfaceImplementation->regionRenders;
                    REQUIRE(regionRenders.size() == 1);
                }
            }
        }

        WHEN("creating region without material")
        {
            auto positions = std::unordered_set<Spatial::Grid::Point>
            {
                Spatial::Grid::Point { 0, 0 },
                Spatial::Grid::Point { 1, 0 },
                Spatial::Grid::Point { 0, 1 },
                Spatial::Grid::Point { 1, 1 },
            };
            fieldReliquary.Do(Arca::Create<GridRegion>{ positions, 0, Arca::Index<Asset::Material>{}});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no lines rendered in graphics manager")
                {
                    auto& regionRenders = mainSurfaceImplementation->regionRenders;
                    REQUIRE(regionRenders.empty());
                }
            }
        }

        WHEN("creating region without positions")
        {
            auto positions = std::unordered_set<Spatial::Grid::Point> {};
            fieldReliquary.Do(Arca::Create<GridRegion>{ positions, 0, materialAsset});

            WHEN("starting engine execution")
            {
                engine.UseField(std::move(field), {}, std::filesystem::current_path() / "Assets.dat");
                engine.StartExecution();

                THEN("no lines rendered in graphics manager")
                {
                    auto& regionRenders = mainSurfaceImplementation->regionRenders;
                    REQUIRE(regionRenders.empty());
                }
            }
        }
    }
}