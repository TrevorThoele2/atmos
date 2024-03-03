#pragma once

#include <memory>
#include "ItemAspect.h"

#include "EquipRegion.h"
#include "Tag.h"
#include "Element.h"
#include "Defense.h"
#include "BattlePatternHolder.h"
#include "AttackRange.h"
#include "CharacterClassGroup.h"

#include "Optional.h"
#include "Serialization.h"

namespace Atmos
{
    namespace Ent { class CombatComponent; }

    class EquippableAspect : public ItemAspect
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        enum class AdditionalType
        {
            AMMO,
            ATTACK,
            SHARD
        };

        class Additional
        {
        public:
            Additional() = default;
            virtual Additional* Clone() const = 0;
            virtual AdditionalType GetType() const = 0;
        };

        class AmmoAdditional : public Additional
        {
        public:
            Tag ammoType;

            AmmoAdditional() = default;
            AmmoAdditional(const AmmoAdditional &arg) = default;
            AmmoAdditional& operator=(const AmmoAdditional &arg) = default;

            bool operator==(const AmmoAdditional &arg) const;
            bool operator!=(const AmmoAdditional &arg) const;

            AmmoAdditional* Clone() const override final;
            AdditionalType GetType() const override final;
        };

        class AttackAdditional : public Additional
        {
        public:
            class UsesAmmoAdditional
            {
            public:
                UsesAmmoAdditional() = default;
                bool operator==(const UsesAmmoAdditional &arg) const;
                bool operator!=(const UsesAmmoAdditional &arg) const;
            };
        public:
            typedef FixedPoint64 Power;
        public:
            Power power;
            BattlePatternHolder pattern;
            AttackRange range;
            CharacterClassGroup allowedClasses;
            Name proficiencyName;
            Optional<UsesAmmoAdditional> usesAmmo;

            AttackAdditional() = default;
            AttackAdditional(const AttackAdditional &arg) = default;
            AttackAdditional& operator=(const AttackAdditional &arg) = default;
            AttackAdditional(AttackAdditional &&arg);
            AttackAdditional& operator=(AttackAdditional &&arg);

            bool operator==(const AttackAdditional &arg) const;
            bool operator!=(const AttackAdditional &arg) const;

            AttackAdditional* Clone() const override final;
            AdditionalType GetType() const override final;

            void SignalUsesAmmo();
            bool IsUsesAmmo() const;

            void ActivatePattern(Ent::CombatComponent &source, const GridPosition &pos);
        };

        class ShardAdditional : public Additional
        {
        public:
            Element element;

            ShardAdditional() = default;
            ShardAdditional(const ShardAdditional &arg) = default;
            ShardAdditional& operator=(const ShardAdditional &arg) = default;

            bool operator==(const ShardAdditional &arg) const;
            bool operator!=(const ShardAdditional &arg) const;

            ShardAdditional* Clone() const override final;
            AdditionalType GetType() const override final;
        };
    public:
        EquipRegion equipRegion;
        Defense defense;
        std::unique_ptr<Additional> additional;

        EquippableAspect() = default;
        EquippableAspect(const EquippableAspect &arg);
        EquippableAspect& operator=(const EquippableAspect &arg);
        EquippableAspect(EquippableAspect &&arg);
        EquippableAspect& operator=(EquippableAspect &&arg);

        bool operator==(const EquippableAspect &arg) const;
        bool operator!=(const EquippableAspect &arg) const;

        void SignalAmmo();
        void SignalAttack();
        void SignalShard();

        AmmoAdditional* AsAmmo();
        const AmmoAdditional* AsAmmo() const;
        AttackAdditional* AsAttack();
        const AttackAdditional* AsAttack() const;
        ShardAdditional* AsShard();
        const ShardAdditional* AsShard() const;

        bool IsAmmo() const;
        bool IsAttack() const;
        bool IsShard() const;
        bool HasAdditional() const;
    };
}