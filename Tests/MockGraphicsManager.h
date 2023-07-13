#pragma once

#include <Atmos/GraphicsManager.h>
#include <Atmos/RasterCommands.h>

using namespace Atmos;
using namespace Render;

class MockGraphicsManager final : public GraphicsManager
{
public:
    std::vector<Raster::Command> commands;
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
    
    void DrawFrameImpl(
        const Raster::Commands& commands,
        const Spatial::Point2D& mapPosition) override;

    void ResourceDestroyingImpl(Asset::Resource::Image& resource) override;
    void ResourceDestroyingImpl(Asset::Resource::Shader& resource) override;
    void ResourceDestroyingImpl(Resource::Surface& resource) override;

    void PruneResourcesImpl() override;

    Buffer CompileShaderImpl(const File::Path& filePath) override;

    [[nodiscard]] bool ShouldReconstructInternals() const override;
    void ReconstructInternals(GraphicsReconstructionObjects objects) override;

    [[nodiscard]] Spatial::Size2D TextBaseSizeImpl(
        const String& string, const Asset::Resource::Font& resource, bool bold, bool italics, float wrapWidth) const override;
};