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

    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation() = default;

        void FullColor(const Color& color) override
        {}

        void Present() override
        {}

        void Reset() override
        {}

        void Release() override
        {}

        [[nodiscard]] ScreenSize Size() const override
        {
            return { 0, 0 };
        }
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

        void Release() override
        {}

        void Reset(ScreenSize size) override
        {}
    };

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageData(
        const Buffer& buffer, const Name& name)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderData(
        const Buffer& buffer, const Name& name)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    std::unique_ptr<SurfaceData> NullGraphicsManager::CreateSurfaceData(
        void* window)
    {
        return std::make_unique<SurfaceDataImplementation>();
    }

    std::unique_ptr<CanvasData> NullGraphicsManager::CreateCanvasData(
        const ScreenSize& size)
    {
        return std::make_unique<CanvasDataImplementation>();
    }

    void NullGraphicsManager::SetFullscreen(bool set)
    {}

    void NullGraphicsManager::SetRenderState(RenderState state, bool set)
    {}

    void NullGraphicsManager::ChangeVerticalSync(bool set)
    {}

    void NullGraphicsManager::ReconstructInternals()
    {}
}