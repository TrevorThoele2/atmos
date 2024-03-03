#pragma once

#include <Atmos/Renderer.h>

using namespace Atmos;
using namespace Render;

class MockRenderer final : public Renderer
{
public:
    std::vector<MaterialRender> materialRenders{};
    std::vector<LineRender> lineRenders{};
public:
    void StageRender(const MaterialRender& materialRender) override;
    void StageRender(const CanvasRender& canvasRender) override;
    void StageRender(const LineRender& lineRender) override;
    void RenderStaged(const ScreenSize& screenSize, const Color& backgroundColor) override;
    void RenderStaged(const SurfaceData& surface, const Color& backgroundColor) override;
};