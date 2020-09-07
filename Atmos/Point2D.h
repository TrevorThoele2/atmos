
#pragma once

#include <vector>
#include <set>

#include "Range.h"
#include "Serialization.h"

namespace Atmos::Spatial
{
    struct AxisAlignedBox2D;

    struct Point2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        Point2D() = default;
        Point2D(Value x, Value y);

        bool operator==(const Point2D& arg) const;
        bool operator!=(const Point2D& arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Spatial::Point2D, BinaryArchive> final :
        public CompositeScribe<::Atmos::Spatial::Point2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Spatial::Point2D>
    {
        typedef ::Atmos::Spatial::Point2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}