#pragma once

#include <Arca/Relic.h>

#include "RenderCore.h"
#include "ImageCore.h"
#include "Bounds.h"

#include "Point3D.h"
#include "Angle2D.h"
#include "Scalers2D.h"

namespace Atmos::Render
{
    template<bool mutableBounds>
    class Image
    {
    public:
        using Index = int;
        using BoundsT = std::conditional_t<mutableBounds, Spatial::Bounds, const Spatial::Bounds>;
    public:
        [[nodiscard]] Arca::Index<Asset::Image> Asset() const;
        [[nodiscard]] Index AssetIndex() const;
        [[nodiscard]] Spatial::AxisAlignedBox2D AssetSlice() const;
        [[nodiscard]] Arca::Index<Asset::Material> Material() const;

        [[nodiscard]] Render::Color Color() const;

        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Size2D Size() const;
        [[nodiscard]] Spatial::Angle2D Rotation() const;
    protected:
        Image(
            Arca::RelicInit init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation,
            Spatial::Space boundsSpace);
        Image(Arca::RelicInit init, Arca::Serialization);
    private:
        Arca::RelicInit init;
    private:
        Arca::Index<RenderCore> renderCore;
        Arca::Index<ImageCore> core;
        Arca::Index<BoundsT> bounds;
    };

    template<bool mutableBounds>
    Arca::Index<Asset::Image> Image<mutableBounds>::Asset() const
    {
        return core->asset;
    }

    template<bool mutableBounds>
    auto Image<mutableBounds>::AssetIndex() const -> Index
    {
        return core->assetIndex;
    }

    template<bool mutableBounds>
    Spatial::AxisAlignedBox2D Image<mutableBounds>::AssetSlice() const
    {
        return core->asset->Slice(core->assetIndex);
    }

    template<bool mutableBounds>
    Arca::Index<Asset::Material> Image<mutableBounds>::Material() const
    {
        return renderCore->material;
    }

    template<bool mutableBounds>
    Render::Color Image<mutableBounds>::Color() const
    {
        return renderCore->color;
    }

    template<bool mutableBounds>
    Spatial::Point3D Image<mutableBounds>::Position() const
    {
        return bounds->Position();
    }

    template<bool mutableBounds>
    Spatial::Size2D Image<mutableBounds>::Size() const
    {
        return bounds->Size();
    }

    template<bool mutableBounds>
    Spatial::Angle2D Image<mutableBounds>::Rotation() const
    {
        return bounds->Rotation();
    }
    
    template<bool mutableBounds>
    Image<mutableBounds>::Image(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation,
        Spatial::Space boundsSpace)
        :
        init(init)
    {
        renderCore = init.Create<RenderCore>(material, color);
        core = init.Create<ImageCore>(asset, assetIndex);
        const auto baseSize = asset
            ? asset->SliceSize()
            : Spatial::Size2D{ 0, 0 };
        bounds = init.Create<BoundsT>(boundsSpace, position, baseSize, scalers, rotation);
    }

    template<bool mutableBounds>
    Image<mutableBounds>::Image(Arca::RelicInit init, Arca::Serialization) :
        init(init),
        renderCore(init.Find<RenderCore>()),
        core(init.Find<ImageCore>()),
        bounds(init.Find<BoundsT>())
    {}
}