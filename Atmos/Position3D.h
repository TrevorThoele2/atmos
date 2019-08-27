#pragma once

#include "Serialization.h"

namespace Atmos
{
    struct Position2D;
    class AxisAlignedBox3D;

    struct Position3D
    {
        using Value = float;
        Value x = 0.0f;
        Value y = 0.0f;
        Value z = 0.0f;

        bool operator==(const Position3D& arg) const;
        bool operator!=(const Position3D& arg) const;

        operator Position2D() const;

        [[nodiscard]] bool Contains(Value left, Value right, Value top, Value bottom, Value nearZ, Value farZ) const;
        [[nodiscard]] bool Contains(const AxisAlignedBox3D& box) const;

        static Position3D FromScreen(const Position3D& convert, const Position3D& topLeftScreen, Value z);
        static Value DistanceBetween(const Position3D& starting, const Position3D& destination);
        static Position3D CenterOf(const std::vector<Position3D>& container);
        static Position3D CenterOf
        (
            const Position3D& nearTopLeft,
            const Position3D& nearBottomRight,
            const Position3D& farTopLeft
        );
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Position3D, BinaryArchive> final : public CompositeScribe<::Atmos::Position3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}