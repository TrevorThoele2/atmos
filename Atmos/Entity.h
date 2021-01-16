#pragma once

#include <Arca/Relic.h>

#include "Name.h"
#include "Tag.h"

#include "GridPoint.h"
#include "Angle2D.h"

#include <Inscription/UnorderedMapScribe.h>

namespace Atmos::Entity
{
    class Entity final
    {
    public:
        Name name;
        Name displayName;

        Spatial::Grid::Point position;
        Spatial::Angle2D direction = 0.0f;

        bool isSolid = false;

        std::vector<Tag> tags;
    public:
        Entity(Arca::RelicInit init);
        explicit Entity(
            Arca::RelicInit init,
            Name name,
            Name displayName,
            Spatial::Grid::Point position,
            Spatial::Angle2D direction,
            bool isSolid);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Entity>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Entity::Entity"; }
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