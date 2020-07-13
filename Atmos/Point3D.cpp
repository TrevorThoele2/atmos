#include "Point3D.h"

#include "AxisAlignedBox3D.h"
#include "MathUtility.h"

#include <Inscription/Scribe.h>

namespace Atmos::Spatial
{
    bool Point3D::operator==(const Point3D& arg) const
    {
        return x == arg.x && y == arg.y && z == arg.z;
    }

    bool Point3D::operator!=(const Point3D& arg) const
    {
        return !(*this == arg);
    }

    bool Point3D::Contains(Value left, Value right, Value top, Value bottom, Value nearZ, Value farZ) const
    {
        return x >= left && x <= right && y >= top && y <= bottom && z <= nearZ && z >= farZ;
    }

    bool Point3D::Contains(const AxisAlignedBox3D& box) const
    {
        return Contains(box.Left(), box.Right(), box.Top(), box.Bottom(), box.NearZ(), box.FarZ());
    }

    Point3D Point3D::FromScreen(const Point3D& convert, const Point3D& topLeftScreen, Value z)
    {
        return Point3D{ convert.x + topLeftScreen.x, convert.y + topLeftScreen.y, z };
    }

    auto Point3D::DistanceBetween(const Point3D& starting, const Point3D& destination) -> Value
    {
        const auto distanceX = (destination.x - starting.x);
        const auto distanceY = (destination.y - starting.y);
        const auto distanceZ = (destination.z - starting.z);

        return sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));
    }

    Point3D Point3D::CenterOf(const std::vector<Point3D>& container)
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

        return CenterOf(nearTopLeft, nearBottomRight, farTopLeft);
    }

    Point3D Point3D::CenterOf
    (
        const Point3D& nearTopLeft,
        const Point3D& nearBottomRight,
        const Point3D& farTopLeft
    ) {
        return Point3D
        {
            nearTopLeft.x + ((nearBottomRight.x - nearTopLeft.x) / 2),
            nearTopLeft.y + ((nearBottomRight.y - nearTopLeft.y) / 2),
            nearTopLeft.z + ((farTopLeft.z - nearTopLeft.z) / 2)
        };
    }
}

namespace Inscription
{
    void Scribe<Atmos::Spatial::Point3D, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.x);
        archive(object.y);
        archive(object.z);
    }
}