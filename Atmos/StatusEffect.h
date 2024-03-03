#pragma once

#include <vector>

#include "RegistryObject.h"
#include "Registry.h"

#include "FixedPoint.h"
#include "Script.h"
#include "AssetReference.h"

#include "Name.h"
#include "GlobalContext.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class CombatComponent;
        class BattleComponent;
    }

    class StatusEffectInstance;
    class StatusEffect : public RegistryObject
    {
    public:
        typedef unsigned char ApplicationCount;
        typedef FixedPoint64 Accuracy;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        StatusEffectInstance& Attach(const Ent::CombatComponent &source, const Ent::BattleComponent &sourceBattle, Ent::CombatComponent &target, Ent::BattleComponent &targetBattle) const;
    public:
        AssetReference<ScriptModuleBase> script;
        ApplicationCount applicationCount;
        Name proficiencyName;
        Accuracy accuracy;
        bool positive;

        StatusEffect();
        StatusEffect(const StatusEffect &arg) = default;
        StatusEffect& operator=(const StatusEffect &arg) = default;
        StatusEffect(StatusEffect &&arg);
        StatusEffect& operator=(StatusEffect &&arg);

        bool operator==(const StatusEffect &arg) const;
        bool operator!=(const StatusEffect &arg) const;

        // Returns nullptr if the status effect wasn't attached
        StatusEffectInstance* AttemptAttach(const Ent::CombatComponent &source, const Ent::BattleComponent &sourceBattle, Ent::CombatComponent &target, Ent::BattleComponent &targetBattle) const;
        void Apply(Ent::CombatComponent &applyTo) const;
    };

    template<>
    class Registry<StatusEffect> : public RegistryBase<StatusEffect, Registry<StatusEffect>>
    {
    private:
        Registry() = default;
        friend RegistryBase<StatusEffect, Registry<StatusEffect>>;
    };

    typedef Registry<StatusEffect> StatusEffectRegistry;

    template<>
    class GlobalContext<StatusEffect> : public GlobalContextBase<StatusEffect, GlobalContext<StatusEffect>>
    {
    private:
        typedef GlobalContextBase<StatusEffect, GlobalContext<StatusEffect>> BaseT;
        friend GlobalContextBase<StatusEffect, GlobalContext<StatusEffect>>;
    private:
        static void SerializeImpl(::inscription::Scribe &scribe);
    public:
        static Name accuracyStat;
        static Name resistanceStat;
    };
}