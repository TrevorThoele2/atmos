#include "StaticImage.h"

#include "ChangeImageCore.h"

namespace Atmos::Render
{
    void StaticImage::Asset(Arca::Index<Asset::ImageAsset> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::asset, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Arca::Index<Asset::ImageAsset> StaticImage::Asset() const
    {
        return core->asset;
    }

    void StaticImage::AssetIndex(Index to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::assetIndex, to);
        Owner().Do<ChangeImageCore>(command);
    }

    auto StaticImage::AssetIndex() const -> Index
    {
        return core->assetIndex;
    }

    void StaticImage::Color(Render::Color to) const
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::color, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Render::Color StaticImage::Color() const
    {
        return core->color;
    }

    void StaticImage::Material(Arca::Index<Asset::MaterialAsset> to)
    {
        const auto command = CreateModificationCommand(&ChangeImageCore::material, to);
        Owner().Do<ChangeImageCore>(command);
    }

    Arca::Index<Asset::MaterialAsset> StaticImage::Material() const
    {
        return core->material;
    }

    AxisAlignedBox2D StaticImage::AssetSlice() const
    {
        return core->assetSlice;
    }

    Position3D StaticImage::Position() const
    {
        return bounds->Position();
    }

    Size2D StaticImage::Size() const
    {
        return bounds->Size();
    }

    AxisAlignedBox2D StaticImage::Box() const
    {
        return bounds->Box();
    }

    Arca::Index<ImageCore> StaticImage::Core() const
    {
        return core;
    }

    Arca::Index<const Bounds> StaticImage::Bounds() const
    {
        return bounds;
    }

    StaticImage::StaticImage(Init init)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<ImageCore>();
        bounds = FindOrCreate<const Atmos::Bounds>();
    }

    StaticImage::StaticImage(Init init, const Position3D& position, const Size2D& size)
        : ClosedTypedRelic(init)
    {
        core = FindOrCreate<ImageCore>();
        bounds = FindOrCreate<const Atmos::Bounds>(position, size);
    }
}