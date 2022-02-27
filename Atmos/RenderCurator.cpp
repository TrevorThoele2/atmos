#include "RenderCurator.h"

#include "RenderCore.h"
#include "ViewSlice.h"
#include "ColorChanged.h"
#include "MainSurface.h"
#include "Camera.h"
#include "StagedRasters.h"
#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    Curator::Curator(Init init, GraphicsManager& graphicsManager) : Arca::Curator(init), graphicsManager(&graphicsManager)
    {}

    void Curator::Handle(const Work&)
    {
        const auto cameraPosition = Owner().Find<Camera>()->Position();
        
        const auto staged = MutablePointer().Of<Raster::Staged>();
        RasterMap rasterMap;
        Add(staged->images, rasterMap);
        Add(staged->lines, rasterMap);
        Add(staged->regions, rasterMap);
        Add(staged->texts, rasterMap);
        const auto allRasters = Raster::All
        {
            .images = staged->images,
            .lines = staged->lines,
            .regions = staged->regions,
            .texts = staged->texts
        };

        graphicsManager->DrawFrame(
            allRasters,
            Spatial::Point2D
            {
                Spatial::Point2D::Value(cameraPosition.x),
                Spatial::Point2D::Value(cameraPosition.y)
            });

        staged->images = {};
        staged->lines = {};
        staged->regions = {};
        staged->texts = {};
    }

    void Curator::Handle(const ChangeColor& command)
    {
        const auto renderCore = MutablePointer().Of<RenderCore>(command.id);
        if (renderCore)
        {
            const auto previous = renderCore->color;
            renderCore->color = command.to;
            Owner().Raise(ColorChanged{ command.id, previous });
        }
    }

    void Curator::Handle(const ChangeMaterialAsset& command)
    {
        const auto renderCore = MutablePointer().Of<RenderCore>(command.id);
        if (renderCore)
            renderCore->material = command.to;
    }

    void Curator::Handle(const ChangeViewSlice& command)
    {
        const auto viewSlice = MutablePointer().Of<ViewSlice>(command.id);
        if (viewSlice)
            viewSlice->box = command.to;
    }
}
