#include "RenderCurator.h"

#include "RenderCore.h"
#include "ViewSlice.h"
#include "ColorChanged.h"
#include "MainSurface.h"
#include "Camera.h"
#include "StagedRasters.h"
#include "SpatialAlgorithms.h"
#include "ExecuteMaterial.h"
#include "ExecutingMaterial.h"

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
        const auto executingMaterial = MutablePointer().Of<Raster::ExecutingMaterial>();
        auto& recordedCommands = executingMaterial->recordedCommands;
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

    auto Curator::Compose(Raster::Staged& staged) -> RasterMap
    {
        RasterMap map;
        const auto add = [this, &map](auto& inputRasters, const auto rastersSelector)
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

        const auto executingMaterial = MutablePointer().Of<Raster::ExecutingMaterial>();
        auto& recordedCommands = executingMaterial->recordedCommands;

        for (auto& surfaceLayer : map)
        {
            auto& recordInto = output.emplace(surfaceLayer.first, std::vector<Raster::Command>{}).first->second;

            for (auto& zLayer : surfaceLayer.second)
            {
                for (auto& scriptLayer : zLayer.second)
                {
                    executingMaterial->images = scriptLayer.second.images;
                    executingMaterial->lines = scriptLayer.second.lines;
                    executingMaterial->regions = scriptLayer.second.regions;
                    executingMaterial->texts = scriptLayer.second.texts;
                    Owner().Do(Scripting::ExecuteMaterial{ scriptLayer.first });
                }
            }

            recordInto.reserve(recordInto.size() + recordedCommands.size());
            for (auto& command : recordedCommands)
            {
                if (std::holds_alternative<Raster::DrawImage>(command))
                    std::get<Raster::DrawImage>(command).surface = surfaceLayer.first;
                else if (std::holds_alternative<Raster::DrawLine>(command))
                    std::get<Raster::DrawLine>(command).surface = surfaceLayer.first;
                else if (std::holds_alternative<Raster::DrawRegion>(command))
                    std::get<Raster::DrawRegion>(command).surface = surfaceLayer.first;
                else if (std::holds_alternative<Raster::DrawText>(command))
                    std::get<Raster::DrawText>(command).surface = surfaceLayer.first;
                recordInto.push_back(command);
            }
        }

        return output;
    }
}
