
#include "nEntity.h"

namespace Atmos
{
    namespace Ent
    {
        nEntity::nEntity(const ::Inscription::Table<nEntity>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        ObjectTypeDescription nEntity::TypeDescription() const
        {
            return ObjectTraits<nEntity>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nEntity>::typeName = "Entity";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nEntity)
    {

    }
}