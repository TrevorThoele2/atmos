#pragma once

#include "ArcaRelicIncludes.h"
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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Entity::CurrentActualizing";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<Atmos::Entity::CurrentActualizing, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::CurrentActualizing>;
    };
}