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
        const auto cameraPosition = camera->Position();
        const auto cameraSize = camera->Size();

        const Spatial::AxisAlignedBox3D queryBox
        {
            Spatial::Point3D
            {
                cameraPosition.x,
                cameraPosition.y,
                0
            },
            Spatial::Size3D
            {
                static_cast<Spatial::Size3D::Value>(cameraSize.width),
                static_cast<Spatial::Size3D::Value>(cameraSize.height),
                std::numeric_limits<Spatial::Size3D::Value>::max()
            }
        };

        auto indices = octree.AllWithin(queryBox);

        const auto mainSurface = Arca::Index<MainSurface>(Owner());

        for (auto& index : indices)
        {
            const auto& value = *index->value;
            const auto material = value.material.Get();
            if (!material || value.points.empty())
                continue;

            auto mesh = ConvertToMesh(Triangulate(value.points));
            for(auto& vertex : mesh.vertices)
            {
                vertex.x -= cameraLeft;
                vertex.y -= cameraTop;
            }

            const auto z = value.z * Spatial::Grid::CellSize<Spatial::Point3D::Value>;

            const RegionRender render
            {
                mesh,
                z,
                material
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

    Spatial::AxisAlignedBox3D GridRegionCurator::BoxFor(
        const std::vector<Spatial::Grid::Point>& points, Spatial::Grid::Point::Value z)
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

        const auto cellSize = Spatial::Grid::CellSize<Spatial::Point3D::Value>;
        const auto width = static_cast<Spatial::Point3D::Value>(maxRight - maxLeft) * cellSize;
        const auto height = static_cast<Spatial::Point3D::Value>(maxBottom - maxTop) * cellSize;
        const auto depth = 1;

        const auto useMaxLeft = maxLeft * cellSize;
        const auto useMaxTop = maxTop * cellSize;

        return Spatial::AxisAlignedBox3D
        {
            Spatial::Point3D
            {
                useMaxLeft + width / 2,
                useMaxTop + height / 2,
                0.5f
            },
            Spatial::Size3D { width, height, depth }
        };
    }

    Spatial::AxisAlignedBox3D GridRegionCurator::BoxFor(const Index& index)
    {
        return BoxFor(index->points, index->z);
    }
}