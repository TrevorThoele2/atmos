#pragma once

#include "Serialization.h"

namespace Atmos::Asset
{
    struct ImageGridSize
    {
        using Dimension = int;
        Dimension columns = 0;
        Dimension rows = 0;

        bool operator==(const ImageGridSize& arg) const;
        bool operator!=(const ImageGridSize& arg) const;
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("columns", object.columns);
            format("rows", object.rows);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Asset::ImageGridSize, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Asset::ImageGridSize>;
    };
}