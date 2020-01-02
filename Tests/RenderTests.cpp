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
            field.Reliquary().Create<StaticMaterialView>(positions[0]);
            field.Reliquary().Create<StaticMaterialView>(positions[1]);
            field.Reliquary().Create<StaticMaterialView>(positions[2]);

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
                        return entry.position.x == positions[0].x && entry.position.y == positions[0].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[1].x && entry.position.y == positions[1].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[2].x && entry.position.y == positions[2].y;
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
            field.Reliquary().Create<DynamicMaterialView>(positions[0]);
            field.Reliquary().Create<DynamicMaterialView>(positions[1]);
            field.Reliquary().Create<DynamicMaterialView>(positions[2]);

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
                        return entry.position.x == positions[0].x && entry.position.y == positions[0].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[1].x && entry.position.y == positions[1].y;
                    }));

                REQUIRE(std::any_of(
                    materialRenders.begin(),
                    materialRenders.end(),
                    [&positions](const MaterialRender& entry)
                    {
                        return entry.position.x == positions[2].x && entry.position.y == positions[2].y;
                    }));
            }
        }
    }
}