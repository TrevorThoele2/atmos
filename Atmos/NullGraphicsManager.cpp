#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    class ImageAssetDataImplementation final : public Asset::ImageData
    {
    public:
        ImageAssetDataImplementation() = default;
    };

    class ShaderAssetDataImplementation final : public Asset::ShaderData
    {
    public:
        ShaderAssetDataImplementation() = default;
    };

    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation() = default;

        void StageRender(const ImageRender& imageRender) override {}
        void StageRender(const LineRender& lineRender) override {}

        void DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor) override
        {}

        [[nodiscard]] ScreenSize Size() const override
        {
            return { 0, 0 };
        }
    };

    bool NullGraphicsManager::IsOk() const
    {
        return true;
    }

    void NullGraphicsManager::SetFullscreen(bool set)
    {}

    void NullGraphicsManager::ChangeVerticalSync(bool set)
    {}

    void NullGraphicsManager::InitializeImpl()
    {}

    std::unique_ptr<Asset::ImageData> NullGraphicsManager::CreateImageDataImpl(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageSize& size)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ShaderData> NullGraphicsManager::CreateShaderDataImpl(
        const Buffer& buffer, const Name& name)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    std::unique_ptr<SurfaceData> NullGraphicsManager::CreateMainSurfaceDataImpl(
        void* window)
    {
        return std::make_unique<SurfaceDataImplementation>();
    }

    std::unique_ptr<SurfaceData> NullGraphicsManager::CreateSurfaceDataImpl(
        void* window)
    {
        return std::make_unique<SurfaceDataImplementation>();
    }

    void NullGraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
    {}

    bool NullGraphicsManager::ShouldReconstructInternals() const
    {
        return false;
    }
}