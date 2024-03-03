#include "NullGraphicsManager.h"

namespace Atmos::Render
{
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

    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation() = default;

        void StageRender(const ImageRender& imageRender) override {}
        void StageRender(const LineRender& lineRender) override {}

        void DrawFrame(const Color& backgroundColor) override
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

    std::unique_ptr<Asset::ImageAssetData> NullGraphicsManager::CreateImageDataImpl(
        const Buffer & buffer, const Name & name, const Size2D & size)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::ShaderAssetData> NullGraphicsManager::CreateShaderDataImpl(
        const Buffer & buffer, const Name & name, const String & entryPoint)
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