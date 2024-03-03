
#include "GraphicsHandlerBase.h"
#include "GameEnvironment.h"
#include "Sprite.h"
#include "CanvasView.h"

#include <AGUI/System.h>
#include <AGUI/GraphicsHandlerBase.h>

namespace Atmos
{
    GraphicsHandlerBase::GraphicsHandlerBase() : currentRenderSurface(renderSurfaces.end())
    {}

    void GraphicsHandlerBase::Reinitialize()
    {
        // Focused render surface
        auto prevRenderSurface = currentRenderSurface;
        SetRenderTargetToMain();

        auto& shaderAssetBatch = GetGlobalObjectManager()->Batch<ShaderAsset>();
        for (auto& loop : shaderAssetBatch)
            loop->Data()->Release();

        ReleaseMainRenderTarget();
        for (auto& loop : renderSurfaces)
            loop.Release();

        for (auto& loop : canvasList)
            loop.Release();

        Agui::System::GetGraphics()->Release();

        ReinitializeImpl();

        Agui::System::GetGraphics()->Reset();

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

    std::unique_ptr<ImageAssetData> GraphicsHandlerBase::CreateImageData(const FilePath& path)
    {
        return CreateImageDataImpl(path);
    }

    std::unique_ptr<ImageAssetData> GraphicsHandlerBase::CreateImageData(void* buffer, std::int32_t size, const FileName& name)
    {
        return CreateImageDataImpl(buffer, size, name);
    }

    std::unique_ptr<ShaderAssetData> GraphicsHandlerBase::CreateShaderData(const FilePath& path)
    {
        return CreateShaderDataImpl(path);
    }

    std::unique_ptr<ShaderAssetData> GraphicsHandlerBase::CreateShaderData(void* buffer, std::int32_t size, const FileName& name)
    {
        return CreateShaderDataImpl(buffer, size, name);
    }

    RenderSurface& GraphicsHandlerBase::CreateRenderSurface(void* window)
    {
        renderSurfaces.push_back(std::move(CreateRenderSurfaceImpl(window)));
        return renderSurfaces.back();
    }

    Canvas& GraphicsHandlerBase::CreateCanvas(const ScreenDimensions& dimensions)
    {
        canvasList.push_back(std::move(CreateCanvasImpl(dimensions)));
        return canvasList.back();
    }

    void GraphicsHandlerBase::DestroyRenderSurface(RenderSurface& destroy)
    {
        auto found = FindRenderSurface(destroy);
        if (found == renderSurfaces.end())
            return;

        renderSurfaces.erase(found);
    }

    bool GraphicsHandlerBase::CanMakeImage(const FilePath& path) const
    {
        return CanMakeImageImpl(path);
    }

    bool GraphicsHandlerBase::CanMakeImage(void* buffer, std::int32_t size) const
    {
        return CanMakeImageImpl(buffer, size);
    }

    void GraphicsHandlerBase::ResizeCanvas(Canvas& canvas, const ScreenDimensions& dimensions)
    {
        canvas.width = dimensions.first;
        canvas.height = dimensions.second;
        ResizeCanvasImpl(canvas, dimensions);
    }

    void GraphicsHandlerBase::SetRenderTarget(RenderSurface& set)
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

    void GraphicsHandlerBase::SetRenderTargetToMain()
    {
        currentRenderSurface = renderSurfaces.end();
        SetRenderTargetToMainImpl();
        SetCameraSizeToCurrentDimensions();
    }

    const RenderSurface* GraphicsHandlerBase::GetCurrentRenderTarget() const
    {
        if (currentRenderSurface == renderSurfaces.end())
            return nullptr;
        else
            return &*currentRenderSurface;
    }

    void GraphicsHandlerBase::RenderSprite(SpriteReference sprite, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderSpriteCameraOffset(
                sprite,
                sprite->bounds.Get().GetLeft(),
                sprite->bounds.Get().GetTop()) :
            
            RenderSpriteImpl(
                sprite,
                sprite->bounds.Get().GetLeft(),
                sprite->bounds.Get().GetTop());
    }

    void GraphicsHandlerBase::RenderSprite(SpriteReference sprite, float X, float Y, bool offsetWithCamera)
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

    void GraphicsHandlerBase::RenderSprite(SpriteReference sprite, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderSpriteCameraOffset(
                sprite,
                position.GetX(),
                position.GetY()) :
            
            RenderSpriteImpl(
                sprite,
                position.GetX(),
                position.GetY());
    }

    void GraphicsHandlerBase::RenderCanvasView(CanvasViewReference view, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderCanvasViewCameraOffset(
                view,
                view->bounds.Get().GetLeft(),
                view->bounds.Get().GetTop()) :
            
            RenderCanvasViewImpl(
                view,
                view->bounds.Get().GetLeft(),
                view->bounds.Get().GetTop());
    }

    void GraphicsHandlerBase::RenderCanvasView(CanvasViewReference view, float X, float Y, bool offsetWithCamera)
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

    void GraphicsHandlerBase::RenderCanvasView(CanvasViewReference view, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderCanvasViewCameraOffset(
                view,
                position.GetX(),
                position.GetY()) :
            
            RenderCanvasViewImpl(
                view,
                position.GetX(),
                position.GetY());
    }

    void GraphicsHandlerBase::RenderUnknownFragment(RenderFragmentReference fragment, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderUnknownFragmentCameraOffset(
                fragment,
                fragment->bounds.Get().GetLeft(),
                fragment->bounds.Get().GetTop()) :
            
            RenderUnknownFragmentImpl(
                fragment,
                fragment->bounds.Get().GetLeft(),
                fragment->bounds.Get().GetTop());
    }

    void GraphicsHandlerBase::RenderUnknownFragment(RenderFragmentReference fragment, float X, float Y, bool offsetWithCamera)
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

    void GraphicsHandlerBase::RenderUnknownFragment(RenderFragmentReference fragment, const Position2D& position, bool offsetWithCamera)
    {
        (offsetWithCamera) ?
            RenderUnknownFragmentCameraOffset(
                fragment,
                position.GetX(),
                position.GetY()) :
            
            RenderUnknownFragmentImpl(
                fragment,
                position.GetX(),
                position.GetY());
    }

    void GraphicsHandlerBase::RenderLine(const LineRender& line)
    {
        RenderLineImpl(line);
    }

    void GraphicsHandlerBase::SetMainDimensions(const ScreenDimensions& set)
    {
        if (!IsUsingNonMainRenderSurface())
        {
            dimensions = set;
            SetMainDimensionsImpl(this->dimensions);
            GameEnvironment::GetCamera().SetSize(Size2D(static_cast<Size2D::ValueT>(set.first), static_cast<Size2D::ValueT>(set.second)));
        }
    }

    GraphicsHandlerBase::ScreenDimensions GraphicsHandlerBase::GetMainDimensions() const
    {
        return std::move(GetMainDimensionsImpl());
    }

    GraphicsHandlerBase::Dimension GraphicsHandlerBase::GetMainWidth() const
    {
        return GetMainDimensions().first;
    }

    GraphicsHandlerBase::Dimension GraphicsHandlerBase::GetMainHeight() const
    {
        return GetMainDimensions().second;
    }

    GraphicsHandlerBase::ScreenDimensions GraphicsHandlerBase::GetCurrentDimensions() const
    {
        if (!IsUsingNonMainRenderSurface())
            return GetMainDimensionsImpl();
        else
            return currentRenderSurface->GetDimensions();
    }

    GraphicsHandlerBase::Dimension GraphicsHandlerBase::GetCurrentWidth() const
    {
        return GetCurrentDimensions().first;
    }

    GraphicsHandlerBase::Dimension GraphicsHandlerBase::GetCurrentHeight() const
    {
        return GetCurrentDimensions().second;
    }

    void GraphicsHandlerBase::Present()
    {
        (currentRenderSurface != renderSurfaces.end()) ? currentRenderSurface->Present() : PresentImpl();
    }

    void GraphicsHandlerBase::Present(void* windowOverride)
    {
        PresentImpl(windowOverride);
    }

    bool GraphicsHandlerBase::IsUsingNonMainRenderSurface() const
    {
        return currentRenderSurface != renderSurfaces.end();
    }

    void GraphicsHandlerBase::RenderSpriteCameraOffset(SpriteReference sprite, float X, float Y)
    {
        RenderSpriteImpl(
            sprite,
            X - GameEnvironment::GetCamera().GetTopLeft().GetX(),
            Y - GameEnvironment::GetCamera().GetTopLeft().GetY());
    }

    void GraphicsHandlerBase::RenderCanvasViewCameraOffset(CanvasViewReference view, float X, float Y)
    {
        RenderCanvasViewImpl(
            view,
            X - GameEnvironment::GetCamera().GetTopLeft().GetX(),
            Y - GameEnvironment::GetCamera().GetTopLeft().GetY());
    }

    void GraphicsHandlerBase::RenderUnknownFragmentCameraOffset(RenderFragmentReference fragment, float X, float Y)
    {
        RenderUnknownFragmentImpl(
            fragment,
            X - GameEnvironment::GetCamera().GetTopLeft().GetX(),
            Y - GameEnvironment::GetCamera().GetTopLeft().GetY());
    }

    GraphicsHandlerBase::RenderSurfaceList::iterator GraphicsHandlerBase::FindRenderSurface(RenderSurface& surface)
    {
        for (auto loop = renderSurfaces.begin(); loop != renderSurfaces.end(); ++loop)
        {
            if (&*loop == &surface)
                return loop;
        }

        return renderSurfaces.end();
    }

    void GraphicsHandlerBase::SetCameraSizeToCurrentDimensions()
    {
        auto &currentDimensions = GetCurrentDimensions();
        GameEnvironment::GetCamera().SetSize(Size2D(static_cast<Size2D::ValueT>(currentDimensions.first), static_cast<Size2D::ValueT>(currentDimensions.second)));
    }
}