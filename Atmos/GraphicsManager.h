#pragma once

#include <cstdint>
#include <list>

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "Surface.h"
#include "Canvas.h"

#include "ScreenDimensions.h"

#include "Sprite.h"
#include "CanvasView.h"
#include "ObjectReference.h"

#include "FilePath.h"
#include "Flags.h"

namespace Atmos::Render
{
    class Color;
    class Line;
    class CameraSystem;

    class GraphicsManager
    {
    public:
        typedef unsigned int Dimension;

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
        typedef TypedObjectReference<Sprite> SpriteReference;
        typedef TypedObjectReference<CanvasView> CanvasViewReference;
        typedef TypedObjectReference<RenderFragment> RenderFragmentReference;
    public:
        virtual ~GraphicsManager() = 0;
        void Reinitialize();

        std::unique_ptr<Asset::ImageAssetData> CreateImageData(const File::Path& path);
        std::unique_ptr<Asset::ImageAssetData> CreateImageData(void* buffer, std::int32_t size, const File::Name& name);
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(const File::Path& path);
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(void* buffer, std::int32_t size, const File::Name& name);
        Surface& CreateSurface(void* window);
        Canvas& CreateCanvas(const ScreenDimensions& dimensions);

        void DestroySurface(Surface& destroy);

        bool CanMakeImage(const File::Path& path) const;
        bool CanMakeImage(void* buffer, std::int32_t size) const;

        void ResizeCanvas(Canvas& canvas, const ScreenDimensions& dimensions);

        void SetRenderTarget(Surface& set);
        void SetRenderTargetToMain();
        const Surface* GetCurrentRenderTarget() const;

        void RenderSprite(SpriteReference sprite, bool offsetWithCamera = true);
        void RenderSprite(SpriteReference sprite, float X, float Y, bool offsetWithCamera = false);
        void RenderSprite(SpriteReference sprite, const Position2D& position, bool offsetWithCamera = false);
        void RenderCanvasView(CanvasViewReference view, bool offsetWithCamera = true);
        void RenderCanvasView(CanvasViewReference view, float X, float Y, bool offsetWithCamera = false);
        void RenderCanvasView(CanvasViewReference view, const Position2D& position, bool offsetWithCamera = false);
        void RenderUnknownFragment(RenderFragmentReference fragment, bool offsetWithCamera = true);
        void RenderUnknownFragment(RenderFragmentReference fragment, float X, float Y, bool offsetWithCamera = true);
        void RenderUnknownFragment(RenderFragmentReference fragment, const Position2D& position, bool offsetWithCamera = true);
        void RenderLine(const Line& line);

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
        ObjectManager* const objectManager;
    protected:
        GraphicsManager(ObjectManager& objectManager);

        bool IsUsingNonMainSurface() const;
    private:
        typedef std::list<Surface> SurfaceList;
        SurfaceList surfaces;
        SurfaceList::iterator currentSurface;

        typedef std::list<Canvas> CanvasList;
        CanvasList canvasList;

        ScreenDimensions dimensions;
    private:
        virtual void ReinitializeImpl() = 0;
        virtual void SetMainDimensionsImpl(const ScreenDimensions& dimensions) = 0;
        virtual ScreenDimensions GetMainDimensionsImpl() const = 0;

        virtual std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(const File::Path& path) = 0;
        virtual std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(void* buffer, std::int32_t size, const File::Name& name) = 0;
        virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(const File::Path& path) = 0;
        virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(void* buffer, std::int32_t size, const File::Name& name) = 0;
        virtual Surface CreateSurfaceImpl(void* window) = 0;
        virtual Canvas CreateCanvasImpl(const ScreenDimensions& dimensions) = 0;

        virtual bool CanMakeImageImpl(const File::Path& path) const = 0;
        virtual bool CanMakeImageImpl(void* buffer, std::int32_t size) const = 0;

        virtual void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) = 0;

        virtual void SetRenderTargetImpl(Surface& set) = 0;
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
        virtual void RenderLineImpl(const Line& line) = 0;

        SurfaceList::iterator FindSurface(Surface& surface);
        void SetCameraSizeToCurrentDimensions();
    private:
        CameraSystem* cameraSystem;
    };
}