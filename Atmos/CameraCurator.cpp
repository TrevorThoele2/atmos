#include "CameraCurator.h"

#include "Camera.h"

#include <Arca/Reliquary.h>
#include "TimeInformation.h"

namespace Atmos::Render
{
    CameraCurator::CameraCurator(Init init) :
        Curator(init), camera(init.owner),
        debugCenterX(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.cameraCenterX = camera->center.x;
            },
            MutablePointer()),
        debugCenterY(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.cameraCenterY = camera->center.y;
            },
            MutablePointer())
    {}

    void CameraCurator::Work()
    {
        debugCenterX.Set();
        debugCenterY.Set();
    }

    void CameraCurator::Handle(const MoveCamera& command)
    {
        DoMove(
            [this, command]()
            {
                Move(command.direction, command.moveBy);
            });
    }

    void CameraCurator::Handle(const MoveCameraBy& command)
    {
        DoMove(
            [this, command]()
            {
                MoveBy(command.x, command.y);
            });
    }

    void CameraCurator::Handle(const MoveCameraTo& command)
    {
        DoMove(
            [this, command]()
            {
                MoveTo(command.toPosition);
            });
    }

    void CameraCurator::Handle(const ResizeCamera& command)
    {
        DoResize(
            [this, command]()
            {
                Resize(command.toSize);
            });
    }

    void CameraCurator::Move(Direction direction, Position3D::Value by)
    {
        auto& lastFrameElapsed = Arca::Index<Time::Information>(Owner())->lastFrameElapsed;
        const auto normalizedDistance =
            by * static_cast<float>(std::chrono::duration_cast<Time::Seconds>(lastFrameElapsed).count());

        auto mutableCamera = MutablePointer().Of(camera);

        switch (direction.Get())
        {
        case Direction::Left:
            mutableCamera->center.x -= normalizedDistance;
            break;
        case Direction::Up:
            mutableCamera->center.y -= normalizedDistance;
            break;
        case Direction::Right:
            mutableCamera->center.x += normalizedDistance;
            break;
        case Direction::Down:
            mutableCamera->center.y += normalizedDistance;
            break;
        default:
            break;
        }
    }

    void CameraCurator::MoveBy(Position3D::Value x, Position3D::Value y)
    {
        auto mutableCamera = MutablePointer().Of(camera);

        mutableCamera->center.x += x;
        mutableCamera->center.y += y;
    }

    void CameraCurator::MoveTo(const Position3D& position)
    {
        auto mutableCamera = MutablePointer().Of(camera);

        mutableCamera->center = position;
    }

    void CameraCurator::Resize(const ScreenSize& size)
    {
        MutablePointer().Of(camera)->size = size;
    }
}
