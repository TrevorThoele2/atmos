#pragma once

#include <Arca/ShardTraits.h>

#include "ScriptInstance.h"

namespace Arca
{
    class Reliquary;
}

namespace Atmos::Entity
{
    class MovementComponent
    {
    public:
        Script::ScriptInstance* movementModulatorCreator;
        Script::ScriptInstance* changeDirectionModulatorCreator;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::MovementComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::MovementComponent, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::MovementComponent, BinaryArchive>
    {};
}