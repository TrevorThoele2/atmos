
#include "Music.h"

#include <Inscription/Scribe.h>

namespace Atmos::Audio
{
    Music::Music(ObjectManager& manager) : Object(manager)
    {}

    Music::Music(const ::Inscription::BinaryTableData<Music>& data) : Object(std::get<0>(data.bases))
    {}

    ObjectTypeDescription Music::TypeDescription() const
    {
        return ObjectTraits<Music>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Audio::Music>::typeName;
}