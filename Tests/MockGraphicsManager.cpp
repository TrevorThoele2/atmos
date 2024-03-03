#include "MockGraphicsManager.h"

#include "MockSurfaceResource.h"
#include "MockImageAssetResource.h"
#include "MockTextResource.h"

class ShaderAssetResourceImplementation final : public Asset::Resource::Shader
{
public:
    ShaderAssetResourceImplementation() = default;
};

MockGraphicsManager::MockGraphicsManager(Logging::Logger& logger) : GraphicsManager(logger, "Mock")
{}

void MockGraphicsManager::SetFullscreen(bool set)
{}

void MockGraphicsManager::ChangeVerticalSync(bool set)
{}

std::unique_ptr<Asset::Resource::Image> MockGraphicsManager::CreateImageResourceImpl(
    const Buffer& buffer,
    const Name& name,
    const Spatial::Size2D& size)
{
    return std::make_unique<MockImageAssetResource>(size);
}

std::unique_ptr<Asset::Resource::Shader> MockGraphicsManager::CreateShaderResourceImpl(
    const Buffer& buffer, const Name& name)
{
    return std::make_unique<ShaderAssetResourceImplementation>();
}

std::unique_ptr<Resource::Surface> MockGraphicsManager::CreateMainSurfaceResourceImpl(
    void* window,
    Arca::Reliquary& reliquary)
{
    return std::make_unique<MockSurfaceResource>();
}

std::unique_ptr<Resource::Surface> MockGraphicsManager::CreateSurfaceResourceImpl(
    void* window,
    Arca::Reliquary& reliquary)
{
    return std::make_unique<MockSurfaceResource>();
}

std::unique_ptr<Resource::Text> MockGraphicsManager::CreateTextResourceImpl(
    const Buffer& buffer,
    const Spatial::Size2D& size)
{
    return std::make_unique<MockTextResource>();
}

bool MockGraphicsManager::ShouldReconstructInternals() const
{
    return false;
}

void MockGraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
{}