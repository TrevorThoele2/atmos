#include "Camera.h"

#include "MoveBoundsTo.h"
#include "MoveBoundsBy.h"
#include "MoveBoundsDirection.h"
#include "ScaleBounds.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Render
{
    void Camera::MoveTo(const Spatial::Point3D& to) const
    {
        Owner().Do(Spatial::MoveBoundsTo{ ID(), to });
    }

    void Camera::MoveBy(const Spatial::Point3D& by) const
    {
        Owner().Do(Spatial::MoveBoundsBy{ ID(), by });
    }

    void Camera::MoveDirection(const Spatial::Angle3D& direction, Spatial::Point3D::Value amount) const
    {
        Owner().Do(Spatial::MoveBoundsDirection{ ID(), direction, amount });
    }

    void Camera::ScaleTo(const Spatial::Scalers2D& to) const
    {
        Owner().Do(Spatial::ScaleBounds{ ID(), to });
    }

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

    Camera::Camera(Init init, Spatial::ScreenSize screenSize) :
        ClosedTypedRelic(init),
        bounds(init.Create<Spatial::Bounds>(
            Spatial::Point3D{},
            Spatial::Size2D{1, 1},
            Spatial::Scalers2D
            {
                static_cast<Spatial::Scalers2D::Value>(screenSize.width),
                static_cast<Spatial::Scalers2D::Value>(screenSize.height)
            },
            Spatial::Angle2D{}))
    {}

    Camera::Camera(Init init, Arca::Serialization) :
        ClosedTypedRelic(init), bounds(init.Find<Spatial::Bounds>())
    {}
}