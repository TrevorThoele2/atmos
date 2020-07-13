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
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;

        Arca::Index<Camera> camera;
    private:
        void OnCreated(const Arca::CreatedKnown<GridRegion>& signal);
        void OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal);
    private:
        static Spatial::AxisAlignedBox3D BoxFor(const std::vector<Spatial::Grid::Point>& points, Spatial::Grid::Point::Value z);
        static Spatial::AxisAlignedBox3D BoxFor(const Index& index);
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