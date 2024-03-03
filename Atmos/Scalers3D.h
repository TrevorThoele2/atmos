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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("x", object.x);
            archive("y", object.y);
            archive("z", object.z);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Scalers3D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Scalers3D>;
    };
}