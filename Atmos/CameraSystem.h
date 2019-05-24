#pragma once

#include "ObjectSystem.h"

#include "Direction.h"
#include "Position3D.h"
#include "Size2D.h"
#include "AxisBoundingBox2D.h"

#include "Event.h"

namespace Atmos
{
    class TimeSystem;

    class CameraSystem : public ObjectSystem
    {
    public:
        typedef Position3D::ValueT Zoom;
    public:
        Event<> eventMoved;
    public:
        CameraSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(CameraSystem);

        void Work();

        void SetSize(const Size2D& size);
        void ResetFocus();
        void SetFocus(const Position3D* setTo);
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
        void MoveToInstant(const Position3D& pos);
        void MoveDeltaInstant(const Position3D& delta);
    private:
        void InitializeImpl() override;
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
    private:
        void CalculateSides();
        bool IsFocusValid();
    private:
        TimeSystem* FindTimeSystem();
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::CameraSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}