#pragma once

#include <Arca/Curator.h>

#include "Entity.h"
#include "EntityPrototype.h"

#include "Work.h"
#include "ActualizeAllEntityPrototypes.h"
#include "FindEntityByName.h"
#include "FindEntityByPosition.h"
#include "MoveEntityTo.h"
#include "ModifyEntityData.h"
#include "ModifyEntityTags.h"

#include "MappedEntities.h"

namespace Atmos::Entity
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Handle(const Work& command);
    public:
        void Handle(const ActualizeAllPrototypes& command);
        Arca::Index<Entity> Handle(const FindByName& command);
        std::set<Arca::Index<Entity>> Handle(const FindByPosition& command);
        void Handle(const MoveTo& command);
        void Handle(const ModifyData& command);
        void Handle(const ModifyTags& command);
    private:
        Arca::Batch<Prototype> prototypes;
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
        static inline const TypeName typeName = "Atmos::Entity::EntityCurator";
        using HandledCommands = HandledCommands<
            Atmos::Work,
            Atmos::Entity::ActualizeAllPrototypes,
            Atmos::Entity::FindByName,
            Atmos::Entity::FindByPosition,
            Atmos::Entity::MoveTo,
            Atmos::Entity::ModifyData,
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