#pragma once

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct ScreenSize
    {
        using Dimension = int;
        Dimension width = 0;
        Dimension height = 0;

        ScreenSize() = default;
        ScreenSize(Dimension width, Dimension height);
        ScreenSize(const ScreenSize& arg) = default;

        bool operator==(const ScreenSize& arg) const;
        bool operator!=(const ScreenSize& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::ScreenSize> final
    {
    public:
        using ObjectT = Atmos::Spatial::ScreenSize;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("width", object.width);
            archive("height", object.height);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::ScreenSize, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::ScreenSize>;
    };
}