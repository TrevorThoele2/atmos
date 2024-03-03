#include "GraphicsManager.h"

#include "MainSurface.h"
#include "SetupMainSurfaceData.h"

#include "Logger.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::Initialize(Arca::Reliquary& reliquary, void* mainWindow)
    {
        InitializeImpl();
        reliquary.Do(Render::SetupMainSurfaceData{ mainWindow });
    }

    void GraphicsManager::SetupDefaults(Arca::Reliquary& reliquary)
    {
        SetupDefaultsImpl(reliquary);
    }

    std::unique_ptr<Asset::ImageData> GraphicsManager::CreateImageData(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageSize& size)
    {
        if (size.width <= 0.0f || size.height <= 0.0f)
        {
            Logging::logger.Log("ImageAsset's require dimensions greater than 0.");
            return {};
        }

        return CreateImageDataImpl(buffer, name, size);
    }

    std::unique_ptr<Asset::ShaderData> GraphicsManager::CreateShaderData(
        const Buffer& buffer, const Name& name)
    {
        return CreateShaderDataImpl(buffer, name);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateMainSurfaceData(
        void* window)
    {
        return CreateMainSurfaceDataImpl(window);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateSurfaceData(
        void* window)
    {
        return CreateSurfaceDataImpl(window);
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
