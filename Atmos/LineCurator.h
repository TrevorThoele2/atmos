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
        using LineIndex = Arca::Index<Line>;
        Grid::Octree<Arca::RelicID, LineIndex> octree;

        Arca::Index<Camera> camera;
    private:
        void OnLineCreated(const Arca::CreatedKnown<Line>& line);
        void OnLineDestroying(const Arca::DestroyingKnown<Line>& line);
    private:
        static AxisAlignedBox3D BoxFor(const std::vector<Position2D>& points, Position2D::Value z);
        static AxisAlignedBox3D BoxFor(const LineIndex& line);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::LineCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "LineCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::LineCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::LineCurator, BinaryArchive>
    {};
}