#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "EntityDatum.h"
#include "EntityAction.h"
#include "EntityTag.h"

#include "GridPoint.h"
#include "Direction.h"

namespace Atmos::Entity
{
    class Entity final : public Arca::ClosedTypedRelic<Entity>
    {
    public:
        Name name;
        Name displayName;

        Spatial::Grid::Point position;
        Spatial::Direction direction;

        bool solid = false;

        std::unordered_map<Name, Datum> data;
        std::unordered_map<Name, Action> actions;
        std::unordered_set<Tag> tags;
    public:
        explicit Entity(Init init);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Entity::Entity";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Entity, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Entity::Entity, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}