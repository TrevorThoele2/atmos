#pragma once

#include "RenderObjectCurator.h"

#include "Octree.h"

#include "RenderCore.h"
#include "ImageCore.h"
#include "Bounds.h"
#include "Camera.h"
#include "ChangeImageCore.h"
#include "FindImagesByBox.h"
#include "ViewSlice.h"
#include "BoundsChanged.h"

namespace Atmos::Render
{
    class ImageCurator final : public ObjectCurator
    {
    public:
        explicit ImageCurator(Init init);
    public:
        using ObjectCurator::Handle;
        void Handle(const ChangeImageCore& command);
        std::vector<Arca::RelicID> Handle(const FindImagesByBox& command) const;
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface) override;
    private:
        using Matrix = Arca::All<RenderCore, ImageCore, Arca::Either<Spatial::Bounds>>;
        using Index = Arca::Index<Matrix>;
        Spatial::Grid::Octree<Arca::RelicID, Index> worldOctree;

        std::vector<Arca::Index<Matrix>> screenList;

        void StageRender(
            Arca::RelicID id,
            const Index::ReferenceValueT& value,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface);

        void OnCreated(const Arca::MatrixFormed<Matrix>& signal);
        void OnDestroying(const Arca::MatrixDissolved<Matrix>& signal);
        void OnChanged(const Spatial::BoundsChanged& signal);
        
        static Spatial::Bounds BoundsFor(const Index& index);
        static Spatial::AxisAlignedBox3D BoxFor(const Spatial::Bounds& bounds);
    private:
        static std::tuple<Spatial::Size2D, Spatial::AxisAlignedBox2D> ViewSliceDependent(
            Arca::Index<ViewSlice> viewSlice,
            const Spatial::AxisAlignedBox2D& assetSlice,
            const Spatial::Size2D& boundsSize,
            const Spatial::Scalers2D& scalers);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ImageCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::ImageCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeImageCore,
            Atmos::Render::FindImagesByBox>;
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