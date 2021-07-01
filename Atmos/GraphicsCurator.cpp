#include "GraphicsCurator.h"

#include "MainSurface.h"

#include "WindowSizeChanged.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init, GraphicsManager& manager) :
        Curator(init),
        manager(&manager)
    {
        Owner().On<Arca::DestroyingKnown<Asset::Image>>([this](const Arca::DestroyingKnown<Asset::Image>& signal)
            {
                const auto resource = MutablePointer().Of(signal.index)->Resource();
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });

        Owner().On<Arca::DestroyingKnown<Asset::Shader>>([this](const Arca::DestroyingKnown<Asset::Shader>& signal)
            {
                const auto resource = MutablePointer().Of(signal.index)->Resource();
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });

        Owner().On<Arca::DestroyingKnown<SurfaceCore>>([this](const Arca::DestroyingKnown<SurfaceCore>& signal)
            {
                const auto& resource = MutablePointer().Of(signal.index)->resource;
                if (!resource)
                    return;

                this->manager->ResourceDestroying(*resource);
            });

        Owner().On<Window::SizeChanged>([this](const Window::SizeChanged& signal)
            {
                AttemptReconstruct(signal.size);
            });
    }

    void GraphicsCurator::Handle(const ReconstructGraphics& command)
    {
        AttemptReconstruct(command.screenSize);
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

    File::Path GraphicsCurator::Handle(const CompileShader& command)
    {
        return manager->CompileShader(command.inputFilePath, command.outputFilePath);
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsCurator::Handle(
        const Asset::Resource::Create<Asset::Resource::Image>& command)
    {
        return manager->CreateImageResource(command.buffer, command.name, command.size);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsCurator::Handle(
        const Asset::Resource::Create<Asset::Resource::Shader>& command)
    {
        return manager->CreateShaderResource(command.buffer, command.name);
    }

    void GraphicsCurator::AttemptReconstruct(const Spatial::Size2D& size)
    {
        if (manager->ShouldReconstruct())
        {
            GraphicsReconstructionObjects reconstructionObjects;
            reconstructionObjects.screenSize = size;
            reconstructionObjects.shaderAssets = MutablePointersOf<Asset::Shader>();
            reconstructionObjects.mainSurface = MutablePointer().Of<MainSurface>();

            manager->Reconstruct(reconstructionObjects);
        }
    }
}