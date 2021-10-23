#pragma once

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "SurfaceResource.h"
#include "TextResource.h"

#include "GraphicsReconstructionObjects.h"

#include "Buffer.h"
#include "FilePath.h"

#include "Logger.h"

namespace Atmos::Render
{
    class MainSurface;

    class GraphicsManager
    {
    public:
        using Dimension = unsigned int;

        enum class RenderState
        {
            Stencil
        };
    public:
        virtual ~GraphicsManager() = 0;

        [[nodiscard]] std::unique_ptr<Asset::Resource::Image> CreateImageResource(
            const Buffer& buffer,
            const Name& name,
            const Spatial::Size2D& size);
        [[nodiscard]] std::unique_ptr<Asset::Resource::Shader> CreateShaderResource(
            const Buffer& buffer, const Name& name);
        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateSurfaceResource(
            void* window);
        [[nodiscard]] std::unique_ptr<Resource::Surface> CreateMainSurfaceResource(
            void* window);
        [[nodiscard]] std::unique_ptr<Resource::Text> CreateTextResource(
            const Buffer& buffer,
            const Spatial::Size2D& size);

        void ResourceDestroying(Asset::Resource::Image& resource);
        void ResourceDestroying(Asset::Resource::Shader& resource);
        void ResourceDestroying(Resource::Surface& resource);
        void ResourceDestroying(Resource::Text& resource);

        void PruneResources();

        Buffer CompileShader(const File::Path& filePath);

        void Reconstruct(GraphicsReconstructionObjects objects);
        [[nodiscard]] bool ShouldReconstruct() const;
        
        virtual void SetFullscreen(bool set) = 0;
        virtual void ChangeVerticalSync(bool set) = 0;
    protected:
        GraphicsManager(Logging::Logger& logger, String typeName);
    protected:
        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Image> CreateImageResourceImpl(
            const Buffer& buffer,
            const Name& name,
            const Spatial::Size2D& size) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::Resource::Shader> CreateShaderResourceImpl(
            const Buffer& buffer, const Name& name) = 0;
        [[nodiscard]] virtual std::unique_ptr<Resource::Surface> CreateMainSurfaceResourceImpl(
            void* window) = 0;
        [[nodiscard]] virtual std::unique_ptr<Resource::Surface> CreateSurfaceResourceImpl(
            void* window) = 0;
        [[nodiscard]] virtual std::unique_ptr<Resource::Text> CreateTextResourceImpl(
            const Buffer& buffer,
            const Spatial::Size2D& size) = 0;

        virtual void ResourceDestroyingImpl(Asset::Resource::Image& resource) {}
        virtual void ResourceDestroyingImpl(Asset::Resource::Shader& resource) {}
        virtual void ResourceDestroyingImpl(Resource::Surface& resource) {}
        virtual void ResourceDestroyingImpl(Resource::Text& resource) {}

        virtual void PruneResourcesImpl() {}

        virtual Buffer CompileShaderImpl(const File::Path& filePath) = 0;

        [[nodiscard]] virtual bool ShouldReconstructInternals() const = 0;
        virtual void ReconstructInternals(GraphicsReconstructionObjects objects) = 0;
    protected:
        Logging::Logger& Logger();
    private:
        String typeName;
        Logging::Logger* logger;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::GraphicsManager>
    {
        static TypeName TypeName() { return "Atmos::Render::GraphicsManager"; }
    };
}