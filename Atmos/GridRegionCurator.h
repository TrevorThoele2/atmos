#pragma once

#include "RenderObjectCurator.h"

#include "Octree.h"

#include "GridRegion.h"
#include "Camera.h"

namespace Atmos::Render
{
    class GridRegionCurator final : public ObjectCurator
    {
    public:
        explicit GridRegionCurator(Init init);

        using ObjectCurator::Handle;
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            Arca::Index<MainSurface> mainSurface) override;
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
        using HandledCommands = HandledCommands<
            Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::GridRegionCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::GridRegionCurator>;
    };
}