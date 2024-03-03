#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>

#include "AvatarComponent.h"
#include "Entity.h"

namespace Atmos::Entity
{
    class CurrentAvatar final : public Arca::ClosedTypedRelicAutomation<CurrentAvatar>
    {
    public:
        AvatarComponent* component = nullptr;
        Entity* entity = nullptr;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::CurrentAvatar>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "CurrentAvatar";
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