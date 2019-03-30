#pragma once

#include <cstdint>
#include <list>

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "RenderSurface.h"
#include "Canvas.h"

#include "Sprite.h"
#include "CanvasView.h"
#include "ObjectReference.h"

#include "Join.h"

namespace Atmos
{
    class Position2D;
    class FilePath;
    template<class enumT>
    class Flags;
    class Color;

    class LineRender;

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
    public:
        typedef TypedObjectReference<nSprite> SpriteReference;
        typedef TypedObjectReference<CanvasView> CanvasViewReference;
        typedef TypedObjectReference<nRenderFragment> RenderFragmentReference;
    public:
        GraphicsHandlerBase();
        virtual ~GraphicsHandlerBase() = 0 {}
        void Reinitialize();

        std::unique_ptr<ImageAssetData> CreateImageData(const FilePath& path);
        std::unique_ptr<ImageAssetData> CreateImageData(void* buffer, std::int32_t size, const FileName& name);
        std::unique_ptr<ShaderAssetData> CreateShaderData(const FilePath& path);
        std::unique_ptr<ShaderAssetData> CreateShaderData(void* buffer, std::int32_t size, const FileName& name);
        RenderSurface& CreateRenderSurface(void* window);
        Canvas& CreateCanvas(const ScreenDimensions& dimensions);

        void DestroyRenderSurface(RenderSurface& destroy);

        bool CanMakeImage(const FilePath& path) const;
        bool CanMakeImage(void* buffer, std::int32_t size) const;

        void ResizeCanvas(Canvas& canvas, const ScreenDimensions& dimensions);

        void SetRenderTarget(RenderSurface& set);
        void SetRenderTargetToMain();
        const RenderSurface* GetCurrentRenderTarget() const;

        void RenderSprite(SpriteReference sprite, bool offsetWithCamera = true);
        void RenderSprite(SpriteReference sprite, float X, float Y, bool offsetWithCamera = false);
        void RenderSprite(SpriteReference sprite, const Position2D& position, bool offsetWithCamera = false);
        void RenderCanvasView(CanvasViewReference view, bool offsetWithCamera = true);
        void RenderCanvasView(CanvasViewReference view, float X, float Y, bool offsetWithCamera = false);
        void RenderCanvasView(CanvasViewReference view, const Position2D& position, bool offsetWithCamera = false);
        void RenderUnknownFragment(RenderFragmentReference fragment, bool offsetWithCamera = true);
        void RenderUnknownFragment(RenderFragmentReference fragment, float X, float Y, bool offsetWithCamera = true);
        void RenderUnknownFragment(RenderFragmentReference fragment, const Position2D& position, bool offsetWithCamera = true);
        void RenderLine(const LineRender& line);

        virtual void SetFullscreen(bool set) = 0;
        // This only has an effect if the main render target is being used
        void SetMainDimensions(const ScreenDimensions& set);
        ScreenDimensions GetMainDimensions() const;
        Dimension GetMainWidth() const;
        Dimension GetMainHeight() const;

        ScreenDimensions GetCurrentDimensions() const;
        Dimension GetCurrentWidth() const;
        Dimension GetCurrentHeight() const;

        virtual void ClearTarget(const Flags<Target>& target) = 0;
        virtual void ClearTarget(const Flags<Target>& target, const Color& color) = 0;

        virtual void Flush() = 0;
        virtual void SetRenderState(RenderState state, bool value) = 0;

        virtual bool Start() = 0;
        virtual void End() = 0;
        void Present();
        void Present(void* windowOverride);

        virtual void StartSprites(size_t spriteCount = 0) = 0;
        virtual void EndSprites() = 0;
        virtual void StartLines() = 0;
        virtual void EndLines() = 0;

        virtual void StartStencil() = 0;
        virtual void StopStencil() = 0;
    protected:
        bool IsUsingNonMainRenderSurface() const;
    private:
        typedef std::list<RenderSurface> RenderSurfaceList;
        RenderSurfaceList renderSurfaces;
        RenderSurfaceList::iterator currentRenderSurface;

        typedef std::list<Canvas> CanvasList;
        CanvasList canvasList;

        ScreenDimensions dimensions;

        virtual void ReinitializeImpl() = 0;
        virtual void SetMainDimensionsImpl(const ScreenDimensions& dimensions) = 0;
        virtual ScreenDimensions GetMainDimensionsImpl() const = 0;

        virtual std::unique_ptr<ImageAssetData> CreateImageDataImpl(const FilePath& path) = 0;
        virtual std::unique_ptr<ImageAssetData> CreateImageDataImpl(void* buffer, std::int32_t size, const FileName& name) = 0;
        virtual std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(const FilePath& path) = 0;
        virtual std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(void* buffer, std::int32_t size, const FileName& name) = 0;
        virtual RenderSurface CreateRenderSurfaceImpl(void* window) = 0;
        virtual Canvas CreateCanvasImpl(const ScreenDimensions& dimensions) = 0;

        virtual bool CanMakeImageImpl(const FilePath& path) const = 0;
        virtual bool CanMakeImageImpl(void* buffer, std::int32_t size) const = 0;

        virtual void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) = 0;

        virtual void SetRenderTargetImpl(RenderSurface& set) = 0;
        virtual void SetRenderTargetToMainImpl() = 0;
        virtual void ReleaseMainRenderTarget() = 0;
        virtual void ResetMainRenderTarget() = 0;

        virtual void PresentImpl() = 0;
        virtual void PresentImpl(void* windowOverride) = 0;

        void RenderSpriteCameraOffset(SpriteReference sprite, float X, float Y);
        virtual void RenderSpriteImpl(SpriteReference sprite, float X, float Y) = 0;
        void RenderCanvasViewCameraOffset(CanvasViewReference view, float X, float Y);
        virtual void RenderCanvasViewImpl(CanvasViewReference view, float X, float Y) = 0;
        void RenderUnknownFragmentCameraOffset(RenderFragmentReference fragment, float X, float Y);
        virtual void RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y) = 0;
        virtual void RenderLineImpl(const LineRender& line) = 0;

        RenderSurfaceList::iterator FindRenderSurface(RenderSurface& surface);
        void SetCameraSizeToCurrentDimensions();
    };
}