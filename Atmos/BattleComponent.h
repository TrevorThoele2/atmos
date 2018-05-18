#pragma once

#include "EntityComponent.h"

#include "GridPosition.h"
#include "MovementRange.h"
#include "ItemStash.h"
#include "Name.h"
#include "StatusEffectInstance.h"
#include "Acumen.h"

#include "Serialization.h"

namespace Atmos
{
    class CharacterClass;
    namespace Ent
    {
        class BattleComponent : public Component<BattleComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            GridPosition startingPos;
            MovementRange movementRangeLeft;
            bool turnEnded;

            ItemStash temporaryStash;

            // Status effects
            std::unordered_map<Name, StatusEffectInstance> statusEffects;

            Acumen acumen;

            bool isAlly;

            BattleComponent();
            BattleComponent(const BattleComponent &arg) = default;
            BattleComponent(BattleComponent &&arg);
            BattleComponent& operator=(const BattleComponent &arg) = default;
            BattleComponent& operator=(BattleComponent &&arg);

            void StartTurn();
            void EndTurn();
            void Attack(Entity target);

            // Returns true if the item was given
            bool GiveTemporaryItem(const Item &item, const RegistryObjectReference<CharacterClass> &charClass);
            // Returns true if the item was removed
            bool RemoveTemporaryItem(ItemStashSize pos, const RegistryObjectReference<CharacterClass> &charClass);
            bool HasSpaceInTemporaryStash(const RegistryObjectReference<CharacterClass> &charClass) const;

            bool IsAlly() const;
            bool IsEnemy() const;
        };

        ENTITY_COMPONENT_MAP_DECLARE("Battle", BattleComponent)
    }
}