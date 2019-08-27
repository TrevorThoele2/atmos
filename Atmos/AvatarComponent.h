#pragma once

#include <Arca/ShardTraits.h>
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
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AvatarComponent, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::AvatarComponent, BinaryArchive>
    {};
}