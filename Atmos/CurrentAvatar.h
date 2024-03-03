#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "AvatarComponent.h"
#include "Entity.h"

namespace Atmos::Entity
{
    class CurrentAvatar final : public Arca::ClosedTypedRelic<CurrentAvatar>
    {
    public:
        Arca::Index<AvatarComponent> component;
        Arca::Index<Entity> entity;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::CurrentAvatar>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Entity::CurrentAvatar";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::CurrentAvatar, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::CurrentAvatar, BinaryArchive>
    {};
}