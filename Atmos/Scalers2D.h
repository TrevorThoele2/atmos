#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Scalers2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        bool operator==(const Scalers2D& arg) const;
        bool operator!=(const Scalers2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Scalers2D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Scalers2D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("x", object.x);
            archive("y", object.y);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Scalers2D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Scalers2D>;
    };
}