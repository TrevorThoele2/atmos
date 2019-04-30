
#include "StatusEffect.h"

#include "CombatComponent.h"

namespace Atmos
{
    nStatusEffect::nStatusEffect(ObjectManager& manager, const Name& name) :
        RegistryObject(manager, name), applicationCount(0), positive(false)
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nStatusEffect)
    {

    }
}