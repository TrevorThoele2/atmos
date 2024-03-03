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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("constructor", object.constructor);
            format("name", object.name);
            format("position", object.position);
            format("solid", object.solid);
        }
    };

    template<class Format>
    struct ScribeTraits<Atmos::Entity::Prototype, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Entity::Prototype>;
    };
}