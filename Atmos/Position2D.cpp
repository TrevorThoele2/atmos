
#include "Position2D.h"
#include "Position3D.h"
#include "AxisBoundingBox2D.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Position2D)
    {
        scribe(X, Y);
        scribe(pixelPerfect);
        if (scribe.IsInput())
            Calc();
    }

    void Position2D::Calc()
    {
        if (pixelPerfect)
        {
            realX = std::floor(X + 0.5f);
            realY = std::floor(Y + 0.5f);
        }
        else
        {
            realX = X;
            realY = Y;
        }
    }

    Position2D::Position2D(ValueT X, ValueT Y, bool pixelPerfect) : X(X), Y(Y), pixelPerfect(pixelPerfect)
    {
        Calc();
    }

    Position2D::Position2D(const Position3D &arg) : X(arg.GetX()), Y(arg.GetY()), pixelPerfect(arg.GetPixelPerfect())
    {
        Calc();
    }

    Position2D::Position2D(const Position2D &arg) : X(arg.X), Y(arg.Y), pixelPerfect(arg.pixelPerfect)
    {
        Calc();
    }

    Position2D& Position2D::operator=(const Position2D &arg)
    {
        X = arg.X;
        Y = arg.Y;
        pixelPerfect = arg.pixelPerfect;
        Calc();
        return *this;
    }

    bool Position2D::operator==(const Position2D &arg) const
    {
        return realX == arg.realX && realY == arg.realY;
    }

    bool Position2D::operator!=(const Position2D &arg) const
    {
        return !(*this == arg);
    }

    void Position2D::Set(ValueT X, ValueT Y)
    {
        this->X = X;
        this->Y = Y;
        Calc();
    }

    void Position2D::SetX(ValueT set)
    {
        X = set;
        Calc();
    }

    void Position2D::IncrementX(ValueT inc)
    {
        X += inc;
        Calc();
    }

    void Position2D::DecrementX(ValueT dec)
    {
        X -= dec;
        Calc();
    }

    void Position2D::SetY(ValueT set)
    {
        Y = set;
        Calc();
    }

    void Position2D::IncrementY(ValueT inc)
    {
        Y += inc;
        Calc();
    }

    void Position2D::DecrementY(ValueT dec)
    {
        Y -= dec;
        Calc();
    }

    void Position2D::SetPixelPerfect(bool set)
    {
        pixelPerfect = set;
        Calc();
    }

    Position2D::ValueT Position2D::GetX() const
    {
        return realX;
    }

    Position2D::ValueT Position2D::GetY() const
    {
        return realY;
    }

    bool Position2D::GetPixelPerfect() const
    {
        return pixelPerfect;
    }

    bool Position2D::Within(ValueT left, ValueT right, ValueT top, ValueT bottom) const
    {
        return realX >= left && realX <= right && realY >= top && realY <= bottom;
    }

    bool Position2D::Within(const AxisBoundingBox2D &box) const
    {
        return Within(box.GetLeft(), box.GetRight(), box.GetTop(), box.GetBottom());
    }

    Position2D Position2D::FromScreen(const Position2D &convert, const Position2D &topLeftScreen)
    {
        return Position2D(convert.GetX() + topLeftScreen.GetX(), convert.GetY() + topLeftScreen.GetY());
    }

    typename Position2D::ValueT Position2D::FindDistance(const Position2D &starting, const Position2D &destination)
    {
        auto distanceX = (destination.GetX() - starting.GetX());
        auto distanceY = (destination.GetY() - starting.GetY());

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    Position2D Position2D::FindCenter(const std::vector<Position2D> &container)
    {
        auto topLeft = *container.begin();
        auto bottomRight = *container.begin();
        for (auto loop = ++container.begin(); loop != container.end(); ++loop)
        {
            if (loop->GetX() < topLeft.GetX())
                topLeft.SetX(loop->GetX());
            else if (loop->GetX() > bottomRight.GetX())
                bottomRight.SetX(loop->GetX());

            if (loop->GetY() < topLeft.GetY())
                topLeft.SetY(loop->GetY());
            else if (loop->GetY() > bottomRight.GetY())
                bottomRight.SetY(loop->GetY());
        }

        return FindCenter(topLeft, bottomRight);
    }

    Position2D Position2D::FindCenter(const std::set<Position2D> &container)
    {
        return FindCenter(*container.begin(), *container.rbegin());
    }

    Position2D Position2D::FindCenter(const Position2D &topLeft, const Position2D &bottomRight)
    {
        return Position2D(topLeft.GetX() + ((bottomRight.GetX() - topLeft.GetX()) / 2), topLeft.GetY() + ((bottomRight.GetY() - topLeft.GetY()) / 2));
    }
}