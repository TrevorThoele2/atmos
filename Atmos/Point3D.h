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
    class Scribe<Atmos::Spatial::Point3D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Point3D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("x", object.x);
            archive("y", object.y);
            archive("z", object.z);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Point3D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Point3D>;
    };
}