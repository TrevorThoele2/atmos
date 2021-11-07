#pragma once

#include "ArcaRelicIncludes.h"

#include "Script.h"
#include "Name.h"
#include "GridPoint.h"

namespace Atmos::Entity
{
    class Prototype final
    {
    public:
        Arca::Index<Scripting::Script> constructor;

        Name name;

        Spatial::Grid::Point position;

        bool solid;
    public:
        Prototype() = default;
        Prototype(
            Arca::Index<Scripting::Script> constructor,
            Name name,
            Spatial::Grid::Point position,
            bool solid);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Prototype>
    {
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Entity::Prototype";
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
            archive("solid", object.solid);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Prototype, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Prototype>;
    };
}