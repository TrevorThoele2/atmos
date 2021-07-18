#include "GraphicsManager.h"

#include "MainSurface.h"
#include "CreateSurfaceResource.h"

#include "Logger.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager()
    {
        logger->Log(
            "Destroyed graphics.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    std::unique_ptr<Asset::Resource::Image> GraphicsManager::CreateImageResource(
        const Buffer& buffer,
        const Name& name,
        const Spatial::Size2D& size)
    {
        if (size.width <= 0 || size.height <= 0)
        {
            logger->Log("ImageAsset's require dimensions greater than 0.");
            return {};
        }

        return CreateImageResourceImpl(buffer, name, size);
    }

    std::unique_ptr<Asset::Resource::Shader> GraphicsManager::CreateShaderResource(
        const Buffer& buffer, const Name& name)
    {
        return CreateShaderResourceImpl(buffer, name);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateSurfaceResource(
        void* window)
    {
        return CreateSurfaceResourceImpl(window);
    }

    std::unique_ptr<Resource::Surface> GraphicsManager::CreateMainSurfaceResource(
        void* window)
    {
        return CreateMainSurfaceResourceImpl(window);
    }

    std::unique_ptr<Resource::Text> GraphicsManager::CreateTextResource(
        const Buffer& buffer,
        const Spatial::Size2D& size)
    {
        return CreateTextResourceImpl(buffer, size);
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

    void GraphicsManager::ResourceDestroying(Resource::Text& resource)
    {
        ResourceDestroyingImpl(resource);
    }

    void GraphicsManager::PruneResources()
    {
        PruneResourcesImpl();
    }

    File::Path GraphicsManager::CompileShader(
        const File::Path& inputFilePath, const std::optional<File::Path>& outputFilePath)
    {
        return CompileShaderImpl(inputFilePath, outputFilePath);
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

    GraphicsManager::GraphicsManager(Logging::Logger& logger, String typeName) :
        typeName(typeName), logger(&logger)
    {
        this->logger->Log(
            "Created graphics.",
            Logging::Severity::Information,
            Logging::Details{ {"Type Name", typeName} });
    }

    Logging::Logger& GraphicsManager::Logger()
    {
        return *logger;
    }
}
