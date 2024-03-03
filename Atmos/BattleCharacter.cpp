
#include "BattleCharacter.h"

namespace Atmos
{
    namespace Battle
    {
        CharacterEntity::CharacterEntity(Entity entity) : ComponentPackageRequiredInherit(entity)
        {}

        Ent::GeneralComponent* CharacterEntity::GetGeneral()
        {
            return package.GetAt<General>();
        }

        Ent::CharacterComponent* CharacterEntity::GetCharacter()
        {
            return package.GetAt<Character>();
        }

        Ent::SenseComponent* CharacterEntity::GetSense()
        {
            return package.GetAt<Sense>();
        }

        Ent::CombatComponent* CharacterEntity::GetCombat()
        {
            return package.GetAt<Combat>();
        }

        Ent::BattleComponent* CharacterEntity::GetBattle()
        {
            return package.GetAt<Battle>();
        }
    }
}