#include "GraphicsManager.h"

#include "Camera.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::Reconstruct(const ScreenSize& screenSize)
    {
        if (!ShouldReconstruct())
            return;

        ReconstructInternals(screenSize);
    }

    bool GraphicsManager::ShouldReconstruct() const
    {
        return ShouldReconstructInternals();
    }

    void GraphicsManager::StageRender(const MaterialRender& materialRender)
    {
        renderer->StageRender(materialRender);
    }

    void GraphicsManager::StageRender(const CanvasRender& canvasRender)
    {
        renderer->StageRender(canvasRender);
    }

    void GraphicsManager::StageRender(const LineRender& lineRender)
    {
        renderer->StageRender(lineRender);
    }

    void GraphicsManager::RenderStaged(const ScreenSize& screenSize, const Color& backgroundColor)
    {
        renderer->RenderStaged(screenSize, backgroundColor);
    }

    void GraphicsManager::RenderStaged(const SurfaceData& surface, const Color& backgroundColor)
    {
        renderer->RenderStaged(surface, backgroundColor);
    }

    GraphicsManager::GraphicsManager(std::unique_ptr<Render::Renderer>&& renderer) :
        renderer(std::move(renderer))
    {}

    Renderer& GraphicsManager::Renderer()
    {
        return *renderer;
    }

    const Renderer& GraphicsManager::Renderer() const
    {
        return *renderer;
    }
}