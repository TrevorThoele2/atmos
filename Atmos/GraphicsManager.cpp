#include "GraphicsManager.h"

#include "Camera.h"

namespace Atmos::Render
{
    GraphicsManager::~GraphicsManager() = default;

    void GraphicsManager::Reconstruct()
    {
        ReconstructInternals();
    }

    void GraphicsManager::StageRender(const MaterialRender& materialRender)
    {
        renderer->StageRender(materialRender);
    }

    void GraphicsManager::StageRender(const CanvasRender& canvasRender)
    {
        renderer->StageRender(canvasRender);
    }

    void GraphicsManager::StageRender(const Line& line)
    {
        renderer->StageRender(line);
    }

    void GraphicsManager::RenderStaged(const SurfaceData& surface)
    {
        renderer->RenderStaged(surface);
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