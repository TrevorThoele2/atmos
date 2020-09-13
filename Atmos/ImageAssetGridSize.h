#pragma once

#include "Serialization.h"

namespace Atmos::Asset
{
    struct ImageGridSize
    {
        using Dimension = int;
        Dimension columns = 0;
        Dimension rows = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageGridSize> final
    {
    public:
        using ObjectT = Atmos::Asset::ImageGridSize;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("columns", object.columns);
            archive("rows", object.rows);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ImageGridSize, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Asset::ImageGridSize>;
    };
}