#pragma once

#include "GraphicsManager.h"

namespace Atmos::Render
{
    class NullGraphicsManager final : public GraphicsManager
    {
    public:
        NullGraphicsManager() = default;
        NullGraphicsManager(const NullGraphicsManager& arg) = delete;
        NullGraphicsManager& operator=(const NullGraphicsManager& arg) = delete;

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            const Buffer& buffer, const Name& name) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            const Buffer& buffer, const Name& name) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceData(
            void* window) override;
        [[nodiscard]] std::unique_ptr<CanvasData> CreateCanvasData(
            const ScreenSize& size) override;

        void SetFullscreen(bool set) override;

        void SetRenderState(RenderState state, bool set) override;

        void ChangeVerticalSync(bool set) override;
    private:
        void ReconstructInternals(const ScreenSize& screenSize) override;
    };
}