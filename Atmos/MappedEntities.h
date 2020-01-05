#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "Entity.h"

namespace Atmos::Entity
{
    class MappedEntities final : public Arca::ClosedTypedRelicAutomation<MappedEntities>
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
        static inline const TypeName typeName = "MappedEntities";
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