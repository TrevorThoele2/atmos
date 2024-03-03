
/*
#include "ProjectileWeapon.h"

#include "Ammo.h"
#include "Entity.h"
#include "CombatComponent.h"
#include "GeneralComponent.h"
#include "InventoryComponent.h"

#include "CurrentField.h"

#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(ProjectileWeapon::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(Weapon::Stack)
    {}

    ProjectileWeapon::Stack::WrappedT& ProjectileWeapon::Stack::RetrieveItem()
    {
        return CastItem<ProjectileWeapon>();
    }

    ProjectileWeapon::Stack::Stack(WrappedT &item, const Count &count) : Weapon::Stack(item, count)
    {}

    ProjectileWeapon::Stack::Stack(const Stack &arg) : Weapon::Stack(arg)
    {}

    ProjectileWeapon::Stack& ProjectileWeapon::Stack::operator=(const Stack &arg)
    {
        Weapon::Stack::operator=(arg);
        return *this;
    }

    ProjectileWeapon::Stack* ProjectileWeapon::Stack::Clone() const
    {
        return new Stack(*this);
    }

    const ProjectileWeapon::Stack::WrappedT& ProjectileWeapon::Stack::GetItem() const
    {
        return CastItem<ProjectileWeapon>();
    }

    bool ProjectileWeapon::Stack::Attack(Entity entity, entity::CombatComponent &target) const
    {
        auto inventoryComponent = GetCurrentEntities()->FindComponent<entity::InventoryComponent>(entity);
        if (!inventoryComponent)
            return false;

        // Check if no ammo equipped
        auto ammo = inventoryComponent->equipment.Find(EquipSlot::AMMO);
        if (!ammo)
            return false;

        // Check if it's actually ammo
        auto newAmmo = dynamic_cast<const Ammo::Stack*>(ammo);
        if (!newAmmo)
            return false;

        // Check if it's the right type
        if (newAmmo->GetAmmoType() != GetItem().ammoType)
            return false;

        // Attack and decrease ammo count by 1

        //auto rangeEntry = GetItem().GetRangePattern().Find(RelTilePosition(entity::FindComponent<entity::PositionComponent>(GetHolder())->position, entity::FindComponent<entity::PositionComponent>(target.entity)->position));
        //auto &damage = rangeEntry->damage.Generate(holder->GetCharacter());

        //DamageProjectile projectile(newAmmo->GetEffect(), newAmmo->GetSpeed());
        //projectile.Attack(Position(holder->GetCharacter().GetPos()), damage, newAmmo->GetDamage().Generate(holder->GetCharacter()), &target, newAmmo->GetEffect());
        //newAmmo->DecCount(1);

        return true;
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(ProjectileWeapon) : INSCRIPTION_TABLE_GET_BASE(Weapon), INSCRIPTION_TABLE_GET_MEM(ammoType)
    {}

    ProjectileWeapon::ProjectileWeapon(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, Range range, HitPattern &&hit, ElementType element, AmmoType ammoType) : Weapon(name, description, buyPrice, sellPrice, classesCanUse, equipRegion, range, std::move(hit), element), ammoType(ammoType)
    {}

    ProjectileWeapon::ProjectileWeapon(const ProjectileWeapon &arg) : Weapon(arg), ammoType(arg.ammoType)
    {}

    ProjectileWeapon& ProjectileWeapon::operator=(const ProjectileWeapon &arg)
    {
        Weapon::operator=(arg);
        ammoType = arg.ammoType;
        return *this;
    }

    bool ProjectileWeapon::operator==(const ProjectileWeapon &arg) const
    {
        return Weapon::operator==(arg) && ammoType == arg.ammoType;
    }

    ProjectileWeapon::Stack* ProjectileWeapon::CreateStack(Stack::SizeT count)
    {
        return new Stack(*this, Stack::Count(GetMaxStackSize(), count));
    }

    void ProjectileWeapon::SetAmmoType(AmmoType set)
    {
        ammoType = set;
    }

    AmmoType ProjectileWeapon::GetAmmoType() const
    {
        return ammoType;
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ProjectileWeapon::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Weapon::Stack)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::ProjectileWeapon::Stack, "ProjectileWeaponStack");
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ProjectileWeapon)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Weapon)
        INSCRIPTION_TABLE_ADD(ammoType)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::ProjectileWeapon, "ProjectileWeapon")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END
}
*/