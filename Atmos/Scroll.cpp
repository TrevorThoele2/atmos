
/*
#include "Scroll.h"

#include "Spell.h"

#include "Entity.h"
#include "CombatComponent.h"

#include "CurrentField.h"

#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(Scroll::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(ConsumableBase::Stack)
    {}

    void Scroll::Stack::ConsumeImpl(Entity entity)
    {
        GetCurrentEntities()->FindComponent<entity::CombatComponent>(entity)->LearnSpell(*GetItem().spell);
    }

    Scroll::Stack::WrappedT& Scroll::Stack::RetrieveItem()
    {
        return CastItem<Scroll>();
    }

    Scroll::Stack::Stack(WrappedT &item, const Count &count) : ConsumableBase::Stack(item, count)
    {}

    Scroll::Stack::Stack(const Stack &arg) : ConsumableBase::Stack(arg)
    {}

    Scroll::Stack& Scroll::Stack::operator=(const Stack &arg)
    {
        ConsumableBase::Stack::operator=(arg);
        return *this;
    }

    Scroll::Stack* Scroll::Stack::Clone() const
    {
        return new Stack(*this);
    }

    const Scroll::Stack::WrappedT& Scroll::Stack::GetItem() const
    {
        return CastItem<Scroll>();
    }

    bool Scroll::Stack::CanConsume(Entity entity) const
    {
        if (GetItem().spell->CanLearn(*GetCurrentEntities()->FindComponent<entity::CombatComponent>(entity)))
            return false;

        return true;
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(Scroll) : INSCRIPTION_TABLE_GET_BASE(ConsumableBase), INSCRIPTION_TABLE_GET_MEM(spell)
    {}

    Scroll::Scroll(const Name &name, const Description &description, Price buyPrice, Price sellPrice, const Spell &spell) : ConsumableBase(name, description, buyPrice, sellPrice), spell(&spell)
    {}

    Scroll::Scroll(const Scroll &arg) : ConsumableBase(arg), spell(arg.spell)
    {}

    Scroll& Scroll::operator=(const Scroll &arg)
    {
        ConsumableBase::operator=(arg);
        spell = arg.spell;
        return *this;
    }

    Scroll::Stack* Scroll::CreateStack(Stack::SizeT count)
    {
        return new Stack(*this, Stack::Count(GetMaxStackSize(), count));
    }

    void Scroll::SetSpell(const Spell &set)
    {
        spell = &set;
    }

    const Spell& Scroll::GetSpell() const
    {
        return *spell;
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Scroll::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ConsumableBase::Stack)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Scroll::Stack, "ScrollStack")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Scroll)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ConsumableBase)
        INSCRIPTION_TABLE_ADD_ASSUME_EXISTS(spell)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER(::Atmos::Scroll, "Scroll")
    INSCRIPTION_INSCRIPTER_DEFINE_CLASS_NAME_CONTAINER_END
}
*/