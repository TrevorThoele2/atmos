#include "Entity.h"

namespace Atmos::Entity
{
    void Entity::PostConstruct()
    {
        general = Create<GeneralComponent>();
    }
}