#pragma once

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "SurfaceResource.h"
#include "TextResource.h"

#include "GraphicsReconstructionObjects.h"

#include "Buffer.h"
#include "FilePath.h"

#include "Logger.h"

//DELETEME
#include "DiagnosticsStatistics.h"

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
            const Buffer& buffer, const Spatial::Size2D& size);

        void Stage(const RenderImage& render);
        void Stage(const RenderLine& render);
        void Stage(const RenderRegion& render);
        void Stage(const RenderText& render);

        void Stage(const UpdateText& update);

        void DrawFrame(Resource::Surface& surface, const Spatial::Point2D& mapPosition, const Color& backgroundColor, Diagnostics::Statistics::Profile& profile);

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
            const Buffer& buffer, const Spatial::Size2D& size) = 0;

        virtual void StageImpl(const RenderImage& render) = 0;
        virtual void StageImpl(const RenderLine& render) = 0;
        virtual void StageImpl(const RenderRegion& render) = 0;
        virtual void StageImpl(const RenderText& render) = 0;

        virtual void StageImpl(const UpdateText& update) = 0;

        virtual void DrawFrameImpl(Resource::Surface& surface, const Spatial::Point2D& mapPosition, const Color& backgroundColor, Diagnostics::Statistics::Profile& profile) = 0;

        virtual void ResourceDestroyingImpl(Asset::Resource::Image& resource) = 0;
        virtual void ResourceDestroyingImpl(Asset::Resource::Shader& resource) = 0;
        virtual void ResourceDestroyingImpl(Resource::Surface& resource) = 0;
        virtual void ResourceDestroyingImpl(Resource::Text& resource) = 0;

        virtual void PruneResourcesImpl() = 0;

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
        static const inline TypeName typeName = "Atmos::Render::GraphicsManager";
    };
}