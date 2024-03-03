
#include "Position3D.h"

#include "Position2D.h"
#include "AxisBoundingBox3D.h"
#include "MathUtility.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Position3D)
    {
        scribe(X);
        scribe(Y);
        scribe(Z);
        scribe(pixelPerfect);
        if (scribe.IsInput())
            Calc();
    }

    void Position3D::Calc()
    {
        if (pixelPerfect)
        {
            realX = Round(X);
            realY = Round(Y);
            realZ = Round(Z);
        }
        else
        {
            realX = X;
            realY = Y;
            realZ = Z;
        }
    }

    Position3D::Position3D(ValueT X, ValueT Y, ValueT Z, bool pixelPerfect) : X(X), Y(Y), Z(Z), pixelPerfect(pixelPerfect)
    {
        Calc();
    }

    Position3D::Position3D(const Position2D &pos, ValueT Z) : X(pos.GetX()), Y(pos.GetY()), Z(Z), pixelPerfect(pos.GetPixelPerfect())
    {
        Calc();
    }

    Position3D::Position3D(const Position3D &arg) : X(arg.X), Y(arg.Y), Z(arg.Z), pixelPerfect(arg.pixelPerfect)
    {
        Calc();
    }

    Position3D& Position3D::operator=(const Position3D &arg)
    {
        X = arg.X;
        Y = arg.Y;
        Z = arg.Z;
        pixelPerfect = arg.pixelPerfect;
        Calc();
        return *this;
    }

    bool Position3D::operator==(const Position3D &arg) const
    {
        return realX == arg.realX && realY == arg.realY && realZ == arg.realZ;
    }

    bool Position3D::operator!=(const Position3D &arg) const
    {
        return !(*this == arg);
    }

    Position3D::operator Position2D() const
    {
        return Position2D(GetX(), GetY(), GetPixelPerfect());
    }

    void Position3D::Set(ValueT X, ValueT Y, ValueT Z)
    {
        this->X = X;
        this->Y = Y;
        this->Z = Z;
        Calc();
    }

    void Position3D::SetX(ValueT set)
    {
        X = set;
        Calc();
    }

    void Position3D::IncrementX(ValueT inc)
    {
        X += inc;
        Calc();
    }

    void Position3D::DecrementX(ValueT dec)
    {
        X -= dec;
        Calc();
    }

    void Position3D::SetY(ValueT set)
    {
        Y = set;
        Calc();
    }

    void Position3D::IncrementY(ValueT inc)
    {
        Y += inc;
        Calc();
    }

    void Position3D::DecrementY(ValueT dec)
    {
        Y -= dec;
        Calc();
    }

    void Position3D::SetZ(ValueT set)
    {
        Z = set;
        Calc();
    }

    void Position3D::IncrementZ(ValueT inc)
    {
        Z += inc;
        Calc();
    }

    void Position3D::DecrementZ(ValueT dec)
    {
        Z -= dec;
        Calc();
    }

    void Position3D::SetPixelPerfect(bool set)
    {
        pixelPerfect = set;
        Calc();
    }

    Position3D::ValueT Position3D::GetX() const
    {
        return realX;
    }

    Position3D::ValueT Position3D::GetY() const
    {
        return realY;
    }

    Position3D::ValueT Position3D::GetZ() const
    {
        return realZ;
    }

    bool Position3D::GetPixelPerfect() const
    {
        return pixelPerfect;
    }

    bool Position3D::Within(ValueT left, ValueT right, ValueT top, ValueT bottom, ValueT nearZ, ValueT farZ) const
    {
        return realX >= left && realX <= right && realY >= top && realY <= bottom && realZ <= nearZ && realZ >= farZ;
    }

    bool Position3D::Within(const AxisBoundingBox3D &box) const
    {
        return Within(box.GetLeft(), box.GetRight(), box.GetTop(), box.GetBottom(), box.GetNearZ(), box.GetFarZ());
    }

    Position3D Position3D::FromScreen(const Position3D &convert, const Position3D &topLeftScreen, ValueT z)
    {
        return Position3D(convert.GetX() + topLeftScreen.GetX(), convert.GetY() + topLeftScreen.GetY(), z);
    }

    typename Position3D::ValueT Position3D::FindDistance(const Position3D &starting, const Position3D &destination)
    {
        ValueT distanceX = (destination.GetX() - starting.GetX());
        ValueT distanceY = (destination.GetY() - starting.GetY());
        ValueT distanceZ = (destination.GetZ() - starting.GetZ());

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));
    }

    Position3D Position3D::FindCenter(const std::vector<Position3D> &container)
    {
        auto nearTopLeft = *container.begin();
        auto nearBottomRight = *container.begin();
        auto farTopLeft = *container.begin();

        for (auto loop = ++container.begin(); loop != container.end(); ++loop)
        {
            // Check X
            if (loop->GetX() < nearTopLeft.GetX())
                nearTopLeft.SetX(loop->GetX());
            else if (loop->GetX() > nearBottomRight.GetX())
                nearBottomRight.SetX(loop->GetX());

            // Check Y
            if (loop->GetY() < nearTopLeft.GetY())
                nearTopLeft.SetY(loop->GetY());
            else if (loop->GetY() > nearBottomRight.GetY())
                nearBottomRight.SetY(loop->GetY());

            // Check Z
            if (loop->GetZ() < nearTopLeft.GetZ())
                nearTopLeft.SetZ(loop->GetZ());
            else if (loop->GetZ() > farTopLeft.GetZ())
                farTopLeft.SetZ(loop->GetZ());
        }

        return FindCenter(nearTopLeft, nearBottomRight, farTopLeft);
    }

    Position3D Position3D::FindCenter(const Position3D &nearTopLeft, const Position3D &nearBottomRight, const Position3D &farTopLeft)
    {
        return Position3D(nearTopLeft.GetX() + ((nearBottomRight.GetX() - nearTopLeft.GetX()) / 2), nearTopLeft.GetY() + ((nearBottomRight.GetY() - nearTopLeft.GetY()) / 2), nearTopLeft.GetZ() + ((farTopLeft.GetZ() - nearTopLeft.GetZ()) / 2));
    }
}