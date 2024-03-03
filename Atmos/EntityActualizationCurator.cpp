#include "EntityActualizationCurator.h"

#include "CurrentActualizingEntity.h"
#include "ExecuteScript.h"

#include <Arca/Reliquary.h>

namespace Atmos::Entity
{
    ActualizationCurator::ActualizationCurator(Init init) :
        Curator(init),
        prototypes(init.owner.Batch<Prototype>())
    {}

    void ActualizationCurator::Handle(const Work& command)
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
                Arca::Create<Entity>(prototype.name, prototype.position, prototype.solid));

            if (prototype.constructor)
                toConstruct.emplace_back(MutablePointer().Of(entity), entity.ID(), prototype.constructor);
        }

        auto currentActualizing = MutablePointer().Of<CurrentActualizing>();

        for (auto& currentToConstruct : toConstruct)
        {
            currentActualizing->entity = Arca::Index<Entity>(currentToConstruct.id, Owner());
            Owner().Do(Scripting::Execute{ currentToConstruct.constructor });
        }

        currentActualizing->entity = Arca::Index<Entity>();

        Owner().Do(Arca::Clear(Arca::TypeFor<Prototype>()));
    }
}