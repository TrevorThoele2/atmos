#pragma once

#include <Atmos/Renderer.h>

using namespace Atmos;
using namespace Render;

class MockRenderer final : public Renderer
{
public:
    std::vector<MaterialRender> materialRenders{};
public:
    void StageRender(const MaterialRender& materialRender) override;
    void StageRender(const CanvasRender& canvasRender) override;
    void StageRender(const Line& line) override;
    void RenderStaged(const SurfaceData& surface) override;
};