#pragma once

#include "GraphicsHandlerBase.h"

namespace Atmos
{
    class NullGraphicsHandler : public GraphicsHandlerBase
    {
    private:
        NullGraphicsHandler(const NullGraphicsHandler &arg) = delete;
        NullGraphicsHandler& operator=(const NullGraphicsHandler &arg) = delete;

        void ReinitializeImpl() override;
        void SetMainDimensionsImpl(const ScreenDimensions &dimensions) override;
        ScreenDimensions GetMainDimensionsImpl() const override;

        std::unique_ptr<ImageAssetData> CreateImageDataImpl(const FilePath &path) override;
        std::unique_ptr<ImageAssetData> CreateImageDataImpl(void *buffer, std::int32_t size, const FileName &name) override;
        std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(const FilePath &path) override;
        std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(void *buffer, std::int32_t size, const FileName &name) override;
        RenderSurface CreateRenderSurfaceImpl(void *window) override;
        Canvas CreateCanvasImpl(const ScreenDimensions &dimensions) override;

        bool CanMakeImageImpl(const FilePath &path) const override;
        bool CanMakeImageImpl(void *buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas &canvas, const ScreenDimensions &dimensions) override;

        void SetRenderTargetImpl(RenderSurface &set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void *windowOverride) override;

        void RenderSpriteImpl(SpriteReference sprite, float X, float Y) override;
        void RenderCanvasViewImpl(CanvasViewReference view, float X, float Y) override;
        void RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y) override;
        void RenderLineImpl(const LineRender &line) override;
    public:
        NullGraphicsHandler() = default;

        void SetFullscreen(bool set) override;

        void ClearTarget(const Flags<Target> &target) override;
        void ClearTarget(const Flags<Target> &target, const Color &color) override;
        void Flush() override;
        void SetRenderState(RenderState state, bool set) override;

        bool Start() override;
        void End() override;

        void StartSprites(size_t spriteCount = 0) override;
        void EndSprites() override;
        void StartLines() override;
        void EndLines() override;

        // Stencil
        void StartStencil() override;
        void StopStencil() override;
    };
}