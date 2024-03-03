#include <catch.hpp>

#include "RenderTests.h"

#include <Atmos/StaticMaterialView.h>
#include <Atmos/DynamicMaterialView.h>
#include <Atmos/TypeRegistration.h>

#include "DerivedEngine.h"

SCENARIO_METHOD(RenderTestsFixture, "rendering material views")
{
    GIVEN("setup engine with field")
    {
        DerivedEngine engine;
        engine.Setup();

        auto fieldOrigin = Arca::ReliquaryOrigin();
        RegisterFieldTypes(fieldOrigin, *engine.globalReliquary);
        World::Field field(0, fieldOrigin.Actualize());

        WHEN("creating static material views and starting execution")
        {
            auto positions = dataGeneration.RandomStackGroup<
                Position3D,
                Position3D::Value,
                Position3D::Value,
                Position3D::Value>(3);
            auto sizes = dataGeneration.RandomStackGroup<
                Size2D,
                Size2D::Value,
                Size2D::Value>(3);
            field.Reliquary().Create<StaticMaterialView>(positions[0], sizes[0]);
            field.Reliquary().Create<StaticMaterialView>(positions[1], sizes[1]);
            field.Reliquary().Create<StaticMaterialView>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->materialRenders;
                REQUIRE(materialRenders.size() == 3);

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return std::any_of(
                            positions.begin(),
                            positions.end(),
                            [&entry](const Position3D& position)
                            {
                                return entry.position == position;
                            });
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&sizes](const MaterialRender& entry)
                    {
                        return std::any_of(
                            sizes.begin(),
                            sizes.end(),
                            [&entry](const Size2D& size)
                            {
                                return entry.size == size;
                            });
                    }));
            }
        }

        WHEN("creating dynamic material views and starting execution")
        {
            auto positions = dataGeneration.RandomStackGroup<
                Position3D,
                Position3D::Value,
                Position3D::Value,
                Position3D::Value>(3);
            auto sizes = dataGeneration.RandomStackGroup<
                Size2D,
                Size2D::Value,
                Size2D::Value>(3);
            field.Reliquary().Create<DynamicMaterialView>(positions[0], sizes[0]);
            field.Reliquary().Create<DynamicMaterialView>(positions[1], sizes[1]);
            field.Reliquary().Create<DynamicMaterialView>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->materialRenders;
                REQUIRE(materialRenders.size() == 3);

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return std::any_of(
                            positions.begin(),
                            positions.end(),
                            [&entry](const Position3D& position)
                            {
                                return entry.position == position;
                            });
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&sizes](const MaterialRender& entry)
                    {
                        return std::any_of(
                            sizes.begin(),
                            sizes.end(),
                            [&entry](const Size2D& size)
                            {
                                return entry.size == size;
                            });
                    }));
            }
        }
    }
}