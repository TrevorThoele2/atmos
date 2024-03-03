#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Entity.h"

namespace Atmos::Entity
{
    class MappedEntities final : public Arca::ClosedTypedRelic<MappedEntities>
    {
    public:
        [[nodiscard]] Entity* EntityWithName(const String& name);

        [[nodiscard]] size_t Size() const;
    private:
        using Map = std::unordered_map<String, Entity*>;
        Map map;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::MappedEntities>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Entity::MappedEntities";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::MappedEntities, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::MappedEntities, BinaryArchive>
    {};
}