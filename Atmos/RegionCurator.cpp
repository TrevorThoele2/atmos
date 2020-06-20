#include "RegionCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    RegionCurator::RegionCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().ExecuteOn<Arca::CreatedKnown<Region>>(
            [this](const Arca::CreatedKnown<Region>& signal)
            {
                OnCreated(signal);
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<Region>>(
            [this](const Arca::DestroyingKnown<Region>& signal)
            {
                OnDestroying(signal);
            });
    }

    void RegionCurator::Work()
    {
        const auto cameraLeft = camera->ScreenSides().Left();
        const auto cameraTop = camera->ScreenSides().Top();
        const auto cameraCenter = camera->center;
        const auto cameraSize = camera->size;

        const AxisAlignedBox3D queryBox
        {
            Position3D
            {
                cameraCenter.x,
                cameraCenter.y,
                0
            },
            Size3D
            {
                static_cast<Size3D::Value>(cameraSize.width),
                static_cast<Size3D::Value>(cameraSize.height),
                std::numeric_limits<Size3D::Value>::max()
            }
        };

        auto indices = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : indices)
        {
            auto& value = *index->value;
            if (!value.material)
                continue;

            std::vector<Position2D> adjustedPoints;
            for (auto& point : value.points)
                adjustedPoints.push_back(Position2D{ point.x - cameraLeft, point.y - cameraTop });

            const RegionRender render
            {
                adjustedPoints,
                value.z,
                value.material,
                value.width,
                value.color
            };
            mainSurface->StageRender(render);
        }
    }

    void RegionCurator::OnCreated(const Arca::CreatedKnown<Region>& signal)
    {
        octree.Add(signal.reference.ID(), signal.reference, BoxFor(signal.reference));
    }

    void RegionCurator::OnDestroying(const Arca::DestroyingKnown<Region>& signal)
    {
        octree.Remove(signal.reference.ID(), BoxFor(signal.reference));
    }

    AxisAlignedBox3D RegionCurator::BoxFor(const std::vector<Position2D>& points, Position2D::Value z)
    {
        if (points.empty())
            return {};

        auto maxLeft = points[0].x;
        auto maxTop = points[0].y;
        auto maxRight = points[0].x;
        auto maxBottom = points[0].y;

        for (auto& point : points)
        {
            if (point.x < maxLeft)
                maxLeft = point.x;
            else if (point.x > maxRight)
                maxRight = point.x;

            if (point.y < maxTop)
                maxTop = point.y;
            else if (point.y > maxBottom)
                maxBottom = point.y;
        }

        const auto width = maxRight - maxLeft;
        const auto height = maxBottom - maxTop;
        const auto depth = 1;

        return AxisAlignedBox3D
        {
            Position3D { maxLeft + width / 2, maxTop + height / 2, 0.5f },
            Size3D { width, height, depth }
        };
    }

    AxisAlignedBox3D RegionCurator::BoxFor(const Index& index)
    {
        return BoxFor(index->Points(), index->Z());
    }
}