
#pragma once

#include <vector>
#include <set>

#include "Range.h"
#include "Serialization.h"

namespace Atmos
{
    class AxisAlignedBox2D;

    struct Position2D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;

        Position2D() = default;
        Position2D(Value x, Value y);

        bool operator==(const Position2D& arg) const;
        bool operator!=(const Position2D& arg) const;

        [[nodiscard]] bool Contains(Value left, Value right, Value top, Value bottom) const;
        [[nodiscard]] bool Contains(const AxisAlignedBox2D& box) const;

        static Position2D FromScreen(const Position2D& convert, const Position2D& topLeftScreen);
        static Position2D CenterOf(const std::vector<Position2D>& container);
        static Position2D CenterOf(const std::set<Position2D>& container);
        static Position2D CenterOf(const Position2D& topLeft, const Position2D& bottomRight);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Position2D, BinaryArchive> final :
        public CompositeScribe<::Atmos::Position2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Position2D>
    {
        typedef ::Atmos::Position2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}