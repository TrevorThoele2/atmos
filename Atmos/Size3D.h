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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("width", object.width);
            archive("height", object.height);
            archive("depth", object.depth);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Size3D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Size3D>;
    };
}