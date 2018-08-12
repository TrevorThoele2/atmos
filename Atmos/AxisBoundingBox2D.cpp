
#include "AxisBoundingBox2D.h"

#include "MathUtility.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AxisBoundingBox2D)
    {
        scribe(left, right, top, bottom);
        scribe(pixelPerfect);
        if (scribe.IsInput())
            Calc();
    }

    void AxisBoundingBox2D::Calc()
    {
        if (!pixelPerfect)
        {
            realTop = top;
            realBottom = bottom;
            realLeft = left;
            realRight = right;
        }
        else
        {
            realTop = Round(top);
            realBottom = Round(bottom);
            realLeft = Round(left);
            realRight = Round(right);
        }

        center.SetX(GetWidth() / 2);
        center.SetY(GetHeight() / 2);
    }

    AxisBoundingBox2D::AxisBoundingBox2D(bool pixelPerfect) : pixelPerfect(pixelPerfect), left(0), right(0), top(0), bottom(0)
    {
        Calc();
    }

    AxisBoundingBox2D::AxisBoundingBox2D(Coordinate left, Coordinate top, Coordinate right, Coordinate bottom, bool pixelPerfect) : left(left), top(top), right(right), bottom(bottom), pixelPerfect(pixelPerfect)
    {
        Calc();
    }

    bool AxisBoundingBox2D::operator==(const AxisBoundingBox2D &arg) const
    {
        return pixelPerfect == arg.pixelPerfect && left == arg.left && right == arg.right && top == arg.top && bottom == arg.bottom && realLeft == arg.realLeft && realRight == arg.realRight && realTop == arg.realTop && realBottom == arg.realBottom;
    }

    bool AxisBoundingBox2D::operator!=(const AxisBoundingBox2D &arg) const
    {
        return !(*this == arg);
    }

    void AxisBoundingBox2D::SetLeft(Coordinate setTo)
    {
        left = setTo;
        Calc();
    }

    void AxisBoundingBox2D::SetRight(Coordinate setTo)
    {
        right = setTo;
        Calc();
    }

    void AxisBoundingBox2D::SetTop(Coordinate setTo)
    {
        top = setTo;
        Calc();
    }

    void AxisBoundingBox2D::SetBottom(Coordinate setTo)
    {
        bottom = setTo;
        Calc();
    }

    void AxisBoundingBox2D::Set(Coordinate top, Coordinate bottom, Coordinate left, Coordinate right)
    {
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
        Calc();
    }

    void AxisBoundingBox2D::IncLeft(Coordinate toInc)
    {
        left += toInc;
        Calc();
    }

    void AxisBoundingBox2D::IncRight(Coordinate toInc)
    {
        right += toInc;
        Calc();
    }

    void AxisBoundingBox2D::IncTop(Coordinate toInc)
    {
        top += toInc;
        Calc();
    }

    void AxisBoundingBox2D::IncBottom(Coordinate toInc)
    {
        bottom += toInc;
        Calc();
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetLeft() const
    {
        return realLeft;
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetRight() const
    {
        return realRight;
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetTop() const
    {
        return realTop;
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetBottom() const
    {
        return realBottom;
    }

    const Position2D& AxisBoundingBox2D::GetCenter() const
    {
        return center;
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetWidth() const
    {
        return GetRight() - GetLeft();
    }

    AxisBoundingBox2D::Coordinate AxisBoundingBox2D::GetHeight() const
    {
        return GetBottom() - GetTop();
    }

    Size2D AxisBoundingBox2D::GetSize() const
    {
        return Size2D(GetWidth(), GetHeight());
    }

    void AxisBoundingBox2D::SetPixelPerfect(bool setTo)
    {
        pixelPerfect = setTo;
    }

    bool AxisBoundingBox2D::IsHit(const Position2D &check) const
    {
        return Within(check);
    }

    bool AxisBoundingBox2D::Within(const Position2D &check) const
    {
        return check.GetX() >= realLeft && check.GetY() <= realRight && check.GetY() >= realTop && check.GetY() <= realBottom;
    }

    bool AxisBoundingBox2D::Within(const AxisBoundingBox2D &box) const
    {
        return realLeft >= box.realLeft && realRight <= box.realRight && realTop >= box.realTop && realBottom <= box.realBottom;
    }

    bool AxisBoundingBox2D::Overlapping(const AxisBoundingBox2D &box) const
    {
        const Position2D topLeft(realLeft, realTop);
        const Position2D topRight(realRight, realTop);
        const Position2D bottomLeft(realLeft, realBottom);
        const Position2D bottomRight(realRight, realBottom);

        return topLeft.Within(box) || bottomRight.Within(box) || topRight.Within(box) || bottomLeft.Within(box);
    }
}