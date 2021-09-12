#pragma once

#include "ArcaRelicIncludes.h"

#include "Name.h"
#include "GridPoint.h"
#include "Tag.h"

#include <Inscription/VectorScribe.h>
#include <Inscription/UnorderedMapScribe.h>

namespace Atmos::Entity
{
    class Entity final
    {
    public:
        Name name;
        Spatial::Grid::Point position;
        bool isSolid = false;
        std::vector<Tag> tags;
    public:
        Entity(Arca::RelicInit init, Arca::Serialization);
        Entity(
            Arca::RelicInit init,
            Name name,
            Spatial::Grid::Point position,
            bool isSolid);

        bool operator==(const Entity& arg) const;
        bool operator!=(const Entity& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Entity::Entity"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Spatial::Grid::Point& position,
            bool isSolid);
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
            archive("position", object.position);
            archive("isSolid", object.isSolid);
            archive("tags", object.tags);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Entity, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Entity>;
    };
}