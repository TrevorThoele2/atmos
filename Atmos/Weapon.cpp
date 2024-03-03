
/*
#include "Weapon.h"

#include "Entity.h"
#include "CombatComponent.h"
#include "GeneralComponent.h"
#include "CurrentField.h"

#include <Inscription\Inscripter.h>
#include <Inscription\UnorderedMap.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(Weapon::DamageComponent, DamageComponent) : INSCRIPTION_TABLE_GET_MEM(selectionSprite), INSCRIPTION_TABLE_GET_MEM(usedEffect),
        INSCRIPTION_TABLE_GET_MEM(damage), INSCRIPTION_TABLE_GET_MEM(statusEffect)
    {}

    Weapon::DamageComponent::DamageComponent(Sprite &&selectionSprite, AVEffectHandle &&usedEffect, const DamageGenerator &damage, StatusEffect *statusEffect) : selectionSprite(std::move(selectionSprite)), damage(damage), statusEffect(statusEffect)
    {}

    bool Weapon::DamageComponent::operator==(const DamageComponent &arg) const
    {
        return selectionSprite == arg.selectionSprite && usedEffect == arg.usedEffect && damage == arg.damage && statusEffect == arg.statusEffect;
    }

    bool Weapon::DamageComponent::operator!=(const DamageComponent &arg) const
    {
        return !(*this == arg);
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(Weapon::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(Equippable::Stack)
    {}

    Weapon::Stack::WrappedT& Weapon::Stack::RetrieveItem()
    {
        return CastItem<Weapon>();
    }

    Weapon::Stack::Stack(WrappedT &item, const Count &count) : Equippable::Stack(item, count)
    {}

    Weapon::Stack::Stack(const Stack &arg) : Equippable::Stack(arg)
    {}

    Weapon::Stack& Weapon::Stack::operator=(const Stack &arg)
    {
        Equippable::Stack::operator=(arg);
        return *this;
    }

    Weapon::Stack* Weapon::Stack::Clone() const
    {
        return new Stack(*this);
    }

    const Weapon::Stack::WrappedT& Weapon::Stack::GetItem() const
    {
        return CastItem<Weapon>();
    }

    bool Weapon::Stack::Attack(Entity entity, entity::CombatComponent &target) const
    {
        auto rangeEntry = GetItem().GetHitPattern().Find(RelativeGridPosition(GetCurrentEntities()->GetPosition(entity).Get(), GetCurrentEntities()->GetPosition(target.GetOwnerEntity()).Get()));
        (*rangeEntry)->damage.Generate(*GetCurrentEntities()->FindComponent<entity::CombatComponent>(entity)).Affect(target);
        return true;
    }

    bool Weapon::Stack::WithinRange(Entity target) const
    {
        return true;
        //return GetItem().range.IsWithin(RelTilePosition(entity::FindComponent<entity::PositionComponent>(GetHolder())->position, entity::FindComponent<entity::PositionComponent>(target)->position));
    }

    Weapon::Range Weapon::Stack::GetRange() const
    {
        return GetItem().GetRange();
    }

    const Weapon::HitPattern& Weapon::Stack::GetHitPattern() const
    {
        return GetItem().GetHitPattern();
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Weapon) : INSCRIPTION_TABLE_GET_BASE(Equippable), INSCRIPTION_TABLE_GET_MEM(range), INSCRIPTION_TABLE_GET_MEM(hit),
        INSCRIPTION_TABLE_GET_MEM(componentRegistry)
    {}

    bool Weapon::InRegistry(const DamageComponent &comp) const
    {
        for (auto &loop : componentRegistry)
        {
            if (&loop.second == &comp)
                return true;
        }

        return false;
    }

    Weapon::Weapon(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const CharacterClassGroup &classesCanUse, const EquipRegion &equipRegion, Range range, HitPattern &&hit, ElementType element) : Equippable(name, description, buyPrice, sellPrice, classesCanUse, equipRegion, element), range(std::move(range)), hit(std::move(hit))
    {}

    Weapon::Weapon(const Weapon &arg) : Equippable(arg), range(arg.range), hit(arg.hit)
    {}

    Weapon& Weapon::operator=(const Weapon &arg)
    {
        Equippable::operator=(arg);
        range = arg.range;
        hit = arg.hit;
        return *this;
    }

    bool Weapon::operator==(const Weapon &arg) const
    {
        return Equippable::operator==(arg) && range == arg.range && hit == arg.hit;
    }

    Weapon::Stack* Weapon::CreateStack(Stack::SizeT count)
    {
        return new Stack(*this, Stack::Count(GetMaxStackSize(), count));
    }

    void Weapon::SetRange(Range set)
    {
        range = set;
    }

    std::pair<bool, Weapon::DamageComponent&> Weapon::AddDamageComponent(const ComponentName &name, DamageComponent &&add)
    {
        auto &emplaced = componentRegistry.emplace(name, std::move(add));
        return std::pair<bool, Weapon::DamageComponent&>(emplaced.second, emplaced.first->second);
    }

    void Weapon::CreateDamageComponent(const ComponentName &name, Sprite &&selectionSprite, AVEffectHandle &&usedEffect, const DamageGenerator &damage, StatusEffect *statusEffect)
    {
        AddDamageComponent(name, DamageComponent(std::move(selectionSprite), std::move(usedEffect), damage, statusEffect));
    }

    Weapon::DamageComponent* Weapon::FindDamageComponent(const ComponentName &name)
    {
        auto found = componentRegistry.find(name);
        if (found == componentRegistry.end())
            return nullptr;

        return &found->second;
    }

    const Weapon::DamageComponent* Weapon::FindDamageComponent(const ComponentName &name) const
    {
        auto found = componentRegistry.find(name);
        if (found == componentRegistry.end())
            return nullptr;

        return &found->second;
    }

    bool Weapon::DestroyDamageComponent(const ComponentName &destroy)
    {
        return componentRegistry.erase(destroy) != 0;
    }

    void Weapon::AddToHitPattern(const RelativeGridPosition &position, DamageComponent &add)
    {
        if(InRegistry(add))
            hit.Add(position, &add);
    }

    void Weapon::RemoveFromHitPattern(const RelativeGridPosition &position)
    {
        hit.Remove(position);
    }

    const Weapon::DamageComponentRegistry& Weapon::GetDamageComponentRegistry() const
    {
        return componentRegistry;
    }

    Weapon::Range Weapon::GetRange() const
    {
        return range;
    }

    const Weapon::HitPattern& Weapon::GetHitPattern() const
    {
        return hit;
    }
}

namespace inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Weapon::DamageComponent)
        INSCRIPTION_TABLE_ADD(selectionSprite)
        INSCRIPTION_TABLE_ADD(usedEffect)
        INSCRIPTION_TABLE_ADD(damage)
        INSCRIPTION_TABLE_ADD(statusEffect)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Weapon::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Equippable::Stack)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Weapon::Stack, "WeaponStack")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Weapon)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Equippable)
        INSCRIPTION_TABLE_ADD(range)
        INSCRIPTION_TABLE_ADD(componentRegistry)
        INSCRIPTION_TABLE_ADD(hit)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Weapon, "Weapon")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END
}
*/