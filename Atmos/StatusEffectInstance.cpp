
#include "StatusEffectInstance.h"
#include "StatusEffect.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StatusEffectInstance)
    {
        scribe(wrapped);
        scribe(applications);
    }

    StatusEffectInstance::StatusEffectInstance() : applications(0)
    {}

    StatusEffectInstance::StatusEffectInstance(const RegistryObjectReference<StatusEffect> &wrap) : wrapped(wrap), applications(wrap->applicationCount)
    {}

    bool StatusEffectInstance::Affect(Ent::CombatComponent &combat)
    {
        wrapped->Apply(combat);
        --applications;
        return applications == 0;
    }

    StatusEffectInstance::ApplicationCount StatusEffectInstance::GetApplicationCount() const
    {
        return applications;
    }
}