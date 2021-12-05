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
            bool isSolid,
            const std::vector<Tag>& tags);

        bool operator==(const Entity& arg) const;
        bool operator!=(const Entity& arg) const;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Entity::Entity";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Spatial::Grid::Point& position,
            bool isSolid,
            const std::vector<Atmos::Tag>& tags);
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("name", object.name);
            format("position", object.position);
            format("isSolid", object.isSolid);
            format("tags", object.tags);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Entity::Entity, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Entity>;
    };
}