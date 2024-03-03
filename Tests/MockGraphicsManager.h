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

    [[nodiscard]] std::unique_ptr<Asset::Resource::Image> CreateImageResourceImpl(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageSize& size) override;
    [[nodiscard]] std::unique_ptr<Asset::Resource::Shader> CreateShaderResourceImpl(
        const Buffer& buffer, const Name& name) override;
    [[nodiscard]] std::unique_ptr<Resource::Surface> CreateMainSurfaceResourceImpl(
        void* window) override;
    [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResourceImpl(
        void* window) override;
private:
    [[nodiscard]] bool ShouldReconstructInternals() const override;
    void ReconstructInternals(GraphicsReconstructionObjects objects) override;
};