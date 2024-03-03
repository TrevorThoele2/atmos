
#pragma once

#include "Position3D.h"
#include "Size2D.h"
#include "AxisBoundingBox2D.h"
#include "FrameTimer.h"

#include "Event.h"

namespace Atmos
{
    class Direction;
    class Camera
    {
    public:
        typedef Position3D::ValueT Zoom;
    private:
        typedef const Position3D* Position3DT;

        typedef Position3DT FocusT;
    private:
        // Holder for any potential positions that aren't stored elsewhere (just looking somewhere, etc)
        Position3D basePosition;
        FocusT focusedPosition;

        // View origin is the middle of the screen
        Position2D viewOrigin;
        Size2D size;
        // Sides of the screen
        AxisBoundingBox2D sides;

        Position2D topLeft;

        Zoom zoom;

        void CalculateSides();

        bool IsFocusValid();
    public:
        NullEvent eventMoved;

        Camera() = default;
        Camera(const Camera &arg) = default;
        Camera& operator=(const Camera &arg) = default;

        void Work();

        void SetSize(const Size2D &size);
        void ResetFocus();
        void SetFocus(const Position3D *setTo);
        void SetZoom(Zoom set);
        void SetX(Position3D::ValueT set);
        void SetY(Position3D::ValueT set);
        void SetZ(Position3D::ValueT set);
        Position3D::ValueT GetX() const;
        Position3D::ValueT GetY() const;
        Position3D::ValueT GetZ() const;

        const Position2D& GetTopLeft() const;
        const Position2D& GetViewOrigin() const;
        // Takes into account the zoom level
        Position3D GetViewOrigin3D() const;
        const Size2D& GetSize() const;
        const AxisBoundingBox2D& GetSides() const;

        void Move(Direction direction, Position3D::ValueT by);
        void MoveBy(Position3D::ValueT x, Position3D::ValueT y, Position3D::ValueT z);
        void MoveToInstant(const Position3D &pos);
        void MoveDeltaInstant(const Position3D &delta);
    };
}