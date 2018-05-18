
#pragma once

#include "Position3D.h"
#include "Size2D.h"
#include "AxisBoundingBox2D.h"
#include "FrameTimer.h"

#include "Event.h"

#include "GameEnvironment.h"
#include "Movement.h"
#include <Affecter\Affecter.h>

namespace Atmos
{
    class Direction;
    class Camera
    {
    public:
        typedef Position3D::ValueT Zoom;
    private:
        union FocusU
        {
            const Position2D *two;
            const Position3D *three;
            FocusU();
        };

        enum class FocusT
        {
            TWO,
            THREE
        };
    private:
        // Holder for any potential positions that aren't stored elsewhere (just looking somewhere, etc)
        static Position2D pos;
        static FocusU focus;
        static FocusT focusType;

        // View origin is the middle of the screen
        static Position2D viewOrigin;
        static Size2D size;
        // Sides of the screen
        static AxisBoundingBox2D sides;

        static Position2D topLeft;

        static Zoom zoom;

        Camera() = default;
        Camera(const Camera &arg) = delete;
        Camera& operator=(const Camera &arg) = delete;

        static void CalculateSides();
        static void SetFocusToPos();

        static bool IsFocusValid();
        // Make sure to check that the focus is valid before calling this, as this does not check
        static Position2D GetFocusAs2D();
    public:
        NullEvent eventMoved;

        static Camera& Instance();
        static void Work();

        static void SetSize(const Size2D &size);
        static void SetFocus(std::nullptr_t setTo);
        static void SetFocus(const Position2D *setTo);
        static void SetFocus(const Position3D *setTo);
        static void SetZoom(Zoom set);

        static const Position2D& GetTopLeft();
        static const Position2D& GetViewOrigin();
        // Takes into account the zoom level
        static Position3D GetViewOrigin3D();
        static const Size2D& GetSize();
        static const AxisBoundingBox2D& GetSides();

        static void Move(const Direction &dir, float distance);
        template<class AffecterType>
        static void MoveTo(const Position2D &pos, const FrameTimer &timer);
        static void MoveToInstant(const Position2D &pos);
        static void MoveDeltaInstant(const Position2D &delta);
        static void MoveToFromScreenPos(const Position2D &pos, const FrameTimer &timer);
    };

    template<class AffecterType>
    void Camera::MoveTo(const Position2D &pos, const FrameTimer &timer)
    {
        SetFocusToPos();
        GameEnvironment::GetAffecterSystem().Create<::affecter::Affecter<AffecterType, affecter::MovementEngine2D>>(timer, &Position2D::operator=, Camera::pos, Camera::pos, pos);
    }
}