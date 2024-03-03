#pragma once

#include "ImageRender.h"
#include "LineRender.h"
#include "RegionRender.h"
#include "TextRender.h"

#include "MaterialAsset.h"

#include "Size2D.h"

namespace Atmos::Render::Resource
{
    class Surface
    {
    public:
        virtual ~Surface() = 0;

        virtual void StageRender(const ImageRender& imageRender) = 0;
        virtual void StageRender(const LineRender& lineRender) = 0;
        virtual void StageRender(const RegionRender& regionRender) = 0;
        virtual void StageRender(const TextRender& textRender) = 0;

        virtual void DrawFrame(const Spatial::Point2D& mapPosition, const Color& backgroundColor) = 0;
        
        virtual void OnMaterialDestroying(const Arca::Index<Asset::Material>& material) = 0;

        [[nodiscard]] virtual Spatial::Size2D Size() const = 0;
    };
}