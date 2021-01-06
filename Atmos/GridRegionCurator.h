#pragma once

#include "RenderObjectCurator.h"

#include "GridRegion.h"
#include "MoveGridRegion.h"
#include "ChangeMaterialAsset.h"

#include "Octree.h"

namespace Atmos::Render
{
    class GridRegionCurator final : public ObjectCurator
    {
    public:
        explicit GridRegionCurator(Init init);

        using ObjectCurator::Handle;
        void Handle(const MoveGridRegion& command);
        void Handle(const ChangeRegionMaterialAsset& command);
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
        template<class Function>
        void AttemptChangeObject(Arca::RelicID id, Function function);
    private:
        void OnCreated(const Arca::CreatedKnown<GridRegion>& signal);
        void OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal);
    private:
        static Spatial::AxisAlignedBox3D BoxFor(const std::vector<Spatial::Grid::Point>& points, Spatial::Grid::Point::Value z);
        static Spatial::AxisAlignedBox3D BoxFor(const Index& index);
    };

    template<class Function>
    void GridRegionCurator::AttemptChangeObject(Arca::RelicID id, Function function)
    {
        const auto index = Arca::Index<GridRegion>(id, Owner());
        if (!index)
            return;

        auto data = MutablePointer().Of(index);

        function(*data);
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GridRegionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::GridRegionCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::MoveGridRegion,
            Atmos::Render::ChangeRegionMaterialAsset>;
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