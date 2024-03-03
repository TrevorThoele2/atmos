#pragma once

#include <Arca/Relic.h>

#include "Script.h"
#include "Name.h"
#include "GridPoint.h"
#include "Angle2D.h"

namespace Atmos::Entity
{
    class Prototype final
    {
    public:
        Arca::Index<Scripting::Script> constructor;

        Name name;

        Spatial::Grid::Point position;
        Spatial::Angle2D direction = 0.0f;
    public:
        Prototype() = default;
        Prototype(
            Arca::Index<Scripting::Script> constructor,
            Name name,
            Spatial::Grid::Point position,
            Spatial::Angle2D direction);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Prototype>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Entity::Prototype"; }
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Prototype> final
    {
    public:
        using ObjectT = Atmos::Entity::Prototype;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            archive("constructor", object.constructor);
            archive("name", object.name);
            archive("position", object.position);
            archive("direction", object.direction);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Prototype, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Prototype>;
    };
}