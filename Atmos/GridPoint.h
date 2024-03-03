#pragma once

#include "Serialization.h"

namespace Atmos::Spatial::Grid
{
    struct Point
    {
        using Value = int;
        Value x = 0;
        Value y = 0;

        bool operator==(const Point& arg) const;
        bool operator!=(const Point& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Grid::Point> final
    {
    public:
        using ObjectT = Atmos::Spatial::Grid::Point;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("x", object.x);
            format("y", object.y);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Spatial::Grid::Point, Format> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Grid::Point>;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::Spatial::Grid::Point>
    {
        using argument_type = Atmos::Spatial::Grid::Point;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}