
#include "NullGraphics.h"

namespace Atmos
{
    class ImageAssetData : public ImageAsset::Data
    {
    public:
        ImageAssetData() = default;
    };

    class ShaderAssetData : public ShaderAsset::Data
    {
    public:
        ShaderAssetData() = default;

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

    ImageAsset NullGraphicsHandler::CreateImageImpl(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        return ImageAsset(new ImageAssetData(), path.GetFileName(), cols, rows, 0, 0);
    }

    ImageAsset NullGraphicsHandler::CreateImageImpl(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows)
    {
        return ImageAsset(new ImageAssetData(), name, cols, rows, 0, 0);
    }

    ShaderAsset NullGraphicsHandler::CreateShaderImpl(const FilePath &path)
    {
        return ShaderAsset(new ShaderAssetData(), path.GetFileName());
    }

    ShaderAsset NullGraphicsHandler::CreateShaderImpl(void *buffer, std::int32_t size, const FileName &name)
    {
        return ShaderAsset(new ShaderAssetData(), name);
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

    void NullGraphicsHandler::RenderSpriteImpl(const Sprite &sprite, float X, float Y)
    {}

    void NullGraphicsHandler::RenderCanvasViewImpl(const CanvasView &view, float X, float Y)
    {}

    void NullGraphicsHandler::RenderUnknownFragmentImpl(const RenderFragment &fragment, float X, float Y)
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