#pragma once

#include <map>
#include <Arca/Curator.h>

#include "Work.h"
#include "ChangeColor.h"
#include "ChangeMaterialAsset.h"
#include "ChangeViewSlice.h"

#include "RasterImage.h"
#include "RasterLine.h"
#include "RasterRegion.h"
#include "RasterText.h"
#include "OrderedRaster.h"

namespace Atmos::Render
{
    class GraphicsManager;

    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init, GraphicsManager& graphicsManager);

        void Handle(const Work& command);
        void Handle(const ChangeColor& command);
        void Handle(const ChangeMaterialAsset& command);
        void Handle(const ChangeViewSlice& command);
    private:
        GraphicsManager* graphicsManager;
    private:
        struct Rasters
        {
            std::vector<Raster::Image> images;
            std::vector<Raster::Line> lines;
            std::vector<Raster::Region> regions;
            std::vector<Raster::Text> texts;
        };

        using RasterMap = std::map<Raster::Order, Rasters>;

        template<class T>
        void Add(const std::vector<Raster::Ordered<T>>& orderedRasters, RasterMap& map);
        template<class T>
        void Add(const Raster::Ordered<T>& orderedRaster, RasterMap& map);
    };

    template<class T>
    void Curator::Add(const std::vector<Raster::Ordered<T>>& orderedRasters, RasterMap& map)
    {
        for (auto& orderedRaster : orderedRasters)
            Add(orderedRaster, map);
    }

    template<class T>
    void Curator::Add(const Raster::Ordered<T>& orderedRaster, RasterMap& map)
    {
        const auto found = map.find(orderedRaster.order);
        if (found == map.end())
            found = map.emplace(orderedRaster.order, {});

        found->second.push_back(orderedRaster.raster);
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
            Atmos::Render::ChangeColor,
            Atmos::Render::ChangeMaterialAsset,
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