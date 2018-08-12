
/*
#include "ConsumableBase.h"
#include "Assert.h"

#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(ConsumableBase::Stack, Stack) : INSCRIPTION_TABLE_GET_BASE(Item::Stack)
    {}

    ConsumableBase::Stack::WrappedT& ConsumableBase::Stack::RetrieveItem()
    {
        return CastItem<ConsumableBase>();
    }

    ConsumableBase::Stack::Stack(WrappedT &item, const Count &count) : Item::Stack(item, count)
    {}

    ConsumableBase::Stack::Stack(const Stack &arg) : Item::Stack(arg)
    {}

    ConsumableBase::Stack& ConsumableBase::Stack::operator=(const Stack &arg)
    {
        Item::Stack::operator=(arg);
        return *this;
    }

    const ConsumableBase::Stack::WrappedT& ConsumableBase::Stack::GetItem() const
    {
        return CastItem<ConsumableBase>();
    }

    void ConsumableBase::Stack::Consume(Entity entity)
    {
        ATMOS_ASSERT_MESSAGE(GetCount() > 0, "Count is checked before this, so must be 0.");
        if (entity == nullEntity || !CanConsume(entity))
            return;

        ConsumeImpl(entity);
        DecCount(1);
    }

    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(ConsumableBase) : INSCRIPTION_TABLE_GET_BASE(Item)
    {}

    ConsumableBase::ConsumableBase(const std::string &name, const std::string &description, Price buyPrice, Price sellPrice) : Item(name, description, buyPrice, sellPrice)
    {}

    ConsumableBase::ConsumableBase(const ConsumableBase &arg) : Item(arg)
    {}

    ConsumableBase& ConsumableBase::operator=(const ConsumableBase &arg)
    {
        Item::operator=(arg);
        return *this;
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ConsumableBase::Stack)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Item::Stack)
    INSCRIPTION_TABLE_END

    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ConsumableBase)
        INSCRIPTION_TABLE_ADD_BASE(::Atmos::Item)
    INSCRIPTION_TABLE_END
}
*/