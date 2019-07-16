
#include "Position3D.h"

#include "Position2D.h"
#include "AxisAlignedBox3D.h"
#include "MathUtility.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Position3D::Position3D() : x(0), y(0), z(0)
    {}

    Position3D::Position3D(Value x, Value y, Value z) : x(x), y(y), z(z)
    {}

    Position3D::Position3D(const Position2D& pos, Value z) : x(pos.x), y(pos.y), z(z)
    {}

    Position3D::Position3D(const Position3D& arg) : x(arg.x), y(arg.y), z(arg.z)
    {}

    Position3D& Position3D::operator=(const Position3D &arg)
    {
        x = arg.x;
        y = arg.y;
        z = arg.z;
        return *this;
    }

    bool Position3D::operator==(const Position3D &arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Position3D::operator!=(const Position3D &arg) const
    {
        return !(*this == arg);
    }

    Position3D::operator Position2D() const
    {
        return Position2D(x, y);
    }

    bool Position3D::Within(Value left, Value right, Value top, Value bottom, Value nearZ, Value farZ) const
    {
        return x >= left && x <= right && y >= top && y <= bottom && z <= nearZ && z >= farZ;
    }

    bool Position3D::Within(const AxisAlignedBox3D& box) const
    {
        return Within(box.left, box.right, box.top, box.bottom, box.nearZ, box.farZ);
    }

    Position3D Position3D::FromScreen(const Position3D& convert, const Position3D& topLeftScreen, Value z)
    {
        return Position3D(convert.x + topLeftScreen.x, convert.y + topLeftScreen.y, z);
    }

    typename Position3D::Value Position3D::FindDistance(const Position3D& starting, const Position3D& destination)
    {
        Value distanceX = (destination.x - starting.x);
        Value distanceY = (destination.y - starting.y);
        Value distanceZ = (destination.z - starting.z);

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));
    }

    Position3D Position3D::FindCenter(const std::vector<Position3D>& container)
    {
        auto nearTopLeft = *container.begin();
        auto nearBottomRight = *container.begin();
        auto farTopLeft = *container.begin();

        for (auto loop = ++container.begin(); loop != container.end(); ++loop)
        {
            // Check X
            if (loop->x < nearTopLeft.x)
                nearTopLeft.x = loop->x;
            else if (loop->x > nearBottomRight.x)
                nearBottomRight.x = loop->x;

            // Check Y
            if (loop->y < nearTopLeft.y)
                nearTopLeft.y = loop->y;
            else if (loop->y > nearBottomRight.y)
                nearBottomRight.y = loop->y;

            // Check Z
            if (loop->z < nearTopLeft.z)
                nearTopLeft.z = loop->z;
            else if (loop->z > farTopLeft.z)
                farTopLeft.z = loop->z;
        }

        return FindCenter(nearTopLeft, nearBottomRight, farTopLeft);
    }

    Position3D Position3D::FindCenter(const Position3D& nearTopLeft, const Position3D& nearBottomRight, const Position3D& farTopLeft)
    {
        return Position3D(
            nearTopLeft.x + ((nearBottomRight.x - nearTopLeft.x) / 2),
            nearTopLeft.y + ((nearBottomRight.y - nearTopLeft.y) / 2),
            nearTopLeft.z + ((farTopLeft.z - nearTopLeft.z) / 2));
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Position3D, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}