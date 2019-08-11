#include "GraphicsManager.h"

#include "CameraSystem.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager()
    {}

    void GraphicsManager::Reinitialize()
    {
        // Focused render surface
        auto prevSurface = currentSurface;
        SetRenderTargetToMain();

        auto& shaderAssetBatch = objectManager->Batch<Asset::ShaderAsset>();
        for (auto& loop : shaderAssetBatch)
            loop->Data()->Release();

        ReleaseMainRenderTarget();
        for (auto& loop : surfaces)
            loop.Release();

        for (auto& loop : canvasList)
            loop.Release();

        ReinitializeImpl();

        for (auto& loop : shaderAssetBatch)
            loop->Data()->Reset();

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
        surfaces.push_back(std::move(CreateSurfaceImpl(window)));
        return surfaces.back();
    }

    Canvas& GraphicsManager::CreateCanvas(const ScreenDimensions& dimensions)
    {
        canvasList.push_back(std::move(CreateCanvasImpl(dimensions)));
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

        auto foundItr = FindSurface(set);
        if (foundItr == surfaces.end())
            return;

        currentSurface = foundItr;
        SetRenderTargetImpl(*currentSurface);
        SetCameraSizeToCurrentDimensions();
        currentSurface->SetAsRenderTargetImpl();
    }

    void GraphicsManager::SetRenderTargetToMain()
    {
        currentSurface = surfaces.end();
        SetRenderTargetToMainImpl();
        SetCameraSizeToCurrentDimensions();
    }

    const Surface* GraphicsManager::GetCurrentRenderTarget() const
    {
        if (currentSurface == surfaces.end())
            return nullptr;
        else
            return &*currentSurface;
    }

    void GraphicsManager::RenderSprite(SpriteReference sprite, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderSpriteCameraOffset(
                sprite,
                sprite->bounds.Get().left,
                sprite->bounds.Get().top) :
            
            RenderSpriteImpl(
                sprite,
                sprite->bounds.Get().left,
                sprite->bounds.Get().top);
    }

    void GraphicsManager::RenderSprite(SpriteReference sprite, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderSpriteCameraOffset(
                sprite,
                X,
                Y) :
            
            RenderSpriteImpl(
                sprite,
                X,
                Y);
    }

    void GraphicsManager::RenderSprite(SpriteReference sprite, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderSpriteCameraOffset(
                sprite,
                position.x,
                position.y) :
            
            RenderSpriteImpl(
                sprite,
                position.x,
                position.y);
    }

    void GraphicsManager::RenderCanvasView(CanvasViewReference view, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderCanvasViewCameraOffset(
                view,
                view->bounds.Get().left,
                view->bounds.Get().top) :
            
            RenderCanvasViewImpl(
                view,
                view->bounds.Get().left,
                view->bounds.Get().top);
    }

    void GraphicsManager::RenderCanvasView(CanvasViewReference view, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderCanvasViewCameraOffset(
                view,
                X,
                Y) :
            
            RenderCanvasViewImpl(
                view,
                X,
                Y);
    }

    void GraphicsManager::RenderCanvasView(CanvasViewReference view, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderCanvasViewCameraOffset(
                view,
                position.x,
                position.y) :
            
            RenderCanvasViewImpl(
                view,
                position.x,
                position.y);
    }

    void GraphicsManager::RenderUnknownFragment(RenderFragmentReference fragment, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderUnknownFragmentCameraOffset(
                fragment,
                fragment->bounds.Get().left,
                fragment->bounds.Get().top) :
            
            RenderUnknownFragmentImpl(
                fragment,
                fragment->bounds.Get().left,
                fragment->bounds.Get().top);
    }

    void GraphicsManager::RenderUnknownFragment(RenderFragmentReference fragment, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderUnknownFragmentCameraOffset(
                fragment,
                X,
                Y) :
            
            RenderUnknownFragmentImpl(
                fragment,
                X,
                Y);
    }

    void GraphicsManager::RenderUnknownFragment(RenderFragmentReference fragment, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderUnknownFragmentCameraOffset(
                fragment,
                position.x,
                position.y) :
            
            RenderUnknownFragmentImpl(
                fragment,
                position.x,
                position.y);
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
            cameraSystem->SetSize(
                Size2D(
                    static_cast<Size2D::Value>(set.width),
                    static_cast<Size2D::Value>(set.height)));
        }
    }

    ScreenDimensions GraphicsManager::GetMainDimensions() const
    {
        return GetMainDimensionsImpl();
    }

    GraphicsManager::Dimension GraphicsManager::GetMainWidth() const
    {
        return GetMainDimensions().width;
    }

    GraphicsManager::Dimension GraphicsManager::GetMainHeight() const
    {
        return GetMainDimensions().height;
    }

    ScreenDimensions GraphicsManager::GetCurrentDimensions() const
    {
        if (!IsUsingNonMainSurface())
            return GetMainDimensionsImpl();
        else
            return currentSurface->GetSize();
    }

    GraphicsManager::Dimension GraphicsManager::GetCurrentWidth() const
    {
        return GetCurrentDimensions().width;
    }

    GraphicsManager::Dimension GraphicsManager::GetCurrentHeight() const
    {
        return GetCurrentDimensions().height;
    }

    void GraphicsManager::Present()
    {
        (currentSurface != surfaces.end()) ? currentSurface->Present() : PresentImpl();
    }

    void GraphicsManager::Present(void* windowOverride)
    {
        PresentImpl(windowOverride);
    }

    GraphicsManager::GraphicsManager(ObjectManager& objectManager) :
        objectManager(&objectManager), currentSurface(surfaces.end())
    {
        cameraSystem = objectManager.FindSystem<CameraSystem>();
    }

    bool GraphicsManager::IsUsingNonMainSurface() const
    {
        return currentSurface != surfaces.end();
    }

    void GraphicsManager::RenderSpriteCameraOffset(SpriteReference sprite, float X, float Y)
    {
        RenderSpriteImpl(
            sprite,
            X - cameraSystem->GetTopLeft().x,
            Y - cameraSystem->GetTopLeft().y);
    }

    void GraphicsManager::RenderCanvasViewCameraOffset(CanvasViewReference view, float X, float Y)
    {
        RenderCanvasViewImpl(
            view,
            X - cameraSystem->GetTopLeft().x,
            Y - cameraSystem->GetTopLeft().y);
    }

    void GraphicsManager::RenderUnknownFragmentCameraOffset(RenderFragmentReference fragment, float X, float Y)
    {
        RenderUnknownFragmentImpl(
            fragment,
            X - cameraSystem->GetTopLeft().x,
            Y - cameraSystem->GetTopLeft().y);
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

    void GraphicsManager::SetCameraSizeToCurrentDimensions()
    {
        auto &currentDimensions = GetCurrentDimensions();
        cameraSystem->SetSize(
            Size2D(
                static_cast<Size2D::Value>(currentDimensions.width),
                static_cast<Size2D::Value>(currentDimensions.height)));
    }
}