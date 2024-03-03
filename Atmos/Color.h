#pragma once

#include "Serialization.h"

namespace Atmos::Render
{
    struct Color
    {
        using Value = unsigned char;
        Value alpha = 255;
        Value red = 255;
        Value green = 255;
        Value blue = 255;

        bool operator==(const Color& arg) const;
        bool operator!=(const Color& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Color> final
    {
    public:
        using ObjectT = Atmos::Render::Color;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("alpha", object.alpha);
            archive("red", object.red);
            archive("green", object.green);
            archive("blue", object.blue);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Render::Color, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Render::Color>;
    };
}