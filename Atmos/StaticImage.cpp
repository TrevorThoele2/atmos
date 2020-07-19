#include "StaticImage.h"

namespace Atmos::Render
{
    StaticImage::StaticImage(
        Init init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
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

    StaticImage::StaticImage(Init init, Arca::Serialization serialization) :
        Image(init, serialization)
    {}
}