#pragma once

#include "Serialization.h"

namespace Atmos::Render
{
    class Color
    {
    public:
        using Value = unsigned char;

        Value alpha;
        Value red;
        Value green;
        Value blue;
    public:
        Color();
        Color(Value alpha, Value red, Value green, Value blue);
        Color(const Color& arg) = default;

        Color& operator=(const Color& arg) = default;

        bool operator==(const Color& arg) const;
        bool operator!=(const Color& arg) const;
    private:
        INSCRIPTION_ACCESS;
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