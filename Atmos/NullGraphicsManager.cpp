#include "NullGraphicsManager.h"

namespace Atmos::Render
{
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

    NullGraphicsManager::NullGraphicsManager(Arca::Reliquary& reliquary) : GraphicsManager(reliquary)
    {}

    void NullGraphicsManager::SetFullscreen(bool set)
    {}

    void NullGraphicsManager::ClearTarget(const Flags<Target>& target)
    {}

    void NullGraphicsManager::ClearTarget(const Flags<Target>& target, const Color& color)
    {}

    void NullGraphicsManager::Flush()
    {}

    void NullGraphicsManager::SetRenderState(RenderState state, bool set)
    {}

    bool NullGraphicsManager::Start()
    {
        return false;
    }

    void NullGraphicsManager::Stop()
    {}

    void NullGraphicsManager::StartObjects(size_t spriteCount)
    {}

    void NullGraphicsManager::StopObjects()
    {}

    void NullGraphicsManager::StartLines()
    {}

    void NullGraphicsManager::StopLines()
    {}

    void NullGraphicsManager::StartStencil()
    {}

    void NullGraphicsManager::StopStencil()
    {}

    void NullGraphicsManager::ReconstructInternals()
    {}

    void NullGraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
    {}

    ScreenDimensions NullGraphicsManager::MainDimensionsImpl() const
    {
        return { 0, 0 };
    }

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageDataImpl(
        const File::Path& path)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(
        const File::Path& path)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    Surface NullGraphicsManager::CreateSurfaceImpl(
        void* window)
    {
        return Surface(std::make_unique<SurfaceData>());
    }

    Canvas NullGraphicsManager::CreateCanvasImpl(
        const ScreenDimensions& dimensions)
    {
        return Canvas(std::make_unique<CanvasData>(), dimensions.width, dimensions.height);
    }

    bool NullGraphicsManager::CanMakeImageImpl(const File::Path& path) const
    {
        return true;
    }

    bool NullGraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
    {
        return true;
    }

    void NullGraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
    {}

    void NullGraphicsManager::SetRenderTargetImpl(Surface& set)
    {}

    void NullGraphicsManager::SetRenderTargetToMainImpl()
    {}

    void NullGraphicsManager::ReleaseMainRenderTarget()
    {}

    void NullGraphicsManager::ResetMainRenderTarget()
    {}

    void NullGraphicsManager::PresentImpl()
    {}

    void NullGraphicsManager::PresentImpl(void* windowOverride)
    {}

    void NullGraphicsManager::RenderMaterialViewImpl(MaterialRender& materialRender)
    {}

    void NullGraphicsManager::RenderCanvasViewImpl(CanvasRender& canvasRender)
    {}

    void NullGraphicsManager::RenderLineImpl(const Line& line)
    {}
}