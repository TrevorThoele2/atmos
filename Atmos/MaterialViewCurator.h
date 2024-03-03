#pragma once

#include <Arca/Curator.h>

#include "Octree.h"

#include "MaterialViewCore.h"
#include "Bounds.h"
#include "Camera.h"

namespace Atmos::Render
{
    class MaterialViewCurator final : public Arca::Curator
    {
    protected:
        void PostConstructImplementation() override;
        void WorkImplementation(Stage& stage) override;
    private:
        using MaterialMatrix = Arca::All<MaterialViewCore, Arca::Either<Bounds>>;
        using MaterialIndex = Arca::MatrixIndex<MaterialMatrix>;
        Grid::Octree<Arca::RelicID, MaterialIndex> octree;

        Arca::GlobalIndex<Camera> camera;
    private:
        void OnMaterialFormed(const Arca::MatrixFormed<MaterialMatrix>& matrix);
        void OnMaterialDissolved(const Arca::MatrixDissolved<MaterialMatrix>& matrix);
    private:
        static AxisAlignedBox3D BoxFor(const MaterialIndex& matrix);
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