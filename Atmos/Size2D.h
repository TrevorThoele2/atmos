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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("width", object.width);
            format("height", object.height);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Size2D, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Size2D>;
    };
}