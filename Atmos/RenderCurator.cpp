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
        const auto commands = Execute(Compose(*staged));
        
        graphicsManager->DrawFrame(
            commands,
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

    void Curator::Handle(const Raster::RecordCommands& command)
    {
        recordedCommands.insert(recordedCommands.end(), command.commands.begin(), command.commands.end());
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

    void Curator::Handle(const ChangeMaterial& command)
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

    auto Curator::Compose(const Raster::Staged& staged) -> RasterMap
    {
        RasterMap map;
        const auto add = [&map](const auto& inputRasters, const auto rastersSelector)
        {
            for (auto& raster : inputRasters)
                (FindRasters(raster, map).*rastersSelector).push_back(std::get<0>(raster));
        };

        add(staged.images, &Rasters::images);
        add(staged.lines, &Rasters::lines);
        add(staged.regions, &Rasters::regions);
        add(staged.texts, &Rasters::texts);

        return map;
    }
    
    Raster::Commands Curator::Execute(const RasterMap& map)
    {
        Raster::Commands output;

        for (auto& surfaceLayer : map)
        {
            const auto recordInto = output.emplace(surfaceLayer.first, std::vector<Raster::Command>{}).first;

            for (auto& zLayer : surfaceLayer.second)
            {
                for (auto& scriptLayer : zLayer.second)
                {
                    // Need to hook this up to script input somehow
                    // Likely will need the script to execute a function which returns this information
                    scriptLayer.second.images

                }
            }

            recordInto->second.insert(recordInto->second.end(), recordedCommands.begin(), recordedCommands.end());
            recordedCommands = {};
        }

        return output;
    }
}
