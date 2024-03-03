
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
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Music)
    {

    }
}