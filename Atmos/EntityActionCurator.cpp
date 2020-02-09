#include "EntityActionCurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

namespace Atmos::Entity
{
    ActionCurator::ActionCurator(Init init) :
        Curator(init),
        actionComponents(init.owner.Batch<ActionComponent>())
    {}

    void ActionCurator::Work()
    {
        for (auto& loop : actionComponents)
        {
            const auto runningScript = loop.script->RunningForThis();
            if (!runningScript)
                continue;

            Data(runningScript)->Resume();
        }

        if (!fieldSet.IsEmpty())
            for (auto& loop : actionComponents)
                loop.FireFieldEntered();

        if (!fieldUnset.IsEmpty())
            for (auto& loop : actionComponents)
                loop.FireFieldLeft();
    }
}