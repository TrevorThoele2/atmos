#pragma once

#include "StatusEffect.h"

#include "CombatComponent.h"
#include "ObjectReference.h"
#include "ReadonlyProperty.h"

#include "Serialization.h"

namespace Atmos
{
    class StatusEffectInstance
    {
    public:
        typedef TypedObjectReference<nStatusEffect> StatusEffectReference;
        typedef TypedObjectReference<Entity::CombatComponent> CombatComponentReference;
    public:
        typedef nStatusEffect::ApplicationCount ApplicationCount;
        ReadonlyProperty<ApplicationCount> applicationsLeft;
    public:
        StatusEffectInstance(StatusEffectReference source);
        StatusEffectInstance(const StatusEffectInstance& arg) = default;
        StatusEffectInstance& operator=(const StatusEffectInstance& arg) = default;

        // Returns true if this instance is done
        bool Affect(CombatComponentReference combat);
    private:
        StatusEffectReference source;
    private:
        ApplicationCount _applicationsLeft;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}