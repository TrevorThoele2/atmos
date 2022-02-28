#include "StaticImage.h"

namespace Atmos::Render
{
    StaticImage::StaticImage(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        const Render::Material& material,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation)
        :
        Image(
            init,
            asset,
            assetIndex,
            material,
            color,
            position,
            scalers,
            rotation,
            Spatial::Space::World)
    {}

    StaticImage::StaticImage(Arca::RelicInit init, Arca::Serialization serialization) :
        Image(init, serialization)
    {}

    bool StaticImage::operator==(const StaticImage& arg) const
    {
        return Image::operator==(arg);
    }

    bool StaticImage::operator!=(const StaticImage& arg) const
    {
        return !(*this == arg);
    }
}