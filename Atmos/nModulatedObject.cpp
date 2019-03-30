
#include "nModulatedObject.h"

namespace Atmos
{
    namespace Modulator
    {
        ModulatedObject::ModulatedObject(SourceReference source) : source(source)
        {}

        ModulatedObject::ModulatedObject(const ::Inscription::Table<ModulatedObject>& table) : INSCRIPTION_TABLE_GET_BASE(Object)
        {}

        ObjectTypeDescription ModulatedObject::TypeDescription() const
        {
            return ObjectTraits<ModulatedObject>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Modulator::ModulatedObject>::typeName = "ModulatedObject";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Modulator::ModulatedObject)
    {

    }
}