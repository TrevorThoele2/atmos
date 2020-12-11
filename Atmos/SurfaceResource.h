#pragma once

#include "ImageRender.h"
#include "LineRender.h"
#include "RegionRender.h"

#include "ImageMaterialAsset.h"
#include "LineMaterialAsset.h"
#include "RegionMaterialAsset.h"

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

        virtual void OnMaterialCreated(const Arca::Index<Asset::ImageMaterial>& material) = 0;
        virtual void OnMaterialCreated(const Arca::Index<Asset::LineMaterial>& material) = 0;
        virtual void OnMaterialCreated(const Arca::Index<Asset::RegionMaterial>& material) = 0;
        virtual void OnMaterialDestroying(const Arca::Index<Asset::ImageMaterial>& material) = 0;
        virtual void OnMaterialDestroying(const Arca::Index<Asset::LineMaterial>& material) = 0;
        virtual void OnMaterialDestroying(const Arca::Index<Asset::RegionMaterial>& material) = 0;

        [[nodiscard]] virtual Spatial::ScreenSize Size() const = 0;
    };
}