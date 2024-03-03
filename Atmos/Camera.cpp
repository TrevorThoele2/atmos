
#include "Camera.h"

#include "WorldManager.h"
#include "Fps.h"
#include "AvatarSystem.h"
#include "Environment.h"

#include "GameEnvironment.h"

namespace Atmos
{
    void Camera::CalculateSides()
    {
        auto halfWidth = size.GetWidth() / 2;
        auto halfHeight = size.GetHeight() / 2;

        sides.SetLeft(viewOrigin.GetX() - halfWidth);
        sides.SetRight(viewOrigin.GetX() + halfWidth);
        sides.SetTop(viewOrigin.GetY() - halfHeight);
        sides.SetBottom(viewOrigin.GetY() + halfHeight);

        topLeft.Set(sides.GetLeft(), sides.GetTop());
    }

    bool Camera::IsFocusValid()
    {
        return focusedPosition != nullptr;
    }

    void Camera::Work()
    {
        if (!IsFocusValid())
            return;

        auto prevVO = viewOrigin;
        viewOrigin = *focusedPosition;
        CalculateSides();

        if (prevVO != viewOrigin)
            eventMoved();
    }

    void Camera::SetSize(const Size2D &size)
    {
        Camera::size = size;
        CalculateSides();
    }

    void Camera::ResetFocus()
    {
        focusedPosition = &basePosition;
        Work();
    }

    void Camera::SetFocus(const Position3D *setTo)
    {
        if (!setTo)
        {
            ResetFocus();
            return;
        }

        focusedPosition = setTo;
        Work();
    }

    void Camera::SetZoom(Zoom set)
    {
        zoom = set;
    }

    void Camera::SetX(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetX(set);
    }

    void Camera::SetY(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetY(set);
    }

    void Camera::SetZ(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetZ(set);
    }

    Position3D::ValueT Camera::GetX() const
    {
        return focusedPosition->GetX();
    }

    Position3D::ValueT Camera::GetY() const
    {
        return focusedPosition->GetY();
    }

    Position3D::ValueT Camera::GetZ() const
    {
        return focusedPosition->GetZ();
    }

    const Position2D& Camera::GetTopLeft() const
    {
        return topLeft;
    }

    const Position2D& Camera::GetViewOrigin() const
    {
        return viewOrigin;
    }

    Position3D Camera::GetViewOrigin3D() const
    {
        return Position3D(viewOrigin, zoom);
    }

    const Size2D& Camera::GetSize() const
    {
        return size;
    }

    const AxisBoundingBox2D& Camera::GetSides() const
    {
        return sides;
    }

    void Camera::Move(Direction direction, Position3D::ValueT by)
    {
        ResetFocus();

        auto &lastElapsed = Environment::GetTime().GetLastElapsed();
        decltype(by) normalizedDistance = by * static_cast<float>(lastElapsed.ConvertValue(TimeValueEpoch::SECONDS));

        switch (direction.Get())
        {
        case Direction::UP:
            basePosition.DecrementY(normalizedDistance);
            break;
        case Direction::DOWN:
            basePosition.IncrementY(normalizedDistance);
            break;
        case Direction::LEFT:
            basePosition.DecrementX(normalizedDistance);
            break;
        case Direction::RIGHT:
            basePosition.IncrementX(normalizedDistance);
            break;
        }

        Work();
    }

    void Camera::MoveBy(Position3D::ValueT x, Position3D::ValueT y, Position3D::ValueT z)
    {
        ResetFocus();

        basePosition.IncrementX(x);
        basePosition.IncrementY(y);
        basePosition.IncrementZ(z);

        Work();
    }

    void Camera::MoveToInstant(const Position3D &pos)
    {
        basePosition = pos;
        ResetFocus();

        Work();
    }

    void Camera::MoveDeltaInstant(const Position3D &delta)
    {
        ResetFocus();

        basePosition.IncrementX(delta.GetX());
        basePosition.IncrementY(delta.GetY());
        basePosition.IncrementZ(delta.GetZ());
    }
}