#include "DynamicImage.h"

namespace Atmos::Render
{
    DynamicImage::DynamicImage(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::ImageMaterial> material,
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
            rotation)
    {}

    DynamicImage::DynamicImage(Arca::RelicInit init, Arca::Serialization serialization) :
        Image(init, serialization)
    {}
}
