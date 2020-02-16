#pragma once

#include "MaterialRender.h"
#include "CanvasRender.h"
#include "LineRender.h"

#include "SurfaceData.h"

namespace Atmos::Render
{
    class Renderer
    {
    public:
        virtual ~Renderer() = 0;

        virtual void StageRender(const MaterialRender& materialRender) = 0;
        virtual void StageRender(const CanvasRender& canvasRender) = 0;
        virtual void StageRender(const LineRender& lineRender) = 0;
        virtual void RenderStaged(const SurfaceData& surface) = 0;
    };
}
