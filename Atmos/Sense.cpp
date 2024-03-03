
#include "Sense.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Sense)
    {
        scribe(center);
        scribe(enabled);
        scribe(dynamic);
    }

    Sense::Sense(bool enabled) : enabled(enabled), dynamic(true)
    {}

    Sense::Sense(const Position3D &center, bool enabled) : center(center), enabled(enabled), dynamic(true)
    {}

    Sense::Sense(const Sense &arg) : center(arg.center), enabled(arg.enabled), dynamic(arg.dynamic)
    {}

    Sense& Sense::operator=(const Sense &arg)
    {
        center = arg.center;
        enabled = arg.enabled;
        dynamic = arg.dynamic;
        return *this;
    }

    Sense::Sense(Sense &&arg) : center(std::move(arg.center)), enabled(std::move(arg.enabled)), dynamic(std::move(arg.dynamic)), movementAffecter(std::move(arg.movementAffecter))
    {}

    Sense& Sense::operator=(Sense &&arg)
    {
        center = std::move(arg.center);
        enabled = std::move(arg.enabled);
        dynamic = std::move(arg.dynamic);
        movementAffecter = std::move(arg.movementAffecter);
        return *this;
    }

    Sense::~Sense()
    {
        if (movementAffecter)
            movementAffecter->Stop();
    }

    bool Sense::operator==(const Sense &arg) const
    {
        return center == arg.center;
    }

    bool Sense::operator!=(const Sense &arg) const
    {
        return !(*this == arg);
    }

    void Sense::SetPosition(const Position3D &set)
    {
        if (IsStatic())
            return;

        if (center == set)
            return;

        center = set;
        OnPositionChanged();
    }

    void Sense::SetX(Position3D::ValueT set)
    {
        if (IsStatic())
            return;

        if (center.GetX() == set)
            return;

        center.SetX(set);
        OnPositionChanged();
    }

    void Sense::SetY(Position3D::ValueT set)
    {
        if (IsStatic())
            return;

        if (center.GetY() == set)
            return;

        center.SetY(set);
        OnPositionChanged();
    }

    void Sense::SetZ(Position3D::ValueT set)
    {
        if (IsStatic())
            return;

        if (center.GetZ() == set)
            return;

        center.SetZ(set);
        OnPositionChanged();
    }

    const Position3D& Sense::GetPosition() const
    {
        return center;
    }

    Position3D::ValueT Sense::GetX() const
    {
        return center.GetX();
    }

    Position3D::ValueT Sense::GetY() const
    {
        return center.GetY();
    }

    Position3D::ValueT Sense::GetZ() const
    {
        return center.GetZ();
    }

    bool Sense::IsMoving() const
    {
        if (movementAffecter)
            return movementAffecter->IsActive();
        else
            return false;
    }

    void Sense::SetDynamic(bool set)
    {
        if (!set)
            SetStatic();

        if (dynamic)
            return;

        dynamic = true;
    }

    void Sense::SetStatic(bool set)
    {
        if (!set)
            SetDynamic();

        if (!dynamic)
            return;

        dynamic = false;
    }

    bool Sense::IsDynamic() const
    {
        return dynamic;
    }

    bool Sense::IsStatic() const
    {
        return !dynamic;
    }

    void Sense::Enable(bool set)
    {
        if (enabled == set)
            return;

        enabled = set;
        OnEnabledChanged();
    }

    void Sense::Disable()
    {
        if (!enabled)
            return;

        enabled = false;
        OnEnabledChanged();
    }

    void Sense::ToggleEnabled()
    {
        enabled = !enabled;
        OnEnabledChanged();
    }

    bool Sense::IsEnabled() const
    {
        return enabled;
    }
}