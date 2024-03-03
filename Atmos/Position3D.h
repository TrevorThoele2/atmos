#pragma once

#include <vector>
#include <set>

#include "Serialization.h"

namespace Atmos
{
    class Position2D;
    class AxisAlignedBox3D;

    class Position3D
    {
    public:
        typedef float Value;
    public:
        Value x, y, z;
    public:
        Position3D();
        Position3D(Value x, Value y, Value z);
        Position3D(const Position2D& pos, Value z);
        Position3D(const Position3D& arg);

        Position3D& operator=(const Position3D& arg);

        bool operator==(const Position3D& arg) const;
        bool operator!=(const Position3D& arg) const;

        explicit operator Position2D() const;

        bool Within(Value left, Value right, Value top, Value bottom, Value nearZ, Value farZ) const;
        bool Within(const AxisAlignedBox3D& box) const;

        static Position3D FromScreen(const Position3D& convert, const Position3D& topLeftScreen, Value z);
        static Value FindDistance(const Position3D& starting, const Position3D& destination);
        static Position3D FindCenter(const std::vector<Position3D>& container);
        static Position3D FindCenter(const Position3D& nearTopLeft, const Position3D& nearBottomRight, const Position3D& farTopLeft);
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Position3D, BinaryArchive> : public CompositeScribe<::Atmos::Position3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}