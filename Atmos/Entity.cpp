#include "Entity.h"

namespace Atmos::Entity
{
    void Entity::PostConstruct()
    {
        general = Find<GeneralComponent>();
    }

    void Entity::Initialize()
    {

        general = Create<GeneralComponent>();
    }
}