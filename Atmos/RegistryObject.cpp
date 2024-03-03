
#include "RegistryObject.h"

namespace Atmos
{
    RegistryObject::RegistryObject(ObjectManager& manager, const Name& name) :
        Object(manager), name(name)
    {}

    RegistryObject::RegistryObject(const ::Inscription::Table<RegistryObject>& table) :
        INSCRIPTION_TABLE_GET_BASE(Object), INSCRIPTION_TABLE_GET_MEM(name)
    {}

    ObjectTypeDescription RegistryObject::TypeDescription() const
    {
        return ObjectTraits<RegistryObject>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<RegistryObject>::typeName = "RegistryObject";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::RegistryObject)
    {
        INSCRIPTION_TABLE_ADD(name);
    }
}