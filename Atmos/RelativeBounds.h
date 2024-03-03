#pragma once

#include <Arca/Shard.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    struct RelativeBounds
    {
        Point3D delta;

        RelativeBounds() = default;
        explicit RelativeBounds(const Point3D& delta) : delta(delta)
        {}
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::RelativeBounds>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static TypeName TypeName() { return "Atmos::Spatial::RelativeBounds"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::RelativeBounds> final
    {
    public:
        using ObjectT = Atmos::Spatial::RelativeBounds;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("delta", object.delta);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::RelativeBounds, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Spatial::RelativeBounds>;
    };
}