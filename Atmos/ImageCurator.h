#pragma once

#include "RenderObjectCurator.h"

#include "Octree.h"

#include "ImageCore.h"
#include "Bounds.h"
#include "Camera.h"
#include "ChangeImageCore.h"
#include "ChangeMaterialAsset.h"

namespace Atmos::Render
{
    class ImageCurator final : public ObjectCurator
    {
    public:
        explicit ImageCurator(Init init);
    public:
        using ObjectCurator::Handle;
        void Handle(const ChangeImageCore& command);
        void Handle(const ChangeImageMaterialAsset& command);
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            Arca::Index<MainSurface> mainSurface) override;
    private:
        using Matrix = Arca::All<ImageCore, Arca::Either<Spatial::Bounds>>;
        using Index = Arca::Index<Matrix>;
        Spatial::Grid::Octree<Arca::RelicID, Index> octree;
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
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeImageCore,
            Atmos::Render::ChangeImageMaterialAsset>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Render::ImageCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::ImageCurator>;
    };
}