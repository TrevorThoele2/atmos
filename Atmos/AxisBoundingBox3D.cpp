
#include "AxisBoundingBox3D.h"

#include "Math.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AxisBoundingBox3D)
    {
        scribe(left, right, top, bottom, nearZ, farZ);
        scribe(pixelPerfect);
        if (scribe.IsInput())
            Calc();
    }

    void AxisBoundingBox3D::Calc()
    {
        if (!pixelPerfect)
        {
            realTop = top;
            realBottom = bottom;
            realLeft = left;
            realRight = right;
            realNearZ = nearZ;
            realFarZ = farZ;
        }
        else
        {
            realTop = Round(top);
            realBottom = Round(bottom);
            realLeft = Round(left);
            realRight = Round(right);
            realNearZ = Round(nearZ);
            realFarZ = Round(farZ);
        }

        center.SetX(GetWidth() / 2);
        center.SetY(GetHeight() / 2);
        center.SetZ(GetDepth() / 2);
    }

    bool AxisBoundingBox3D::OverlappingImpl(const AxisBoundingBox3D &box) const
    {
        const Position3D nearTopLeft(realLeft, realTop, realNearZ);
        const Position3D nearTopRight(realRight, realTop, realNearZ);
        const Position3D nearBottomLeft(realLeft, realBottom, realNearZ);
        const Position3D nearBottomRight(realRight, realBottom, realNearZ);
        const Position3D farTopLeft(realLeft, realTop, realFarZ);
        const Position3D farTopRight(realRight, realTop, realFarZ);
        const Position3D farBottomLeft(realLeft, realBottom, realFarZ);
        const Position3D farBottomRight(realRight, realBottom, realFarZ);

        return nearTopLeft.Within(box) || nearBottomRight.Within(box) || nearTopRight.Within(box) || nearBottomLeft.Within(box) || farTopLeft.Within(box) || farTopRight.Within(box) || farBottomLeft.Within(box) || farBottomRight.Within(box);
    }

    AxisBoundingBox3D::AxisBoundingBox3D(bool pixelPerfect) : pixelPerfect(pixelPerfect), left(0), right(0), top(0), bottom(0)
    {
        Calc();
    }

    AxisBoundingBox3D::AxisBoundingBox3D(Coordinate left, Coordinate top, Coordinate farZ, Coordinate right, Coordinate bottom, Coordinate nearZ, bool pixelPerfect) : left(left), top(top), farZ(farZ), right(right), bottom(bottom), nearZ(nearZ), pixelPerfect(pixelPerfect)
    {
        Calc();
    }

    bool AxisBoundingBox3D::operator==(const AxisBoundingBox3D &arg) const
    {
        return pixelPerfect == arg.pixelPerfect && left == arg.left && right == arg.right && top == arg.top && bottom == arg.bottom && realLeft == arg.realLeft && realRight == arg.realRight && realTop == arg.realTop && realBottom == arg.realBottom && realNearZ == arg.realNearZ && realFarZ == arg.realFarZ;
    }

    bool AxisBoundingBox3D::operator!=(const AxisBoundingBox3D &arg) const
    {
        return !(*this == arg);
    }

    void AxisBoundingBox3D::SetLeft(Coordinate setTo)
    {
        left = setTo;
        Calc();
    }

    void AxisBoundingBox3D::SetRight(Coordinate setTo)
    {
        right = setTo;
        Calc();
    }

    void AxisBoundingBox3D::SetTop(Coordinate setTo)
    {
        top = setTo;
        Calc();
    }

    void AxisBoundingBox3D::SetBottom(Coordinate setTo)
    {
        bottom = setTo;
        Calc();
    }

    void AxisBoundingBox3D::SetNearZ(Coordinate setTo)
    {
        nearZ = setTo;
        Calc();
    }

    void AxisBoundingBox3D::SetFarZ(Coordinate setTo)
    {
        farZ = setTo;
        Calc();
    }

    void AxisBoundingBox3D::Set(Coordinate top, Coordinate bottom, Coordinate left, Coordinate right, Coordinate nearZ, Coordinate farZ)
    {
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
        this->nearZ = nearZ;
        this->farZ = farZ;
        Calc();
    }

    void AxisBoundingBox3D::IncLeft(Coordinate toInc)
    {
        left += toInc;
        Calc();
    }

    void AxisBoundingBox3D::IncRight(Coordinate toInc)
    {
        right += toInc;
        Calc();
    }

    void AxisBoundingBox3D::IncTop(Coordinate toInc)
    {
        top += toInc;
        Calc();
    }

    void AxisBoundingBox3D::IncBottom(Coordinate toInc)
    {
        bottom += toInc;
        Calc();
    }

    void AxisBoundingBox3D::IncNearZ(Coordinate toInc)
    {
        nearZ += toInc;
        Calc();
    }

    void AxisBoundingBox3D::IncFarZ(Coordinate toInc)
    {
        farZ += toInc;
        Calc();
    }

    void AxisBoundingBox3D::DecLeft(Coordinate toDec)
    {
        left -= toDec;
        Calc();
    }

    void AxisBoundingBox3D::DecRight(Coordinate toDec)
    {
        right -= toDec;
        Calc();
    }

    void AxisBoundingBox3D::DecTop(Coordinate toDec)
    {
        top -= toDec;
        Calc();
    }

    void AxisBoundingBox3D::DecBottom(Coordinate toDec)
    {
        bottom -= toDec;
        Calc();
    }

    void AxisBoundingBox3D::DecNearZ(Coordinate toDec)
    {
        nearZ -= toDec;
        Calc();
    }

    void AxisBoundingBox3D::DecFarZ(Coordinate toDec)
    {
        farZ -= toDec;
        Calc();
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetLeft() const
    {
        return realLeft;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetRight() const
    {
        return realRight;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetTop() const
    {
        return realTop;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetBottom() const
    {
        return realBottom;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetNearZ() const
    {
        return realNearZ;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetFarZ() const
    {
        return realFarZ;
    }

    const Position3D& AxisBoundingBox3D::GetCenter() const
    {
        return center;
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetWidth() const
    {
        return GetRight() - GetLeft();
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetHeight() const
    {
        return GetBottom() - GetTop();
    }

    AxisBoundingBox3D::Coordinate AxisBoundingBox3D::GetDepth() const
    {
        return GetNearZ() - GetFarZ();
    }

    void AxisBoundingBox3D::SetPixelPerfect(bool setTo)
    {
        pixelPerfect = setTo;
    }

    bool AxisBoundingBox3D::IsHit(const Position3D &check) const
    {
        return Within(check);
    }

    bool AxisBoundingBox3D::Within(const Position3D &check) const
    {
        return check.GetX() >= realLeft && check.GetY() <= realRight && check.GetY() >= realTop && check.GetY() <= realBottom && check.GetZ() >= realFarZ && check.GetZ() <= realNearZ;
    }

    bool AxisBoundingBox3D::Within(const AxisBoundingBox3D &box) const
    {
        return realLeft >= box.realLeft && realRight <= box.realRight && realTop >= box.realTop && realBottom <= box.realBottom && realFarZ >= box.realFarZ && realNearZ <= box.realNearZ;
    }

    bool AxisBoundingBox3D::Overlapping(const AxisBoundingBox3D &box) const
    {
        return OverlappingImpl(box) || box.OverlappingImpl(*this);
    }
}