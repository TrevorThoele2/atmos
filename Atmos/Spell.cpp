
#include "Spell.h"

namespace Atmos
{
    nSpell::nSpell(ObjectManager& manager, const Name& name) : RegistryObject(manager, name)
    {}

    nSpell::nSpell(const ::Inscription::Table<nSpell>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    ObjectTypeDescription nSpell::TypeDescription() const
    {
        return ObjectTraits<nSpell>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nSpell>::typeName = "Spell";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nSpell)
    {

    }
}