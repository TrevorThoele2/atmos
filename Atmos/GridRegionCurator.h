#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "GridRegion.h"
#include "Camera.h"

namespace Atmos::Render
{
    class GridRegionCurator final : public Arca::Curator
    {
    public:
        explicit GridRegionCurator(Init init);

        void Work();
    private:
        using Index = Arca::Index<GridRegion>;
        Grid::Octree<Arca::RelicID, Index> octree;

        Arca::Index<Camera> camera;
    private:
        void OnCreated(const Arca::CreatedKnown<GridRegion>& signal);
        void OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal);
    private:
        static AxisAlignedBox3D BoxFor(const std::vector<Grid::Position>& points, Grid::Position::Value z);
        static AxisAlignedBox3D BoxFor(const Index& index);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Render::GridRegionCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::GridRegionCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::GridRegionCurator, BinaryArchive>
    {};
}