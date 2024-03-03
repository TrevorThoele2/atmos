#include "DynamicImage.h"

#include "ChangeImageCore.h"
#include "MoveBounds.h"
#include "ResizeBounds.h"

namespace Atmos::Render
{
    DynamicImage::DynamicImage(Init init)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<ImageCore>();
        bounds = FindOrCreate<Atmos::Bounds>();
    }

    DynamicImage::DynamicImage(
        Init init, const Position3D& position, const Size2D& size)
        :
        ClosedTypedRelic(init)
    {
        core = Create<ImageCore>();
        bounds = Create<Atmos::Bounds>(position, size);
    }

    void DynamicImage::Asset(Arca::Index<Asset::ImageAsset> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::asset, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Arca::Index<Asset::ImageAsset> DynamicImage::Asset() const
    {
        return core->asset;
    }

    void DynamicImage::AssetIndex(Index to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::assetIndex, to);
        Owner().Do<ChangeImageCore>(command);
    }

    auto DynamicImage::AssetIndex() const -> Index
    {
        return core->assetIndex;
    }

    AxisAlignedBox2D DynamicImage::AssetSlice() const
    {
        return core->assetSlice;
    }

    void DynamicImage::Color(Render::Color to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::color, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Render::Color DynamicImage::Color() const
    {
        return core->color;
    }

    void DynamicImage::Material(Arca::Index<Asset::MaterialAsset> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::material, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Arca::Index<Asset::MaterialAsset> DynamicImage::Material() const
    {
        return core->material;
    }

    void DynamicImage::Position(const Position3D& to) const
    {
        Owner().Do<MoveBounds>(ID(), to);
    }

    void DynamicImage::Size(const Size2D& to) const
    {
        Owner().Do<ResizeBounds>(ID(), to);
    }

    Position3D DynamicImage::Position() const
    {
        return bounds->Position();
    }

    Size2D DynamicImage::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox2D DynamicImage::Box() const
    {
        return bounds->Box();
    }

    Arca::Index<ImageCore> DynamicImage::Core() const
    {
        return core;
    }

    Arca::Index<Bounds> DynamicImage::Bounds() const
    {
        return bounds;
    }
}