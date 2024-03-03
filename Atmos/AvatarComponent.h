#pragma once

#include <Arca/Shard.h>
#include <Arca/NullScribe.h>

namespace Atmos::Entity
{
    class AvatarComponent
    {
    public:
        using Currency = int;
        Currency currency = 0;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::AvatarComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "Atmos::Entity::AvatarComponent";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AvatarComponent, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::AvatarComponent, BinaryArchive>
    {};
}