#pragma once

#include "RenderObjectCurator.h"

#include "Line.h"
#include "MoveLine.h"
#include "FindLinesByBox.h"
#include "RasterLine.h"
#include "PreparedRaster.h"

#include "Octree.h"

namespace Atmos::Render
{
    class LineCurator final : public ObjectCurator
    {
    public:
        explicit LineCurator(Init init);
    public:
        using ObjectCurator::Handle;
        void Handle(const MoveLine& command);
        [[nodiscard]] std::vector<Arca::RelicID> Handle(const FindLinesByBox& command) const;
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface) override;
    private:
        using Index = Arca::Index<Line>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;

        [[nodiscard]] std::optional<Raster::Prepared<Raster::Line>> Raster(
            const Line& value,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface);

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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::LineCurator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::MoveLine,
            Atmos::Render::FindLinesByBox>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::LineCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::LineCurator>;
    };
}