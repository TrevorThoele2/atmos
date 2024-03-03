#pragma once

#include "Position3D.h"
#include "Size3D.h"

#include "Property.h"
#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class AxisAlignedBox3D
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
        CoordinateProperty farZ = CoordinateProperty(
            [this]() { return MakeMinusCoordinate(center.z, size.scaledHeight); },
            [this](Coordinate newValue) { SetCoordinate(newValue, farZ, center.z, size.scaledDepth); });
        CoordinateProperty right = CoordinateProperty(
            [this]() { return MakePlusCoordinate(center.x, size.scaledWidth); },
            [this](Coordinate newValue) { SetCoordinate(newValue, right, center.x, size.scaledWidth); });
        CoordinateProperty bottom = CoordinateProperty(
            [this]() { return MakePlusCoordinate(center.y, size.scaledHeight); },
            [this](Coordinate newValue) { SetCoordinate(newValue, bottom, center.y, size.scaledHeight); });
        CoordinateProperty nearZ = CoordinateProperty(
            [this]() { return MakePlusCoordinate(center.z, size.scaledHeight); },
            [this](Coordinate newValue) { SetCoordinate(newValue, nearZ, center.z, size.scaledDepth); });

        Position3D center;
        Size3D size;
    public:
        AxisAlignedBox3D();
        AxisAlignedBox3D(Coordinate left, Coordinate top, Coordinate farZ, Coordinate right, Coordinate bottom, Coordinate nearZ);

        AxisAlignedBox3D& operator=(const AxisAlignedBox3D& arg);

        bool operator==(const AxisAlignedBox3D& arg) const;
        bool operator!=(const AxisAlignedBox3D& arg) const;

        bool IsHit(const Position3D& check) const;
        bool Within(const Position3D& check) const;
        bool Within(const AxisAlignedBox3D& box) const;
        bool Overlapping(const AxisAlignedBox3D& box) const;
    private:
        void SetCoordinate(Coordinate newValue, Coordinate focused, Position3D::Value centerValue, Size3D::Value sizeValue);

        Coordinate MakeMinusCoordinate(Position3D::Value centerValue, Size3D::Value sizeValue) const;
        Coordinate MakePlusCoordinate(Position3D::Value centerValue, Size3D::Value sizeValue) const;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AxisAlignedBox3D, BinaryArchive> : public CompositeScribe<::Atmos::AxisAlignedBox3D, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}