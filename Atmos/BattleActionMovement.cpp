
#include "BattleActionMovement.h"
#include "EntityPositionSystem.h"

namespace Atmos
{
    namespace Battle
    {
        void ActionMovement::RangePattern::SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value)
        {
            value.SetPosition(GridPosition(center, relPos));
        }

        void ActionMovement::StartImpl()
        {
            ConstructRangePattern();
        }

        void ActionMovement::StopImpl()
        {
            rangePattern.Clear();
        }

        bool ActionMovement::CanStartImpl() const
        {
            return IsSelectedEntityPlayer() && !HasSelectedEntityTurnEnded();
        }

        void ActionMovement::OnDeselectEntityImpl()
        {
            ThisIsDone();
        }

        void ActionMovement::OnActionActiveImpl(const Input::Action &arg)
        {
            if (GetSelectedEntity().GetBattle()->movementRangeLeft == 0)
                return;

            bool moved = false;
            switch (arg.id)
            {
            case Input::ActionID::MOVE_UP:
            {
                moved = ::Atmos::Ent::PositionSystem::MoveEntity(GetSelectedEntity()->GetEntity(), Direction(Direction::UP));
                break;
            }
            case Input::ActionID::MOVE_DOWN:
            {
                moved = ::Atmos::Ent::PositionSystem::MoveEntity(GetSelectedEntity()->GetEntity(), Direction(Direction::DOWN));
                break;
            }
            case Input::ActionID::MOVE_LEFT:
            {
                moved = ::Atmos::Ent::PositionSystem::MoveEntity(GetSelectedEntity()->GetEntity(), Direction(Direction::LEFT));
                break;
            }
            case Input::ActionID::MOVE_RIGHT:
            {
                moved = ::Atmos::Ent::PositionSystem::MoveEntity(GetSelectedEntity()->GetEntity(), Direction(Direction::RIGHT));
                break;
            }
            }

            if (moved)
            {
                --GetSelectedEntity().GetBattle()->movementRangeLeft;
                ConstructRangePattern();
            }
        }

        void ActionMovement::OnActionPressedImpl(const Input::Action &arg)
        {
            switch (arg.id)
            {
            case Input::ActionID::CANCEL:
                ThisIsDone();
                break;
            }
        }

        void ActionMovement::ConstructRangePattern()
        {
            rangePattern.Clear();
            rangePattern.SetupDiamond(GetSelectedEntity().GetBattle()->startingPos, GetSelectedEntity().GetBattle()->movementRangeLeft, Sprite("overlay.png", 1, Atmos::Color(127, 0, 255, 0)));
            for (auto &loop : rangePattern)
                GetCurrentRenderFragments()->Add(loop.second);
        }
    }
}