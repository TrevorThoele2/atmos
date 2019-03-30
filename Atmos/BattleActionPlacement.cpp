
#include "BattleActionPlacement.h"

#include "Battle.h"
#include "WorldManager.h"

namespace Atmos
{
    /*
    namespace Battle
    {
        void ActionPlacement::StartImpl()
        {
            // Copy player party
            charactersToPlace.clear();
            for (auto &squadLoop : Ent::PlayerParty::Instance().squads)
            {
                for (auto &playerLoop : squadLoop)
                {
                    if (playerLoop.GetEntity() != Ent::nullEntity)
                        charactersToPlace.push_back(playerLoop);
                }
            }
        }

        void ActionPlacement::StopImpl()
        {
            placementSet.clear();
        }

        bool ActionPlacement::CanStartImpl() const
        {
            return true;
        }

        void ActionPlacement::OnDeselectEntityImpl()
        {}

        void ActionPlacement::OnMouseKeyPressedImpl(const Input::MouseKey &arg)
        {
            switch (arg.id)
            {
            case Input::MouseButton::LEFT:
                {
                    auto &mousePos = Environment::GetInput()->GetMousePositionInGameCoordinates();
                    auto found = GetCurrentTiles()->FindHighest(GridPosition::DimensionFromPosition(static_cast<Position2D::ValueT>(mousePos.x)), GridPosition::DimensionFromPosition(static_cast<Position2D::ValueT>(mousePos.y)));
                    if(found)
                        PlaceAt(found->GetPosition());

                    break;
                }
            }
        }

        void ActionPlacement::PlaceAt(const GridPosition &position)
        {
            auto found = placementSet.find(position);
            if (found == placementSet.end())
                return;

            AddPlayer(charactersToPlace.back(), position);
            charactersToPlace.erase(--charactersToPlace.end());

            if (charactersToPlace.empty())
            {
                StartBattle();
                DeselectEntity();
            }
        }

        void ActionPlacement::AddPosition(const GridPosition &add)
        {
            placementSet.emplace(add);
        }
    }
    */
}