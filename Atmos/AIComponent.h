#pragma once

#include <Arca/ShardTraits.h>

#include "ScriptInstance.h"

namespace Atmos::Entity
{
    class AIComponent
    {
    public:
        Script::ScriptInstance* script;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::AIComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AIComponent, BinaryArchive> :
        public ArcaNullScribe<::Atmos::Entity::AIComponent, BinaryArchive>
    {};
}