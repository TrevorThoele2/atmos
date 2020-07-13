#include "EntityActionCurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

#include "FieldSet.h"
#include "FieldUnset.h"

namespace Atmos::Entity
{
    ActionCurator::ActionCurator(Init init) :
        Curator(init),
        entities(init.owner.Batch<Entity>())
    {
        init.owner.On<World::FieldSet>([this](const World::FieldSet&)
            {
                Fire<Action::Activation::EnterField>();
            });

        init.owner.On<World::FieldUnset>([this](const World::FieldUnset&)
            {
                Fire<Action::Activation::LeaveField>();
            });
    }

    void ActionCurator::Work()
    {
        for (auto& entity : entities)
        {
            for (auto& action : entity.actions)
            {
                const auto runningScript = action.second.script->RunningForThis();
                if (!runningScript)
                    continue;

                MutablePointer().Of(runningScript)->Resume();
            }
        }
    }
}