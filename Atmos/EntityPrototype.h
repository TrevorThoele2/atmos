#pragma once

#include <Arca/ClosedTypedRelic.h>

#include "ScriptInstance.h"
#include "Name.h"
#include "GridPoint.h"
#include "Angle2D.h"

namespace Atmos::Entity
{
    class Prototype final : public Arca::ClosedTypedRelic<Prototype>
    {
    public:
        Arca::Index<Script::Instance> initializer;

        Name name;

        Spatial::Grid::Point position;
        Spatial::Angle2D direction;
    public:
        explicit Prototype(Init init);
        explicit Prototype(
            Init init,
            Arca::Index<Script::Instance> initializer,
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
        static inline const TypeName typeName = "Atmos::Entity::Prototype";
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
            archive("initializer", object.initializer);
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