#include "GraphicsManager.h"

#include "MainSurface.h"
#include "SetupMainSurfaceResource.h"

#include "Logger.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::Initialize(Arca::Reliquary& reliquary, void* mainWindow)
    {
        InitializeImpl();
        reliquary.Do(Resource::SetupMainSurface{ mainWindow });
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsManager::CreateImageResource(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageSize& size)
    {
        if (size.width <= 0.0f || size.height <= 0.0f)
        {
            Logging::logger.Log("ImageAsset's require dimensions greater than 0.");
            return {};
        }

        return CreateImageResourceImpl(buffer, name, size);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsManager::CreateShaderResource(
        const Buffer& buffer, const Name& name)
    {
        return CreateShaderResourceImpl(buffer, name);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateMainSurfaceResource(
        void* window)
    {
        return CreateMainSurfaceResourceImpl(window);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateSurfaceResource(
        void* window)
    {
        return CreateSurfaceResourceImpl(window);
    }

    void GraphicsManager::ResourceDestroying(Asset::Resource::Image& resource)
    {
        ResourceDestroyingImpl(resource);
    }

    void GraphicsManager::ResourceDestroying(Asset::Resource::Shader& resource)
    {
        ResourceDestroyingImpl(resource);
    }

    void GraphicsManager::ResourceDestroying(Resource::Surface& resource)
    {
        ResourceDestroyingImpl(resource);
    }

    void GraphicsManager::PruneResources(Arca::Reliquary& reliquary)
    {
        PruneResourcesImpl(reliquary);
    }

    void GraphicsManager::Reconstruct(GraphicsReconstructionObjects objects)
    {
        if (!ShouldReconstruct())
            return;

        ReconstructInternals(objects);
    }

    bool GraphicsManager::ShouldReconstruct() const
    {
        return ShouldReconstructInternals();
    }
}
