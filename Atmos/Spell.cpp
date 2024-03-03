
#include "Spell.h"

#include "Battle.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Spell)
    {
        ::Inscription::BaseSerialize<RegistryObject>(scribe, *this);
        scribe(attackPattern);
        //scribe(pieceOverworld);
        scribe(range);
        scribe(resourceCost);
        scribe(allowedClasses);
    }

    Spell::Spell(Spell &&arg) : AbilityBase(std::move(arg)), attackPattern(std::move(arg.attackPattern)), pieceOverworld(std::move(arg.pieceOverworld)), range(std::move(arg.range)), resourceCost(std::move(arg.resourceCost)), allowedClasses(std::move(arg.allowedClasses))
    {}

    Spell& Spell::operator=(Spell &&arg)
    {
        AbilityBase::operator=(std::move(arg));
        attackPattern = std::move(arg.attackPattern);
        pieceOverworld = std::move(arg.pieceOverworld);
        range = std::move(arg.range);
        resourceCost = std::move(arg.resourceCost);
        allowedClasses = std::move(arg.allowedClasses);
        return *this;
    }

    bool Spell::operator==(const Spell &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool Spell::operator!=(const Spell &arg) const
    {
        return !(*this == arg);
    }
}