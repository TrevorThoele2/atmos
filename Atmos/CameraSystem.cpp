
#include "CameraSystem.h"

#include "ObjectManager.h"
#include "DebugStatisticsSystem.h"

#include "TimeSystem.h"

#include "StringUtility.h"

namespace Atmos
{
    CameraSystem::CameraSystem(ObjectManager& manager) : ObjectSystem(manager)
    {}

    CameraSystem::CameraSystem(const ::Inscription::Table<CameraSystem>& table) :
        INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
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

    void CameraSystem::SetX(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetX(set);
    }

    void CameraSystem::SetY(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetY(set);
    }

    void CameraSystem::SetZ(Position3D::ValueT set)
    {
        ResetFocus();
        basePosition.SetZ(set);
    }

    Position3D::ValueT CameraSystem::GetX() const
    {
        return focusedPosition->GetX();
    }

    Position3D::ValueT CameraSystem::GetY() const
    {
        return focusedPosition->GetY();
    }

    Position3D::ValueT CameraSystem::GetZ() const
    {
        return focusedPosition->GetZ();
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

    const AxisBoundingBox2D& CameraSystem::GetSides() const
    {
        return sides;
    }

    void CameraSystem::Move(Direction direction, Position3D::ValueT by)
    {
        ResetFocus();

        auto& lastElapsed = FindTimeSystem()->GetLastElapsed();
        decltype(by) normalizedDistance = by * static_cast<float>(lastElapsed.ConvertValue(TimeValueEpoch::SECONDS));

        switch (direction.Get())
        {
        case Direction::UP:
            basePosition.DecrementY(normalizedDistance);
            break;
        case Direction::DOWN:
            basePosition.IncrementY(normalizedDistance);
            break;
        case Direction::LEFT:
            basePosition.DecrementX(normalizedDistance);
            break;
        case Direction::RIGHT:
            basePosition.IncrementX(normalizedDistance);
            break;
        }

        Work();
    }

    void CameraSystem::MoveBy(Position3D::ValueT x, Position3D::ValueT y, Position3D::ValueT z)
    {
        ResetFocus();

        basePosition.IncrementX(x);
        basePosition.IncrementY(y);
        basePosition.IncrementZ(z);

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

        basePosition.IncrementX(delta.GetX());
        basePosition.IncrementY(delta.GetY());
        basePosition.IncrementZ(delta.GetZ());
    }

    void CameraSystem::InitializeImpl()
    {
        auto debugStatistics = Manager()->FindSystem<DebugStatisticsSystem>();
        debugStatistics->windowPage.viewOriginX.retrievalFunction = [this]() -> String
        {
            return ToString(GetViewOrigin().GetX());
        };
        debugStatistics->windowPage.viewOriginY.retrievalFunction = [this]() -> String
        {
            return ToString(GetViewOrigin().GetY());
        };
    }

    void CameraSystem::CalculateSides()
    {
        auto halfWidth = size.GetWidth() / 2;
        auto halfHeight = size.GetHeight() / 2;

        sides.SetLeft(viewOrigin.GetX() - halfWidth);
        sides.SetRight(viewOrigin.GetX() + halfWidth);
        sides.SetTop(viewOrigin.GetY() - halfHeight);
        sides.SetBottom(viewOrigin.GetY() + halfHeight);

        topLeft.Set(sides.GetLeft(), sides.GetTop());
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
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::CameraSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::CameraSystem, "CameraSystem");
}