#include "LineCurator.h"

#include "MainSurface.h"
#include "RenderAlgorithms.h"
#include "StagedRasters.h"

namespace Atmos::Render
{
    LineCurator::LineCurator(Init init) : ObjectCurator(init)
    {
        Owner().On<Arca::CreatedKnown<Line>>(
            [this](const Arca::CreatedKnown<Line>& signal)
            {
                OnCreated(signal);
            });

        Owner().On<Arca::DestroyingKnown<Line>>(
            [this](const Arca::DestroyingKnown<Line>& signal)
            {
                OnDestroying(signal);
            });
    }

    void LineCurator::Handle(const MoveLine& command)
    {
        const auto index = Owner().Find<Line>(command.id);
        if (index)
        {
            const auto prevPoints = index->points;
            const auto prevZ = index->z;

            const auto data = MutablePointer().Of(index);
            if (command.points)
                data->points = *command.points;

            if (command.z)
                data->z = *command.z;

            octree.Move(index.ID(), index, BoxFor(prevPoints, prevZ), BoxFor(index));
        }
    }
    
    std::vector<Arca::RelicID> LineCurator::Handle(const FindLinesByBox& command) const
    {
        const auto indices = octree.AllWithin(command.box);
        std::vector<Arca::RelicID> returnValue;
        returnValue.reserve(indices.size());
        for (auto& index : indices)
            returnValue.push_back(index->id);
        return returnValue;
    }

    void LineCurator::WorkImpl(
        Spatial::AxisAlignedBox3D cameraBox,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto indices = octree.AllWithin(cameraBox);

        std::vector<Raster::Prepared<Raster::Line>> rasters;
        rasters.reserve(indices.size());
        for (auto& index : indices)
        {
            const auto raster = Raster(*index->value, cameraTopLeft, mainSurface);
            if (raster)
                rasters.push_back(*raster);
        }

        const auto stagedRasters = MutablePointer().Of<Raster::Staged>();
        stagedRasters->lines.insert(stagedRasters->lines.end(), rasters.begin(), rasters.end());
    }

    std::optional<Raster::Prepared<Raster::Line>> LineCurator::Raster(
        const Line& value,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto material = value.renderCore->material;
        if (material)
        {
            std::vector<Spatial::Point2D> adjustedPoints;
            for (auto& point : value.points)
                adjustedPoints.push_back(Spatial::Point2D{ point.x - cameraTopLeft.x, point.y - cameraTopLeft.y });

            return std::tuple
            {
                Raster::Line
                {
                    .material = material,
                    .points = adjustedPoints,
                    .width = value.width,
                    .color = value.renderCore->color
                },
                mainSurface.Resource(),
                Raster::Order
                {
                    .space = Ordering(Spatial::Space::World),
                    .z = value.z
                }
            };
        }
        else
            return {};
    }

    void LineCurator::OnCreated(const Arca::CreatedKnown<Line>& signal)
    {
        octree.Add(signal.index.ID(), signal.index, BoxFor(signal.index));
    }

    void LineCurator::OnDestroying(const Arca::DestroyingKnown<Line>& signal)
    {
        octree.Remove(signal.index.ID(), BoxFor(signal.index));
    }

    Spatial::AxisAlignedBox3D LineCurator::BoxFor(const std::vector<Spatial::Point2D>& points, Spatial::Point2D::Value z)
    {
        if (points.empty())
            return {};
        else
        {
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
            constexpr auto depth = 1;

            return Spatial::AxisAlignedBox3D
            {
                Spatial::Point3D { maxLeft + width / 2, maxTop + height / 2, 0.5f },
                Spatial::Size3D { width, height, depth }
            };
        }
    }

    Spatial::AxisAlignedBox3D LineCurator::BoxFor(const Index& index)
    {
        return BoxFor(index->points, index->z);
    }
}