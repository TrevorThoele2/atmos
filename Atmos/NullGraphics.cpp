
#include "NullGraphics.h"

namespace Atmos
{
    class ImageAssetDataImplementation : public ImageAssetData
    {
    public:
        ImageAssetDataImplementation() = default;

        std::unique_ptr<ImageAssetData> Clone() const override
        {
            return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation(*this));
        }
    };

    class ShaderAssetDataImplementation : public ShaderAssetData
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

    class RenderSurfaceData : public RenderSurface::Data
    {
    public:
        RenderSurfaceData() = default;

        void SetAsRenderTarget() override
        {}

        void Present() override
        {}

        void Reset() override
        {}

        void Release() override
        {}

        RenderSurface::Size GetSize() override
        {
            return RenderSurface::Size(0, 0);
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

        void PaintPixel(const Canvas::PositionT& position, const Color& color, Canvas::Dimension height) override
        {}

        void Clear(const Color& color) override
        {}

        void Release() override
        {}

        void Reset(Canvas::Dimension width, Canvas::Dimension height) override
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

    std::unique_ptr<ImageAssetData> NullGraphicsManager::CreateImageDataImpl(const FilePath& path)
    {
        return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<ImageAssetData> NullGraphicsManager::CreateImageDataImpl(void* buffer, std::int32_t size, const FileName& name)
    {
        return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(const FilePath& path)
    {
        return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    std::unique_ptr<ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(void* buffer, std::int32_t size, const FileName& name)
    {
        return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    RenderSurface NullGraphicsManager::CreateRenderSurfaceImpl(void* window)
    {
        return RenderSurface(RenderSurface::DataPtr(new RenderSurfaceData()));
    }

    Canvas NullGraphicsManager::CreateCanvasImpl(const ScreenDimensions& dimensions)
    {
        return Canvas(new CanvasData(), dimensions.width, dimensions.height);
    }

    bool NullGraphicsManager::CanMakeImageImpl(const FilePath& path) const
    {
        return true;
    }

    bool NullGraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
    {
        return true;
    }

    void NullGraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
    {}

    void NullGraphicsManager::SetRenderTargetImpl(RenderSurface& set)
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

    void NullGraphicsManager::RenderLineImpl(const LineRender& line)
    {}
}