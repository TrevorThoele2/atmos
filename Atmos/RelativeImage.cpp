#include "RelativeImage.h"

namespace Atmos::Render
{
    RelativeImage::RelativeImage(
        Init init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
        const Render::Color& color,
        const Spatial::Point3D& delta,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation)
        :
        Image(
            init,
            asset,
            assetIndex,
            material,
            color,
            Spatial::Point3D{},
            scalers,
            rotation),
        relativeBounds(init.Create<Spatial::RelativeBounds>(delta))
    {}

    RelativeImage::RelativeImage(Init init, Arca::Serialization serialization) :
        Image(init, serialization),
        relativeBounds(init.Find<Spatial::RelativeBounds>())
    {}
}