#include "MockGraphicsManager.h"

#include "MockSurfaceResource.h"
#include "MockImageAssetResource.h"

class ShaderAssetResourceImplementation final : public Asset::Resource::Shader
{
public:
    ShaderAssetResourceImplementation() = default;
};

bool MockGraphicsManager::IsOk() const
{
    return true;
}

void MockGraphicsManager::SetFullscreen(bool set)
{}

void MockGraphicsManager::ChangeVerticalSync(bool set)
{}

void MockGraphicsManager::InitializeImpl()
{}

std::unique_ptr<Asset::Resource::Image> MockGraphicsManager::CreateImageResourceImpl(
    const Buffer& buffer,
    const Name& name,
    const Asset::ImageSize& size)
{
    return std::make_unique<ImageAssetResourceImplementation>();
}

std::unique_ptr<Asset::Resource::Shader> MockGraphicsManager::CreateShaderResourceImpl(
    const Buffer& buffer, const Name& name)
{
    return std::make_unique<ShaderAssetResourceImplementation>();
}

std::unique_ptr<Resource::Surface> MockGraphicsManager::CreateMainSurfaceResourceImpl(
    void* window)
{
    return std::make_unique<MockSurfaceResourceImplementation>();
}

std::unique_ptr<Resource::Surface> MockGraphicsManager::CreateSurfaceResourceImpl(
    void* window)
{
    return std::make_unique<MockSurfaceResourceImplementation>();
}

bool MockGraphicsManager::ShouldReconstructInternals() const
{
    return false;
}

void MockGraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
{}