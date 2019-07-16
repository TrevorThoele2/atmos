
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