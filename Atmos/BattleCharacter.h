#pragma once

#include "EntityComponentPackage.h"

#include "GeneralComponent.h"
#include "CharacterComponent.h"
#include "SenseComponent.h"
#include "CombatComponent.h"
#include "BattleComponent.h"

namespace Atmos
{
    namespace Battle
    {
        class CharacterEntity : public Ent::ComponentPackageRequiredInherit<Ent::GeneralComponent*, Ent::CharacterComponent*, Ent::SenseComponent*, Ent::CombatComponent*, Ent::BattleComponent*>
        {
        public:
            enum ID
            {
                General,
                Character,
                Sense,
                Combat,
                Battle
            };

            CharacterEntity(Entity entity = ::Atmos::Ent::nullEntity);
            ::Atmos::Ent::GeneralComponent* GetGeneral();
            ::Atmos::Ent::CharacterComponent* GetCharacter();
            ::Atmos::Ent::SenseComponent* GetSense();
            ::Atmos::Ent::CombatComponent* GetCombat();
            ::Atmos::Ent::BattleComponent* GetBattle();
        };
    }
}