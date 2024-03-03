#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Size2D
    {
        using Value = float;
        Value width = 0.0f;
        Value height = 0.0f;

        bool operator==(const Size2D& arg) const;
        bool operator!=(const Size2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Size2D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Size2D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("width", object.width);
            archive("height", object.height);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Size2D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Size2D>;
    };
}