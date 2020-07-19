#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Angle3D
    {
        using Value = float;

        Value yaw;
        Value pitch;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Angle3D, BinaryArchive> final :
        public CompositeScribe<Atmos::Spatial::Angle3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}