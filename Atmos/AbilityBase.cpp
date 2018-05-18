
#include "AbilityBase.h"

#include "Battle.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(AbilityBase)
    {
        inscription::BaseSerialize<RegistryObject>(scribe, *this);
    }

    AbilityBase::AbilityBase(AbilityBase &&arg) : RegistryObject(std::move(arg))
    {}

    AbilityBase& AbilityBase::operator=(AbilityBase &&arg)
    {
        RegistryObject::operator=(std::move(arg));
        return *this;
    }

    bool AbilityBase::operator==(const AbilityBase &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool AbilityBase::operator!=(const AbilityBase &arg) const
    {
        return !(*this == arg);
    }

    void AbilityBase::Start() const
    {
        if (!CanStart())
            return;

        AbilityUsing::current = this;
        AbilityUsing::Instance().eventStarted(*AbilityUsing::current);
    }

    bool AbilityBase::CanStart() const
    {
        return battleState.IsTop() && !AbilityUsing::IsActive();
    }

    const AbilityBase* AbilityUsing::current(nullptr);

    AbilityUsing& AbilityUsing::Instance()
    {
        static AbilityUsing instance;
        return instance;
    }

    void AbilityUsing::Finish(Tile &tile)
    {

    }

    const AbilityBase* AbilityUsing::Current()
    {
        return current;
    }

    bool AbilityUsing::IsActive()
    {
        return current != nullptr;
    }
}