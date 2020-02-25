#pragma once

#include "ScreenSize.h"

namespace Atmos::Render
{
    class SurfaceData
    {
    public:
        virtual ~SurfaceData() = 0;

        virtual void Present() = 0;

        virtual void Reset() = 0;
        virtual void Release() = 0;

        [[nodiscard]] virtual ScreenSize Size() const = 0;
    };
}