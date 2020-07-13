#pragma once

#include <Arca/Shard.h>

#include "Point3D.h"

namespace Atmos::Spatial
{
    struct RelativeBoundsPosition
    {
        Point3D delta;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Spatial::RelativeBoundsPosition>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Spatial::RelativeBoundsPosition";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::RelativeBoundsPosition, BinaryArchive> final :
        public ArcaCompositeScribe<Atmos::Spatial::RelativeBoundsPosition, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}