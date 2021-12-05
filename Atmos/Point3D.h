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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("x", object.x);
            format("y", object.y);
            format("z", object.z);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Point3D, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Point3D>;
    };
}