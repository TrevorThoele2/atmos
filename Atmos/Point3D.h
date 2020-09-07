#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    class AxisAlignedBox3D;

    struct Point3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Point3D& arg) const;
        bool operator!=(const Point3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Point3D, BinaryArchive> final :
        public CompositeScribe<Atmos::Spatial::Point3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}