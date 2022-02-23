#pragma once

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "SurfaceResource.h"
#include "FontAssetResource.h"
#include "AllRenders.h"

#include "GraphicsReconstructionObjects.h"

#include "Buffer.h"
#include "FilePath.h"

#include "Logger.h"

namespace Atmos::Render
{
    class MainSurface;
    struct RenderImage;
    struct RenderLine;
    struct RenderRegion;
    struct RenderText;
    struct UpdateText;

    class GraphicsManager
    {
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
        
        void DrawFrame(const AllRenders& allRenders, const Spatial::Point2D& mapPosition);

        void ResourceDestroying(Asset::Resource::Image& resource);
        void ResourceDestroying(Asset::Resource::Shader& resource);
        void ResourceDestroying(Resource::Surface& resource);

        void PruneResources();

        Buffer CompileShader(const File::Path& filePath);

        void Reconstruct(GraphicsReconstructionObjects objects);
        [[nodiscard]] bool ShouldReconstruct() const;
        
        virtual void SetFullscreen(bool set) = 0;
        virtual void ChangeVerticalSync(bool set) = 0;

        [[nodiscard]] Spatial::Size2D TextBaseSize(
            const String& string, const Asset::Resource::Font& resource, bool bold, bool italics, float wrapWidth) const;
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
        
        virtual void DrawFrameImpl(const AllRenders& allRenders, const Spatial::Point2D& mapPosition) = 0;

        virtual void ResourceDestroyingImpl(Asset::Resource::Image& resource) = 0;
        virtual void ResourceDestroyingImpl(Asset::Resource::Shader& resource) = 0;
        virtual void ResourceDestroyingImpl(Resource::Surface& resource) = 0;

        virtual void PruneResourcesImpl() = 0;

        virtual Buffer CompileShaderImpl(const File::Path& filePath) = 0;

        [[nodiscard]] virtual bool ShouldReconstructInternals() const = 0;
        virtual void ReconstructInternals(GraphicsReconstructionObjects objects) = 0;

        [[nodiscard]] virtual Spatial::Size2D TextBaseSizeImpl(
            const String& string, const Asset::Resource::Font& resource, bool bold, bool italics, float wrapWidth) const = 0;
    protected:
        [[nodiscard]] Logging::Logger& Logger() const;
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
        static const inline TypeName typeName = "Atmos::Render::GraphicsManager";
    };
}