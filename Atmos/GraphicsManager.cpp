
#include "GraphicsManager.h"

#include "CameraSystem.h"

namespace Atmos
{
    GraphicsManager::~GraphicsManager()
    {}

    void GraphicsManager::Reinitialize()
    {
        // Focused render surface
        auto prevRenderSurface = currentRenderSurface;
        SetRenderTargetToMain();

        auto& shaderAssetBatch = objectManager->Batch<ShaderAsset>();
        for (auto& loop : shaderAssetBatch)
            loop->Data()->Release();

        ReleaseMainRenderTarget();
        for (auto& loop : renderSurfaces)
            loop.Release();

        for (auto& loop : canvasList)
            loop.Release();

        ReinitializeImpl();

        for (auto& loop : shaderAssetBatch)
            loop->Data()->Reset();

        ResetMainRenderTarget();
        for (auto& loop : renderSurfaces)
            loop.Reset();

        for (auto& loop : canvasList)
            loop.Reset();

        // Reset the focused render target
        if (prevRenderSurface != renderSurfaces.end())
            SetRenderTarget(*prevRenderSurface);
    }

    std::unique_ptr<ImageAssetData> GraphicsManager::CreateImageData(const FilePath& path)
    {
        return CreateImageDataImpl(path);
    }

    std::unique_ptr<ImageAssetData> GraphicsManager::CreateImageData(void* buffer, std::int32_t size, const FileName& name)
    {
        return CreateImageDataImpl(buffer, size, name);
    }

    std::unique_ptr<ShaderAssetData> GraphicsManager::CreateShaderData(const FilePath& path)
    {
        return CreateShaderDataImpl(path);
    }

    std::unique_ptr<ShaderAssetData> GraphicsManager::CreateShaderData(void* buffer, std::int32_t size, const FileName& name)
    {
        return CreateShaderDataImpl(buffer, size, name);
    }

    RenderSurface& GraphicsManager::CreateRenderSurface(void* window)
    {
        renderSurfaces.push_back(std::move(CreateRenderSurfaceImpl(window)));
        return renderSurfaces.back();
    }

    Canvas& GraphicsManager::CreateCanvas(const ScreenDimensions& dimensions)
    {
        canvasList.push_back(std::move(CreateCanvasImpl(dimensions)));
        return canvasList.back();
    }

    void GraphicsManager::DestroyRenderSurface(RenderSurface& destroy)
    {
        auto found = FindRenderSurface(destroy);
        if (found == renderSurfaces.end())
            return;

        renderSurfaces.erase(found);
    }

    bool GraphicsManager::CanMakeImage(const FilePath& path) const
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

    void GraphicsManager::SetRenderTarget(RenderSurface& set)
    {
        if (currentRenderSurface != renderSurfaces.end() && &*currentRenderSurface == &set)
            return;

        auto foundItr = FindRenderSurface(set);
        if (foundItr == renderSurfaces.end())
            return;

        currentRenderSurface = foundItr;
        SetRenderTargetImpl(*currentRenderSurface);
        SetCameraSizeToCurrentDimensions();
        currentRenderSurface->SetAsRenderTargetImpl();
    }

    void GraphicsManager::SetRenderTargetToMain()
    {
        currentRenderSurface = renderSurfaces.end();
        SetRenderTargetToMainImpl();
        SetCameraSizeToCurrentDimensions();
    }

    const RenderSurface* GraphicsManager::GetCurrentRenderTarget() const
    {
        if (currentRenderSurface == renderSurfaces.end())
            return nullptr;
        else
            return &*currentRenderSurface;
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

    void GraphicsManager::RenderLine(const LineRender& line)
    {
        RenderLineImpl(line);
    }

    void GraphicsManager::SetMainDimensions(const ScreenDimensions& set)
    {
        if (!IsUsingNonMainRenderSurface())
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
        if (!IsUsingNonMainRenderSurface())
            return GetMainDimensionsImpl();
        else
            return currentRenderSurface->GetSize();
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
        (currentRenderSurface != renderSurfaces.end()) ? currentRenderSurface->Present() : PresentImpl();
    }

    void GraphicsManager::Present(void* windowOverride)
    {
        PresentImpl(windowOverride);
    }

    GraphicsManager::GraphicsManager(ObjectManager& objectManager) :
        objectManager(&objectManager), currentRenderSurface(renderSurfaces.end())
    {
        cameraSystem = objectManager.FindSystem<CameraSystem>();
    }

    bool GraphicsManager::IsUsingNonMainRenderSurface() const
    {
        return currentRenderSurface != renderSurfaces.end();
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

    GraphicsManager::RenderSurfaceList::iterator GraphicsManager::FindRenderSurface(RenderSurface& surface)
    {
        for (auto loop = renderSurfaces.begin(); loop != renderSurfaces.end(); ++loop)
        {
            if (&*loop == &surface)
                return loop;
        }

        return renderSurfaces.end();
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