#pragma once

#include "StatusEffect.h"
#include "RegistryObjectReference.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent { class CombatComponent; }

    class StatusEffectInstance
    {
    public:
        typedef StatusEffect::ApplicationCount ApplicationCount;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        RegistryObjectReference<StatusEffect> wrapped;
        ApplicationCount applications;
    public:
        StatusEffectInstance();
        StatusEffectInstance(const RegistryObjectReference<StatusEffect> &wrap);
        StatusEffectInstance(const StatusEffectInstance &arg) = default;
        StatusEffectInstance& operator=(const StatusEffectInstance &arg) = default;

        // Returns true if this instance is done
        bool Affect(Ent::CombatComponent &combat);
        ApplicationCount GetApplicationCount() const;
    };
}