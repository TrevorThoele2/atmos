#pragma once

#include "Serialization.h"

namespace Atmos::Spatial::Grid
{
    struct Point
    {
        using Value = int;
        Value x = 0, y = 0;

        Point() = default;
        Point(Value x, Value y);
        Point(const Point& arg) = default;

        Point& operator=(const Point& arg) = default;

        bool operator==(const Point &arg) const;
        bool operator!=(const Point &arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Grid::Point, BinaryArchive> final :
        public CompositeScribe<Atmos::Spatial::Grid::Point, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
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