
#include "BattleActionAbility.h"

namespace Atmos
{
    /*
    namespace Battle
    {
        void ActionAbility::AttackPattern::SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value)
        {
            value.selectionSprite.SetPosition(GridPosition(center, relPos));
        }

        void ActionAbility::RangePattern::SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value)
        {
            value.SetPosition(GridPosition(center, relPos));
        }

        void ActionAbility::StartImpl()
        {
            auto ability = AbilityUsing::Current();
            for (auto& loop : ability->GetHitPattern())
                attackPattern.AddPiece(loop.first, AttackPiece(*loop.second));
            
            auto &position = GetSelectedEntity()->GetPosition().Get();
            // Setup the range pattern
            rangePattern.SetupDiamond(position, spell->GetRange(), Sprite("overlay.png", 1, Atmos::Color(127, 0, 0, 255)));
            for (auto& loop : rangePattern)
                GetCurrentRenderFragments()->Add(loop.second);

            // Setup the attack pattern
            attackPattern.SetPosition(position);
            for (auto& loop : attackPattern)
            {
                loop.second.selectionSprite.InformPositionChange(position.z + 0.999f);
                GetCurrentRenderFragments()->Add(loop.second.selectionSprite.Get());
            }
        }

        void ActionAbility::StopImpl()
        {
            for (auto& loop : attackPattern)
                GetCurrentRenderFragments()->Remove(loop.second.selectionSprite);

            rangePattern.Clear();
            attackPattern.Clear();
        }

        bool ActionAbility::CanStartImpl() const
        {
            return AbilityUsing::IsActive();
        }

        void ActionAbility::OnDeselectEntityImpl()
        {
            ThisIsDone();
        }

        void ActionAbility::OnActionPressedImpl(const Input::Action &arg)
        {
            auto handleMoveKey = [&](GridPosition::ValueT x, GridPosition::ValueT y)
            {
                // First, find the relative tile position between the entity and the current attack pattern position
                RelativeGridPosition relPosition(GetSelectedEntity()->GetPosition()->Difference(attackPattern.GetPosition()));
                relPosition.x += x;
                relPosition.y += y;
                // Find if this relative tile position is in the range pattern
                if (rangePattern.IsWithin(relPosition))
                    attackPattern.SetPosition(GridPosition(*GetSelectedEntity()->GetPosition(), relPosition));
            };

            switch (arg.id)
            {
            case Atmos::Input::ActionID::USE:
            {
                auto found = GetCurrentTiles()->Find(attackPattern.GetPosition());
                if (found)
                {
                    AbilityUsing::Finish(*found);
                    SelectedEntityEndTurn();
                    ThisIsDone();
                    return;
                }

                break;
            }
            case Input::ActionID::MOVE_UP:
                handleMoveKey(0, -1);
                break;
            case Input::ActionID::MOVE_DOWN:
                handleMoveKey(0, 1);
                break;
            case Input::ActionID::MOVE_LEFT:
                handleMoveKey(-1, 0);
                break;
            case Input::ActionID::MOVE_RIGHT:
                handleMoveKey(1, 0);
                break;
            case Input::ActionID::CANCEL:
                ThisIsDone();
                break;
            }
        }

        ActionAbility::ActionAbility()
        {}
    }
    */
}