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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("alpha", object.alpha);
            format("red", object.red);
            format("green", object.green);
            format("blue", object.blue);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Render::Color, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Render::Color>;
    };
}