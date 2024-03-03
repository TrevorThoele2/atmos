
#include "BattleActionAttack.h"

#include "InventoryComponent.h"

namespace Atmos
{
    /*   namespace Battle
    {
        void ActionAttack::AttackPattern::SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value)
        {
            value.selectionSprite.SetPosition(GridPosition(center, relPos));
        }

        void ActionAttack::RangePattern::SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value)
        {
            value.SetPosition(GridPosition(center, relPos));
        }

        void ActionAttack::StartImpl()
        {
            auto inventory = GetSelectedEntity()->FindOther<::Atmos::entity::InventoryComponent>();
            if (!inventory)
            {
                weapon = nullptr;
                return;
            }

            weapon = static_cast<const Weapon::Stack*>(inventory->equipment.Find(EquipSlot::MAINHAND));
            if (!weapon)
                return;

            auto &position = GetSelectedEntity()->GetPosition().Get();
            // Setup the range pattern
            rangePattern.SetupDiamond(position, weapon->GetRange(), Sprite("overlay.png", 1, Atmos::Color(127, 255, 0, 0)));
            for (auto& loop : rangePattern)
                GetCurrentRenderFragments()->Add(loop.second);

            // Setup the attack pattern
            attackPattern.Setup(position, weapon->GetHitPattern());
            for (auto& loop : attackPattern)
            {
                loop.second->selectionSprite.SetZ(position.z + 0.999f);
                GetCurrentRenderFragments()->Add(loop.second->selectionSprite);
            }
        }

        void ActionAttack::StopImpl()
        {
            for (auto& loop : attackPattern)
                GetCurrentRenderFragments()->Remove(loop.second.selectionSprite);

            rangePattern.Clear();
            attackPattern.Clear();
        }

        bool ActionAttack::CanStartImpl() const
        {
            return IsSelectedEntityPlayer() && !HasSelectedEntityTurnEnded();
        }

        void ActionAttack::OnDeselectEntityImpl()
        {
            ThisIsDone();
        }

        void ActionAttack::OnActionPressedImpl(const Input::Action &arg)
        {
            auto handleMoveKey = [&](int x, int y)
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
            case Atmos::Input::ActionID::ATTACK:
                for (auto& loop : attackPattern)
                {
                    auto found = FindCharacter(GridPosition(attackPattern.GetPosition(), loop.first));
                    if (found)
                    {
                        //weapon->Attack(found->GetEntity(), *found->GetAt<CharacterEntity::Combat>());
                        SelectedEntityEndTurn();
                        ThisIsDone();
                        return;
                    }
                }

                break;
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

        ActionAttack::ActionAttack() //: weapon(nullptr)
        {}
    }
    */
}