#pragma once

#include "GraphicsManager.h"

namespace Atmos::Render
{
    class NullGraphicsManager final : public GraphicsManager
    {
    public:
        NullGraphicsManager(Logging::Logger& logger);
        NullGraphicsManager(const NullGraphicsManager& arg) = delete;
        NullGraphicsManager& operator=(const NullGraphicsManager& arg) = delete;

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
            void* window,
            Arca::Reliquary& reliquary) override;
        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResourceImpl(
            void* window,
            Arca::Reliquary& reliquary) override;

        void ReconstructInternals(GraphicsReconstructionObjects objects) override;
        [[nodiscard]] bool ShouldReconstructInternals() const override;
    };
}