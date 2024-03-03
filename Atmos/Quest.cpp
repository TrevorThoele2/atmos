
#include "Quest.h"

namespace Atmos
{
    nQuest::nQuest(const Name& name) : RegistryObject(name)
    {}

    nQuest::nQuest(const ::Inscription::Table<nQuest>& table) : INSCRIPTION_TABLE_GET_BASE(RegistryObject)
    {}

    ObjectTypeDescription nQuest::TypeDescription() const
    {
        return ObjectTraits<nQuest>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nQuest>::typeName = "Quest";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nQuest)
    {

    }
}