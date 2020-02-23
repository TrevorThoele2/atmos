#include "CameraCurator.h"

#include "Camera.h"
#include "CameraMoved.h"
#include "CameraResized.h"

#include <Arca/Reliquary.h>
#include "TimeInformation.h"

namespace Atmos::Render
{
    CameraCurator::CameraCurator(Init init) :
        Curator(init), camera(init.owner),
        debugViewOriginX(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.viewOriginX = camera->ViewOrigin().x;
            },
            init.owner),
        debugViewOriginY(
            [this](Debug::Statistics& statistics)
            {
                statistics.window.viewOriginY = camera->ViewOrigin().y;
            },
            init.owner)
    {}

    void CameraCurator::Work()
    {
        if (!IsFocusValid())
            return;

        auto viewOrigin = camera->ViewOrigin();
        const auto previousViewOrigin = viewOrigin;
        viewOrigin = static_cast<Position2D>(*camera->FocusedPosition());
        CalculateSides();

        if (previousViewOrigin != viewOrigin)
            Owner().Raise<CameraMoved>(camera);

        debugViewOriginX.Set();
        debugViewOriginY.Set();
    }

    void CameraCurator::Handle(const MoveCamera& command)
    {
        Move(command.direction, command.moveBy);
    }

    void CameraCurator::Handle(const MoveCameraBy& command)
    {
        MoveBy(command.x, command.y, command.z);
    }

    void CameraCurator::Handle(const MoveCameraToInstant& command)
    {
        MoveToInstant(command.toPosition);
    }

    void CameraCurator::Handle(const MoveCameraDeltaInstant& command)
    {
        MoveDeltaInstant(command.x, command.y, command.z);
    }

    void CameraCurator::Handle(const ResizeCamera& command)
    {
        Resize(command.toSize);
    }

    void CameraCurator::Move(Direction direction, Position3D::Value by)
    {
        ResetFocus();

        auto& lastFrameElapsed = Arca::Index<Time::Information>(Owner())->lastFrameElapsed;
        const auto normalizedDistance =
            by * static_cast<float>(lastFrameElapsed.GetAs(Time::Epoch::Seconds));

        switch (direction.Get())
        {
        case Direction::Left:
            basePosition.x -= normalizedDistance;
            break;
        case Direction::Up:
            basePosition.y -= normalizedDistance;
            break;
        case Direction::Right:
            basePosition.x += normalizedDistance;
            break;
        case Direction::Down:
            basePosition.y += normalizedDistance;
            break;
        default:
            break;
        }
    }

    void CameraCurator::MoveBy(Position3D::Value x, Position3D::Value y, Position3D::Value z)
    {
        ResetFocus();

        basePosition.x += x;
        basePosition.y += y;
        basePosition.z += z;
    }

    void CameraCurator::MoveToInstant(const Position3D& pos)
    {
        basePosition = pos;
        ResetFocus();
    }

    void CameraCurator::MoveDeltaInstant(Position3D::Value x, Position3D::Value y, Position3D::Value z)
    {
        ResetFocus();

        basePosition.x += x;
        basePosition.y += y;
        basePosition.z += z;
    }

    void CameraCurator::Resize(const ScreenSize& size)
    {
        MutablePointer(camera)->Size(size);
        Owner().Raise<CameraResized>(Arca::Index<Camera>(Owner()));
    }

    void CameraCurator::ResetFocus()
    {
        MutablePointer(camera)->FocusedPosition(&basePosition);
    }

    void CameraCurator::CalculateSides()
    {
        const auto dimensions = camera->Size();
        const auto viewOrigin = camera->ViewOrigin();
        auto screenSides = camera->ScreenSides();
        Position2D topLeft { screenSides.Left(), screenSides.Top() };

        const auto halfWidth = dimensions.width / 2;
        const auto halfHeight = dimensions.height / 2;

        screenSides.Left(viewOrigin.x - halfWidth);
        screenSides.Top(viewOrigin.y - halfHeight);
        screenSides.Right(viewOrigin.x + halfWidth);
        screenSides.Bottom(viewOrigin.y + halfHeight);

        topLeft.x = screenSides.Left();
        topLeft.y = screenSides.Top();
    }

    bool CameraCurator::IsFocusValid()
    {
        return camera->FocusedPosition() != nullptr;
    }
}
