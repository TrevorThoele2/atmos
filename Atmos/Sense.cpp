
#include "Sense.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Sense::Sense(ObjectManager& manager) : PositionalObject(manager), enabled(true)
    {}

    Sense::Sense(const ::Inscription::Table<Sense>& table) : INSCRIPTION_TABLE_GET_BASE(PositionalObject)
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