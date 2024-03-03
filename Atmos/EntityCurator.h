#pragma once

#include <Arca/Curator.h>

#include "Entity.h"
#include "EntityPrototype.h"

#include "ActualizeAllEntityPrototypes.h"
#include "MoveEntity.h"

#include "MappedEntities.h"

namespace Atmos::Entity
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Work();
    public:
        void Handle(const ActualizeAllPrototypes& command);
        void Handle(const Move& command);
    private:
        Arca::Batch<Prototype> prototypes;
        Arca::Batch<Entity> entities;

        Arca::Index<MappedEntities> mappedEntities;
    private:
        template<Action::Activation MatchActivation>
        void Fire();
    };

    template<Action::Activation MatchActivation>
    void Curator::Fire()
    {
        for (auto& entity : entities)
        {
            for (auto& action : entity.actions)
            {
                if (action.second.activation == MatchActivation)
                {
                    auto mutableScript = MutablePointer().Of(action.second.script);
                    mutableScript->ExecuteDeferred();
                }
            }
        }
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::Curator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "Atmos::Entity::EntityCurator";
        using HandledCommands = HandledCommands<
            Atmos::Entity::ActualizeAllPrototypes,
            Atmos::Entity::Move>;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::Curator, BinaryArchive> final
        : public ArcaNullScribe<::Atmos::Entity::Curator, BinaryArchive>
    {};
}