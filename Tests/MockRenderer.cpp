#include "MockRenderer.h"

void MockRenderer::StageRender(const MaterialRender& materialRender)
{
    materialRenders.push_back(materialRender);
}

void MockRenderer::StageRender(const CanvasRender& canvasRender) {}
void MockRenderer::StageRender(const LineRender& lineRender)
{
    lineRenders.push_back(lineRender);
}

void MockRenderer::RenderStaged(const ScreenSize& screenSize, const Color& backgroundColor) {}
void MockRenderer::RenderStaged(const SurfaceData& surface, const Color& backgroundColor) {}