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
        static inline const TypeName typeName = "Atmos::Spatial::RelativeBounds";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::RelativeBounds, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Spatial::RelativeBounds, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}