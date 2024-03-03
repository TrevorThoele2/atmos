#pragma once

#include "Serialization.h"
#include <Chroma/Enum.h>

namespace Atmos
{
    class Direction
    {
    public:
        enum Value : unsigned char
        {
            Left,
            Up,
            Right,
            Down,
            ZUp,
            ZDown
        };

        enum class RelativeValue : unsigned char
        {
            Same,
            Left,
            Right,
            Opposed
        };
    public:
        Direction(Value value = Up);
        Direction(const Direction& arg) = default;

        Direction& operator=(const Direction& arg) = default;
        Direction& operator=(Value arg);

        bool operator==(const Direction& arg) const;
        bool operator!=(const Direction& arg) const;

        explicit operator Value() const;

        void Set(Value set);
        [[nodiscard]] Value Get() const;

        void MoveInRelativeDirection(RelativeValue move);
        [[nodiscard]] RelativeValue GetRelativeDirection(Value value) const;
        [[nodiscard]] RelativeValue GetRelativeDirection(const Direction& direction) const;

        static Value FromUnderlyingType(std::underlying_type<Value>::type from);
    private:
        Value value;

        template<Value value>
        static constexpr std::underlying_type<Value>::type CastFromValue();
    private:
        INSCRIPTION_ACCESS;
    };

    template<Direction::Value value>
    constexpr std::underlying_type<Direction::Value>::type Direction::CastFromValue()
    {
        return static_cast<std::underlying_type<Direction::Value>::type>(value);
    }

    using DirectionIterationTraits =
        ::Chroma::EnumIterationTraits<Direction::Value, Direction::Value::Left, Direction::Value::Down>;
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Direction, BinaryArchive> final :
        public CompositeScribe<::Atmos::Direction, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::Atmos::Direction::Value, BinaryArchive> final :
        public EnumScribe<::Atmos::Direction::Value, BinaryArchive>
    {};
}