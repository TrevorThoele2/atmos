#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    class ImageAssetDataImplementation : public Asset::ImageAssetData
    {
    public:
        ImageAssetDataImplementation() = default;

        std::unique_ptr<ImageAssetData> Clone() const override
        {
            return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation(*this));
        }
    };

    class ShaderAssetDataImplementation : public Asset::ShaderAssetData
    {
    public:
        ShaderAssetDataImplementation() = default;

        std::unique_ptr<ShaderAssetData> Clone() const override
        {
            return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation(*this));
        }

        void Reset() override
        {}

        void Release() override
        {}

        PassT Begin() const override
        {
            return 0;
        }

        void End() const override
        {}

        void BeginNextPass(PassT pass) const override
        {}

        void EndPass() const override
        {}
    };

    class SurfaceData : public Surface::Data
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

        Surface::Size GetSize() override
        {
            return Surface::Size(0, 0);
        }
    };

    class CanvasData : public Canvas::Data
    {
    public:
        CanvasData() = default;

        void StartPainting() override
        {}

        void StopPainting() override
        {}

        void PaintPixel(const Canvas::Position& position, const Color& color, Canvas::DimensionValue height) override
        {}

        void Clear(const Color& color) override
        {}

        void Release() override
        {}

        void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override
        {}
    };

    NullGraphicsManager::NullGraphicsManager(ObjectManager& objectManager) : GraphicsManager(objectManager)
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

    void NullGraphicsManager::End()
    {}

    void NullGraphicsManager::StartSprites(size_t spriteCount)
    {}

    void NullGraphicsManager::EndSprites()
    {}

    void NullGraphicsManager::StartLines()
    {}

    void NullGraphicsManager::EndLines()
    {}

    void NullGraphicsManager::StartStencil()
    {}

    void NullGraphicsManager::StopStencil()
    {}

    void NullGraphicsManager::ReinitializeImpl()
    {}

    void NullGraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
    {}

    ScreenDimensions NullGraphicsManager::GetMainDimensionsImpl() const
    {
        return ScreenDimensions(0, 0);
    }

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageDataImpl(
        const File::Path& path)
    {
        return std::unique_ptr<Asset::ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        return std::unique_ptr<Asset::ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(
        const File::Path& path)
    {
        return std::unique_ptr<Asset::ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        return std::unique_ptr<Asset::ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    Surface NullGraphicsManager::CreateSurfaceImpl(
        void* window)
    {
        return Surface(Surface::DataPtr(new SurfaceData()));
    }

    Canvas NullGraphicsManager::CreateCanvasImpl(
        const ScreenDimensions& dimensions)
    {
        return Canvas(Canvas::DataPtr(new CanvasData()), dimensions.width, dimensions.height);
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

    void NullGraphicsManager::RenderSpriteImpl(SpriteReference sprite, float X, float Y)
    {}

    void NullGraphicsManager::RenderCanvasViewImpl(CanvasViewReference view, float X, float Y)
    {}

    void NullGraphicsManager::RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y)
    {}

    void NullGraphicsManager::RenderLineImpl(const Line& line)
    {}
}