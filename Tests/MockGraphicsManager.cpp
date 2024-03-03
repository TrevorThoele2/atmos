#include "MockGraphicsManager.h"

class ImageAssetDataImplementation final : public Asset::ImageAssetData
{
public:
    ImageAssetDataImplementation() = default;

    [[nodiscard]] std::unique_ptr<ImageAssetData> Clone() const override
    {
        return std::make_unique<ImageAssetDataImplementation>(*this);
    }
};

class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
{
public:
    ShaderAssetDataImplementation() = default;

    [[nodiscard]] std::unique_ptr<ShaderAssetData> Clone() const override
    {
        return std::make_unique<ShaderAssetDataImplementation>(*this);
    }

    void Reset() override
    {}

    void Release() override
    {}

    PassCount Begin() const override
    {
        return 0;
    }

    void End() const override
    {}

    void BeginNextPass(PassCount pass) const override
    {}

    void EndPass() const override
    {}
};

class SurfaceData final : public Surface::Data
{
public:
    SurfaceData() = default;

    void SetAsRenderTarget() override
    {}

    void Present() override
    {}

    void Reset() override
    {}

    void Release() override
    {}

    ScreenDimensions Size() override
    {
        return { 0, 0 };
    }
};

class CanvasData final : public Canvas::Data
{
public:
    CanvasData() = default;

    void StartPainting() override
    {}

    void StopPainting() override
    {}

    void PaintPixel(
        const Canvas::Position& position,
        const Color& color,
        Canvas::DimensionValue height) override
    {}

    void Clear(const Color& color) override
    {}

    void Release() override
    {}

    void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override
    {}
};

MockGraphicsManager::MockGraphicsManager(Arca::Reliquary& reliquary) :
    GraphicsManager(reliquary)
{}

void MockGraphicsManager::SetFullscreen(bool set)
{}

void MockGraphicsManager::ClearTarget(const Chroma::Flags<Target>& target)
{}

void MockGraphicsManager::ClearTarget(const Chroma::Flags<Target>& target, const Color& color)
{}

void MockGraphicsManager::Flush()
{}

void MockGraphicsManager::SetRenderState(RenderState state, bool set)
{}

bool MockGraphicsManager::Start()
{
    return false;
}

void MockGraphicsManager::Stop()
{}

void MockGraphicsManager::StartObjects(size_t count)
{}

void MockGraphicsManager::StopObjects()
{}

void MockGraphicsManager::StartLines()
{}

void MockGraphicsManager::StopLines()
{}

void MockGraphicsManager::StartStencil()
{}

void MockGraphicsManager::StopStencil()
{}

void MockGraphicsManager::ReconstructInternals()
{}

void MockGraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
{}

ScreenDimensions MockGraphicsManager::MainDimensionsImpl() const
{
    return { 0, 0 };
}

std::unique_ptr<Asset::ImageAssetData> MockGraphicsManager::CreateImageDataImpl(
    const File::Path& path)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ImageAssetData> MockGraphicsManager::CreateImageDataImpl(void* buffer,
    std::int32_t size, const File::Name& name)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> MockGraphicsManager::CreateShaderDataImpl(
    const File::Path& path)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> MockGraphicsManager::CreateShaderDataImpl(void* buffer,
    std::int32_t size, const File::Name& name)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

Surface MockGraphicsManager::CreateSurfaceImpl(void* window)
{
    return Surface(std::make_unique<SurfaceData>());
}

Canvas MockGraphicsManager::CreateCanvasImpl(const ScreenDimensions& dimensions)
{
    return Canvas(std::make_unique<CanvasData>(), dimensions.width, dimensions.height);
}

bool MockGraphicsManager::CanMakeImageImpl(const File::Path& path) const
{
    return false;
}

bool MockGraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
{
    return false;
}

void MockGraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
{}

void MockGraphicsManager::SetRenderTargetImpl(Surface& set)
{}

void MockGraphicsManager::SetRenderTargetToMainImpl()
{}

void MockGraphicsManager::ReleaseMainRenderTarget()
{}

void MockGraphicsManager::ResetMainRenderTarget()
{}

void MockGraphicsManager::PresentImpl()
{}

void MockGraphicsManager::PresentImpl(void* windowOverride)
{}

void MockGraphicsManager::RenderMaterialViewImpl(MaterialRender& materialRender)
{
    materialRenders.emplace_back(materialRender);
}

void MockGraphicsManager::RenderCanvasViewImpl(CanvasRender& canvasRender)
{}

void MockGraphicsManager::RenderLineImpl(const Line& line)
{}