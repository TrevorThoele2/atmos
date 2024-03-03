#pragma once

#include <Arca/Curator.h>
#include <Arca/Reliquary.h>

#include "Bounds.h"

#include "MoveBounds.h"
#include "ScaleBounds.h"
#include "RotateBounds.h"

#include "Log.h"

#include "StringUtility.h"

namespace Atmos::Spatial
{
    class BoundsCurator final : public Arca::Curator
    {
    public:
        explicit BoundsCurator(Init init);
    public:
        void Handle(const MoveBounds& command);
        void Handle(const ScaleBounds& command);
        void Handle(const RotateBounds& command);
    private:
        void DoMovement(Bounds& bounds, const Point3D& to, Arca::RelicID id);
        void DoScale(Bounds& bounds, const Scalers2D& to, Arca::RelicID id);
        void DoRotation(Bounds& bounds, const Angle2D& to, Arca::RelicID id);

        template<class BoundsT>
        void DoWithRequiredBounds(Arca::RelicID id, const std::function<void(BoundsT&)>& function);
    };

    template<class BoundsT>
    void BoundsCurator::DoWithRequiredBounds(Arca::RelicID id, const std::function<void(BoundsT&)>& function)
    {
        const auto bounds = MutablePointer().Of<Bounds>(id);
        if (!bounds)
        {
            const auto log = Logging::Log{
                "Attempted movement of relic without " + Arca::TypeFor<Bounds>().name + ".",
                Logging::Severity::Warning,
                { { "RelicID", ToString(id) } } };
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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Spatial::BoundsCurator";
        using HandledCommands = HandledCommands<
            Atmos::Spatial::MoveBounds,
            Atmos::Spatial::ScaleBounds,
            Atmos::Spatial::RotateBounds>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::BoundsCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Spatial::BoundsCurator>;
    };
}