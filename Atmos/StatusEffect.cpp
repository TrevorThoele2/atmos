
#include "StatusEffect.h"

#include "CombatComponent.h"
#include "BattleComponent.h"
#include "CapCombatValue.h"

#include "ScriptRegistry.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StatusEffect)
    {
        ::Inscription::BaseSerialize<RegistryObject>(scribe, *this);
        scribe(script);
        scribe(applicationCount);
        scribe(proficiencyName);
        scribe(accuracy);
        scribe(positive);
    }

    StatusEffectInstance& StatusEffect::Attach(const Ent::CombatComponent &source, const Ent::BattleComponent &sourceBattle, Ent::CombatComponent &target, Ent::BattleComponent &targetBattle) const
    {
        auto &instance = targetBattle.statusEffects.emplace(GetName(), StatusEffectInstance(RegistryObjectReference<StatusEffect>(*this))).first->second;
        instance.Affect(target);
        return instance;
    }

    StatusEffect::StatusEffect() : applicationCount(0), positive(false)
    {}

    StatusEffect::StatusEffect(StatusEffect &&arg) : RegistryObject(std::move(arg)), script(std::move(arg.script)), applicationCount(std::move(arg.applicationCount)), proficiencyName(std::move(arg.proficiencyName)), accuracy(std::move(arg.accuracy)), positive(std::move(arg.positive))
    {}

    StatusEffect& StatusEffect::operator=(StatusEffect &&arg)
    {
        RegistryObject::operator=(std::move(arg));
        script = std::move(arg.script);
        applicationCount = std::move(arg.applicationCount);
        proficiencyName = std::move(arg.proficiencyName);
        accuracy = std::move(arg.accuracy);
        positive = std::move(arg.positive);
        return *this;
    }

    bool StatusEffect::operator==(const StatusEffect &arg) const
    {
        return RegistryObject::operator==(arg);
    }

    bool StatusEffect::operator!=(const StatusEffect &arg) const
    {
        return !(*this == arg);
    }

    StatusEffectInstance* StatusEffect::AttemptAttach(const Ent::CombatComponent &source, const Ent ::BattleComponent &sourceBattle, Ent::CombatComponent &target, Ent::BattleComponent &targetBattle) const
    {
        if (positive && sourceBattle.IsAlly() == targetBattle.IsAlly())
        {
            // Always allow status effects that target the same team to go through if this status effect is positive
            return &Attach(source, sourceBattle, target, targetBattle);
        }

        // Calculate if to affect or not
        Accuracy adjustedAccuracy(CalculateValueCap(accuracy, { Accuracy::Split(*source.stats.GetValue(GlobalContext<StatusEffect>::accuracyStat), 0), source.proficiencies.GetLevel(proficiencyName) }));
        Accuracy resistance(*target.stats.GetValue(GlobalContext<StatusEffect>::resistanceStat), 0);
        if (adjustedAccuracy > resistance)
            return &Attach(source, sourceBattle, target, targetBattle);

        return nullptr;
    }

    void StatusEffect::Apply(Ent::CombatComponent &applyTo) const
    {

    }

    Name GlobalContext<StatusEffect>::accuracyStat;
    Name GlobalContext<StatusEffect>::resistanceStat;
}