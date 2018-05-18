#pragma once

#include <cstdint>
#include <list>

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "RenderSurface.h"
#include "Canvas.h"

#include "Join.h"

namespace Atmos
{
    class Position2D;
    class FilePath;
    template<class enumT>
    class Flags;
    class Color;

    class RenderFragment;
    class Sprite;
    class LineRender;
    class CanvasView;

    class GraphicsHandlerBase
    {
    public:
        typedef unsigned int Dimension;
        typedef Join2<Dimension> ScreenDimensions;

        enum class Target
        {
            MAIN = 1 << 0,
            STENCIL = 1 << 1
        };

        enum class RenderState
        {
            STENCIL
        };
    private:
        typedef std::list<RenderSurface> RenderSurfaceList;
        RenderSurfaceList renderSurfaces;
        RenderSurfaceList::iterator currentRenderSurface;

        typedef std::list<Canvas> CanvasList;
        CanvasList canvasList;

        ScreenDimensions dimensions;

        virtual void ReinitializeImpl() = 0;
        virtual void SetMainDimensionsImpl(const ScreenDimensions &dimensions) = 0;
        virtual ScreenDimensions GetMainDimensionsImpl() const = 0;

        virtual ImageAsset CreateImageImpl(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows) = 0;
        virtual ImageAsset CreateImageImpl(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows) = 0;
        virtual ShaderAsset CreateShaderImpl(const FilePath &path) = 0;
        virtual ShaderAsset CreateShaderImpl(void *buffer, std::int32_t size, const FileName &name) = 0;
        virtual RenderSurface CreateRenderSurfaceImpl(void *window) = 0;
        virtual Canvas CreateCanvasImpl(const ScreenDimensions &dimensions) = 0;

        virtual bool CanMakeImageImpl(const FilePath &path) const = 0;
        virtual bool CanMakeImageImpl(void *buffer, std::int32_t size) const = 0;

        virtual void ResizeCanvasImpl(Canvas &canvas, const ScreenDimensions &dimensions) = 0;

        virtual void SetRenderTargetImpl(RenderSurface &set) = 0;
        virtual void SetRenderTargetToMainImpl() = 0;
        virtual void ReleaseMainRenderTarget() = 0;
        virtual void ResetMainRenderTarget() = 0;

        virtual void PresentImpl() = 0;
        virtual void PresentImpl(void *windowOverride) = 0;

        void RenderSpriteCameraOffset(const Sprite &sprite, float X, float Y);
        virtual void RenderSpriteImpl(const Sprite &sprite, float X, float Y) = 0;
        void RenderCanvasViewCameraOffset(const CanvasView &view, float X, float Y);
        virtual void RenderCanvasViewImpl(const CanvasView &view, float X, float Y) = 0;
        void RenderUnknownFragmentCameraOffset(const RenderFragment &fragment, float X, float Y);
        virtual void RenderUnknownFragmentImpl(const RenderFragment &fragment, float X, float Y) = 0;
        virtual void RenderLineImpl(const LineRender &line) = 0;

        RenderSurfaceList::iterator FindRenderSurface(RenderSurface &surface);
        void SetCameraSizeToCurrentDimensions();
    protected:
        bool IsUsingNonMainRenderSurface() const;
    public:
        GraphicsHandlerBase();
        virtual ~GraphicsHandlerBase() = 0 {}
        void Reinitialize();

        ImageAsset CreateImage(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);
        ImageAsset CreateImage(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows);
        ShaderAsset CreateShader(const FilePath &path);
        ShaderAsset CreateShader(void *buffer, std::int32_t size, const FileName &name);
        RenderSurface& CreateRenderSurface(void *window);
        Canvas& CreateCanvas(const ScreenDimensions &dimensions);

        void DestroyRenderSurface(RenderSurface &destroy);

        bool CanMakeImage(const FilePath &path) const;
        bool CanMakeImage(void *buffer, std::int32_t size) const;

        void ResizeCanvas(Canvas &canvas, const ScreenDimensions &dimensions);

        void SetRenderTarget(RenderSurface &set);
        void SetRenderTargetToMain();
        const RenderSurface* GetCurrentRenderTarget() const;

        void RenderSprite(const Sprite &sprite, bool offsetWithCamera = true);
        void RenderSprite(const Sprite &sprite, float X, float Y, bool offsetWithCamera = false);
        void RenderSprite(const Sprite &sprite, const Position2D &position, bool offsetWithCamera = false);
        void RenderCanvasView(const CanvasView &view, bool offsetWithCamera = true);
        void RenderCanvasView(const CanvasView &view, float X, float Y, bool offsetWithCamera = false);
        void RenderCanvasView(const CanvasView &view, const Position2D &position, bool offsetWithCamera = false);
        void RenderUnknownFragment(const RenderFragment &fragment, bool offsetWithCamera = true);
        void RenderUnknownFragment(const RenderFragment &fragment, float X, float Y, bool offsetWithCamera = true);
        void RenderUnknownFragment(const RenderFragment &fragment, const Position2D &position, bool offsetWithCamera = true);
        void RenderLine(const LineRender &line);

        virtual void SetFullscreen(bool set) = 0;
        // This only has an effect if the main render target is being used
        void SetMainDimensions(const ScreenDimensions &set);
        ScreenDimensions GetMainDimensions() const;
        Dimension GetMainWidth() const;
        Dimension GetMainHeight() const;

        ScreenDimensions GetCurrentDimensions() const;
        Dimension GetCurrentWidth() const;
        Dimension GetCurrentHeight() const;

        virtual void ClearTarget(const Flags<Target> &target) = 0;
        virtual void ClearTarget(const Flags<Target> &target, const Color &color) = 0;

        virtual void Flush() = 0;
        virtual void SetRenderState(RenderState state, bool value) = 0;

        virtual bool Start() = 0;
        virtual void End() = 0;
        void Present();
        void Present(void *windowOverride);

        virtual void StartSprites(size_t spriteCount = 0) = 0;
        virtual void EndSprites() = 0;
        virtual void StartLines() = 0;
        virtual void EndLines() = 0;

        virtual void StartStencil() = 0;
        virtual void StopStencil() = 0;
    };
}