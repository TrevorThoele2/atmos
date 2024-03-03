
#include "Ability.h"

namespace Atmos
{
    nAbility::nAbility(const Name& name) : RegistryObject(name)
    {}

    nAbility::nAbility(const ::Inscription::Table<nAbility>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    ObjectTypeDescription nAbility::TypeDescription() const
    {
        return ObjectTraits<nAbility>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nAbility>::typeName = "Ability";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nAbility)
    {

    }
}