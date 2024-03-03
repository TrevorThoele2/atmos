#include "GridRegionCurator.h"

#include "MainSurface.h"
#include "StagedRenders.h"

#include "RenderAlgorithms.h"
#include "RenderRegion.h"

namespace Atmos::Render
{
    GridRegionCurator::GridRegionCurator(Init init) : ObjectCurator(init)
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

    void GridRegionCurator::Handle(const MoveGridRegion& command)
    {
        AttemptChangeObject(
            command.id,
            [this, command](GridRegion& object)
            {
                const auto oldBox = BoxFor(object);

                if (command.points)
                    object.points = *command.points;

                if (command.z)
                    object.z = *command.z;

                const auto newBox = BoxFor(object);
                const auto index = Owner().Find<GridRegion>(command.id);
                octree.Move(command.id, index, oldBox, newBox);
            });
    }

    std::vector<Arca::RelicID> GridRegionCurator::Handle(const FindGridRegionsByBox& command) const
    {
        const auto indices = octree.AllWithin(command.box);
        std::vector<Arca::RelicID> returnValue;
        returnValue.reserve(indices.size());
        for (auto& index : indices)
            returnValue.push_back(index->id);
        return returnValue;
    }

    void GridRegionCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto indices = octree.AllWithin(cameraBox);

        std::vector<RenderRegion> renders;
        renders.reserve(indices.size());
        for (auto& index : indices)
        {
            const auto render = StageRender(*index->value, cameraTopLeft, mainSurface);
            if (render)
                renders.push_back(*render);
        }
        
        const auto stagedRenders = MutablePointer().Of<StagedRenders>();
        stagedRenders->regions.insert(stagedRenders->regions.end(), renders.begin(), renders.end());
    }

    std::optional<RenderRegion> GridRegionCurator::StageRender(
        const GridRegion& value,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto material = value.renderCore->material;
        if (material && !value.points.empty())
        {
            const auto points = std::vector<Spatial::Grid::Point>{ value.points.begin(), value.points.end() };
            auto mesh = ConvertToMesh(Triangulate(points));
            for (auto& vertex : mesh.vertices)
            {
                vertex.x -= cameraTopLeft.x;
                vertex.y -= cameraTopLeft.y;
            }

            const auto z = static_cast<Spatial::Point3D::Value>(value.z)
                * Spatial::Grid::CellSize<Spatial::Point3D::Value>;

            return RenderRegion
            {
                .mesh = mesh,
                .z = z,
                .material = material,
                .space = ToRenderSpace(Spatial::Space::World),
                .surface = mainSurface.Resource()
            };
        }
        else
            return {};
    }

    void GridRegionCurator::OnCreated(const Arca::CreatedKnown<GridRegion>& signal)
    {
        octree.Add(signal.index.ID(), signal.index, BoxFor(signal.index));
    }

    void GridRegionCurator::OnDestroying(const Arca::DestroyingKnown<GridRegion>& signal)
    {
        octree.Remove(signal.index.ID(), BoxFor(signal.index));
    }

    Spatial::AxisAlignedBox3D GridRegionCurator::BoxFor(
        const std::vector<Spatial::Grid::Point>& points, Spatial::Grid::Point::Value z)
    {
        if (points.empty())
            return {};
        else
        {
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
    }

    Spatial::AxisAlignedBox3D GridRegionCurator::BoxFor(const GridRegion& region)
    {
        const auto points = std::vector<Spatial::Grid::Point>{ region.points.begin(), region.points.end() };
        return BoxFor(points, region.z);
    }

    Spatial::AxisAlignedBox3D GridRegionCurator::BoxFor(const Index& index)
    {
        return BoxFor(*index);
    }
}