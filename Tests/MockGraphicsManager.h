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

    void SetFullscreen(bool set) override;

    void ChangeVerticalSync(bool set) override;
protected:
    void InitializeImpl() override;

    [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageAssetSize& size,
        const Asset::ImageAssetGridSize& gridSize) override;
    [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
        const Buffer& buffer, const Name& name, const String& entryPoint) override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceDataImpl(
        void* window) override;
    [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceDataImpl(
        void* window) override;
private:
    [[nodiscard]] bool ShouldReconstructInternals() const override;
    void ReconstructInternals(GraphicsReconstructionObjects objects) override;
};