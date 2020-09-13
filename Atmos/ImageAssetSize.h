#pragma once

#include "Serialization.h"

namespace Atmos::Asset
{
    struct ImageSize
    {
        using Dimension = int;
        Dimension width = 0;
        Dimension height = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageSize> final
    {
    public:
        using ObjectT = Atmos::Asset::ImageSize;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("width", object.width);
            archive("height", object.height);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ImageSize, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Asset::ImageSize>;
    };
}