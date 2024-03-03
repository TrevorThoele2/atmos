#pragma once

#include "Serialization.h"

namespace Atmos::Spatial::Grid
{
    struct Size
    {
        using Value = int;
        Value width = 0;
        Value height = 0;

        bool operator==(const Size& arg) const;
        bool operator!=(const Size& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Grid::Size> final
    {
    public:
        using ObjectT = Atmos::Spatial::Grid::Size;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("width", object.width);
            format("height", object.height);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Grid::Size, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Grid::Size>;
    };
}