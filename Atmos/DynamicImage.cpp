#include "DynamicImage.h"

#include "MoveBounds.h"
#include "ScaleBounds.h"
#include "RotateBounds.h"

namespace Atmos::Render
{
    void DynamicImage::Position(const Position3D& to) const
    {
        Owner().Do<MoveBounds>(ID(), to);
    }

    void DynamicImage::Scalers(const Scalers2D& to) const
    {
        Owner().Do<ScaleBounds>(ID(), to);
    }

    void DynamicImage::Rotation(const Angle& to) const
    {
        Owner().Do<RotateBounds>(ID(), to);
    }

    DynamicImage::DynamicImage(
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

    DynamicImage::DynamicImage(Init init, Arca::Serialization serialization) :
        Image(init, serialization)
    {}
}
