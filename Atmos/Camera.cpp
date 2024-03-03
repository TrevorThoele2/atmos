
#include "Camera.h"

#include "WorldManager.h"
#include "Fps.h"
#include "AvatarSystem.h"
#include "Environment.h"

#include "GameEnvironment.h"
#include <Affecter/SmoothstepType.h>

namespace Atmos
{
    Camera::FocusU::FocusU() : two(nullptr)
    {}

    Position2D Camera::pos(0, 0, true);
    Camera::FocusU Camera::focus;
    Camera::FocusT Camera::focusType = Camera::FocusT::TWO;

    Position2D Camera::viewOrigin;
    Size2D Camera::size;
    AxisBoundingBox2D Camera::sides;
    Position2D Camera::topLeft;

    Camera::Zoom Camera::zoom = 0.0f;

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

    void Camera::SetFocusToPos()
    {
        if (IsFocusValid())
            pos = GetFocusAs2D();

        focus.two = &pos;
        focusType = FocusT::TWO;
    }

    bool Camera::IsFocusValid()
    {
        return (focusType == FocusT::TWO) ? focus.two != nullptr : focus.three != nullptr;
    }

    Position2D Camera::GetFocusAs2D()
    {
        return (focusType == FocusT::TWO) ? *focus.two : *focus.three;
    }

    Camera& Camera::Instance()
    {
        static Camera instance;
        return instance;
    }

    void Camera::Work()
    {
        if (!IsFocusValid())
            return;

        auto prevVO = viewOrigin;
        viewOrigin = GetFocusAs2D();
        CalculateSides();

        if (prevVO != viewOrigin)
            Instance().eventMoved();
    }

    void Camera::SetSize(const Size2D &size)
    {
        Camera::size = size;
        CalculateSides();
    }

    void Camera::SetFocus(std::nullptr_t setTo)
    {
        focus.two = &Camera::pos;
        focusType = FocusT::TWO;
        Work();
    }

    void Camera::SetFocus(const Position2D *setTo)
    {
        if (!setTo)
        {
            SetFocus(nullptr);
            return;
        }

        focus.two = setTo;
        focusType = FocusT::TWO;
        Work();
    }

    void Camera::SetFocus(const Position3D *setTo)
    {
        if (!setTo)
        {
            SetFocus(&Camera::pos);
            return;
        }

        focus.three = setTo;
        focusType = FocusT::THREE;
        Work();
    }

    void Camera::SetZoom(Zoom set)
    {
        zoom = set;
    }

    const Position2D& Camera::GetTopLeft()
    {
        return topLeft;
    }

    const Position2D& Camera::GetViewOrigin()
    {
        return viewOrigin;
    }

    Position3D Camera::GetViewOrigin3D()
    {
        return Position3D(viewOrigin, zoom);
    }

    const Size2D& Camera::GetSize()
    {
        return size;
    }

    const AxisBoundingBox2D& Camera::GetSides()
    {
        return sides;
    }

    void Camera::Move(const Direction &dir, float distance)
    {
        SetFocusToPos();

        auto &lastElapsed = Environment::GetTime().GetLastElapsed();
        decltype(distance) normalizedDistance = distance * static_cast<float>(lastElapsed.ConvertValue(TimeValueEpoch::SECONDS));

        switch (dir.Get())
        {
        case Direction::ValueT::UP:
            pos.DecrementY(normalizedDistance);
            break;
        case Direction::ValueT::DOWN:
            pos.IncrementY(normalizedDistance);
            break;
        case Direction::ValueT::LEFT:
            pos.DecrementX(normalizedDistance);
            break;
        case Direction::ValueT::RIGHT:
            pos.IncrementX(normalizedDistance);
            break;
        }
    }

    void Camera::MoveToInstant(const Position2D &pos)
    {
        Camera::pos = pos;
        focus.two = &Camera::pos;
        focusType = FocusT::TWO;

        Work();
    }

    void Camera::MoveDeltaInstant(const Position2D &delta)
    {
        SetFocusToPos();

        pos.IncrementX(delta.GetX());
        pos.IncrementY(delta.GetY());
    }

    void Camera::MoveToFromScreenPos(const Position2D &pos, const FrameTimer &timer)
    {
        SetFocusToPos();
        GameEnvironment::GetAffecterSystem().Create<::Affecter::Affecter<::Affecter::SmoothstepType, ::Affecter::MovementEngine2D>>(timer, &Position2D::operator=, Camera::pos, Camera::pos, Position2D(pos.GetX() + viewOrigin.GetX(), pos.GetY() + viewOrigin.GetY()));
    }
}