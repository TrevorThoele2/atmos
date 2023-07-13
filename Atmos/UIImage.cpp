#include "UIImage.h"

namespace Atmos::UI
{
    Image::Image(
        Arca::RelicInit init,
        Arca::Index<Asset::Image> asset,
        Render::ImageCore::Index assetIndex,
        const Arca::Index<Asset::Material>& material,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle2D& rotation)
        :
        Render::Image<true>(
            init,
            asset,
            assetIndex,
            material,
            color,
            position,
            scalers,
            rotation,
            Spatial::Space::Screen)
    {}

    Image::Image(Arca::RelicInit init, Arca::Serialization serialization) :
        Render::Image<true>(init, serialization)
    {}

    bool Image::operator==(const Image& arg) const
    {
        return Render::Image<true>::operator==(arg);
    }

    bool Image::operator!=(const Image& arg) const
    {
        return !(*this == arg);
    }
}
