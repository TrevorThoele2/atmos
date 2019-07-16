#include "CameraSystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "TimeSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    CameraSystem::CameraSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    void CameraSystem::Work()
    {
        if (!IsFocusValid())
            return;

        auto prevVO = viewOrigin;
        viewOrigin = *focusedPosition;
        CalculateSides();

        if (prevVO != viewOrigin)
            eventMoved();
    }

    void CameraSystem::SetSize(const Size2D& size)
    {
        this->size = size;
        CalculateSides();
    }

    void CameraSystem::ResetFocus()
    {
        focusedPosition = &basePosition;
        Work();
    }

    void CameraSystem::SetFocus(const Position3D* setTo)
    {
        if (!setTo)
        {
            ResetFocus();
            return;
        }

        focusedPosition = setTo;
        Work();
    }

    void CameraSystem::SetZoom(Zoom set)
    {
        zoom = set;
    }

    void CameraSystem::SetX(Position3D::Value set)
    {
        ResetFocus();
        basePosition.x = set;
    }

    void CameraSystem::SetY(Position3D::Value set)
    {
        ResetFocus();
        basePosition.y = set;
    }

    void CameraSystem::SetZ(Position3D::Value set)
    {
        ResetFocus();
        basePosition.z = set;
    }

    Position3D::Value CameraSystem::GetX() const
    {
        return focusedPosition->x;
    }

    Position3D::Value CameraSystem::GetY() const
    {
        return focusedPosition->y;
    }

    Position3D::Value CameraSystem::GetZ() const
    {
        return focusedPosition->z;
    }

    const Position2D& CameraSystem::GetTopLeft() const
    {
        return topLeft;
    }

    const Position2D& CameraSystem::GetViewOrigin() const
    {
        return viewOrigin;
    }

    Position3D CameraSystem::GetViewOrigin3D() const
    {
        return Position3D(viewOrigin, zoom);
    }

    const Size2D& CameraSystem::GetSize() const
    {
        return size;
    }

    const AxisAlignedBox2D& CameraSystem::GetSides() const
    {
        return sides;
    }

    void CameraSystem::Move(Direction direction, Position3D::Value by)
    {
        ResetFocus();

        auto& lastElapsed = FindTimeSystem()->GetLastElapsed();
        decltype(by) normalizedDistance = by * static_cast<float>(lastElapsed.ConvertValue(TimeValueEpoch::SECONDS));

        switch (direction.Get())
        {
        case Direction::LEFT:
            basePosition.x -= normalizedDistance;
            break;
        case Direction::UP:
            basePosition.y -= normalizedDistance;
            break;
        case Direction::RIGHT:
            basePosition.x += normalizedDistance;
            break;
        case Direction::DOWN:
            basePosition.y += normalizedDistance;
            break;
        }

        Work();
    }

    void CameraSystem::MoveBy(Position3D::Value x, Position3D::Value y, Position3D::Value z)
    {
        ResetFocus();

        basePosition.x += x;
        basePosition.y += y;
        basePosition.z += z;

        Work();
    }

    void CameraSystem::MoveToInstant(const Position3D& pos)
    {
        basePosition = pos;
        ResetFocus();

        Work();
    }

    void CameraSystem::MoveDeltaInstant(const Position3D& delta)
    {
        ResetFocus();

        basePosition.x += delta.x;
        basePosition.y += delta.y;
        basePosition.z += delta.z;
    }

    void CameraSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->windowPage.viewOriginX.retrievalFunction = [this]() -> String
        {
            return ToString(GetViewOrigin().x);
        };
        debugStatistics->windowPage.viewOriginY.retrievalFunction = [this]() -> String
        {
            return ToString(GetViewOrigin().y);
        };
    }

    void CameraSystem::CalculateSides()
    {
        auto halfWidth = size.scaledWidth / 2;
        auto halfHeight = size.scaledHeight / 2;

        sides.left = viewOrigin.x - halfWidth;
        sides.top = viewOrigin.y - halfHeight;
        sides.right = viewOrigin.x + halfWidth;
        sides.bottom = viewOrigin.y + halfHeight;

        topLeft.x = sides.left;
        topLeft.y = sides.top;
    }

    bool CameraSystem::IsFocusValid()
    {
        return focusedPosition != nullptr;
    }

    TimeSystem* CameraSystem::FindTimeSystem()
    {
        return Manager()->FindSystem<TimeSystem>();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::CameraSystem, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}