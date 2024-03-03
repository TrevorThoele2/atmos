
#include "StatusEffectInstance.h"

#include "StatusEffect.h"

namespace Atmos
{
    StatusEffectInstance::StatusEffectInstance(StatusEffectReference source) : source(source),
        applicationsLeft([this]() { return _applicationsLeft; }), _applicationsLeft(source->applicationCount)
    {}

    bool StatusEffectInstance::Affect(CombatComponentReference combat)
    {
        source->Apply(combat);
        --_applicationsLeft;
        return _applicationsLeft == 0;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(StatusEffectInstance)
    {
        scribe(source);
        scribe(_applicationsLeft);
    }
}