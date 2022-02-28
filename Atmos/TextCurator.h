#pragma once

#include "RenderObjectCurator.h"

#include "TextManager.h"
#include "RenderCore.h"
#include "TextCore.h"
#include "Bounds.h"
#include "ViewSlice.h"
#include "RasterText.h"
#include "PreparedRaster.h"

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
        explicit TextCurator(Init init, TextManager& manager);
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
        using Matrices = std::unordered_map<Arca::RelicID, Arca::Index<Matrix>>;
        Matrices matrices;

        TextManager* manager;
        
        [[nodiscard]] std::optional<Raster::Prepared<Raster::Text>> Raster(
            Arca::RelicID id,
            const Matrix::ReferenceTuple& tuple,
            Spatial::Point2D cameraTopLeft,
            const MainSurface& mainSurface);
    private:
        void AttemptChangeBaseSize(const Matrix::ReferenceTuple& tuple);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::TextCurator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::TextCurator";
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
        template<class Format>
        void Scriven(ObjectT&, Format&)
        {}
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::TextCurator, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Render::TextCurator>;
    };
}