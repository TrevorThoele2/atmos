#include "catch.hpp"

#include "OctreeTests.h"

SCENARIO_METHOD(OctreeTestsFixture, "empty octree", "[octree]")
{
    GIVEN("octree at origin")
    {
        using Octree = Octree<int, std::string>;
        Octree octree;

        WHEN("finding all within maximum bounds")
        {
            THEN("returns empty")
            {
                const auto maxDimension = std::numeric_limits<Size3D::Value>::max();
                const auto bounds = AxisAlignedBox3D
                {
                    Position3D { 0, 0, 0 },
                    Size3D { maxDimension, maxDimension, maxDimension }
                };

                auto allWithin = octree.AllWithin(bounds);

                REQUIRE(allWithin.empty());
            }
        }

        WHEN("removing a nonexistent object")
        {
            THEN("returns false")
            {
                auto returned = octree.Remove(dataGeneration.Random<int>(), Octree::Bounds());

                REQUIRE(returned == false);
            }
        }

        WHEN("moving object")
        {
            THEN("does not throw")
            {
                REQUIRE_NOTHROW(octree.Move(
                    dataGeneration.Random<int>(),
                    dataGeneration.Random<std::string>(),
                    Octree::Bounds(),
                    Octree::Bounds()));
            }
        }

        WHEN("querying total bounds")
        {
            THEN("is centered at origin and 2 grid cells in size")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D());
                REQUIRE(totalBounds.Size() == Size3D
                    {
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2
                    });
            }
        }

        WHEN("querying size")
        {
            THEN("is empty")
            {
                REQUIRE(octree.Size() == 0);
            }
        }
    }

    GIVEN("octree at different center")
    {
        using Octree = Octree<int, std::string>;
        Octree octree(Position3D { 100, 100, 100 });

        WHEN("querying total bounds")
        {
            THEN("is centered at position and 2 grid cells in size")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D { 100, 100, 100 });
                REQUIRE(totalBounds.Size() == Size3D
                    {
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2
                    });
            }
        }
    }
}

SCENARIO_METHOD(OctreeTestsFixture, "single object octree", "[octree]")
{
    GIVEN("octree at origin and grid-fitting bounds")
    {
        using Octree = Octree<int, std::string>;
        Octree octree;

        static constexpr auto gridCellSize = CellSize<Position3D::Value>;
        static constexpr auto halfGridCellSize = gridCellSize / 2;

        const auto movedToBounds = Octree::Bounds
        {
            Position3D { 1000, 1000, 1000 },
            Size3D{ gridCellSize, gridCellSize, gridCellSize }
        };

        const auto bounds = GENERATE(
            AxisAlignedBox3D
            {
                Position3D { -halfGridCellSize, -halfGridCellSize, -halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { -halfGridCellSize, -halfGridCellSize, halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { -halfGridCellSize, halfGridCellSize, -halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { -halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { halfGridCellSize, -halfGridCellSize, -halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { halfGridCellSize, -halfGridCellSize, halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { halfGridCellSize, halfGridCellSize, -halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { halfGridCellSize, halfGridCellSize, halfGridCellSize },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            }
        );

        WHEN("adding object")
        {
            const auto addedID = dataGeneration.Random<int>();
            const auto addedValue = dataGeneration.Random<std::string>();

            octree.Add(addedID, addedValue, bounds);

            THEN("finding all within same bounds returns object")
            {
                auto allWithin = octree.AllWithin(bounds);

                REQUIRE(allWithin.size() == 1);
                REQUIRE(allWithin[0]->id == addedID);
                REQUIRE(allWithin[0]->value == addedValue);
            }

            THEN("total bounds is centered at origin and 2 grid cells in size")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D());
                REQUIRE(totalBounds.Size() == Size3D
                    {
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2
                    });
            }

            THEN("contains an object")
            {
                REQUIRE(octree.Size() == 1);
            }

            WHEN("removing object")
            {
                octree.Remove(addedID, bounds);

                THEN("finding all within same bounds returns empty container")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("is empty")
                {
                    REQUIRE(octree.Size() == 0);
                }
            }

            WHEN("moving object to same bounds")
            {
                octree.Move(addedID, addedValue, bounds, bounds);

                THEN("finding all within same bounds returns object")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.size() == 1);
                    REQUIRE(allWithin[0]->id == addedID);
                    REQUIRE(allWithin[0]->value == addedValue);
                }

                THEN("contains an object")
                {
                    REQUIRE(octree.Size() == 1);
                }
            }

            WHEN("moving object a further distance away")
            {
                octree.Move(addedID, addedValue, bounds, movedToBounds);

                THEN("finding all within added bounds returns empty container")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("finding all within moved to bounds returns object")
                {
                    auto allWithin = octree.AllWithin(movedToBounds);

                    REQUIRE(allWithin.size() == 1);
                    REQUIRE(allWithin[0]->id == addedID);
                    REQUIRE(allWithin[0]->value == addedValue);
                }

                THEN("contains an object")
                {
                    REQUIRE(octree.Size() == 1);
                }
            }
        }
    }

    GIVEN("octree at origin and offset bounds")
    {
        using Octree = Octree<int, std::string>;
        Octree octree;

        static constexpr auto gridCellSize = CellSize<Position3D::Value>;

        const auto movedToBounds = Octree::Bounds
        {
            Position3D { 1000, 1000, 1000 },
            Size3D{ gridCellSize, gridCellSize, gridCellSize }
        };

        const auto bounds = GENERATE(
            AxisAlignedBox3D
            {
                Position3D { 0, 0, 0 },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            },
            AxisAlignedBox3D
            {
                Position3D { 32, 32, 32 },
                Size3D { gridCellSize, gridCellSize, gridCellSize }
            }
        );

        WHEN("adding object")
        {
            const auto addedID = dataGeneration.Random<int>();
            const auto addedValue = dataGeneration.Random<std::string>();

            octree.Add(addedID, addedValue, bounds);

            THEN("finding all within same bounds returns object")
            {
                auto allWithin = octree.AllWithin(bounds);

                REQUIRE(allWithin.size() == 1);
                REQUIRE(allWithin[0]->id == addedID);
                REQUIRE(allWithin[0]->value == addedValue);
            }

            THEN("total bounds is centered at origin and contains bounds")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D());
                REQUIRE(totalBounds.Contains(bounds));
            }

            THEN("contains an object")
            {
                REQUIRE(octree.Size() == 1);
            }

            WHEN("removing object")
            {
                octree.Remove(addedID, bounds);

                THEN("finding all within same bounds returns empty container")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("is empty")
                {
                    REQUIRE(octree.Size() == 0);
                }
            }

            WHEN("moving object to same bounds")
            {
                octree.Move(addedID, addedValue, bounds, bounds);

                THEN("finding all within same bounds returns object")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.size() == 1);
                    REQUIRE(allWithin[0]->id == addedID);
                    REQUIRE(allWithin[0]->value == addedValue);
                }

                THEN("contains an object")
                {
                    REQUIRE(octree.Size() == 1);
                }
            }

            WHEN("moving object a further distance away")
            {
                octree.Move(addedID, addedValue, bounds, movedToBounds);

                THEN("finding all within added bounds returns empty container")
                {
                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("finding all within moved to bounds returns object")
                {
                    auto allWithin = octree.AllWithin(movedToBounds);

                    REQUIRE(allWithin.size() == 1);
                    REQUIRE(allWithin[0]->id == addedID);
                    REQUIRE(allWithin[0]->value == addedValue);
                }

                THEN("contains an object")
                {
                    REQUIRE(octree.Size() == 1);
                }
            }
        }
    }

    GIVEN("octree at origin")
    {
        using Octree = Octree<int, std::string>;
        Octree octree;

        static constexpr auto gridCellSize = CellSize<Position3D::Value>;

        const auto bounds = AxisAlignedBox3D
        {
            Position3D { 1000, 1000, 1000 },
            Size3D { gridCellSize, gridCellSize, gridCellSize }
        };

        WHEN("adding object")
        {
            const auto addedID = dataGeneration.Random<int>();
            const auto addedValue = dataGeneration.Random<std::string>();

            octree.Add(addedID, addedValue, bounds);

            THEN("finding object at opposite point returns empty container")
            {
                auto allWithin = octree.AllWithin(AxisAlignedBox3D
                    {
                        Position3D { -1000, -1000, -1000 },
                        Size3D { gridCellSize, gridCellSize, gridCellSize }
                    });

                REQUIRE(allWithin.empty());
            }

            THEN("finding object outside octree returns empty container")
            {
                auto allWithin = octree.AllWithin(AxisAlignedBox3D
                    {
                        Position3D { 10000, 10000, 10000 },
                        Size3D { gridCellSize, gridCellSize, gridCellSize }
                    });

                REQUIRE(allWithin.empty());
            }
        }

        WHEN("adding object at origin")
        {
            const auto addedID = dataGeneration.Random<int>();
            const auto addedValue = dataGeneration.Random<std::string>();

            octree.Add(addedID, addedValue, Octree::Bounds
                {
                    Position3D { 0, 0, 0 },
                    Size3D { gridCellSize, gridCellSize, gridCellSize }
                });

            THEN("finding object outside octree returns empty container")
            {
                auto allWithin = octree.AllWithin(AxisAlignedBox3D
                    {
                        Position3D { 10000, 10000, 10000 },
                        Size3D { gridCellSize, gridCellSize, gridCellSize }
                    });

                REQUIRE(allWithin.empty());
            }
        }
    }
}

SCENARIO_METHOD(OctreeTestsFixture, "multiple objects octree", "[octree]")
{
    GIVEN("octree at origin")
    {
        using Octree = Octree<int, std::string>;
        Octree octree;

        WHEN("adding an object per initial placement")
        {
            auto ids = dataGeneration.RandomGroup<int>(8);
            auto values = dataGeneration.RandomGroup<std::string>(8);

            const auto gridCellSize = CellSize<Position3D::Value>;
            const auto halfGridCellSize = gridCellSize / 2;

            auto addObject = [&octree, &ids, &values, gridCellSize](size_t i, const Position3D& center)
            {
                octree.Add(
                    ids[i],
                    values[i],
                    AxisAlignedBox3D
                    {
                        center,
                        Size3D { gridCellSize, gridCellSize, gridCellSize }
                    });
            };

            addObject(0, Position3D{ -halfGridCellSize, -halfGridCellSize, -halfGridCellSize });
            addObject(1, Position3D{ -halfGridCellSize, -halfGridCellSize, halfGridCellSize });
            addObject(2, Position3D{ -halfGridCellSize, halfGridCellSize, -halfGridCellSize });
            addObject(3, Position3D{ -halfGridCellSize, halfGridCellSize, halfGridCellSize });
            addObject(4, Position3D{ halfGridCellSize, -halfGridCellSize, -halfGridCellSize });
            addObject(5, Position3D{ halfGridCellSize, -halfGridCellSize, halfGridCellSize });
            addObject(6, Position3D{ halfGridCellSize, halfGridCellSize, -halfGridCellSize });
            addObject(7, Position3D{ halfGridCellSize, halfGridCellSize, halfGridCellSize });

            THEN("finding all within initial bounds returns all")
            {
                const auto bounds = AxisAlignedBox3D
                {
                    Position3D { 0, 0, 0 },
                    Size3D { gridCellSize * 2, gridCellSize * 2, gridCellSize * 2 }
                };

                auto allWithin = octree.AllWithin(bounds);

                REQUIRE(allWithin.size() == 8);
                for (size_t i = 0; i < 8; ++i)
                {
                    REQUIRE(std::any_of(
                        allWithin.begin(),
                        allWithin.end(),
                        [&ids, &values, i](const Octree::Object* object)
                        {
                            return object->id == ids[i] && object->value == values[i];
                        }));
                }
            }

            THEN("total bounds is centered at origin and 2 grid cells in size")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D());
                REQUIRE(totalBounds.Size() == Size3D
                    {
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2,
                        CellSize<Position3D::Value> * 2
                    });
            }

            THEN("has size of 8")
            {
                REQUIRE(octree.Size() == 8);
            }

            WHEN("removing all objects")
            {
                auto removeObject = [&octree, &ids, gridCellSize](size_t i, const Position3D& center)
                {
                    octree.Remove(
                        ids[i],
                        AxisAlignedBox3D
                        {
                            center,
                            Size3D { gridCellSize, gridCellSize, gridCellSize }
                        });
                };

                removeObject(0, Position3D{ -halfGridCellSize, -halfGridCellSize, -halfGridCellSize });
                removeObject(1, Position3D{ -halfGridCellSize, -halfGridCellSize, halfGridCellSize });
                removeObject(2, Position3D{ -halfGridCellSize, halfGridCellSize, -halfGridCellSize });
                removeObject(3, Position3D{ -halfGridCellSize, halfGridCellSize, halfGridCellSize });
                removeObject(4, Position3D{ halfGridCellSize, -halfGridCellSize, -halfGridCellSize });
                removeObject(5, Position3D{ halfGridCellSize, -halfGridCellSize, halfGridCellSize });
                removeObject(6, Position3D{ halfGridCellSize, halfGridCellSize, -halfGridCellSize });
                removeObject(7, Position3D{ halfGridCellSize, halfGridCellSize, halfGridCellSize });

                THEN("finding all within same bounds returns empty container")
                {
                    const auto bounds = AxisAlignedBox3D
                    {
                        Position3D { 0, 0, 0 },
                        Size3D { gridCellSize * 2, gridCellSize * 2, gridCellSize * 2 }
                    };

                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("total bounds is centered at origin and 2 grid cells in size")
                {
                    auto totalBounds = octree.TotalBounds();
                    REQUIRE(totalBounds.Center() == Position3D());
                    REQUIRE(totalBounds.Size() == Size3D
                        {
                            CellSize<Position3D::Value> * 2,
                            CellSize<Position3D::Value> * 2,
                            CellSize<Position3D::Value> * 2
                        });
                }

                THEN("is empty")
                {
                    REQUIRE(octree.Size() == 0);
                }
            }
        }

        WHEN("adding objects at most extreme corners")
        {
            auto ids = dataGeneration.RandomGroup<int>(8);
            auto values = dataGeneration.RandomGroup<std::string>(8);

            const auto gridCellSize = CellSize<Position3D::Value>;
            const auto distance = std::numeric_limits<Position3D::Value>::max() - gridCellSize;

            auto addObject = [&octree, &ids, &values, gridCellSize](size_t i, const Position3D& center)
            {
                octree.Add(
                    ids[i],
                    values[i],
                    AxisAlignedBox3D
                    {
                        center,
                        Size3D { gridCellSize, gridCellSize, gridCellSize }
                    });
            };

            addObject(0, Position3D{ -distance, -distance, -distance });
            addObject(1, Position3D{ -distance, -distance, distance });
            addObject(2, Position3D{ -distance, distance, -distance });
            addObject(3, Position3D{ -distance, distance, distance });
            addObject(4, Position3D{ distance, -distance, -distance });
            addObject(5, Position3D{ distance, -distance, distance });
            addObject(6, Position3D{ distance, distance, -distance });
            addObject(7, Position3D{ distance, distance, distance });

            THEN("finding all within initial bounds returns all")
            {
                const auto bounds = AxisAlignedBox3D
                {
                    Position3D { 0, 0, 0 },
                    Size3D { gridCellSize * 2, gridCellSize * 2, gridCellSize * 2 }
                };

                auto allWithin = octree.AllWithin(bounds);

                REQUIRE(allWithin.size() == 8);
                for (size_t i = 0; i < 8; ++i)
                {
                    REQUIRE(std::any_of(
                        allWithin.begin(),
                        allWithin.end(),
                        [&ids, &values, i](const Octree::Object* object)
                        {
                            return object->id == ids[i] && object->value == values[i];
                        }));
                }
            }

            THEN("total bounds is centered at origin and at least distance in size")
            {
                auto totalBounds = octree.TotalBounds();
                REQUIRE(totalBounds.Center() == Position3D());
                REQUIRE(totalBounds.Size().width >= distance);
                REQUIRE(totalBounds.Size().height >= distance);
                REQUIRE(totalBounds.Size().depth >= distance);
            }

            THEN("has size of 8")
            {
                REQUIRE(octree.Size() == 8);
            }

            WHEN("removing all objects")
            {
                auto removeObject = [&octree, &ids, gridCellSize](size_t i, const Position3D& center)
                {
                    octree.Remove(
                        ids[i],
                        AxisAlignedBox3D
                        {
                            center,
                            Size3D { gridCellSize, gridCellSize, gridCellSize }
                        });
                };

                removeObject(0, Position3D{ -distance, -distance, -distance });
                removeObject(1, Position3D{ -distance, -distance, distance });
                removeObject(2, Position3D{ -distance, distance, -distance });
                removeObject(3, Position3D{ -distance, distance, distance });
                removeObject(4, Position3D{ distance, -distance, -distance });
                removeObject(5, Position3D{ distance, -distance, distance });
                removeObject(6, Position3D{ distance, distance, -distance });
                removeObject(7, Position3D{ distance, distance, distance });

                THEN("finding all within same bounds returns empty container")
                {
                    const auto bounds = AxisAlignedBox3D
                    {
                        Position3D { 0, 0, 0 },
                        Size3D { gridCellSize * 2, gridCellSize * 2, gridCellSize * 2 }
                    };

                    auto allWithin = octree.AllWithin(bounds);

                    REQUIRE(allWithin.empty());
                }

                THEN("total bounds is centered at origin and 2 grid cells in size")
                {
                    auto totalBounds = octree.TotalBounds();
                    REQUIRE(totalBounds.Center() == Position3D());
                    REQUIRE(totalBounds.Size() == Size3D
                        {
                            CellSize<Position3D::Value> * 2,
                            CellSize<Position3D::Value> * 2,
                            CellSize<Position3D::Value> * 2
                        });
                }

                THEN("is empty")
                {
                    REQUIRE(octree.Size() == 0);
                }
            }
        }
    }
}