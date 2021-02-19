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
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("width", object.width);
            archive("height", object.height);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Grid::Size, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Grid::Size>;
    };
}