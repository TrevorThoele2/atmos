#include "EntityActionCurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

#include "FieldSet.h"
#include "FieldUnset.h"

namespace Atmos::Entity
{
    ActionCurator::ActionCurator(Init init) :
        Curator(init),
        actionComponents(init.owner.Batch<ActionComponent>())
    {
        init.owner.On<World::FieldSet>([this](const World::FieldSet& signal)
            {
                for (auto& loop : actionComponents)
                    loop.FireFieldEntered();
            });

        init.owner.On<World::FieldUnset>([this](const World::FieldUnset& signal)
            {
                for (auto& loop : actionComponents)
                    loop.FireFieldLeft();
            });
    }

    void ActionCurator::Work()
    {
        for (auto& loop : actionComponents)
        {
            const auto runningScript = loop.script->RunningForThis();
            if (!runningScript)
                continue;

            MutablePointer().Of(runningScript)->Resume();
        }
    }
}