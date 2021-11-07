#include "GraphicsManager.h"

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
        const Buffer& buffer, const Spatial::Size2D& size)
    {
        return CreateTextResourceImpl(buffer, size);
    }

    void GraphicsManager::Stage(const RenderImage& render)
    {
        StageImpl(render);
    }

    void GraphicsManager::Stage(const RenderLine& render)
    {
        StageImpl(render);
    }

    void GraphicsManager::Stage(const RenderRegion& render)
    {
        StageImpl(render);
    }

    void GraphicsManager::Stage(const RenderText& render)
    {
        StageImpl(render);
    }

    void GraphicsManager::Stage(const UpdateText& update)
    {
        StageImpl(update);
    }

    void GraphicsManager::DrawFrame(Resource::Surface& surface, const Spatial::Point2D& mapPosition, const Color& backgroundColor, Diagnostics::Statistics::Profile& profile)
    {
        DrawFrameImpl(surface, mapPosition, backgroundColor, profile);
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

    Buffer GraphicsManager::CompileShader(const File::Path& filePath)
    {
        return CompileShaderImpl(filePath);
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
