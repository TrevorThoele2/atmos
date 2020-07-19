#pragma once

#include <Arca/Curator.h>
#include <Arca/Reliquary.h>

#include "Bounds.h"
#include "RelativeBounds.h"

#include "MoveBoundsTo.h"
#include "MoveBoundsBy.h"
#include "MoveBoundsDirection.h"
#include "ScaleBounds.h"
#include "RotateBounds.h"

#include "Log.h"

namespace Atmos::Spatial
{
    class BoundsCurator final : public Arca::Curator
    {
    public:
        explicit BoundsCurator(Init init);
    public:
        void Handle(const MoveBoundsTo& command);
        void Handle(const MoveBoundsBy& command);
        void Handle(const MoveBoundsDirection& command);
        void Handle(const ScaleBounds& command);
        void Handle(const RotateBounds& command);
    private:
        void DoMovement(Bounds& bounds, const Point3D& to, Arca::RelicID id);
        void DoScale(Bounds& bounds, const Scalers2D& to, Arca::RelicID id);
        void DoRotation(Bounds& bounds, const Angle2D& to, Arca::RelicID id);

        template<class Function>
        void DoWithRequiredBounds(Arca::RelicID id, Function function);
        static void CalculatePosition(
            Bounds& bounds, RelativeBounds& relativeBounds, Bounds& parentBounds);
        static void CalculateRelativePosition(
            RelativeBounds& relativeBounds, Bounds& parentBounds, const Point3D& newPosition);

        Bounds* RetrieveParentBounds(Arca::RelicID id);
        void UpdateChildrenRelativeBounds(const Point3D& parentPosition, Arca::RelicID id);
    };

    template<class Function>
    void BoundsCurator::DoWithRequiredBounds(Arca::RelicID id, Function function)
    {
        const auto bounds = MutablePointer().Of<Bounds>(id);
        if (!bounds)
        {
            const auto log = Logging::Log{
                "Attempted movement of relic without " + Arca::TypeFor<Bounds>().name + ".",
                Logging::Severity::Warning,
                { { "RelicID", id } } };
            Owner().Do(log);
        }
        else
            function(*bounds);
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::BoundsCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Spatial::BoundsCurator";
        using HandledCommands = HandledCommands<
            Atmos::Spatial::MoveBoundsTo,
            Atmos::Spatial::MoveBoundsBy,
            Atmos::Spatial::MoveBoundsDirection,
            Atmos::Spatial::ScaleBounds,
            Atmos::Spatial::RotateBounds>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::BoundsCurator, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Spatial::BoundsCurator, BinaryArchive>
    {};
}