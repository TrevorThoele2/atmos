#pragma once

#include "ImageRender.h"
#include "LineRender.h"
#include "RegionRender.h"

#include "ScreenSize.h"

namespace Atmos::Render::Resource
{
    class Surface
    {
    public:
        virtual ~Surface() = 0;

        virtual void StageRender(const ImageRender& imageRender) = 0;
        virtual void StageRender(const LineRender& lineRender) = 0;
        virtual void StageRender(const RegionRender& regionRender) = 0;

        virtual void DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor) = 0;

        [[nodiscard]] virtual ScreenSize Size() const = 0;
    };
}