
#include "Sense.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Sense::Sense(ObjectManager& manager) : AxisAlignedObject(manager), enabled(true)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(Sense) : INSCRIPTION_TABLE_GET_BASE(AxisAlignedObject)
    {}

    Sense::~Sense()
    {}

    ObjectTypeDescription Sense::TypeDescription() const
    {
        return ObjectTraits<Sense>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Sense>::typeName = "Sense";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Sense)
    {

    }
}