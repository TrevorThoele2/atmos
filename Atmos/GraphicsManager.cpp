#include "GraphicsManager.h"

#include "Camera.h"

#include "WindowDimensionsChanged.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::ReconstructAll()
    {
        // Focused render surface
        const auto prevSurface = currentSurface;
        SetRenderTargetToMain();

        auto shaderAssetBatch = reliquary->Batch<Asset::ShaderAsset>();
        for (auto& loop : shaderAssetBatch)
            loop.Data()->Release();

        ReleaseMainRenderTarget();
        for (auto& loop : surfaces)
            loop.Release();

        for (auto& loop : canvasList)
            loop.Release();

        ReconstructInternals();

        for (auto& loop : shaderAssetBatch)
            loop.Data()->Reset();

        ResetMainRenderTarget();
        for (auto& loop : surfaces)
            loop.Reset();

        for (auto& loop : canvasList)
            loop.Reset();

        // Reset the focused render target
        if (prevSurface != surfaces.end())
            SetRenderTarget(*prevSurface);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageData(const File::Path& path)
    {
        return CreateImageDataImpl(path);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageData(void* buffer, std::int32_t size, const File::Name& name)
    {
        return CreateImageDataImpl(buffer, size, name);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderData(const File::Path& path)
    {
        return CreateShaderDataImpl(path);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderData(void* buffer, std::int32_t size, const File::Name& name)
    {
        return CreateShaderDataImpl(buffer, size, name);
    }

    Surface& GraphicsManager::CreateSurface(void* window)
    {
        surfaces.push_back(CreateSurfaceImpl(window));
        return surfaces.back();
    }

    Canvas& GraphicsManager::CreateCanvas(const ScreenDimensions& dimensions)
    {
        canvasList.push_back(CreateCanvasImpl(dimensions));
        return canvasList.back();
    }

    void GraphicsManager::DestroySurface(Surface& destroy)
    {
        auto found = FindSurface(destroy);
        if (found == surfaces.end())
            return;

        surfaces.erase(found);
    }

    bool GraphicsManager::CanMakeImage(const File::Path& path) const
    {
        return CanMakeImageImpl(path);
    }

    bool GraphicsManager::CanMakeImage(void* buffer, std::int32_t size) const
    {
        return CanMakeImageImpl(buffer, size);
    }

    void GraphicsManager::ResizeCanvas(Canvas& canvas, const ScreenDimensions& dimensions)
    {
        canvas.width = dimensions.width;
        canvas.height = dimensions.height;
        ResizeCanvasImpl(canvas, dimensions);
    }

    void GraphicsManager::SetRenderTarget(Surface& set)
    {
        if (currentSurface != surfaces.end() && &*currentSurface == &set)
            return;

        const auto foundSurface = FindSurface(set);
        if (foundSurface == surfaces.end())
            return;

        currentSurface = foundSurface;
        SetRenderTargetImpl(*currentSurface);
        currentSurface->SetAsRenderTargetImpl();
    }

    void GraphicsManager::SetRenderTargetToMain()
    {
        currentSurface = surfaces.end();
        SetRenderTargetToMainImpl();
    }

    const Surface* GraphicsManager::GetCurrentRenderTarget() const
    {
        if (currentSurface == surfaces.end())
            return nullptr;
        else
            return &*currentSurface;
    }

    void GraphicsManager::RenderMaterialView(MaterialRender& materialRender)
    {
        RenderMaterialViewImpl(materialRender);
    }

    void GraphicsManager::RenderCanvasView(CanvasRender& canvasRender)
    {
        RenderCanvasViewImpl(canvasRender);
    }

    void GraphicsManager::RenderLine(const Line& line)
    {
        RenderLineImpl(line);
    }

    void GraphicsManager::SetMainDimensions(const ScreenDimensions& set)
    {
        if (!IsUsingNonMainSurface())
        {
            dimensions = set;
            SetMainDimensionsImpl(this->dimensions);
        }
    }

    ScreenDimensions GraphicsManager::MainDimensions() const
    {
        return MainDimensionsImpl();
    }

    ScreenDimensions GraphicsManager::CurrentDimensions() const
    {
        return !IsUsingNonMainSurface()
            ? MainDimensionsImpl()
            : currentSurface->Size();
    }

    void GraphicsManager::Present()
    {
        (currentSurface != surfaces.end()) ? currentSurface->Present() : PresentImpl();
    }

    void GraphicsManager::Present(void* windowOverride)
    {
        PresentImpl(windowOverride);
    }

    GraphicsManager::GraphicsManager(Arca::Reliquary& reliquary) :
        reliquary(&reliquary), currentSurface(surfaces.end())
    {
        reliquary.ExecuteOn<Window::DimensionsChanged>(
            [this](const Window::DimensionsChanged& signal)
            {
                SetMainDimensions(signal.dimensions);
            });
    }

    bool GraphicsManager::IsUsingNonMainSurface() const
    {
        return currentSurface != surfaces.end();
    }

    GraphicsManager::SurfaceList::iterator GraphicsManager::FindSurface(Surface& surface)
    {
        for (auto loop = surfaces.begin(); loop != surfaces.end(); ++loop)
        {
            if (&*loop == &surface)
                return loop;
        }

        return surfaces.end();
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Render::GraphicsManager>::typeName = "GraphicsManager";
}