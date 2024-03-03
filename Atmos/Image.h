#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ImageCore.h"
#include "Bounds.h"
#include "ChangeImageCore.h"

namespace Atmos::Render
{
    template<class Derived, bool mutableBounds>
    class Image : public Arca::ClosedTypedRelic<Derived>
    {
    private:
        using BaseT = Arca::ClosedTypedRelic<Derived>;
    public:
        using Index = int;
        using BoundsT = std::conditional_t<mutableBounds, Bounds, const Bounds>;
    public:
        void Asset(Arca::Index<Asset::Image> to);
        [[nodiscard]] Arca::Index<Asset::Image> Asset() const;
        void AssetIndex(Index to) const;
        [[nodiscard]] Index AssetIndex() const;
        [[nodiscard]] AxisAlignedBox2D AssetSlice() const;
        void Material(Arca::Index<Asset::Material> to);
        [[nodiscard]] Arca::Index<Asset::Material> Material() const;

        void Color(Render::Color to) const;
        [[nodiscard]] Render::Color Color() const;

        [[nodiscard]] Position3D Position() const;
        [[nodiscard]] Size2D Size() const;
        [[nodiscard]] AxisAlignedBox2D Box() const;

        [[nodiscard]] Arca::Index<ImageCore> Core() const;
        [[nodiscard]] Arca::Index<BoundsT> Bounds() const;
    public:
        using BaseT::Owner;
        using BaseT::ID;
    protected:
        using Init = typename BaseT::Init;

        Image(Init init);
        Image(
            Init init,
            Arca::Index<Asset::Image> asset,
            ImageCore::Index assetIndex,
            Arca::Index<Asset::Material> material,
            const Render::Color& color,
            const Position3D& position,
            const Scalers2D& scalers);

        using BaseT::FindOrCreate;
    private:
        Arca::Index<ImageCore> core;
        Arca::Index<BoundsT> bounds;
    private:
        template<class CommandT, class MemberT>
        CommandT CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const;
    };

    template<class Derived, bool mutableBounds>
    void Image<Derived, mutableBounds>::Asset(Arca::Index<Asset::Image> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::asset, to);
        Owner().Do<ChangeImageCore>(command);
    }

    template<class Derived, bool mutableBounds>
    Arca::Index<Asset::Image> Image<Derived, mutableBounds>::Asset() const
    {
        return core->asset;
    }

    template<class Derived, bool mutableBounds>
    void Image<Derived, mutableBounds>::AssetIndex(Index to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::assetIndex, to);
        Owner().Do<ChangeImageCore>(command);
    }

    template<class Derived, bool mutableBounds>
    auto Image<Derived, mutableBounds>::AssetIndex() const -> Index
    {
        return core->assetIndex;
    }

    template<class Derived, bool mutableBounds>
    AxisAlignedBox2D Image<Derived, mutableBounds>::AssetSlice() const
    {
        return core->assetSlice;
    }

    template<class Derived, bool mutableBounds>
    Arca::Index<Asset::Material> Image<Derived, mutableBounds>::Material() const
    {
        return core->material;
    }

    template<class Derived, bool mutableBounds>
    void Image<Derived, mutableBounds>::Material(Arca::Index<Asset::Material> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::material, to);
        Owner().Do<ChangeImageCore>(command);
    }

    template<class Derived, bool mutableBounds>
    Render::Color Image<Derived, mutableBounds>::Color() const
    {
        return core->color;
    }

    template<class Derived, bool mutableBounds>
    void Image<Derived, mutableBounds>::Color(Render::Color to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::color, to);
        Owner().Do<ChangeImageCore>(command);
    }

    template<class Derived, bool mutableBounds>
    Position3D Image<Derived, mutableBounds>::Position() const
    {
        return bounds->Position();
    }

    template<class Derived, bool mutableBounds>
    Size2D Image<Derived, mutableBounds>::Size() const
    {
        return bounds->Size();
    }

    template<class Derived, bool mutableBounds>
    AxisAlignedBox2D Image<Derived, mutableBounds>::Box() const
    {
        return bounds->Box();
    }

    template<class Derived, bool mutableBounds>
    Arca::Index<ImageCore> Image<Derived, mutableBounds>::Core() const
    {
        return core;
    }

    template<class Derived, bool mutableBounds>
    auto Image<Derived, mutableBounds>::Bounds() const -> Arca::Index<BoundsT>
    {
        return bounds;
    }

    template<class Derived, bool mutableBounds>
    Image<Derived, mutableBounds>::Image(Init init) :
        Arca::ClosedTypedRelic<Derived>(init),
        core(FindOrCreate<ImageCore>()),
        bounds(FindOrCreate<BoundsT>())
    {}

    template<class Derived, bool mutableBounds>
    Image<Derived, mutableBounds>::Image(
        Init init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
        const Render::Color& color,
        const Position3D& position,
        const Scalers2D& scalers)
        :
        Arca::ClosedTypedRelic<Derived>(init)
    {
        core = FindOrCreate<ImageCore>(asset, assetIndex, material, color);
        const auto baseSize = asset
            ? asset->SliceSize()
            : Size2D{ 0, 0 };
        bounds = FindOrCreate<BoundsT>(position, baseSize, scalers);
    }

    template<class Derived, bool mutableBounds>
    template<class CommandT, class MemberT>
    CommandT Image<Derived, mutableBounds>::CreateModificationCommand(std::optional<MemberT> CommandT::* member, MemberT value) const
    {
        CommandT command;
        command.id = ID();
        (command.*member) = value;
        return command;
    }
}