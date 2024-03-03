#include "Camera.h"

namespace Atmos::Render
{
    Camera::Camera(Init init) : ClosedTypedRelic(init)
    {}

    void Camera::FocusedPosition(const Position3D* to)
    {
        focusedPosition = to;
    }

    const Position3D* Camera::FocusedPosition() const
    {
        return focusedPosition;
    }

    void Camera::ViewOrigin(const Position2D& to)
    {
        viewOrigin = to;
    }

    Position2D Camera::ViewOrigin() const
    {
        return viewOrigin;
    }

    void Camera::Size(const ScreenSize& to)
    {
        size = to;
    }

    ScreenSize Camera::Size() const
    {
        return size;
    }

    AxisAlignedBox2D Camera::ScreenSides() const
    {
        return screenSides;
    }

    void Camera::Zoom(Position3D::Value to)
    {
        zoom = to;
    }

    Position3D::Value Camera::Zoom() const
    {
        return zoom;
    }
}