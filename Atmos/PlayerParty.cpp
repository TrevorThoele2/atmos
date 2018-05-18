
#include "PlayerParty.h"

#include "GeneralComponent.h"
#include "CharacterComponent.h"
#include "CombatComponent.h"
#include "MovementComponent.h"
#include "AvatarComponent.h"

#include "CurrentField.h"
#include "WorldManager.h"
#include "MainGame.h"
#include "Battle.h"

namespace Atmos
{
    namespace Ent
    {
        PlayerParty::PlayerParty()
        {
            ResetSquads();
        }

        void PlayerParty::Init()
        {
            SubscribeEvent(::Atmos::WorldManager::Instance().eventFieldSet, &PlayerParty::OnFieldSet, *this);
        }

        void PlayerParty::OnFieldSet(Field &field)
        {
            ResetSquads();
            for (auto &loop : GetCurrentEntities()->GetRawMap<CharacterComponent>())
            {
                if (loop.second.IsPlayer() && !GetCurrentEntities()->FindComponent<AvatarComponent>(loop.first))
                    Add(loop.first);
            }

            // Show/hide the render components
            for (auto &squadLoop : squads)
            {
                for(auto &loop : squadLoop)
                    if (loop.GetEntity() != nullEntity)
                        loop.GetSense()->Show(!battleState.IsTop());
            }
        }

        void PlayerParty::ResetSquads()
        {
            Squad fillWith;
            fillWith.fill(nullEntity);
            squads.fill(fillWith);
        }

        bool PlayerParty::IsOutOfRange(SizeT position)
        {
            return position < maxSize;
        }

        bool PlayerParty::IsOutOfRange(SizeT squad, SizeT position)
        {
            return squad < squadCount || position < squadSize;
        }

        Entity PlayerParty::Add(Entity add)
        {
            if (add == nullEntity)
                return nullEntity;

            // Check if this is a character and has a combat component
            {
                auto &components = GetCurrentEntities()->FindMultipleComponents<CharacterComponent*, CombatComponent*, MovementComponent*, BattleComponent*>(add);
                auto character = std::get<0>(components);
                auto combat = std::get<1>(components);
                auto movement = std::get<2>(components);
                auto battle = std::get<3>(components);

                if (!character || !combat || !movement)
                    return nullEntity;

                if (!battle)
                    GetCurrentEntities()->CreateComponent<BattleComponent>(add);

                character->SetPlayer();
            }

            for (auto &squadLoop : Instance().squads)
            {
                for(auto &loop : squadLoop)
                {
                    if (loop->GetEntity() == nullEntity)
                    {
                        if (!loop.SetEntity(add))
                            return nullEntity;

                        GetCurrentEntities()->SignalTransfer(add);
                        return loop->GetEntity();
                    }
                }
            }

            return nullEntity;
        }

        bool PlayerParty::Erase(SizeT position)
        {
            if (IsOutOfRange(position))
                return false;

            Instance().squads[position / squadCount][position % squadCount] = nullEntity;
            return true;
        }

        bool PlayerParty::Erase(SizeT squad, SizeT position)
        {
            if (IsOutOfRange(squad, position))
                return false;

            Instance().squads[squad][position] = nullEntity;
            return true;
        }

        Entity PlayerParty::Find(const Name &name)
        {
            for (auto &squadLoop : Instance().squads)
            {
                for(auto &loop : squadLoop)
                    if (loop->GetEntity() != nullEntity && GetCurrentEntities()->FindComponent<GeneralComponent>(loop->GetEntity())->name == name)
                        return loop->GetEntity();
            }

            return nullEntity;
        }

        Entity PlayerParty::Find(SizeT position)
        {
            if (IsOutOfRange(position))
                return nullEntity;

            return Instance().squads[position / squadCount][position % squadCount]->GetEntity();
        }

        Entity PlayerParty::Find(SizeT squad, SizeT position)
        {
            if (IsOutOfRange(squad, position))
                return nullEntity;

            return Instance().squads[squad][position]->GetEntity();
        }

        bool PlayerParty::Move(SizeT from, SizeT to)
        {
            if (IsOutOfRange(from) || IsOutOfRange(to))
                return false;

            Instance().squads[to / squadCount][to % squadCount] = Instance().squads[from / squadCount][from % squadCount];
            return true;
        }

        bool PlayerParty::Move(SizeT squadFrom, SizeT positionFrom, SizeT squadTo, SizeT positionTo)
        {
            if (IsOutOfRange(squadFrom, positionFrom) || IsOutOfRange(squadTo, positionTo))
                return false;

            Instance().squads[squadTo][positionTo] = Instance().squads[squadFrom][positionFrom];
            return true;
        }

        bool PlayerParty::IsHere(Entity entity)
        {
            for (auto &squadLoop : Instance().squads)
            {
                for(auto &loop : squadLoop)
                    if (loop->GetEntity() == entity)
                        return true;
            }

            return false;
        }

        bool PlayerParty::CanAddMore()
        {
            for (auto &squadLoop : Instance().squads)
            {
                for (auto &loop : squadLoop)
                    if (!loop)
                        return true;
            }

            return false;
        }

        ENTITY_SYSTEM_FORCE_INSTANTIATION(PlayerParty);
    }
}