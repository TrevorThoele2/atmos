#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    class ImageAssetDataImplementation final : public Asset::Resource::Image
    {
    public:
        ImageAssetDataImplementation() = default;

        [[nodiscard]] Asset::ImageSize Size() const
        {
            return Asset::ImageSize();
        }
    };

    class ShaderAssetDataImplementation final : public Asset::Resource::Shader
    {
    public:
        ShaderAssetDataImplementation() = default;
    };

    class SurfaceDataImplementation final : public Resource::Surface
    {
    public:
        SurfaceDataImplementation() = default;

        void StageRender(const ImageRender& imageRender) override {}
        void StageRender(const LineRender& lineRender) override {}
        void StageRender(const RegionRender& regionRender) override {}

        void DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor) override
        {}

        void OnMaterialCreated(const Arca::Index<Asset::Material>& material) override
        {}

        void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) override
        {}

        [[nodiscard]] Spatial::ScreenSize Size() const override
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

    std::unique_ptr<Asset::Resource::Image> NullGraphicsManager::CreateImageResourceImpl(
        const DataBuffer& buffer,
        const Name& name,
        const Asset::ImageSize& size)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::Resource::Shader> NullGraphicsManager::CreateShaderResourceImpl(
        const DataBuffer& buffer, const Name& name)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    std::unique_ptr<Resource::Surface> NullGraphicsManager::CreateMainSurfaceResourceImpl(
        void* window,
        Arca::Reliquary& reliquary)
    {
        return std::make_unique<SurfaceDataImplementation>();
    }

    std::unique_ptr<Resource::Surface> NullGraphicsManager::CreateSurfaceResourceImpl(
        void* window,
        Arca::Reliquary& reliquary)
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