#pragma once

#include "ObjectSystem.h"

#include "Direction.h"
#include "Position3D.h"
#include "Size2D.h"
#include "AxisAlignedBox2D.h"

#include "Event.h"

namespace Atmos::Time
{
    class TimeSystem;
}

namespace Atmos::Render
{
    class CameraSystem : public ObjectSystem
    {
    public:
        typedef Position3D::Value Zoom;
    public:
        Event<> eventMoved;
    public:
        CameraSystem(ObjectManager& manager);

        void Work();

        void SetSize(const Size2D& size);
        void ResetFocus();
        void SetFocus(const Position3D* setTo);
        void SetZoom(Zoom set);
        void SetX(Position3D::Value set);
        void SetY(Position3D::Value set);
        void SetZ(Position3D::Value set);
        Position3D::Value GetX() const;
        Position3D::Value GetY() const;
        Position3D::Value GetZ() const;

        const Position2D& GetTopLeft() const;
        const Position2D& GetViewOrigin() const;
        // Takes into account the zoom level
        Position3D GetViewOrigin3D() const;
        const Size2D& GetSize() const;
        const AxisAlignedBox2D& GetSides() const;

        void Move(Direction direction, Position3D::Value by);
        void MoveBy(Position3D::Value x, Position3D::Value y, Position3D::Value z);
        void MoveToInstant(const Position3D& pos);
        void MoveDeltaInstant(const Position3D& delta);
    protected:
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
        AxisAlignedBox2D sides;

        Position2D topLeft;

        Zoom zoom;
    private:
        void CalculateSides();
        bool IsFocusValid();
    private:
        Time::TimeSystem* FindTimeSystem();
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Render::CameraSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Render::CameraSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}