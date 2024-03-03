#include "BoundsCurator.h"

#include "BoundsMoved.h"
#include "BoundsScaled.h"
#include "BoundsRotated.h"

#include "TimeInformation.h"

namespace Atmos::Spatial
{
    BoundsCurator::BoundsCurator(Init init) : Curator(init)
    {}

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
                const auto toPosition = Point3D
                {
                    fromPosition.x + command.delta.x,
                    fromPosition.y + command.delta.y,
                    fromPosition.z + command.delta.z
                };

                DoMovement(bounds, toPosition, command.id);
            });
    }

    void BoundsCurator::Handle(const MoveBoundsDirection& command)
    {
        DoWithRequiredBounds(
            command.id,
            [this, command](Bounds& bounds)
            {
                auto& lastFrameElapsed = Arca::Index<Time::Information>(Owner())->lastFrameElapsed;
                const auto normalizedDistance =
                    command.amount
                    * static_cast<float>(std::chrono::duration_cast<Time::Seconds>(lastFrameElapsed).count());
                const auto fromPosition = bounds.Position();
                auto toPosition = fromPosition;
                switch (command.direction.Get())
                {
                case Direction::Left:
                    toPosition.x -= normalizedDistance;
                    break;
                case Direction::Up:
                    toPosition.y -= normalizedDistance;
                    break;
                case Direction::Right:
                    toPosition.x += normalizedDistance;
                    break;
                case Direction::Down:
                    toPosition.y += normalizedDistance;
                    break;
                default:
                    break;
                }

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
        auto [relativePosition, parentBounds] = RetrieveRelativePositionAndParentBounds(id);
        if (relativePosition)
        {
            if (!parentBounds)
                return;

            const auto parentPosition = parentBounds->Position();
            auto& delta = relativePosition->delta;

            delta.x = parentPosition.x - to.x;
            delta.y = parentPosition.y - to.y;
            delta.z = parentPosition.z - to.z;
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

    void BoundsCurator::DoRotation(Bounds& bounds, const Angle& to, Arca::RelicID id)
    {
        bounds.Rotation(to);

        Owner().Raise(BoundsRotated(Arca::Index<Bounds>(id, Owner())));
    }

    auto BoundsCurator::RetrieveRelativePositionAndParentBounds(Arca::RelicID id)
        -> RelativePositionAndParentBounds
    {
        auto relativePosition = MutablePointer().Of<RelativeBoundsPosition>(id);
        if (!relativePosition)
            return { nullptr, {} };

        const auto parent = Owner().ParentOf(id);
        if (!parent)
        {
            const auto log = Logging::Log{
                "Relics with " + Arca::TypeFor<RelativeBoundsPosition>().name + " must be parented.",
                Logging::Severity::Warning,
                { { "RelicID", id } } };
            return { relativePosition, {} };
        }

        const auto parentBounds = MutablePointer().Of<Bounds>(parent->ID());
        if (!parentBounds)
        {
            const auto log = Logging::Log{
                "Relic parents of " + Arca::TypeFor<RelativeBoundsPosition>().name + " must have a Bounds.",
                Logging::Severity::Warning,
                { { "RelicID", id } } };
            return { relativePosition, {} };
        }

        return { relativePosition, parentBounds };
    }

    void BoundsCurator::UpdateChildrenRelativeBounds(const Point3D& parentPosition, Arca::RelicID id)
    {
        auto children = Owner().ChildrenOf(id);
        for (auto& child : children)
        {
            const auto relativePosition = MutablePointer().Of<RelativeBoundsPosition>(child.ID());
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