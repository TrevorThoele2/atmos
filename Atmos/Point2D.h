
#pragma once

#include <vector>
#include <set>

#include "Serialization.h"

namespace Atmos::Spatial
{
    struct AxisAlignedBox2D;

    struct Point2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        bool operator==(const Point2D& arg) const;
        bool operator!=(const Point2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Spatial::Point2D> final
    {
    public:
        using ObjectT = Atmos::Spatial::Point2D;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("x", object.x);
            archive("y", object.y);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Spatial::Point2D, Archive> final
    {
        using Category = CompositeScribeCategory<Atmos::Spatial::Point2D>;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Spatial::Point2D>
    {
        typedef Atmos::Spatial::Point2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}