#include "CameraCurator.h"

#include "Camera.h"
#include "CameraMoved.h"

#include <Arca/Reliquary.h>
#include "TimeInformation.h"

namespace Atmos::Render
{
    void CameraCurator::InitializeImplementation()
    {
        Owner().ExecuteOn<MoveCamera>(
            [this](const MoveCamera& signal)
            {
                Move(signal.direction, signal.moveBy);
            });

        Owner().ExecuteOn<MoveCameraBy>(
            [this](const MoveCameraBy& signal)
            {
                MoveBy(signal.x, signal.y, signal.z);
            });

        Owner().ExecuteOn<MoveCameraToInstant>(
            [this](const MoveCameraToInstant& signal)
            {
                MoveToInstant(signal.toPosition);
            });

        Owner().ExecuteOn<MoveCameraDeltaInstant>(
            [this](const MoveCameraDeltaInstant& signal)
            {
                MoveDeltaInstant(signal.x, signal.y, signal.z);
            });

        debugStatistics = Owner().Find<Debug::Statistics>();
    }

    void CameraCurator::WorkImplementation(Stage& stage)
    {
        if (!IsFocusValid())
            return;

        auto viewOrigin = camera->ViewOrigin();
        const auto previousViewOrigin = viewOrigin;
        viewOrigin = static_cast<Position2D>(*camera->FocusedPosition());
        CalculateSides();

        if (previousViewOrigin != viewOrigin)
            Owner().Raise<CameraMoved>(*camera);

        debugStatistics->window.viewOriginX = camera->ViewOrigin().x;
        debugStatistics->window.viewOriginY = camera->ViewOrigin().y;
    }

    void CameraCurator::Move(Direction direction, Position3D::Value by)
    {
        ResetFocus();

        auto& lastFrameElapsed = Owner().Find<Time::Information>()->lastFrameElapsed;
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

    void CameraCurator::ResetFocus()
    {
        camera->FocusedPosition(&basePosition);
    }

    void CameraCurator::CalculateSides()
    {
        const auto size = camera->Size();
        const auto viewOrigin = camera->ViewOrigin();
        auto screenSides = camera->ScreenSides();
        Position2D topLeft { screenSides.Left(), screenSides.Top() };

        const auto halfWidth = size.width / 2;
        const auto halfHeight = size.height / 2;

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