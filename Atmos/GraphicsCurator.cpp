#include "GraphicsCurator.h"

#include "MainSurface.h"
#include "TextCore.h"

#include "WindowSizeChanged.h"

namespace Atmos::Render
{
    GraphicsCurator::GraphicsCurator(Init init, GraphicsManager& manager) :
        Curator(init),
        manager(&manager)
    {
        OnResourceDestroying<Asset::Image>();
        OnResourceDestroying<Asset::Shader>();
        OnResourceDestroying<SurfaceCore>();
        
        Owner().On<Window::SizeChanged>([this](const Window::SizeChanged& signal)
            {
                AttemptReconstruct(signal.size);
            });

        Owner().On<World::FieldChanging>([this](const World::FieldChanging&) { OnFieldChanging(); });
    }
    
    void GraphicsCurator::Handle(const ReconstructGraphics& command)
    {
        AttemptReconstruct(command.screenSize);
    }

    std::unique_ptr<Resource::Surface> GraphicsCurator::Handle(const Resource::CreateSurface& command)
    {
        return manager->CreateSurfaceResource(command.window);
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
        manager->PruneResources();
    }

    Buffer GraphicsCurator::Handle(const CompileShader& command)
    {
        return manager->CompileShader(command.filePath);
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

    Spatial::Size2D GraphicsCurator::Handle(const TextBaseSize& command)
    {
        const auto font = Owner().Find<Asset::Font>(command.font);
        if (!font)
            return {};

        const auto& fontResource = *const_cast<Asset::Resource::Font*>(font->Resource());
        return manager->TextBaseSize(
            command.string,
            fontResource,
            command.bold,
            command.italics,
            command.wrapWidth);
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

    void GraphicsCurator::OnFieldChanging()
    {
        const auto surface = Owner().Find<MainSurface>();
        const auto resource = surface->Resource();
        manager->ResourceDestroying(*resource);
        manager->PruneResources();
    }
}