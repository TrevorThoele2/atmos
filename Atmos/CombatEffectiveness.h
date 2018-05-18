#pragma once

#include "Percentage.h"
#include "Name.h"

#include "Serialization.h"

namespace Atmos
{
    class CombatEffectiveness
    {
    public:
        typedef Percentage Value;
        typedef std::unordered_map<Name, Value> Map;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        Map map;

        CombatEffectiveness() = default;
        CombatEffectiveness(const CombatEffectiveness &arg) = default;
        CombatEffectiveness& operator=(const CombatEffectiveness &arg) = default;
        CombatEffectiveness(CombatEffectiveness &&arg);
        CombatEffectiveness& operator=(CombatEffectiveness &&arg);

        bool operator==(const CombatEffectiveness &arg) const;
        bool operator!=(const CombatEffectiveness &arg) const;
    };
}