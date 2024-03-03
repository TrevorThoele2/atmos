#pragma once

#include "Position2D.h"
#include "Size2D.h"

#include "Property.h"
#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class AxisAlignedBox2D
    {
    public:
        typedef float Coordinate;
    public:
        typedef Property<Coordinate> CoordinateProperty;
        CoordinateProperty left = CoordinateProperty(
            [this]() { return MakeMinusCoordinate(center.x, size.scaledWidth); },
            [this](Coordinate newValue) { SetCoordinate(newValue, left, center.x, size.scaledWidth); });
        CoordinateProperty top = CoordinateProperty(
            [this]() { return MakeMinusCoordinate(center.y, size.scaledHeight); },
            [this](Coordinate newValue) { SetCoordinate(newValue, top, center.y, size.scaledHeight); });
        CoordinateProperty right = CoordinateProperty(
            [this]() { return MakePlusCoordinate(center.x, size.scaledWidth); },
            [this](Coordinate newValue) { SetCoordinate(newValue, right, center.x, size.scaledWidth); });
        CoordinateProperty bottom = CoordinateProperty(
            [this]() { return MakePlusCoordinate(center.y, size.scaledHeight); },
            [this](Coordinate newValue) { SetCoordinate(newValue, bottom, center.y, size.scaledHeight); });

        Position2D center;
        Size2D size;
    public:
        AxisAlignedBox2D();
        AxisAlignedBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom);

        AxisAlignedBox2D& operator=(const AxisAlignedBox2D& arg);

        bool operator==(const AxisAlignedBox2D &arg) const;
        bool operator!=(const AxisAlignedBox2D &arg) const;

        bool IsHit(const Position2D& check) const;
        bool Within(const Position2D& check) const;
        bool Within(const AxisAlignedBox2D& box) const;
        bool Overlapping(const AxisAlignedBox2D& box) const;
    private:
        void SetCoordinate(Coordinate newValue, Coordinate focused, Position2D::Value centerValue, Size2D::Value sizeValue);

        Coordinate MakeMinusCoordinate(Position2D::Value centerValue, Size2D::Value sizeValue) const;
        Coordinate MakePlusCoordinate(Position2D::Value centerValue, Size2D::Value sizeValue) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AxisAlignedBox2D, BinaryArchive> :
        public CompositeScribe<::Atmos::AxisAlignedBox2D, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}