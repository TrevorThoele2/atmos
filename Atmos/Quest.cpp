
#include "Quest.h"

namespace Atmos
{
    nQuest::nQuest(ObjectManager& manager, const Name& name) : RegistryObject(manager, name)
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::nQuest)
    {

    }
}