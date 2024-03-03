#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "MaterialViewCore.h"
#include "Bounds.h"
#include "Camera.h"
#include "ChangeMaterialViewCore.h"

namespace Atmos::Render
{
    class MaterialViewCurator final : public Arca::Curator
    {
    public:
        explicit MaterialViewCurator(Init init);

        void Work();
    public:
        void Handle(const ChangeMaterialViewCore& command);
    private:
        using Matrix = Arca::All<MaterialViewCore, Arca::Either<Bounds>>;
        using MatrixIndex = Arca::MatrixIndex<Matrix>;
        Grid::Octree<Arca::RelicID, MatrixIndex> octree;

        Arca::GlobalIndex<Camera> camera;
    private:
        void CalculateMaterialSlice(MaterialViewCore& core);
    private:
        void OnViewFormed(const Arca::MatrixFormed<Matrix>& view);
        void OnViewDissolved(const Arca::MatrixDissolved<Matrix>& view);
    private:
        static AxisAlignedBox3D BoxFor(const MatrixIndex& view);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::MaterialViewCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "MaterialViewCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::MaterialViewCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Render::MaterialViewCurator, BinaryArchive>
    {};
}