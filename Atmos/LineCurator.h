#pragma once

#include "RenderObjectCurator.h"

#include "Octree.h"

#include "Line.h"
#include "MoveLine.h"
#include "Camera.h"

namespace Atmos::Render
{
    class LineCurator final : public ObjectCurator
    {
    public:
        explicit LineCurator(Init init);
    public:
        void Handle(const MoveLine& command);
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
        static inline const TypeName typeName = "Atmos::Render::LineCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::LineCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::LineCurator, BinaryArchive>
    {};
}