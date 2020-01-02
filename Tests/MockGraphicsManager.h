#pragma once

#include <Atmos/GraphicsManager.h>

using namespace Atmos;
using namespace Render;

class MockGraphicsManager final : public GraphicsManager
{
public:
    std::vector<MaterialRender> materialRenders;
public:
    explicit MockGraphicsManager(Arca::Reliquary& reliquary);
    MockGraphicsManager(const MockGraphicsManager& arg) = delete;
    MockGraphicsManager& operator=(const MockGraphicsManager& arg) = delete;

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

    void StartStencil() override;
    void StopStencil() override;
private:
    void ReconstructInternals() override;
    void SetMainDimensionsImpl(const ScreenDimensions& dimensions) override;
    [[nodiscard]] ScreenDimensions MainDimensionsImpl() const override;

    [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
        const File::Path& path) override;
    [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
        void* buffer, std::int32_t size, const File::Name& name) override;
    [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
        const File::Path& path) override;
    [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
        void* buffer, std::int32_t size, const File::Name& name) override;
    [[nodiscard]] Surface CreateSurfaceImpl(
        void* window) override;
    [[nodiscard]] Canvas CreateCanvasImpl(
        const ScreenDimensions& dimensions) override;

    [[nodiscard]] bool CanMakeImageImpl(const File::Path& path) const override;
    [[nodiscard]] bool CanMakeImageImpl(void* buffer, std::int32_t size) const override;

    void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) override;

    void SetRenderTargetImpl(Surface& set) override;
    void SetRenderTargetToMainImpl() override;
    void ReleaseMainRenderTarget() override;
    void ResetMainRenderTarget() override;

    void PresentImpl() override;
    void PresentImpl(void* windowOverride) override;

    void RenderMaterialViewImpl(MaterialRender& materialRender) override;
    void RenderCanvasViewImpl(CanvasRender& canvasRender) override;
    void RenderLineImpl(const Line& line) override;
};