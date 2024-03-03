#pragma once

#include "Position3D.h"
#include "Serialization.h"

namespace Atmos
{
    class AxisBoundingBox3D
    {
    public:
        typedef float Coordinate;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        Coordinate left, right, top, bottom, nearZ, farZ;
        Coordinate realLeft, realRight, realTop, realBottom, realNearZ, realFarZ;
        bool pixelPerfect;

        Position3D center;

        void Calc();
        bool OverlappingImpl(const AxisBoundingBox3D &box) const;
    public:
        AxisBoundingBox3D(bool pixelPerfect = true);
        AxisBoundingBox3D(Coordinate left, Coordinate top, Coordinate farZ, Coordinate right, Coordinate bottom, Coordinate nearZ, bool pixelPerfect = true);
        bool operator==(const AxisBoundingBox3D &arg) const;
        bool operator!=(const AxisBoundingBox3D &arg) const;

        void SetLeft(Coordinate setTo);
        void SetRight(Coordinate setTo);
        void SetTop(Coordinate setTo);
        void SetBottom(Coordinate setTo);
        void SetNearZ(Coordinate setTo);
        void SetFarZ(Coordinate setTo);
        void Set(Coordinate top, Coordinate bottom, Coordinate left, Coordinate right, Coordinate nearZ, Coordinate farZ);

        void IncLeft(Coordinate toInc);
        void IncRight(Coordinate toInc);
        void IncTop(Coordinate toInc);
        void IncBottom(Coordinate toInc);
        void IncNearZ(Coordinate toInc);
        void IncFarZ(Coordinate toInc);

        void DecLeft(Coordinate toDec);
        void DecRight(Coordinate toDec);
        void DecTop(Coordinate toDec);
        void DecBottom(Coordinate toDec);
        void DecNearZ(Coordinate toDec);
        void DecFarZ(Coordinate toDec);

        Coordinate GetLeft() const;
        Coordinate GetRight() const;
        Coordinate GetTop() const;
        Coordinate GetBottom() const;
        Coordinate GetNearZ() const;
        Coordinate GetFarZ() const;
        const Position3D& GetCenter() const;
        Coordinate GetWidth() const;
        Coordinate GetHeight() const;
        Coordinate GetDepth() const;

        void SetPixelPerfect(bool setTo);
        // Returns true if the position is inside the AABB
        bool IsHit(const Position3D &check) const;
        // Returns true if the position is inside the AABB
        bool Within(const Position3D &check) const;
        // Returns true if this AABB is completely enclosed in the box
        bool Within(const AxisBoundingBox3D &box) const;
        // Returns true if the box overlaps any part of this AABB
        bool Overlapping(const AxisBoundingBox3D &box) const;
    };
}