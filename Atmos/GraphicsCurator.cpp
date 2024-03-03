#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "AncillarySurface.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init, GraphicsManager& manager) :
        Curator(init),
        manager(&manager)
    {
        Owner().On<Arca::DestroyingKnown<Asset::Image>>([this](const Arca::DestroyingKnown<Asset::Image>& signal)
            {
                const auto resource = MutablePointer().Of(signal.reference)->Resource();
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });

        Owner().On<Arca::DestroyingKnown<Asset::Shader>>([this](const Arca::DestroyingKnown<Asset::Shader>& signal)
            {
                const auto resource = MutablePointer().Of(signal.reference)->Resource();
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });

        Owner().On<Arca::DestroyingKnown<SurfaceCore>>([this](const Arca::DestroyingKnown<SurfaceCore>& signal)
            {
                const auto& resource = MutablePointer().Of(signal.reference)->resource;
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });
    }

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

    void GraphicsCurator::Handle(const Resource::SetupMainSurface& command)
    {
        const Arca::Index<MainSurface> mainSurface(Owner());

        auto resource = manager->CreateMainSurfaceResource(command.window, Owner());
        MutablePointer().Of<SurfaceCore>(mainSurface.ID())->resource = std::move(resource);
    }

    std::unique_ptr<Resource::Surface> GraphicsCurator::Handle(const Resource::CreateSurface& command)
    {
        return manager->CreateSurfaceResource(command.window, Owner());
    }

    void GraphicsCurator::Handle(const SetFullscreen& command)
    {
        manager->SetFullscreen(command.to);
    }

    void GraphicsCurator::Handle(const ChangeVerticalSync& command)
    {
        manager->ChangeVerticalSync(command.to);
    }

    void GraphicsCurator::Handle(const PruneGraphicsResources&)
    {
        manager->PruneResources(Owner());
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsCurator::Handle(const Asset::Resource::Create<Asset::Resource::Image>& command)
    {
        return manager->CreateImageResource(command.buffer, command.name, command.size);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsCurator::Handle(const Asset::Resource::Create<Asset::Resource::Shader>& command)
    {
        return manager->CreateShaderResource(command.buffer, command.name);
    }
}
