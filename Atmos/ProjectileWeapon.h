
/*
#pragma once

#include "Weapon.h"
#include "AmmoType.h"
#include "Serialization.h"

namespace Atmos
{
    class ProjectileWeapon : public Weapon
    {
    public:
        class Stack : public Weapon::Stack
        {
        private:
            INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(Stack, private);
        private:
            typedef ProjectileWeapon WrappedT;
        protected:
            WrappedT& RetrieveItem() override;
        public:
            Stack(WrappedT &item, const Count &count);
            Stack(const Stack &arg);
            Stack& operator=(const Stack &arg);

            Stack* Clone() const override;
            const WrappedT& GetItem() const override;

            bool Attack(Entity entity, entity::CombatComponent &target) const override;
        };
    private:
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(ProjectileWeapon, private);
    private:
        AmmoType ammoType;
    public:
        ProjectileWeapon(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, Range range, HitPattern &&hit, ElementType element, AmmoType ammoType);
        ProjectileWeapon(const ProjectileWeapon &arg);
        ProjectileWeapon& operator=(const ProjectileWeapon &arg);
        bool operator==(const ProjectileWeapon &arg) const;
        Stack* CreateStack(Stack::SizeT count) override;

        void SetAmmoType(AmmoType set);
        AmmoType GetAmmoType() const;
    };
}

namespace inscription
{
    template<>
    class Inscripter<::Atmos::ProjectileWeapon::Stack> : public InscripterBase<::Atmos::ProjectileWeapon::Stack>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };

    template<>
    class Inscripter<::Atmos::ProjectileWeapon> : public InscripterBase<::Atmos::ProjectileWeapon>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_INSCRIPTER_DECLARE_CLASS_NAME_CONTAINER;
    };
}
*/