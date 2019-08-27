#pragma once

#include "Position3D.h"
#include "Size3D.h"

#include "Serialization.h"

namespace Atmos
{
    class AxisAlignedBox3D
    {
    public:
        using Coordinate = float;
    public:
        AxisAlignedBox3D() = default;
        AxisAlignedBox3D(const Position3D& center, const Size3D& size);
        AxisAlignedBox3D(
            Coordinate left,
            Coordinate top,
            Coordinate farZ,
            Coordinate right,
            Coordinate bottom,
            Coordinate nearZ
        );

        bool operator==(const AxisAlignedBox3D& arg) const;
        bool operator!=(const AxisAlignedBox3D& arg) const;

        void Center(const Position3D& to);
        void Size(const Size3D& to);
        [[nodiscard]] Position3D Center() const;
        [[nodiscard]] Size3D Size() const;

        void Edit(const Position3D& center, const Size3D& size);
        void Edit(
            Coordinate left,
            Coordinate top,
            Coordinate farZ,
            Coordinate right,
            Coordinate bottom,
            Coordinate nearZ
        );

        void Left(Coordinate set);
        void Top(Coordinate set);
        void FarZ(Coordinate set);
        void Right(Coordinate set);
        void Bottom(Coordinate set);
        void NearZ(Coordinate set);

        [[nodiscard]] Coordinate Left() const;
        [[nodiscard]] Coordinate Top() const;
        [[nodiscard]] Coordinate FarZ() const;
        [[nodiscard]] Coordinate Right() const;
        [[nodiscard]] Coordinate Bottom() const;
        [[nodiscard]] Coordinate NearZ() const;

        [[nodiscard]] Size3D::Value Width() const;
        [[nodiscard]] Size3D::Value Height() const;
        [[nodiscard]] Size3D::Value Depth() const;

        [[nodiscard]] bool Contains(const Position3D& check) const;
        [[nodiscard]] bool Contains(const AxisAlignedBox3D& box) const;
        [[nodiscard]] bool Intersects(const AxisAlignedBox3D& box) const;
    private:
        Position3D center;
        Size3D size;

        Coordinate left = 0.0f;
        Coordinate top = 0.0f;
        Coordinate farZ = 0.0f;
        Coordinate right = 0.0f;
        Coordinate bottom = 0.0f;
        Coordinate nearZ = 0.0f;

        void ChangeCoordinate(Coordinate& change,Coordinate to);

        void CalculateCoordinates();
        void CalculateCenterAndSize();

        void CoordinatePrecondition(
            const Coordinate& low,
            const Coordinate& high,
            const std::string& lowName,
            const std::string& highName
        );
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AxisAlignedBox3D, BinaryArchive> :
        public CompositeScribe<::Atmos::AxisAlignedBox3D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}