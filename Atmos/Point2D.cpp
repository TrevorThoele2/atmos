#include "Point2D.h"

#include "AxisAlignedBox2D.h"
#include <Inscription/Scribe.h>

namespace Atmos::Spatial
{
    Point2D::Point2D(Value x, Value y) : x(x), y(y)
    {}

    bool Point2D::operator==(const Point2D& arg) const
    {
        return x == arg.x && y == arg.y;
    }

    bool Point2D::operator!=(const Point2D& arg) const
    {
        return !(*this == arg);
    }

    bool Point2D::Contains(Value left, Value right, Value top, Value bottom) const
    {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    bool Point2D::Contains(const AxisAlignedBox2D& box) const
    {
        return Contains(box.Left(), box.Right(), box.Top(), box.Bottom());
    }

    Point2D Point2D::FromScreen(const Point2D& convert, const Point2D& topLeftScreen)
    {
        return Point2D{ convert.x + topLeftScreen.x, convert.y + topLeftScreen.y };
    }

    Point2D Point2D::CenterOf(const std::vector<Point2D>& container)
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

    Point2D Point2D::CenterOf(const std::set<Point2D>& container)
    {
        return CenterOf(*container.begin(), *container.rbegin());
    }

    Point2D Point2D::CenterOf(const Point2D& topLeft, const Point2D& bottomRight)
    {
        return Point2D
        {
            topLeft.x + ((bottomRight.x - topLeft.x) / 2),
            topLeft.y + ((bottomRight.y - topLeft.y) / 2)
        };
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Point2D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
    }
}