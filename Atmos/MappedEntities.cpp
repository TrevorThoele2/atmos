#include "MappedEntities.h"

namespace Atmos::Entity
{
    Entity* MappedEntities::EntityWithName(const String& name)
    {
        const auto found = map.find(name);
        if (found == map.end())
            return nullptr;

        return found->second;
    }

    size_t MappedEntities::Size() const
    {
        return map.size();
    }
}