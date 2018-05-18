#pragma once

#include "AbilityBase.h"

#include "BattlePatternHolder.h"
#include "AttackRange.h"
#include "Acumen.h"

#include "Serialization.h"

namespace Atmos
{
    class Ability : public AbilityBase
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        BattlePatternHolder pattern;
        AttackRange range;
        Acumen cost;

        Ability();
        Ability(const Ability &arg) = default;
        Ability& operator=(const Ability &arg) = default;
        Ability(Ability &&arg);
        Ability& operator=(Ability &&arg);

        bool operator==(const Ability &arg) const;
        bool operator!=(const Ability &arg) const;
    };

    template<>
    class Registry<Ability> : public RegistryBase<Ability, Registry<Ability>>
    {
    private:
        Registry() = default;
        friend RegistryBase<Ability, Registry<Ability>>;
    };

    typedef Registry<Ability> AbilityRegistry;
}