#pragma once

#include "ScreenPosition.h"
#include "ScreenSize.h"
#include "Color.h"

namespace Atmos::Render
{
    class CanvasData
    {
    public:
        virtual ~CanvasData() = 0;

        virtual void Resize(ScreenSize size) = 0;

        virtual void StartPainting() = 0;
        virtual void StopPainting() = 0;
        virtual void PaintPixel(const ScreenPosition& position, const Color& color, ScreenPosition::Value height) = 0;
        virtual void Clear(const Color& color) = 0;

        virtual void Release() = 0;
        virtual void Reset(ScreenSize size) = 0;
    };
}