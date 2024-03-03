#pragma once

#include <Arca/OpenTypedRelic.h>
#include "GeneralComponent.h"

namespace Atmos::Entity
{
    class Entity final : public Arca::OpenTypedRelic<Entity>
    {
    public:
        Arca::Index<GeneralComponent> general;

        void PostConstruct();
        void Initialize();
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Entity";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Entity, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::Entity, BinaryArchive>
    {};
}