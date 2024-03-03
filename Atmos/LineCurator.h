#pragma once

#include "RenderObjectCurator.h"

#include "Octree.h"

#include "Line.h"
#include "MoveLine.h"
#include "ChangeMaterialAsset.h"

namespace Atmos::Render
{
    class LineCurator final : public ObjectCurator
    {
    public:
        explicit LineCurator(Init init);
    public:
        using ObjectCurator::Handle;
        void Handle(const MoveLine& command);
        void Handle(const ChangeLineMaterialAsset& command);
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            Arca::Index<MainSurface> mainSurface) override;
    private:
        using Index = Arca::Index<Line>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;
    private:
        void OnCreated(const Arca::CreatedKnown<Line>& signal);
        void OnDestroying(const Arca::DestroyingKnown<Line>& signal);
    private:
        static Spatial::AxisAlignedBox3D BoxFor(const std::vector<Spatial::Point2D>& points, Spatial::Point2D::Value z);
        static Spatial::AxisAlignedBox3D BoxFor(const Index& index);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::LineCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::LineCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::MoveLine,
            Atmos::Render::ChangeLineMaterialAsset>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::LineCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::LineCurator>;
    };
}