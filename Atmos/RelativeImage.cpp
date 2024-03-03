#include "RelativeImage.h"

namespace Atmos::Render
{
    RelativeImage::RelativeImage(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::ImageMaterial> material,
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

    RelativeImage::RelativeImage(Arca::RelicInit init, Arca::Serialization serialization) :
        Image(init, serialization),
        relativeBounds(init.Find<Spatial::RelativeBounds>())
    {}
}