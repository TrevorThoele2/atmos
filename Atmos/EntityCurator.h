#pragma once

#include <Arca/Curator.h>

#include "Entity.h"
#include "EntityPrototype.h"

#include "Work.h"
#include "FindEntityByName.h"
#include "FindEntityByPosition.h"
#include "MoveEntityTo.h"
#include "ModifyEntityTags.h"

#include "MappedEntities.h"

namespace Atmos::Entity
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);
    public:
        Arca::Index<Entity> Handle(const FindByName& command);
        std::set<Arca::Index<Entity>> Handle(const FindByPosition& command);
        void Handle(const MoveTo& command);
        void Handle(const ModifyTags& command);
    private:
        Arca::Batch<Entity> entities;

        Arca::Index<Mapped> mapped;

        static void AddEntityTo(Mapped::NameToEntity& to, const String& name, Arca::Index<Entity> entity);
        static void AddEntityTo(Mapped::PositionToEntity& to, const Spatial::Grid::Point& position, Arca::Index<Entity> entity);
        static void RemoveEntityFrom(Mapped::NameToEntity& from, Arca::Index<Entity> entity);
        static void RemoveEntityFrom(Mapped::PositionToEntity& from, Arca::Index<Entity> entity);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static TypeName TypeName() { return "Atmos::Entity::Curator"; }
        using HandledCommands = HandledCommands<
            Atmos::Entity::FindByName,
            Atmos::Entity::FindByPosition,
            Atmos::Entity::MoveTo,
            Atmos::Entity::ModifyTags>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Entity::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Entity::Curator>;
    };
}