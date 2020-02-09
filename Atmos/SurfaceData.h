#pragma once

#include "ScreenSize.h"
#include "Color.h"

namespace Atmos::Render
{
    class SurfaceData
    {
    public:
        virtual ~SurfaceData() = 0;

        virtual void FullColor(const Color& color) = 0;

        virtual void Present() = 0;

        virtual void Reset() = 0;
        virtual void Release() = 0;

        [[nodiscard]] virtual ScreenSize Size() const = 0;
    };
}