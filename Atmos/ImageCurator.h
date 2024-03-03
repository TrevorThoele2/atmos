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

#include <Arca/All.h>
#include <Arca/Either.h>

namespace Atmos::Render
{
    class ImageCurator final : public ObjectCurator
    {
    public:
        explicit ImageCurator(Init init, GraphicsManager& graphicsManager);
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
        GraphicsManager* graphicsManager;

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
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::ImageCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::ImageCurator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeImageCore,
            Atmos::Render::FindImagesByBox>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::ImageCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::ImageCurator>;
    };
}