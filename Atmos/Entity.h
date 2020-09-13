#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "Name.h"
#include "EntityDatum.h"
#include "EntityTag.h"

#include "GridPoint.h"
#include "Angle2D.h"

#include <Inscription/UnorderedMapScribe.h>

namespace Atmos::Entity
{
    class Entity final : public Arca::ClosedTypedRelic<Entity>
    {
    public:
        Name name;
        Name displayName;

        Spatial::Grid::Point position;
        Spatial::Angle2D direction;

        bool solid = false;

        std::unordered_map<Name, Datum> data;
        std::unordered_set<Tag> tags;
    public:
        explicit Entity(Init init);
        explicit Entity(
            Init init,
            Name name,
            Spatial::Grid::Point position,
            Spatial::Angle2D direction);
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
    class Scribe<Atmos::Entity::Entity> final
    {
    public:
        using ObjectT = Atmos::Entity::Entity;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("name", object.name);
            archive("displayName", object.displayName);
            archive("position", object.position);
            archive("solid", object.solid);
            archive("data", object.data);
            archive("tags", object.tags);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Entity, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Entity>;
    };
}