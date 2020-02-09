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
        RegisterFieldTypes(fieldOrigin, *engine.TheGlobalReliquary());
        World::Field field(0, fieldOrigin.Actualize());

        WHEN("creating static material views and starting execution")
        {
            auto positions = std::vector<Position3D>
            {
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                }
            };
            auto sizes = std::vector<Size2D>
            {
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                }
            };
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<StaticMaterialView>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->renderer.materialRenders;
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
            auto positions = std::vector<Position3D>
            {
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                },
                Position3D
                {
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000)),
                    dataGeneration.Random<Position3D::Value>(TestFramework::Range<Position3D::Value>(-1000, 1000))
                }
            };
            auto sizes = std::vector<Size2D>
            {
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                },
                Size2D
                {
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000)),
                    dataGeneration.Random<Size2D::Value>(TestFramework::Range<Size2D::Value>(1, 1000))
                }
            };
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[0], sizes[0]);
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[1], sizes[1]);
            field.Reliquary().Do<Arca::Create<DynamicMaterialView>>(positions[2], sizes[2]);

            engine.UseField(std::move(field));
            engine.StartExecution();

            THEN("all materials rendered in graphics manager")
            {
                auto& materialRenders = engine.mockGraphicsManager->renderer.materialRenders;
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