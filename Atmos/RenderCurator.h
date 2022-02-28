#pragma once

#include <map>
#include <Arca/Curator.h>

#include "Work.h"
#include "RecordRasterCommands.h"
#include "ChangeColor.h"
#include "ChangeMaterial.h"
#include "ChangeViewSlice.h"

#include "RasterImage.h"
#include "RasterLine.h"
#include "RasterRegion.h"
#include "RasterText.h"
#include "PreparedRaster.h"
#include "StagedRasters.h"
#include "RasterCommands.h"

namespace Atmos::Render
{
    class GraphicsManager;

    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, GraphicsManager& graphicsManager);

        void Handle(const Work& command);
        void Handle(const Raster::RecordCommands& command);
        void Handle(const ChangeColor& command);
        void Handle(const ChangeMaterial& command);
        void Handle(const ChangeViewSlice& command);
    private:
        GraphicsManager* graphicsManager;
    private:
        struct Rasters
        {
            std::vector<Raster::Image> images = {};
            std::vector<Raster::Line> lines = {};
            std::vector<Raster::Region> regions = {};
            std::vector<Raster::Text> texts = {};
        };

        using RasterMap = std::map<Resource::Surface*, std::map<Raster::Order, std::map<Asset::Script*, Rasters>>>;

        std::vector<Raster::Command> recordedCommands = {};

        [[nodiscard]] static RasterMap Compose(const Raster::Staged& staged);
        [[nodiscard]] Raster::Commands Execute(const RasterMap& map);
        template<class T>
        [[nodiscard]] static Rasters& FindRasters(const Raster::Prepared<T>& preparedRaster, RasterMap& map);
    };
    
    template<class T>
    auto Curator::FindRasters(const Raster::Prepared<T>& preparedRaster, RasterMap& map) -> Rasters&
    {
        const auto [raster, surface, order] = preparedRaster;

        const auto surfaceLayer = map.find(surface);
        if (surfaceLayer == map.end())
            surfaceLayer = map.emplace(surface);
            
        const auto zLayer = surfaceLayer->second.find(order);
        if (zLayer == surfaceLayer->second.end())
            zLayer = surfaceLayer->second.emplace(order, {});

        const auto script = raster.material.script;
        const auto scriptLayer = zLayer->second.find(script);
        if (scriptLayer == zLayer->second.end())
            scriptLayer = map.emplace(script, {});

        return scriptLayer->second;
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Curator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Render::Curator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Render::Raster::RecordCommands,
            Atmos::Render::ChangeColor,
            Atmos::Render::ChangeMaterial,
            Atmos::Render::ChangeViewSlice>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Render::Curator, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Render::Curator>;
    };
}