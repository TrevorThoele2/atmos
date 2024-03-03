#pragma once

#include "GraphicsManager.h"

namespace Atmos::Render
{
    class NullGraphicsManager final : public GraphicsManager
    {
    public:
        NullGraphicsManager(const NullGraphicsManager& arg) = delete;
        NullGraphicsManager& operator=(const NullGraphicsManager& arg) = delete;

        [[nodiscard]] bool IsOk() const override;

        void SetFullscreen(bool set) override;

        void ChangeVerticalSync(bool set) override;
    protected:
        void InitializeImpl() override;

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const Buffer& buffer, const Name& name, const Size2D& size) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const Buffer& buffer, const Name& name, const String& entryPoint) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceDataImpl(
            void* window) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceDataImpl(
            void* window) override;

        void ReconstructInternals(GraphicsReconstructionObjects objects) override;
        [[nodiscard]] bool ShouldReconstructInternals() const override;
    };
}