
/*
#include "Consumable.h"

#include "Entity.h"
#include "CombatComponent.h"

#include "CurrentField.h"

#include <Inscription/Table.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(Consumable::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(ConsumableBase::Stack)
    {}

    void Consumable::Stack::ConsumeImpl(Entity entity)
    {
        auto statusEffect = GetItem().statusEffect;
        auto combatComponent = GetCurrentEntities()->FindComponent<entity::CombatComponent>(entity);
        if (combatComponent && statusEffect)
            statusEffect->Affect(*combatComponent, *combatComponent);
    }

    Consumable::Stack::WrappedT& Consumable::Stack::RetrieveItem()
    {
        return CastItem<Consumable>();
    }

    Consumable::Stack::Stack(WrappedT &item, const Count &count) : ConsumableBase::Stack(item, count)
    {}

    Consumable::Stack::Stack(const Stack &arg) : ConsumableBase::Stack(arg)
    {}

    Consumable::Stack& Consumable::Stack::operator=(const Stack &arg)
    {
        ConsumableBase::Stack::operator=(arg);
        return *this;
    }

    Consumable::Stack* Consumable::Stack::Clone() const
    {
        return new Stack(*this);
    }

    const Consumable::Stack::WrappedT& Consumable::Stack::GetItem() const
    {
        return CastItem<Consumable>();
    }

    bool Consumable::Stack::CanConsume(Entity entity) const
    {
        return true;
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Consumable) : INSCRIPTION_TABLE_GET_BASE(ConsumableBase), INSCRIPTION_TABLE_GET_MEM(statusEffect)
    {}

    Consumable::Consumable(const Name &name, const Description &description, Price buyPrice, Price sellPrice, StatusEffect &statusEffect) : ConsumableBase(name, description, buyPrice, sellPrice), statusEffect(&statusEffect)
    {}

    Consumable::Consumable(const Consumable &arg) : ConsumableBase(arg), statusEffect(arg.statusEffect)
    {}

    Consumable& Consumable::operator=(const Consumable &arg)
    {
        Item::operator=(arg);
        statusEffect = arg.statusEffect;
        return *this;
    }

    Consumable::Stack* Consumable::CreateStack(Stack::SizeT count)
    {
        return new Stack(*this, Stack::Count(GetMaxStackSize(), count));
    }

    void Consumable::SetStatusEffect(StatusEffect &set)
    {
        statusEffect = &set;
    }

    StatusEffect& Consumable::GetStatusEffect()
    {
        return *statusEffect;
    }
}

namespace inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Consumable::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ConsumableBase::Stack)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Consumable::Stack, "ConsumableStack")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Consumable)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ConsumableBase)
        INSCRIPTION_TABLE_ADD(statusEffect)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Consumable, "Consumable")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END
}
*/