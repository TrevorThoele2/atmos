#include "BoundsCurator.h"

#include "BoundsMoved.h"
#include "BoundsScaled.h"
#include "BoundsRotated.h"

namespace Atmos::Spatial
{
    BoundsCurator::BoundsCurator(Init init) : Curator(init)
    {}

    void BoundsCurator::Handle(const MoveBounds& command)
    {
        DoWithRequiredBounds<Bounds>(
            command.id,
            { [this, command](Bounds& bounds) { DoMovement(bounds, command.to, command.id); } });
    }
    
    void BoundsCurator::Handle(const ScaleBounds& command)
    {
        DoWithRequiredBounds<Bounds>(
            command.id,
            { [this, command](Bounds& bounds) { DoScale(bounds, command.to, command.id); } });
    }

    void BoundsCurator::Handle(const RotateBounds& command)
    {
        DoWithRequiredBounds<Bounds>(
            command.id,
            { [this, command](Bounds& bounds) { DoRotation(bounds, command.to, command.id); } });
    }

    void BoundsCurator::DoMovement(Bounds& bounds, const Point3D& to, Arca::RelicID id)
    {
        const auto previousPosition = bounds.Position();

        bounds.Position(to);

        Owner().Raise(BoundsMoved(id, previousPosition));
    }

    void BoundsCurator::DoScale(Bounds& bounds, const Scalers2D& to, Arca::RelicID id)
    {
        const auto previousScalers = bounds.Scalers();

        bounds.Scalers(to);

        Owner().Raise(BoundsScaled(id, previousScalers));
    }

    void BoundsCurator::DoRotation(Bounds& bounds, const Angle2D& to, Arca::RelicID id)
    {
        const auto previousRotation = bounds.Rotation();

        bounds.Rotation(to);

        Owner().Raise(BoundsRotated(id, previousRotation));
    }
}