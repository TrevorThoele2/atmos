#pragma once

#include "GraphicsManager.h"

namespace Atmos::Render
{
    class NullGraphicsManager : public GraphicsManager
    {
    public:
        NullGraphicsManager(ObjectManager& objectManager);

        void SetFullscreen(bool set) override;

        void ClearTarget(const Flags<Target>& target) override;
        void ClearTarget(const Flags<Target>& target, const Color& color) override;
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
    private:
        NullGraphicsManager(const NullGraphicsManager& arg) = delete;
        NullGraphicsManager& operator=(const NullGraphicsManager& arg) = delete;

        void ReinitializeImpl() override;
        void SetMainDimensionsImpl(const ScreenDimensions& dimensions) override;
        ScreenDimensions GetMainDimensionsImpl() const override;

        std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const File::Path& path) override;
        std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            void *buffer, std::int32_t size, const File::Name& name) override;
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const File::Path& path) override;
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            void *buffer, std::int32_t size, const File::Name& name) override;
        Surface CreateSurfaceImpl(
            void* window) override;
        Canvas CreateCanvasImpl(
            const ScreenDimensions& dimensions) override;

        bool CanMakeImageImpl(const File::Path& path) const override;
        bool CanMakeImageImpl(void* buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) override;

        void SetRenderTargetImpl(Surface& set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void* windowOverride) override;

        void RenderSpriteImpl(SpriteReference sprite, float X, float Y) override;
        void RenderCanvasViewImpl(CanvasViewReference view, float X, float Y) override;
        void RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y) override;
        void RenderLineImpl(const Line& line) override;
    };
}