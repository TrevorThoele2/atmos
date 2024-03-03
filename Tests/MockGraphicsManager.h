#pragma once

#include <Atmos/GraphicsManager.h>
#include "MockRenderer.h"

using namespace Atmos;
using namespace Render;

class MockGraphicsManager final : public GraphicsManager
{
public:
    MockRenderer& renderer;
public:
    MockGraphicsManager();
    MockGraphicsManager(const MockGraphicsManager& arg) = delete;
    MockGraphicsManager& operator=(const MockGraphicsManager& arg) = delete;

    void Initialize(Arca::Reliquary& reliquary) override;

    [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
        const Buffer& buffer, const Name& name) override;
    [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
        const Buffer& buffer, const Name& name) override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceData() override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceData(
        void* window) override;
    [[nodiscard]] std::unique_ptr<CanvasData> CreateCanvasData(
        const ScreenSize& size) override;

    void SetFullscreen(bool set) override;

    void ClearStencil(const Color& color) override;

    void SetRenderState(RenderState state, bool set) override;

    void ChangeVerticalSync(bool set) override;
private:
    void ReconstructInternals() override;
};