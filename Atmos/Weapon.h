
/*
#pragma once

#include "Equippable.h"

#include "RangePattern.h"
#include "Sprite.h"
#include "AVEffect.h"
#include "ObjectOffsetHandle.h"

#include "Serialization.h"

namespace Atmos
{
    class Weapon : public Equippable
    {
    public:
        typedef unsigned short Range;
        typedef OwningOffsetHandleSolitary<AVEffect, Position3D> AVEffectHandle;
    public:
        class DamageComponent
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(DamageComponent, private);
        public:
            Sprite selectionSprite;
            AVEffectHandle usedEffect;
            DamageGenerator damage;
            StatusEffect *statusEffect;

            DamageComponent(Sprite &&selectionSprite, AVEffectHandle &&usedEffect, const DamageGenerator &damage, StatusEffect *statusEffect);
            bool operator==(const DamageComponent &arg) const;
            bool operator!=(const DamageComponent &arg) const;
        };

        typedef RelativeRangePattern<DamageComponent*> HitPattern;
        typedef std::string ComponentName;
        typedef std::unordered_map<ComponentName, DamageComponent> DamageComponentRegistry;

        class Stack : public Equippable::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef Weapon WrappedT;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            Stack* Clone() const override;

            const WrappedT& GetItem() const override;

            virtual bool Attack(Entity entity, entity::CombatComponent &target) const;
            bool WithinRange(Entity target) const;
            Weapon::Range GetRange() const;
            const Weapon::HitPattern& GetHitPattern() const;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Weapon, private);
    private:
        DamageComponentRegistry componentRegistry;

        Range range;
        HitPattern hit;

        bool InRegistry(const DamageComponent &comp) const;
    public:
        Weapon(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, Range range, HitPattern &&hit, ElementType element);
        Weapon(const Weapon &arg);
        Weapon& operator=(const Weapon &arg);
        bool operator==(const Weapon &arg) const;
        Stack* CreateStack(Stack::SizeT count) override;

        void SetRange(Range set);

        std::pair<bool, DamageComponent&> AddDamageComponent(const ComponentName &name, DamageComponent &&add);
        void CreateDamageComponent(const ComponentName &name, Sprite &&selectionSprite, AVEffectHandle &&usedEffect, const DamageGenerator &damage, StatusEffect *statusEffect);
        DamageComponent* FindDamageComponent(const ComponentName &name);
        const DamageComponent* FindDamageComponent(const ComponentName &name) const;
        bool DestroyDamageComponent(const ComponentName &destroy);
        void AddToHitPattern(const RelativeGridPosition &position, DamageComponent &add);
        void RemoveFromHitPattern(const RelativeGridPosition &position);
        const DamageComponentRegistry& GetDamageComponentRegistry() const;

        Range GetRange() const;
        const HitPattern& GetHitPattern() const;
    };
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::Weapon::DamageComponent> : public InscripterBase<::Atmos::Weapon::DamageComponent>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };

    template<>
    class Inscripter<::Atmos::Weapon::Stack> : public InscripterBase<::Atmos::Weapon::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::Weapon> : public InscripterBase<::Atmos::Weapon>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/