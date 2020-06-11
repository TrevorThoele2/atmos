#include "MockGraphicsManager.h"

#include "MockSurfaceData.h"
#include "MockImageAssetData.h"

class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
{
public:
    ShaderAssetDataImplementation() : ShaderAssetData("")
    {}
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

std::unique_ptr<Asset::ImageAssetData> MockGraphicsManager::CreateImageDataImpl(
    const Buffer& buffer,
    const Name& name,
    const Asset::ImageAssetSize& size,
    const Asset::ImageAssetGridSize& gridSize)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> MockGraphicsManager::CreateShaderDataImpl(
    const Buffer& buffer, const Name& name, const String & entryPoint)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

std::unique_ptr<SurfaceData> MockGraphicsManager::CreateMainSurfaceDataImpl(
    void* window)
{
    return std::make_unique<MockSurfaceDataImplementation>();
}

std::unique_ptr<SurfaceData> MockGraphicsManager::CreateSurfaceDataImpl(
    void* window)
{
    return std::make_unique<MockSurfaceDataImplementation>();
}

bool MockGraphicsManager::ShouldReconstructInternals() const
{
    return false;
}

void MockGraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
{}