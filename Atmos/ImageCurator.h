#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "ImageCore.h"
#include "Bounds.h"
#include "Camera.h"
#include "ChangeImageCore.h"

namespace Atmos::Render
{
    class ImageCurator final : public Arca::Curator
    {
    public:
        explicit ImageCurator(Init init);

        void Work();
    public:
        void Handle(const ChangeImageCore& command);
    private:
        using Matrix = Arca::All<ImageCore, Arca::Either<Spatial::Bounds>>;
        using Index = Arca::Index<Matrix>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;

        Arca::Index<Camera> camera;
    private:
        void OnCreated(const Arca::MatrixFormed<Matrix>& signal);
        void OnDestroying(const Arca::MatrixDissolved<Matrix>& signal);
    private:
        static Spatial::AxisAlignedBox3D BoxFor(const Index& index);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ImageCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Render::ImageCurator";
        using HandledCommands = HandledCommands<Atmos::Render::ChangeImageCore>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::ImageCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::ImageCurator, BinaryArchive>
    {};
}