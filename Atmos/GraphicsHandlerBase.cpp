
#include "GraphicsHandlerBase.h"
#include "Camera.h"
#include "Sprite.h"
#include "CanvasView.h"
#include "LightFragment.h"

#include "ShaderRegistry.h"

#include <AGUI/System.h>
#include <AGUI/GraphicsHandlerBase.h>

namespace Atmos
{
    void GraphicsHandlerBase::RenderSpriteCameraOffset(const Sprite &sprite, float X, float Y)
    {
        RenderSpriteImpl(sprite, X - Camera::GetTopLeft().GetX(), Y - Camera::GetTopLeft().GetY());
    }

    void GraphicsHandlerBase::RenderCanvasViewCameraOffset(const CanvasView &view, float X, float Y)
    {
        RenderCanvasViewImpl(view, X - Camera::GetTopLeft().GetX(), Y - Camera::GetTopLeft().GetY());
    }

    void GraphicsHandlerBase::RenderUnknownFragmentCameraOffset(const RenderFragment &fragment, float X, float Y)
    {
        RenderUnknownFragmentImpl(fragment, X - Camera::GetTopLeft().GetX(), Y - Camera::GetTopLeft().GetY());
    }

    GraphicsHandlerBase::RenderSurfaceList::iterator GraphicsHandlerBase::FindRenderSurface(RenderSurface &surface)
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
        Camera::SetSize(Size2D(static_cast<Size2D::ValueT>(currentDimensions.first), static_cast<Size2D::ValueT>(currentDimensions.second)));
    }

    bool GraphicsHandlerBase::IsUsingNonMainRenderSurface() const
    {
        return currentRenderSurface != renderSurfaces.end();
    }

    GraphicsHandlerBase::GraphicsHandlerBase() : currentRenderSurface(renderSurfaces.end())
    {}

    void GraphicsHandlerBase::Reinitialize()
    {
        // Focused render surface
        auto prevRenderSurface = currentRenderSurface;
        SetRenderTargetToMain();

        for (auto &loop : ShaderRegistry::Instance())
            loop.second.GetData()->Release();

        ReleaseMainRenderTarget();
        for (auto &loop : renderSurfaces)
            loop.Release();

        for (auto &loop : canvasList)
            loop.Release();

        agui::System::GetGraphics()->Release();

        ReinitializeImpl();

        agui::System::GetGraphics()->Reset();

        for (auto &loop : ShaderRegistry::Instance())
            loop.second.GetData()->Reset();

        ResetMainRenderTarget();
        for (auto &loop : renderSurfaces)
            loop.Reset();

        for (auto &loop : canvasList)
            loop.Reset();

        // Reset the focused render target
        if (prevRenderSurface != renderSurfaces.end())
            SetRenderTarget(*prevRenderSurface);
    }

    ImageAsset GraphicsHandlerBase::CreateImage(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        return std::move(CreateImageImpl(path, cols, rows));
    }

    ImageAsset GraphicsHandlerBase::CreateImage(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        return std::move(CreateImageImpl(buffer, size, name, cols, rows));
    }

    ShaderAsset GraphicsHandlerBase::CreateShader(const FilePath &path)
    {
        return std::move(CreateShaderImpl(path));
    }

    ShaderAsset GraphicsHandlerBase::CreateShader(void *buffer, std::int32_t size, const FileName &name)
    {
        return std::move(CreateShaderImpl(buffer, size, name));
    }

    RenderSurface& GraphicsHandlerBase::CreateRenderSurface(void *window)
    {
        renderSurfaces.push_back(std::move(CreateRenderSurfaceImpl(window)));
        return renderSurfaces.back();
    }

    Canvas& GraphicsHandlerBase::CreateCanvas(const ScreenDimensions &dimensions)
    {
        canvasList.push_back(std::move(CreateCanvasImpl(dimensions)));
        return canvasList.back();
    }

    void GraphicsHandlerBase::DestroyRenderSurface(RenderSurface &destroy)
    {
        auto found = FindRenderSurface(destroy);
        if (found == renderSurfaces.end())
            return;

        renderSurfaces.erase(found);
    }

    bool GraphicsHandlerBase::CanMakeImage(const FilePath &path) const
    {
        return CanMakeImageImpl(path);
    }

    bool GraphicsHandlerBase::CanMakeImage(void *buffer, std::int32_t size) const
    {
        return CanMakeImageImpl(buffer, size);
    }

    void GraphicsHandlerBase::ResizeCanvas(Canvas &canvas, const ScreenDimensions &dimensions)
    {
        canvas.width = dimensions.first;
        canvas.height = dimensions.second;
        ResizeCanvasImpl(canvas, dimensions);
    }

    void GraphicsHandlerBase::SetRenderTarget(RenderSurface &set)
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

    void GraphicsHandlerBase::RenderSprite(const Sprite &sprite, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderSpriteCameraOffset(sprite, sprite.GetBounds().GetLeft(), sprite.GetBounds().GetTop()) : RenderSpriteImpl(sprite, sprite.GetBounds().GetLeft(), sprite.GetBounds().GetTop());
    }

    void GraphicsHandlerBase::RenderSprite(const Sprite &sprite, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderSpriteCameraOffset(sprite, X, Y) : RenderSpriteImpl(sprite, X, Y);
    }

    void GraphicsHandlerBase::RenderSprite(const Sprite &sprite, const Position2D &position, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderSpriteCameraOffset(sprite, position.GetX(), position.GetY()) : RenderSpriteImpl(sprite, position.GetX(), position.GetY());
    }

    void GraphicsHandlerBase::RenderCanvasView(const CanvasView &view, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderCanvasViewCameraOffset(view, view.GetBounds().GetLeft(), view.GetBounds().GetTop()) : RenderCanvasViewImpl(view, view.GetBounds().GetLeft(), view.GetBounds().GetTop());
    }

    void GraphicsHandlerBase::RenderCanvasView(const CanvasView &view, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderCanvasViewCameraOffset(view, X, Y) : RenderCanvasViewImpl(view, X, Y);
    }

    void GraphicsHandlerBase::RenderCanvasView(const CanvasView &view, const Position2D &position, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderCanvasViewCameraOffset(view, position.GetX(), position.GetY()) : RenderCanvasViewImpl(view, position.GetX(), position.GetY());
    }

    void GraphicsHandlerBase::RenderUnknownFragment(const RenderFragment &fragment, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderUnknownFragmentCameraOffset(fragment, fragment.GetBounds().GetLeft(), fragment.GetBounds().GetTop()) : RenderUnknownFragmentImpl(fragment, fragment.GetBounds().GetLeft(), fragment.GetBounds().GetTop());
    }

    void GraphicsHandlerBase::RenderUnknownFragment(const RenderFragment &fragment, float X, float Y, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderUnknownFragmentCameraOffset(fragment, X, Y) : RenderUnknownFragmentImpl(fragment, X, Y);
    }

    void GraphicsHandlerBase::RenderUnknownFragment(const RenderFragment &fragment, const Position2D &position, bool offsetWithCamera)
    {
        (offsetWithCamera) ? RenderUnknownFragmentCameraOffset(fragment, position.GetX(), position.GetY()) : RenderUnknownFragmentImpl(fragment, position.GetX(), position.GetY());
    }

    void GraphicsHandlerBase::RenderLine(const LineRender &line)
    {
        RenderLineImpl(line);
    }

    void GraphicsHandlerBase::SetMainDimensions(const ScreenDimensions &set)
    {
        if (!IsUsingNonMainRenderSurface())
        {
            dimensions = set;
            SetMainDimensionsImpl(this->dimensions);
            Camera::SetSize(Size2D(static_cast<Size2D::ValueT>(set.first), static_cast<Size2D::ValueT>(set.second)));
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

    void GraphicsHandlerBase::Present(void *windowOverride)
    {
        PresentImpl(windowOverride);
    }
}