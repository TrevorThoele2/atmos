#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "Region.h"
#include "Camera.h"

namespace Atmos::Render
{
    class RegionCurator final : public Arca::Curator
    {
    public:
        explicit RegionCurator(Init init);

        void Work();
    private:
        using Index = Arca::Index<Region>;
        Grid::Octree<Arca::RelicID, Index> octree;

        Arca::Index<Camera> camera;
    private:
        void OnCreated(const Arca::CreatedKnown<Region>& signal);
        void OnDestroying(const Arca::DestroyingKnown<Region>& signal);
    private:
        static AxisAlignedBox3D BoxFor(const std::vector<Position2D>& points, Position2D::Value z);
        static AxisAlignedBox3D BoxFor(const Index& index);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::RegionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Render::RegionCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::RegionCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::RegionCurator, BinaryArchive>
    {};
}