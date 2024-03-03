#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "Line.h"
#include "MoveLine.h"
#include "Camera.h"

namespace Atmos::Render
{
    class LineCurator final : public Arca::Curator
    {
    public:
        explicit LineCurator(Init init);

        void Work();
    public:
        void Handle(const MoveLine& command);
    private:
        using Index = Arca::Index<Line>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;

        Arca::Index<Camera> camera;
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