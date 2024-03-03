#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct Size3D
    {
        using Value = float;
        Value width = 0.0f;
        Value height = 0.0f;
        Value depth = 0.0f;

        bool operator==(const Size3D& arg) const;
        bool operator!=(const Size3D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Size3D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Size3D;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("width", object.width);
            format("height", object.height);
            format("depth", object.depth);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Size3D, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Size3D>;
    };
}