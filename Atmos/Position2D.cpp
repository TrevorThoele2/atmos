#include "Position2D.h"

#include "Position3D.h"
#include "AxisAlignedBox2D.h"
#include <Inscription\Scribe.h>

namespace Atmos
{
    bool Position2D::operator==(const Position2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Position2D::operator!=(const Position2D& arg) const
    {
        return !(*this == arg);
    }

    bool Position2D::Contains(Value left, Value right, Value top, Value bottom) const
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    bool Position2D::Contains(const AxisAlignedBox2D& box) const
    {
        return Contains(box.Left(), box.Right(), box.Top(), box.Bottom());
    }

    Position2D Position2D::FromScreen(const Position2D& convert, const Position2D& topLeftScreen)
    {
        return Position2D{ convert.x + topLeftScreen.x, convert.y + topLeftScreen.y };
    }

    typename Position2D::Value Position2D::DistanceBetween(const Position2D& starting, const Position2D& destination)
    {
        auto distanceX = (destination.x - starting.x);
        auto distanceY = (destination.y - starting.y);

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    }

    Position2D Position2D::CenterOf(const std::vector<Position2D>& container)
    {
        auto topLeft = *container.begin();
        auto bottomRight = *container.begin();
        for (auto loop = ++container.begin(); loop != container.end(); ++loop)
        {
            if (loop->x < topLeft.x)
                topLeft.x = loop->x;
            else if (loop->x > bottomRight.x)
                bottomRight.x = loop->x;

            if (loop->y < topLeft.y)
                topLeft.y = loop->y;
            else if (loop->y > bottomRight.y)
                bottomRight.y = loop->y;
        }

        return CenterOf(topLeft, bottomRight);
    }

    Position2D Position2D::CenterOf(const std::set<Position2D>& container)
    {
        return CenterOf(*container.begin(), *container.rbegin());
    }

    Position2D Position2D::CenterOf(const Position2D& topLeft, const Position2D& bottomRight)
    {
        return Position2D
        {
            topLeft.x + ((bottomRight.x - topLeft.x) / 2),
            topLeft.y + ((bottomRight.y - topLeft.y) / 2)
        };
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Position2D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
    }
}