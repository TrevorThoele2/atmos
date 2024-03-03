#include "MockGraphicsManager.h"

#include "MockSurfaceData.h"

class ImageAssetDataImplementation final : public Asset::ImageAssetData
{
public:
    ImageAssetDataImplementation() : ImageAssetData(0, 0)
    {}
};

class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
{
public:
    ShaderAssetDataImplementation() : ShaderAssetData("")
    {}
};

class CanvasDataImplementation final : public CanvasData
{
public:
    CanvasDataImplementation() = default;

    void Resize(ScreenSize size) override
    {}

    void StartPainting() override
    {}

    void StopPainting() override
    {}

    void PaintPixel(
        const ScreenPosition& position,
        const Color& color,
        ScreenPosition::Value height) override
    {}

    void Clear(const Color& color) override
    {}
};

bool MockGraphicsManager::IsOk() const
{
    return true;
}

std::unique_ptr<Asset::ImageAssetData> MockGraphicsManager::CreateImageData(
    const Buffer& buffer, const Name& name, const Size2D& size)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> MockGraphicsManager::CreateShaderData(
    const Buffer& buffer, const Name& name, const String& entryPoint)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

std::unique_ptr<SurfaceData> MockGraphicsManager::CreateMainSurfaceData(
    void* window)
{
    return std::make_unique<MockSurfaceDataImplementation>();
}

std::unique_ptr<SurfaceData> MockGraphicsManager::CreateSurfaceData(
    void* window)
{
    return std::make_unique<MockSurfaceDataImplementation>();
}

std::unique_ptr<CanvasData> MockGraphicsManager::CreateCanvasData(
    const ScreenSize& size)
{
    return std::make_unique<CanvasDataImplementation>();
}

void MockGraphicsManager::SetFullscreen(bool set)
{}

void MockGraphicsManager::ChangeVerticalSync(bool set)
{}

void MockGraphicsManager::InitializeImpl()
{}

bool MockGraphicsManager::ShouldReconstructInternals() const
{
    return false;
}

void MockGraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
{}