
#pragma once

#include <vector>
#include <set>

#include "Range.h"
#include "Serialization.h"

namespace Atmos
{
    class Position3D;
    class AxisAlignedBox2D;

    class Position2D
    {
    public:
        typedef float Value;
    public:
        Value x, y;
    public:
        Position2D();
        Position2D(Value x, Value y);
        Position2D(const Position3D& arg);
        Position2D(const Position2D& arg);

        Position2D& operator=(const Position2D& arg);

        bool operator==(const Position2D& arg) const;
        bool operator!=(const Position2D& arg) const;

        bool Within(Value left, Value right, Value top, Value bottom) const;
        bool Within(const AxisAlignedBox2D& box) const;

        static Position2D FromScreen(const Position2D& convert, const Position2D& topLeftScreen);
        static Value FindDistance(const Position2D& starting, const Position2D& destination);
        static Position2D FindCenter(const std::vector<Position2D>& container);
        static Position2D FindCenter(const std::set<Position2D>& container);
        static Position2D FindCenter(const Position2D& topLeft, const Position2D& bottomRight);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Position2D, BinaryArchive> : public CompositeScribe<::Atmos::Position2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}

namespace std
{
    template<>
    struct hash<::Atmos::Position2D>
    {
        typedef ::Atmos::Position2D argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& arg) const
        {
            const result_type first(std::hash<argument_type::Value>()(arg.x));
            const result_type second(std::hash<argument_type::Value>()(arg.y));
            return first ^ (second << 1);
        }
    };
}