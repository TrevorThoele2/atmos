
#include "Music.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    ObjectTypeDescription Music::TypeDescription() const
    {
        return ObjectTraits<Music>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Music>::typeName;
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Music)
    {

    }
}