#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    class ImageAssetDataImplementation final : public Asset::Resource::Image
    {
    public:
        ImageAssetDataImplementation() = default;

        [[nodiscard]] Spatial::Size2D Size() const override
        {
            return Spatial::Size2D();
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
        
        [[nodiscard]] Spatial::Size2D Size() const override
        {
            return { 0, 0 };
        }
    };

    NullGraphicsManager::NullGraphicsManager(Logging::Logger& logger) : GraphicsManager(logger, "Null")
    {}
    
    void NullGraphicsManager::SetFullscreen(bool set)
    {}

    void NullGraphicsManager::ChangeVerticalSync(bool set)
    {}

    std::unique_ptr<Asset::Resource::Image> NullGraphicsManager::CreateImageResourceImpl(
        const Buffer& buffer,
        const Name& name,
        const Spatial::Size2D& size)
    {
        return std::make_unique<ImageAssetDataImplementation>();
    }

    std::unique_ptr<Asset::Resource::Shader> NullGraphicsManager::CreateShaderResourceImpl(
        const Buffer& buffer, const Name& name)
    {
        return std::make_unique<ShaderAssetDataImplementation>();
    }

    std::unique_ptr<Resource::Surface> NullGraphicsManager::CreateMainSurfaceResourceImpl(
        void* window)
    {
        return std::make_unique<SurfaceDataImplementation>();
    }

    std::unique_ptr<Resource::Surface> NullGraphicsManager::CreateSurfaceResourceImpl(
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