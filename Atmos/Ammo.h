
/*
#pragma once

#include "Equippable.h"
#include "AmmoType.h"
#include "AVEffect.h"
#include "ObjectOffsetHandle.h"

namespace Atmos
{
    class Ammo : public Equippable
    {
    public:
        typedef float Speed;
        typedef OwningOffsetHandleSolitary<AVEffect, Position3D> AVEffectHandle;
    public:
        class Stack : public Equippable::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef Ammo WrappedT;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            Stack* Clone() const override;
            const WrappedT& GetItem() const override;

            const DamageGenerator& GetDamage() const;
            const AmmoType GetAmmoType() const;
            const AVEffectHandle& GetAVEffect() const;
            Speed GetSpeed() const;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Ammo, private);
    private:
        DamageGenerator damage;
        AmmoType type;
        AVEffectHandle avEffect;
        Speed speed;
    public:
        Ammo(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const DamageGenerator &damage, ElementType element, AmmoType ammoType, AVEffectHandle &&avEffect, Speed speed);
        Ammo(const Ammo &arg);
        Ammo& operator=(const Ammo &arg);
        bool operator==(const Ammo& arg) const;
        Stack* CreateStack(Stack::SizeT count) override;

        void SetDamage(const DamageGenerator &set);
        void SetAmmoType(AmmoType set);
        void SetAVEffect(AVEffectHandle &&set);
        void SetSpeed(Speed set);

        const DamageGenerator& GetDamage() const;
        const AmmoType GetAmmoType() const;
        const AVEffectHandle& GetAVEffect() const;
        Speed GetSpeed() const;
    };
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::Ammo::Stack> : public InscripterBase<::Atmos::Ammo::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::Ammo> : public InscripterBase<::Atmos::Ammo>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/