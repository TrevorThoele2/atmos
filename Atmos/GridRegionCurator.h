#pragma once

#include "RenderObjectCurator.h"

#include "GridRegion.h"
#include "MoveGridRegion.h"
#include "ChangeMaterialAsset.h"
#include "FindGridRegionsByBox.h"
#include "RasterRegion.h"
#include "OrderedRaster.h"

#include "Octree.h"

namespace Atmos::Render
{
    class GridRegionCurator final : public ObjectCurator
    {
    public:
        explicit GridRegionCurator(Init init);

        using ObjectCurator::Handle;
        void Handle(const MoveGridRegion& command);
        [[nodiscard]] std::vector<Arca::RelicID> Handle(const FindGridRegionsByBox& command) const;
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface) override;
    private:
        using Index = Arca::Index<GridRegion>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;

        [[nodiscard]] std::optional<Raster::Ordered<Raster::Region>> Raster(
            const GridRegion& value,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface);

        void OnCreated(const Arca::CreatedKnown<GridRegion>& signal);
        void OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal);
    private:
        Arca::Index<Camera> camera;
    private:
        template<class Function>
        void AttemptChangeObject(Arca::RelicID id, Function function);
    private:
        static Spatial::AxisAlignedBox3D BoxFor(const std::vector<Spatial::Grid::Point>& points, Spatial::Grid::Point::Value z);
        static Spatial::AxisAlignedBox3D BoxFor(const GridRegion& region);
        static Spatial::AxisAlignedBox3D BoxFor(const Index& index);
    };

    template<class Function>
    void GridRegionCurator::AttemptChangeObject(Arca::RelicID id, Function function)
    {
        const auto index = Owner().Find<GridRegion>(id);
        if (index)
        {
            const auto data = MutablePointer().Of(index);
            function(*data);
        }
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegionCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::GridRegionCurator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::MoveGridRegion,
            Atmos::Render::FindGridRegionsByBox>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::GridRegionCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::GridRegionCurator>;
    };
}