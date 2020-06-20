#include "StaticImage.h"

namespace Atmos::Render
{
    StaticImage::StaticImage(Init init) : Image(init)
    {}

    StaticImage::StaticImage(
        Init init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
        const Render::Color& color,
        const Position3D& position,
        const Scalers2D& scalers,
        const Angle& rotation)
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
}