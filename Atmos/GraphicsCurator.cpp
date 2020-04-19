#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "AncillarySurface.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init) :
        Curator(init),
        manager(init.owner)
    {}

    void GraphicsCurator::Handle(const ReconstructGraphics& command)
    {
        if (!manager->ShouldReconstruct())
            return;

        GraphicsReconstructionObjects reconstructionObjects;
        reconstructionObjects.screenSize = command.screenSize;
        reconstructionObjects.shaderAssets = MutablePointersOf<Asset::ShaderAsset>();
        reconstructionObjects.mainSurface = MutablePointer().Of<MainSurface>();
        reconstructionObjects.ancillarySurfaces = MutablePointersOf<AncillarySurface>();

        manager->Reconstruct(reconstructionObjects);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsCurator::Handle(const Asset::CreateImageAssetData& command)
    {
        return manager->CreateImageData(command.buffer, command.name, command.size);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsCurator::Handle(const Asset::CreateShaderAssetData& command)
    {
        return manager->CreateShaderData(command.buffer, command.name, command.entryPoint);
    }

    std::unique_ptr<SurfaceData> GraphicsCurator::Handle(const CreateSurfaceData& command)
    {
        return manager->CreateSurfaceData(command.window);
    }
}
