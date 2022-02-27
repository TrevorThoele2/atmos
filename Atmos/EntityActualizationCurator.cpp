#include "EntityActualizationCurator.h"

#include "CurrentActualizingEntity.h"
#include "ExecuteScript.h"

#include <Arca/Create.h>
#include <Arca/Clear.h>

namespace Atmos::Entity
{
    ActualizationCurator::ActualizationCurator(Init init) :
        Curator(init),
        prototypes(init.owner.Batch<Prototype>())
    {}

    void ActualizationCurator::Handle(const Work&)
    {
        if (worked)
            return;

        struct ToConstruct
        {
            Entity* entity;
            Arca::RelicID id;
            Arca::Index<Scripting::Script> constructor;
            ToConstruct(Entity* entity, Arca::RelicID id, Arca::Index<Scripting::Script> constructor) :
                entity(entity), id(id), constructor(constructor)
            {}
        };

        std::vector<ToConstruct> toConstruct;
        toConstruct.reserve(prototypes.Size());
        for (auto& prototype : prototypes)
        {
            auto entity = Owner().Do(
                Arca::Create<Entity>(prototype.name, prototype.position, prototype.solid, std::vector<Tag>{}));

            if (prototype.constructor)
                toConstruct.emplace_back(MutablePointer().Of(entity), entity.ID(), prototype.constructor);
        }

        const auto currentActualizing = MutablePointer().Of<CurrentActualizing>();

        for (const auto& currentToConstruct : toConstruct)
        {
            currentActualizing->entity = Owner().Find<Entity>(currentToConstruct.id);
            Owner().Do(Scripting::Execute{ currentToConstruct.constructor });
        }

        currentActualizing->entity = Arca::Index<Entity>();

        Owner().Do(Arca::Clear(Arca::TypeFor<Prototype>()));
    }
}