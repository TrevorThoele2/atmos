#include "BoundsCurator.h"

#include "BoundsMoved.h"
#include "BoundsScaled.h"
#include "BoundsRotated.h"

#include "SpatialAlgorithms.h"

namespace Atmos::Spatial
{
    BoundsCurator::BoundsCurator(Init init) : Curator(init)
    {
        const auto onBoundsAndRelativeBounds = [this](
            const Arca::MatrixFormed<Arca::All<Bounds, RelativeBounds>>& signal)
        {
            const auto id = signal.index.ID();
            const auto parentBounds = RetrieveParentBounds(id);
            if (!parentBounds)
                return;

            const auto relativeBounds = MutablePointer().Of<RelativeBounds>(id);
            const auto bounds = MutablePointer().Of<Bounds>(id);

            CalculatePosition(*bounds, *relativeBounds, *parentBounds);
        };
        Owner().On<Arca::MatrixFormed<Arca::All<Bounds, RelativeBounds>>>(onBoundsAndRelativeBounds);
    }

    void BoundsCurator::Handle(const MoveBoundsTo& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds) { DoMovement(bounds, command.to, command.id); });
    }

    void BoundsCurator::Handle(const MoveBoundsBy& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds)
            {
                const auto fromPosition = bounds.Position();
                const auto toPosition = fromPosition + command.delta;

                DoMovement(bounds, toPosition, command.id);
            });
    }

    void BoundsCurator::Handle(const MoveBoundsDirection& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds)
            {
                const auto fromPosition = bounds.Position();
                const auto toPosition = fromPosition + ToPoint3D(command.direction, command.amount);

                DoMovement(bounds, toPosition, command.id);
            });
    }

    void BoundsCurator::Handle(const ScaleBounds& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds) { DoScale(bounds, command.to, command.id); });
    }

    void BoundsCurator::Handle(const RotateBounds& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds) { DoRotation(bounds, command.to, command.id); });
    }

    void BoundsCurator::DoMovement(Bounds& bounds, const Point3D& to, Arca::RelicID id)
    {
        const auto relativeBounds = MutablePointer().Of<RelativeBounds>(id);
        if (relativeBounds)
        {
            const auto parentBounds = RetrieveParentBounds(id);
            if (!parentBounds)
                return;

            CalculateRelativePosition(*relativeBounds, *parentBounds, to);
        }

        bounds.Position(to);

        Owner().Raise(BoundsMoved(Arca::Index<Bounds>(id, Owner())));

        UpdateChildrenRelativeBounds(to, id);
    }

    void BoundsCurator::DoScale(Bounds& bounds, const Scalers2D& to, Arca::RelicID id)
    {
        bounds.Scalers(to);

        Owner().Raise(BoundsScaled(Arca::Index<Bounds>(id, Owner())));
    }

    void BoundsCurator::DoRotation(Bounds& bounds, const Angle2D& to, Arca::RelicID id)
    {
        bounds.Rotation(to);

        Owner().Raise(BoundsRotated(Arca::Index<Bounds>(id, Owner())));
    }

    void BoundsCurator::CalculatePosition(
        Bounds& bounds, RelativeBounds& relativeBounds, Bounds& parentBounds)
    {
        const auto parentPosition = parentBounds.Position();
        const auto delta = relativeBounds.delta;

        const auto position = Point3D
        {
            parentPosition.x + delta.x,
            parentPosition.y + delta.y,
            parentPosition.z + delta.z
        };

        bounds.Position(position);
    }

    void BoundsCurator::CalculateRelativePosition(
        RelativeBounds& relativeBounds, Bounds& parentBounds, const Point3D& newPosition)
    {
        const auto parentPosition = parentBounds.Position();
        auto& delta = relativeBounds.delta;

        delta.x = newPosition.x - parentPosition.x;
        delta.y = newPosition.y - parentPosition.y;
        delta.z = newPosition.z - parentPosition.z;
    }

    Bounds* BoundsCurator::RetrieveParentBounds(Arca::RelicID id)
    {
        const auto parent = Owner().ParentOf(id);
        if (!parent)
        {
            const auto log = Logging::Log{
                "Relics with " + Arca::TypeFor<RelativeBounds>().name + " must be parented.",
                Logging::Severity::Warning,
                { { "RelicID", id } } };
            Owner().Do(log);
            return nullptr;
        }

        const auto parentBounds = MutablePointer().Of<Bounds>(parent->ID());
        if (!parentBounds)
        {
            const auto log = Logging::Log{
                "Relic parents of " + Arca::TypeFor<RelativeBounds>().name + " must have a Bounds.",
                Logging::Severity::Warning,
                { { "RelicID", id } } };
            Owner().Do(log);
            return nullptr;
        }

        return parentBounds;
    }

    void BoundsCurator::UpdateChildrenRelativeBounds(const Point3D& parentPosition, Arca::RelicID id)
    {
        auto children = Owner().ChildrenOf(id);
        for (auto& child : children)
        {
            const auto relativePosition = MutablePointer().Of<RelativeBounds>(child.ID());
            auto childBounds = MutablePointer().Of<Bounds>(child.ID());
            if (!relativePosition || !childBounds)
                continue;

            const auto childPosition = Point3D
            {
                parentPosition.x + relativePosition->delta.x,
                parentPosition.y + relativePosition->delta.y,
                parentPosition.z + relativePosition->delta.z
            };

            childBounds->Position(childPosition);

            Owner().Raise(BoundsMoved(Arca::Index<Bounds>(child.ID(), Owner())));
        }
    }
}