
#include "Ability.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Ability)
    {
        ::Inscription::BaseSerialize<AbilityBase>(scribe, *this);
        scribe(pattern);
        scribe(range);
        scribe(cost);
    }

    Ability::Ability() : range(0), cost(0)
    {}

    Ability::Ability(Ability &&arg) : AbilityBase(std::move(arg)), pattern(std::move(arg.pattern)), range(std::move(arg.range)), cost(std::move(arg.cost))
    {}

    Ability& Ability::operator=(Ability &&arg)
    {
        AbilityBase::operator=(std::move(arg));
        pattern = std::move(arg.pattern);
        range = std::move(arg.range);
        cost = std::move(arg.cost);
        return *this;
    }

    bool Ability::operator==(const Ability &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool Ability::operator!=(const Ability &arg) const
    {
        return !(*this == arg);
    }
}