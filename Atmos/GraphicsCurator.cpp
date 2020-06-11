#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "AncillarySurface.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init, GraphicsManager& manager) :
        Curator(init),
        manager(&manager)
    {}

    void GraphicsCurator::Handle(const InitializeGraphics& command)
    {
        manager->Initialize(Owner(), command.window);
    }

    void GraphicsCurator::Handle(const ReconstructGraphics& command)
    {
        if (!manager->ShouldReconstruct())
            return;

        GraphicsReconstructionObjects reconstructionObjects;
        reconstructionObjects.screenSize = command.screenSize;
        reconstructionObjects.shaderAssets = MutablePointersOf<Asset::Shader>();
        reconstructionObjects.mainSurface = MutablePointer().Of<MainSurface>();
        reconstructionObjects.ancillarySurfaces = MutablePointersOf<AncillarySurface>();

        manager->Reconstruct(reconstructionObjects);
    }

    void GraphicsCurator::Handle(const SetupMainSurfaceData& command)
    {
        const Arca::Index<MainSurface> mainSurface(Owner());

        auto data = manager->CreateMainSurfaceData(command.window);
        MutablePointer().Of<SurfaceCore>(mainSurface.ID())->data = std::move(data);
    }

    std::unique_ptr<SurfaceData> GraphicsCurator::Handle(const CreateSurfaceData& command)
    {
        return manager->CreateSurfaceData(command.window);
    }

    void GraphicsCurator::Handle(const SetFullscreen& command)
    {
        manager->SetFullscreen(command.to);
    }

    void GraphicsCurator::Handle(const ChangeVerticalSync& command)
    {
        manager->ChangeVerticalSync(command.to);
    }

    std::unique_ptr<Asset::ImageData> GraphicsCurator::Handle(const Asset::CreateData<Asset::ImageData>& command)
    {
        return manager->CreateImageData(command.buffer, command.name, command.size);
    }

    std::unique_ptr<Asset::ShaderData> GraphicsCurator::Handle(const Asset::CreateData<Asset::ShaderData>& command)
    {
        return manager->CreateShaderData(command.buffer, command.name);
    }
}
