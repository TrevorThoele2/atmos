#include "DynamicImage.h"

#include "MoveBoundsTo.h"
#include "MoveBoundsBy.h"
#include "MoveBoundsDirection.h"
#include "ScaleBounds.h"
#include "RotateBounds.h"

namespace Atmos::Render
{
    void DynamicImage::MoveTo(const Spatial::Point3D& to) const
    {
        Owner().Do<Spatial::MoveBoundsTo>(ID(), to);
    }

    void DynamicImage::MoveBy(const Spatial::Point3D& by) const
    {
        Owner().Do<Spatial::MoveBoundsBy>(ID(), by);
    }

    void DynamicImage::MoveDirection(const Spatial::Direction& direction, Spatial::Point3D::Value amount) const
    {
        Owner().Do<Spatial::MoveBoundsDirection>(ID(), direction, amount);
    }

    void DynamicImage::Scalers(const Spatial::Scalers2D& to) const
    {
        Owner().Do<Spatial::ScaleBounds>(ID(), to);
    }

    void DynamicImage::Rotation(const Spatial::Angle& to) const
    {
        Owner().Do<Spatial::RotateBounds>(ID(), to);
    }

    DynamicImage::DynamicImage(
        Init init,
        Arca::Index<Asset::Image> asset,
        ImageCore::Index assetIndex,
        Arca::Index<Asset::Material> material,
        const Render::Color& color,
        const Spatial::Point3D& position,
        const Spatial::Scalers2D& scalers,
        const Spatial::Angle& rotation)
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
