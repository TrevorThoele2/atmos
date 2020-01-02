#pragma once

#include <Arca/OpenTypedRelicAutomation.h>
#include "GeneralComponent.h"

namespace Atmos::Entity
{
    class Entity final : public Arca::OpenTypedRelicAutomation<Entity, GeneralComponent>
    {
    public:
        Arca::LocalPtr<GeneralComponent> general;

        void PostConstruct(ShardTuple shards);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static const TypeName typeName;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Entity, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::Entity, BinaryArchive>
    {};
}