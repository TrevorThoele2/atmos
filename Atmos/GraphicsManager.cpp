#include "GraphicsManager.h"

#include "MainSurface.h"
#include "CreateMainSurfaceData.h"

#include "Log.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::Initialize(Arca::Reliquary& reliquary, void* mainWindow)
    {
        this->reliquary = &reliquary;
        InitializeImpl();
        reliquary.Do(Render::CreateMainSurfaceData{ mainWindow });
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageData(
        const Buffer& buffer, const Name& name, const Size2D& size)
    {
        if (size.width <= 0.0f || size.height <= 0.0f)
        {
            reliquary->Do(Logging::Log("ImageAsset's require dimensions greater than 0."));
            return {};
        }

        return CreateImageDataImpl(buffer, name, size);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderData(
        const Buffer& buffer, const Name& name, const String& entryPoint)
    {
        return CreateShaderDataImpl(buffer, name, entryPoint);
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

    Arca::Reliquary& GraphicsManager::Reliquary()
    {
        return *reliquary;
    }

    const Arca::Reliquary& GraphicsManager::Reliquary() const
    {
        return *reliquary;
    }
}
