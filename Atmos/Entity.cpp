#include "Entity.h"

namespace Atmos::Entity
{
    void Entity::PostConstruct(ShardTuple shards)
    {
        general = std::get<0>(shards);
    }
}