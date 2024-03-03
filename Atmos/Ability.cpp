
#include "Ability.h"

namespace Atmos
{
    nAbility::nAbility(ObjectManager& manager, const Name& name) : RegistryObject(manager, name)
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nAbility)
    {

    }
}