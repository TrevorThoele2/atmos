#pragma once

#include "Renderer.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "Canvas.h"

#include "ScreenSize.h"

#include "MaterialRender.h"
#include "CanvasRender.h"
#include "LineRender.h"

#include "Buffer.h"

namespace Atmos::Render
{
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

        virtual void Initialize(Arca::Reliquary& reliquary) = 0;

        void Reconstruct(const ScreenSize& screenSize);
        [[nodiscard]] bool ShouldReconstruct() const;

        [[nodiscard]] virtual bool IsOk() const = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            const Buffer& buffer, const Name& name) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            const Buffer& buffer, const Name& name) = 0;
        [[nodiscard]] virtual std::unique_ptr<SurfaceData> CreateMainSurfaceData() = 0;
        [[nodiscard]] virtual std::unique_ptr<SurfaceData> CreateSurfaceData(
            void* window) = 0;
        [[nodiscard]] virtual std::unique_ptr<CanvasData> CreateCanvasData(
            const ScreenSize& dimensions) = 0;

        void StageRender(const MaterialRender& materialRender);
        void StageRender(const CanvasRender& canvasRender);
        void StageRender(const LineRender& lineRender);
        void RenderStaged(const ScreenSize& screenSize, const Color& backgroundColor);
        void RenderStaged(const SurfaceData& surface, const Color& backgroundColor);

        virtual void SetFullscreen(bool set) = 0;

        virtual void ClearStencil(const Color& color = Color()) = 0;

        virtual void SetRenderState(RenderState state, bool value) = 0;

        virtual void ChangeVerticalSync(bool set) = 0;
    protected:
        explicit GraphicsManager(std::unique_ptr<Renderer>&& renderer);
    protected:
        [[nodiscard]] Renderer& Renderer();
        [[nodiscard]] const Render::Renderer& Renderer() const;
        template<class RendererT>
        [[nodiscard]] RendererT& Renderer();
        template<class RendererT>
        [[nodiscard]] const RendererT& Renderer() const;
    protected:
        [[nodiscard]] virtual bool ShouldReconstructInternals() const = 0;
        virtual void ReconstructInternals(const ScreenSize& screenSize) = 0;
    private:
        std::unique_ptr<Render::Renderer> renderer;
    };

    template<class RendererT>
    RendererT& GraphicsManager::Renderer()
    {
        return static_cast<RendererT&>(Renderer());
    }

    template<class RendererT>
    const RendererT& GraphicsManager::Renderer() const
    {
        return static_cast<const RendererT&>(Renderer());
    }
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::GraphicsManager>
    {
        static inline const TypeName typeName = "GraphicsManager";
    };
}