#pragma once

#include <Arca/Relic.h>
#include "ImageCore.h"
#include "Bounds.h"

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
        [[nodiscard]] Arca::Index<Asset::ImageMaterial> Material() const;

        [[nodiscard]] Render::Color Color() const;

        [[nodiscard]] Spatial::Point3D Position() const;
        [[nodiscard]] Spatial::Size2D Size() const;
        [[nodiscard]] Spatial::Angle2D Rotation() const;

        [[nodiscard]] Arca::Index<ImageCore> Core() const;
        [[nodiscard]] Arca::Index<BoundsT> Bounds() const;
    protected:
        Image(
            Arca::RelicInit init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::ImageMaterial> material,
            const Render::Color& color,
            const Spatial::Point3D& position,
            const Spatial::Scalers2D& scalers,
            const Spatial::Angle2D& rotation);
        Image(Arca::RelicInit init, Arca::Serialization);
    private:
        Arca::RelicInit init;
    private:
        Arca::Index<ImageCore> core;
        Arca::Index<BoundsT> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const;
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
    Arca::Index<Asset::ImageMaterial> Image<mutableBounds>::Material() const
    {
        return core->material;
    }

    template<bool mutableBounds>
    Render::Color Image<mutableBounds>::Color() const
    {
        return core->color;
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
    Arca::Index<ImageCore> Image<mutableBounds>::Core() const
    {
        return core;
    }

    template<bool mutableBounds>
    auto Image<mutableBounds>::Bounds() const -> Arca::Index<BoundsT>
    {
        return bounds;
    }

    template<bool mutableBounds>
    Image<mutableBounds>::Image(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::ImageMaterial> material,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation)
        :
        init(init)
    {
        core = init.Create<ImageCore>(asset, assetIndex, material, color);
        const auto baseSize = asset
            ? asset->SliceSize()
            : Spatial::Size2D{ 0, 0 };
        bounds = init.Create<BoundsT>(position, baseSize, scalers, rotation);
    }

    template<bool mutableBounds>
    Image<mutableBounds>::Image(Arca::RelicInit init, Arca::Serialization) :
        init(init),
        core(init.Find<ImageCore>()),
        bounds(init.Find<BoundsT>())
    {}

    template<bool mutableBounds>
    template<class CommandT, class MemberT>
    CommandT Image<mutableBounds>::CreateModificationCommand(
        std::optional<MemberT> CommandT::* member, MemberT value) const
    {
        CommandT command;
        command.id = init.id;
        (command.*member) = value;
        return command;
    }
}