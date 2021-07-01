#include "LineCurator.h"

#include "MainSurface.h"

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
        const auto index = Arca::Index<Line>(command.id, Owner());
        if (index)
        {
            const auto prevPoints = index->points;
            const auto prevZ = index->z;

            auto data = MutablePointer().Of(index);
            if (command.points)
                data->points = *command.points;

            if (command.z)
                data->z = *command.z;

            octree.Move(index.ID(), index, BoxFor(prevPoints, prevZ), BoxFor(index));
        }
    }
    
    std::vector<Arca::RelicID> LineCurator::Handle(const FindLinesByBox& command) const
    {
        auto indices = octree.AllWithin(command.box);
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
        auto indices = octree.AllWithin(cameraBox);

        for (auto& index : indices)
            StageRender(*index->value, cameraTopLeft, mainSurface);
    }

    void LineCurator::StageRender(
        const Line& value,
        Spatial::Point2D cameraTopLeft,
        const MainSurface& mainSurface)
    {
        const auto material = value.renderCore->material;
        if (material)
        {
            const auto z = value.z;
            const auto width = value.width;
            const auto color = value.renderCore->color;

            std::vector<Spatial::Point2D> adjustedPoints;
            for (auto& point : value.points)
                adjustedPoints.push_back(Spatial::Point2D{ point.x - cameraTopLeft.x, point.y - cameraTopLeft.y });

            const LineRender render
            {
                adjustedPoints,
                z,
                material,
                width,
                color,
                ToRenderSpace(Spatial::Space::World)
            };
            mainSurface.StageRender(render);
        }
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
            const auto depth = 1;

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