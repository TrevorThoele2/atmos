#include "LineCurator.h"

#include "MainSurface.h"

namespace Atmos::Render
{
    LineCurator::LineCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().ExecuteOn<Arca::CreatedKnown<Line>>(
            [this](const Arca::CreatedKnown<Line>& signal)
            {
                OnLineCreated(signal);
            });

        Owner().ExecuteOn<Arca::DestroyingKnown<Line>>(
            [this](const Arca::DestroyingKnown<Line>& signal)
            {
                OnLineDestroying(signal);
            });
    }

    void LineCurator::Work()
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

        auto lines = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : lines)
        {
            auto& line = *index->value;
            if (!line.material)
                continue;

            std::vector<Position2D> adjustedPoints;
            for (auto& point : line.points)
                adjustedPoints.push_back(Position2D{ point.x - cameraLeft, point.y - cameraTop });

            const LineRender render
            {
                adjustedPoints,
                line.z,
                line.material,
                line.width,
                line.color
            };
            mainSurface->StageRender(render);
        }
    }

    void LineCurator::Handle(const MoveLine& command)
    {
        const auto index = Arca::Index<Line>(command.id, Owner());
        if (!index)
            return;

        const auto prevPoints = index->points;
        const auto prevZ = index->z;

        auto data = MutablePointer().Of(index);
        if(command.points)
            data->points = *command.points;

        if (command.z)
            data->z = *command.z;

        octree.Move(index.ID(), index, BoxFor(prevPoints, prevZ), BoxFor(index));
    }

    void LineCurator::OnLineCreated(const Arca::CreatedKnown<Line>& line)
    {
        octree.Add(line.reference.ID(), line.reference, BoxFor(line.reference));
    }

    void LineCurator::OnLineDestroying(const Arca::DestroyingKnown<Line>& line)
    {
        octree.Remove(line.reference.ID(), BoxFor(line.reference));
    }

    AxisAlignedBox3D LineCurator::BoxFor(const std::vector<Position2D>& points, Position2D::Value z)
    {
        if (points.empty())
            return {};

        auto maxLeft = points[0].x;
        auto maxTop = points[0].y;
        auto maxRight = points[0].x;
        auto maxBottom = points[0].y;

        for(auto& point : points)
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

    AxisAlignedBox3D LineCurator::BoxFor(const LineIndex& line)
    {
        return BoxFor(line->points, line->z);
    }
}