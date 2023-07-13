#include "DynamicImage.h"

namespace Atmos::Render
{
    DynamicImage::DynamicImage(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        const Arca::Index<Asset::Material>& material,
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

    DynamicImage::DynamicImage(Arca::RelicInit init, Arca::Serialization serialization) :
        Image(init, serialization)
    {}

    bool DynamicImage::operator==(const DynamicImage& arg) const
    {
        return Image::operator==(arg);
    }

    bool DynamicImage::operator!=(const DynamicImage& arg) const
    {
        return !(*this == arg);
    }
}
