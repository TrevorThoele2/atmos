#pragma once

#include <Atmos/GraphicsManager.h>
#include <Atmos/RenderImage.h>
#include <Atmos/RenderLine.h>
#include <Atmos/RenderRegion.h>
#include <Atmos/RenderText.h>

using namespace Atmos;
using namespace Render;

class MockGraphicsManager final : public GraphicsManager
{
public:
    std::vector<Atmos::Render::RenderImage> imageRenders;
    std::vector<Atmos::Render::RenderLine> lineRenders;
    std::vector<Atmos::Render::RenderRegion> regionRenders;
    std::vector<Atmos::Render::RenderText> textRenders;
public:
    MockGraphicsManager(Logging::Logger& logger);
    MockGraphicsManager(const MockGraphicsManager& arg) = delete;
    MockGraphicsManager& operator=(const MockGraphicsManager& arg) = delete;

    void SetFullscreen(bool set) override;

    void ChangeVerticalSync(bool set) override;
protected:
    [[nodiscard]] std::unique_ptr<Asset::Resource::Image> CreateImageResourceImpl(
        const Buffer& buffer,
        const Name& name,
        const Spatial::Size2D& size) override;
    [[nodiscard]] std::unique_ptr<Asset::Resource::Shader> CreateShaderResourceImpl(
        const Buffer& buffer, const Name& name) override;
    [[nodiscard]] std::unique_ptr<Resource::Surface> CreateMainSurfaceResourceImpl(
        void* window) override;
    [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResourceImpl(
        void* window) override;
    [[nodiscard]] std::unique_ptr<Resource::Text> CreateTextResourceImpl(
        const Buffer& buffer,
        const Spatial::Size2D& size) override;

    void StageImpl(const RenderImage& render) override;
    void StageImpl(const RenderLine& render) override;
    void StageImpl(const RenderRegion& render) override;
    void StageImpl(const RenderText& render) override;

    void StageImpl(const UpdateText& update) override;

    void DrawFrameImpl(Resource::Surface& surface, const Spatial::Point2D& mapPosition, const Color& backgroundColor, Diagnostics::Statistics::Profile& profile) override;

    void ResourceDestroyingImpl(Asset::Resource::Image& resource) override;
    void ResourceDestroyingImpl(Asset::Resource::Shader& resource) override;
    void ResourceDestroyingImpl(Resource::Surface& resource) override;
    void ResourceDestroyingImpl(Resource::Text& resource) override;

    void PruneResourcesImpl() override;

    Buffer CompileShaderImpl(const File::Path& filePath) override;

    [[nodiscard]] bool ShouldReconstructInternals() const override;
    void ReconstructInternals(GraphicsReconstructionObjects objects) override;
};