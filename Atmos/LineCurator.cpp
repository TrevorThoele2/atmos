#include "LineCurator.h"

#include "GraphicsManager.h"

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
        auto graphics = Arca::Postulate<GraphicsManager*>(Owner()).Get();

        const AxisAlignedBox3D queryBox
        {
            Position3D
            {
                camera->ViewOrigin().x,
                camera->ViewOrigin().y,
                0
            },
            Size3D
            {
                static_cast<Size3D::Value>(camera->Size().width),
                static_cast<Size3D::Value>(camera->Size().height),
                std::numeric_limits<Size3D::Value>::max()
            }
        };

        auto lines = octree.AllWithin(queryBox);
        for (auto& index : lines)
        {
            auto& line = *index->value;

            auto from = line.from;
            from.x -= camera->ScreenSides().Left();
            from.y -= camera->ScreenSides().Top();

            auto to = line.to;
            to.x -= camera->ScreenSides().Left();
            to.y -= camera->ScreenSides().Top();

            LineRender render
            {
                from,
                to,
                line.z,
                line.width,
                line.color
            };
            graphics->StageRender(render);
        }
    }

    void LineCurator::Handle(const MoveLine& command)
    {
        const auto index = Arca::Index<Line>(command.id, Owner());
        if (!index)
            return;

        const auto prevFrom = index->from;
        const auto prevTo = index->to;
        const auto prevZ = index->z;

        auto data = MutablePointer(index);
        if(command.from)
            data->from = *command.from;

        if(command.to)
            data->to = *command.to;

        if (command.z)
            data->z = *command.z;

        octree.Move(index.ID(), index, BoxFor(prevFrom, prevTo, prevZ), BoxFor(index));
    }

    void LineCurator::OnLineCreated(const Arca::CreatedKnown<Line>& line)
    {
        octree.Add(line.reference.ID(), line.reference, BoxFor(line.reference));
    }

    void LineCurator::OnLineDestroying(const Arca::DestroyingKnown<Line>& line)
    {
        octree.Remove(line.reference.ID(), BoxFor(line.reference));
    }

    AxisAlignedBox3D LineCurator::BoxFor(const Position2D& from, const Position2D& to, Position2D::Value z)
    {
        const auto width = std::abs(from.x - to.x);
        const auto height = std::abs(from.y - to.y);
        const auto depth = 1;

        return AxisAlignedBox3D
        {
            Position3D { width / 2, height / 2, 0.5f },
            Size3D { width, height, depth }
        };
    }

    AxisAlignedBox3D LineCurator::BoxFor(const LineIndex& line)
    {
        return BoxFor(line->from, line->to, line->z);
    }
}