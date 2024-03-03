#pragma once

#include "AbilityBase.h"
#include "Registry.h"

#include "BattlePatternHolder.h"
#include "AttackRange.h"
#include "ResourceAttribute.h"
#include "CharacterClassGroup.h"

namespace Atmos
{
    class Spell : public AbilityBase
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        BattlePatternHolder attackPattern;
        BattlePatternHolder::Piece *pieceOverworld;
        AttackRange range;
        Attribute resourceCost;
        CharacterClassGroup allowedClasses;

        Spell() = default;
        Spell(const Spell &arg) = default;
        Spell& operator=(const Spell &arg) = default;
        Spell(Spell &&arg);
        Spell& operator=(Spell &&arg);

        bool operator==(const Spell &arg) const;
        bool operator!=(const Spell &arg) const;
    };

    template<>
    class Registry<Spell> : public RegistryBase<Spell, Registry<Spell>>
    {
    private:
        Registry() = default;
        friend RegistryBase<Spell, Registry<Spell>>;
    };

    typedef Registry<Spell> SpellRegistry;
}