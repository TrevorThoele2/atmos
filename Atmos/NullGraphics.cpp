
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

        RenderSurface::ScreenDimensions GetDimensions() override
        {
            return RenderSurface::ScreenDimensions(0, 0);
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

        void PaintPixel(const Canvas::PositionT &position, const Color &color, Canvas::Dimension height) override
        {}

        void Clear(const Color &color) override
        {}

        void Release() override
        {}

        void Reset(Canvas::Dimension width, Canvas::Dimension height) override
        {}
    };

    void NullGraphicsHandler::ReinitializeImpl()
    {}

    void NullGraphicsHandler::SetMainDimensionsImpl(const ScreenDimensions &dimensions)
    {}

    NullGraphicsHandler::ScreenDimensions NullGraphicsHandler::GetMainDimensionsImpl() const
    {
        return ScreenDimensions(0, 0);
    }

    std::unique_ptr<ImageAssetData> NullGraphicsHandler::CreateImageDataImpl(const FilePath &path)
    {
        return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<ImageAssetData> NullGraphicsHandler::CreateImageDataImpl(void *buffer, std::int32_t size, const FileName &name)
    {
        return std::unique_ptr<ImageAssetData>(new ImageAssetDataImplementation());
    }

    std::unique_ptr<ShaderAssetData> NullGraphicsHandler::CreateShaderDataImpl(const FilePath &path)
    {
        return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    std::unique_ptr<ShaderAssetData> NullGraphicsHandler::CreateShaderDataImpl(void *buffer, std::int32_t size, const FileName &name)
    {
        return std::unique_ptr<ShaderAssetData>(new ShaderAssetDataImplementation());
    }

    RenderSurface NullGraphicsHandler::CreateRenderSurfaceImpl(void *window)
    {
        return RenderSurface(new RenderSurfaceData());
    }

    Canvas NullGraphicsHandler::CreateCanvasImpl(const ScreenDimensions &dimensions)
    {
        return Canvas(new CanvasData(), dimensions.first, dimensions.second);
    }

    bool NullGraphicsHandler::CanMakeImageImpl(const FilePath &path) const
    {
        return true;
    }

    bool NullGraphicsHandler::CanMakeImageImpl(void *buffer, std::int32_t size) const
    {
        return true;
    }

    void NullGraphicsHandler::ResizeCanvasImpl(Canvas &canvas, const ScreenDimensions &dimensions)
    {}

    void NullGraphicsHandler::SetRenderTargetImpl(RenderSurface &set)
    {}

    void NullGraphicsHandler::SetRenderTargetToMainImpl()
    {}

    void NullGraphicsHandler::ReleaseMainRenderTarget()
    {}

    void NullGraphicsHandler::ResetMainRenderTarget()
    {}

    void NullGraphicsHandler::PresentImpl()
    {}

    void NullGraphicsHandler::PresentImpl(void *windowOverride)
    {}

    void NullGraphicsHandler::RenderSpriteImpl(SpriteReference sprite, float X, float Y)
    {}

    void NullGraphicsHandler::RenderCanvasViewImpl(CanvasViewReference view, float X, float Y)
    {}

    void NullGraphicsHandler::RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y)
    {}

    void NullGraphicsHandler::RenderLineImpl(const LineRender &line)
    {}

    void NullGraphicsHandler::SetFullscreen(bool set)
    {}

    void NullGraphicsHandler::ClearTarget(const Flags<Target> &target)
    {}

    void NullGraphicsHandler::ClearTarget(const Flags<Target> &target, const Color &color)
    {}

    void NullGraphicsHandler::Flush()
    {}

    void NullGraphicsHandler::SetRenderState(RenderState state, bool set)
    {}

    bool NullGraphicsHandler::Start()
    {
        return false;
    }

    void NullGraphicsHandler::End()
    {}

    void NullGraphicsHandler::StartSprites(size_t spriteCount)
    {}

    void NullGraphicsHandler::EndSprites()
    {}

    void NullGraphicsHandler::StartLines()
    {}

    void NullGraphicsHandler::EndLines()
    {}

    void NullGraphicsHandler::StartStencil()
    {}

    void NullGraphicsHandler::StopStencil()
    {}
}