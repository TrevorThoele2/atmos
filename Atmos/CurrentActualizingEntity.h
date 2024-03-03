#pragma once

#include <Arca/Relic.h>
#include "Entity.h"

namespace Atmos::Entity
{
    struct CurrentActualizing final
    {
        Arca::Index<Entity> entity;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::CurrentActualizing>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Entity::CurrentActualizing"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Entity::CurrentActualizing, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::CurrentActualizing>;
    };
}