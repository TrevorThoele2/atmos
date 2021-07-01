#include "Camera.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    Spatial::Point3D Camera::Position() const
    {
        return bounds->Position();
    }

    Spatial::Size2D Camera::Size() const
    {
        return bounds->Size();
    }

    Spatial::AxisAlignedBox2D Camera::Sides() const
    {
        const auto size = bounds->Size();

        return Spatial::AxisAlignedBox2D
        {
            ToPoint2D(bounds->Position()),
            Spatial::Size2D
            {
                static_cast<Spatial::Size2D::Value>(size.width),
                static_cast<Spatial::Size2D::Value>(size.height)
            }
        };
    }

    Camera::Camera(Arca::RelicInit init, Spatial::Size2D screenSize) :
        bounds(init.Create<Spatial::Bounds>(
            Spatial::Space::World,
            Spatial::Point3D{},
            Spatial::Size2D{1, 1},
            Spatial::Scalers2D
            {
                static_cast<Spatial::Scalers2D::Value>(screenSize.width),
                static_cast<Spatial::Scalers2D::Value>(screenSize.height)
            },
            Spatial::Angle2D{}))
    {}

    Camera::Camera(Arca::RelicInit init, Arca::Serialization) :
        bounds(init.Find<Spatial::Bounds>())
    {}
}