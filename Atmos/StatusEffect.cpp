
#include "StatusEffect.h"

#include "CombatComponent.h"
#include "CapCombatValue.h"

namespace Atmos
{
    nStatusEffect::nStatusEffect(const Name& name) : RegistryObject(name), applicationCount(0), positive(false)
    {}

    nStatusEffect::nStatusEffect(const ::Inscription::Table<nStatusEffect>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    void nStatusEffect::Apply(CombatComponentReference applyTo) const
    {}

    ObjectTypeDescription nStatusEffect::TypeDescription() const
    {
        return ObjectTraits<nStatusEffect>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nStatusEffect>::typeName = "StatusEffect";

    Name GlobalContext<nStatusEffect>::accuracyStat;
    Name GlobalContext<nStatusEffect>::resistanceStat;
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nStatusEffect)
    {

    }
}