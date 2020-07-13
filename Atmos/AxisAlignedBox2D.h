#pragma once

#include "Point2D.h"
#include "Size2D.h"

#include "Serialization.h"

namespace Atmos::Spatial
{
    class AxisAlignedBox2D
    {
    public:
        using Coordinate = float;
    public:
        AxisAlignedBox2D() = default;
        AxisAlignedBox2D(const Point2D& center, const Size2D& size);
        AxisAlignedBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom);

        bool operator==(const AxisAlignedBox2D& arg) const;
        bool operator!=(const AxisAlignedBox2D& arg) const;

        void Center(const Point2D& to);
        void Size(const Size2D& to);
        [[nodiscard]] Point2D Center() const;
        [[nodiscard]] Size2D Size() const;

        void Edit(const Point2D& center, const Size2D& size);
        void Edit(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom);

        void Left(Coordinate set);
        void Top(Coordinate set);
        void Right(Coordinate set);
        void Bottom(Coordinate set);

        [[nodiscard]] Coordinate Left() const;
        [[nodiscard]] Coordinate Top() const;
        [[nodiscard]] Coordinate Right() const;
        [[nodiscard]] Coordinate Bottom() const;

        [[nodiscard]] Size2D::Value Width() const;
        [[nodiscard]] Size2D::Value Height() const;

        [[nodiscard]] bool Contains(const Point2D& check) const;
        [[nodiscard]] bool Contains(const AxisAlignedBox2D& box) const;
        [[nodiscard]] bool Intersects(const AxisAlignedBox2D& box) const;
    private:
        Point2D center;
        Size2D size;

        Coordinate left = 0.0f;
        Coordinate top = 0.0f;
        Coordinate right = 0.0f;
        Coordinate bottom = 0.0f;

        void ChangeCoordinate(Coordinate& change, Coordinate to);

        void CalculateCoordinates();
        void CalculateCenterAndSize();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Spatial::AxisAlignedBox2D, BinaryArchive> :
        public CompositeScribe<::Atmos::Spatial::AxisAlignedBox2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}