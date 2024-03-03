
#include "Spell.h"

namespace Atmos
{
    nSpell::nSpell(const Name& name) : RegistryObject(name)
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
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nSpell)
    {

    }
}