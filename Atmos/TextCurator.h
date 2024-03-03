#pragma once

#include "RenderObjectCurator.h"

#include "TextManager.h"
#include "RenderCore.h"
#include "TextCore.h"
#include "Bounds.h"
#include "ViewSlice.h"

#include "Work.h"
#include "ChangeTextCore.h"
#include "CreateFontAssetResource.h"

#include <Arca/All.h>
#include <Arca/Either.h>

namespace Atmos::Render
{
    class TextCurator final : public ObjectCurator
    {
    public:
        explicit TextCurator(Init init, TextManager& manager, GraphicsManager& graphicsManager);
    public:
        using ObjectCurator::Handle;

        void Handle(const ChangeTextCore& command);

        std::unique_ptr<Asset::Resource::Font> Handle(const Asset::Resource::Create<Asset::Resource::Font>& command);
    protected:
        void WorkImpl(
            Spatial::AxisAlignedBox3D cameraBox,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface) override;
    private:
        using Matrix = Arca::All<RenderCore, TextCore, Spatial::Bounds>;

        TextManager* manager;
        GraphicsManager* graphicsManager;

        void StageRender(
            Arca::RelicID id,
            const RenderCore& renderCore,
            const TextCore& core,
            const Spatial::Bounds& bounds,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface);
    private:
        static std::tuple<Spatial::Size2D, Spatial::AxisAlignedBox2D> ViewSliceDependent(
            Arca::Index<ViewSlice> viewSlice,
            const Spatial::Size2D& baseSize,
            const Spatial::Size2D& boundsSize,
            const Spatial::Scalers2D& scalers);
        void CalculateForText(Arca::RelicID id);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::TextCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Render::TextCurator"; }
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::ChangeTextCore,
            Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::TextCurator> final
    {
    public:
        using ObjectT = Atmos::Render::TextCurator;
    public:
        template<class Archive>
        void Scriven(ObjectT&, Archive&)
        {}
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::TextCurator, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::TextCurator>;
    };
}