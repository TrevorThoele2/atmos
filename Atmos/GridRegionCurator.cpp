#include "GridRegionCurator.h"

#include "MainSurface.h"

#include "RenderAlgorithms.h"

namespace Atmos::Render
{
    GridRegionCurator::GridRegionCurator(Init init) :
        Curator(init), camera(init.owner)
    {
        Owner().On<Arca::CreatedKnown<GridRegion>>(
            [this](const Arca::CreatedKnown<GridRegion>& signal)
            {
                OnCreated(signal);
            });

        Owner().On<Arca::DestroyingKnown<GridRegion>>(
            [this](const Arca::DestroyingKnown<GridRegion>& signal)
            {
                OnDestroying(signal);
            });
    }

    void GridRegionCurator::Work()
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
            if (!value.material || value.points.empty())
                continue;

            auto mesh = ConvertToMesh(Triangulate(value.points));
            for(auto& vertex : mesh.vertices)
            {
                vertex.x -= cameraLeft;
                vertex.y -= cameraTop;
            }

            const RegionRender render
            {
                mesh,
                value.z * Grid::CellSize<Position3D::Value>,
                value.material
            };
            mainSurface->StageRender(render);
        }
    }

    void GridRegionCurator::OnCreated(const Arca::CreatedKnown<GridRegion>& signal)
    {
        octree.Add(signal.reference.ID(), signal.reference, BoxFor(signal.reference));
    }

    void GridRegionCurator::OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal)
    {
        octree.Remove(signal.reference.ID(), BoxFor(signal.reference));
    }

    AxisAlignedBox3D GridRegionCurator::BoxFor(const std::vector<Grid::Position>& points, Grid::Position::Value z)
    {
        if (points.empty())
            return {};

        auto maxLeft = points.begin()->x;
        auto maxTop = points.begin()->y;
        auto maxRight = points.begin()->x;
        auto maxBottom = points.begin()->y;

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

        const auto cellSize = Grid::CellSize<Position3D::Value>;
        const auto width = static_cast<Position3D::Value>(maxRight - maxLeft) * cellSize;
        const auto height = static_cast<Position3D::Value>(maxBottom - maxTop) * cellSize;
        const auto depth = 1;

        const auto useMaxLeft = maxLeft * cellSize;
        const auto useMaxTop = maxTop * cellSize;

        return AxisAlignedBox3D
        {
            Position3D
            {
                useMaxLeft + width / 2,
                useMaxTop + height / 2,
                0.5f
            },
            Size3D { width, height, depth }
        };
    }

    AxisAlignedBox3D GridRegionCurator::BoxFor(const Index& index)
    {
        return BoxFor(index->points, index->z);
    }
}