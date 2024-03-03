#pragma once

#include "Position3D.h"
#include "FrameTimer.h"

#include "Serialization.h"

namespace Atmos
{
    // A base class for things that you can sense
    // RenderFragments and Sounds
    class Sense
    {
    public:
        typedef Position3D PositionT;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Position3D center;

        bool enabled;
        bool dynamic;

        virtual void OnPositionChanged() = 0;
        virtual void OnEnabledChanged() = 0;
    public:
        Sense(bool enabled);
        Sense(const Position3D &center, bool enabled);
        Sense(const Sense &arg);
        Sense& operator=(const Sense &arg);
        Sense(Sense &&arg);
        Sense& operator=(Sense &&arg);
		virtual ~Sense() = 0 {}

        bool operator==(const Sense &arg) const;
        bool operator!=(const Sense &arg) const;

        void SetPosition(const Position3D &set);
        void SetX(Position3D::ValueT set);
        void SetY(Position3D::ValueT set);
        void SetZ(Position3D::ValueT set);

        const Position3D& GetPosition() const;
        Position3D::ValueT GetX() const;
        Position3D::ValueT GetY() const;
        Position3D::ValueT GetZ() const;

        void SetDynamic(bool set = true);
        void SetStatic(bool set = true);
        bool IsDynamic() const;
        bool IsStatic() const;

        // If a Sense is enabled, then it will be rendered/sound played
        void Enable(bool set = true);
        void Disable();
        void ToggleEnabled();
        bool IsEnabled() const;
    };
}