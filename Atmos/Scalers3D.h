#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Scalers3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Scalers3D& arg) const;
        bool operator!=(const Scalers3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Scalers3D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Scalers3D;
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
    struct ScribeTraits<Atmos::Spatial::Scalers3D, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Scalers3D>;
    };
}