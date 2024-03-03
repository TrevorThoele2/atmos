#pragma once

#include <Atmos/GraphicsManager.h>

using namespace Atmos;
using namespace Render;

class MockGraphicsManager final : public GraphicsManager
{
public:
    MockGraphicsManager() = default;
    MockGraphicsManager(const MockGraphicsManager& arg) = delete;
    MockGraphicsManager& operator=(const MockGraphicsManager& arg) = delete;

    [[nodiscard]] bool IsOk() const override;

    [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
        const Buffer& buffer, const Name& name, const Size2D& size) override;
    [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
        const Buffer& buffer, const Name& name, const String& entryPoint) override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceData(
        void* window) override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceData(
        void* window) override;
    [[nodiscard]] std::unique_ptr<CanvasData> CreateCanvasData(
        const ScreenSize& size) override;

    void SetFullscreen(bool set) override;

    void ChangeVerticalSync(bool set) override;
protected:
    void InitializeImpl() override;
private:
    [[nodiscard]] bool ShouldReconstructInternals() const override;
    void ReconstructInternals(GraphicsReconstructionObjects objects) override;
};