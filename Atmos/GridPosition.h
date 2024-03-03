#pragma once

#include "Serialization.h"

namespace Atmos::Grid
{
    struct Position
    {
        using Value = int;
        Value x = 0, y = 0;

        Position() = default;
        Position(Value x, Value y);
        Position(const Position& arg) = default;

        Position& operator=(const Position& arg) = default;

        bool operator==(const Position &arg) const;
        bool operator!=(const Position &arg) const;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Grid::Position, BinaryArchive> final :
        public CompositeScribe<::Atmos::Grid::Position, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Grid::Position>
    {
        using argument_type = ::Atmos::Grid::Position;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const noexcept
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}