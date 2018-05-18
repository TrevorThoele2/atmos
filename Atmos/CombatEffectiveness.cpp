
#include "CombatEffectiveness.h"

#include <Inscription/UnorderedMap.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(CombatEffectiveness)
    {
        scribe(map);
    }

    CombatEffectiveness::CombatEffectiveness(CombatEffectiveness &&arg) : map(std::move(arg.map))
    {}

    CombatEffectiveness& CombatEffectiveness::operator=(CombatEffectiveness &&arg)
    {
        map = std::move(arg.map);
        return *this;
    }

    bool CombatEffectiveness::operator==(const CombatEffectiveness &arg) const
    {
        return map == arg.map;
    }

    bool CombatEffectiveness::operator!=(const CombatEffectiveness &arg) const
    {
        return !(*this == arg);
    }
}