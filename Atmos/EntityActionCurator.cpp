#include "EntityActionCurator.h"

#include <Arca/Reliquary.h>

#include "RunningScript.h"

namespace Atmos::Entity
{
    void ActionCurator::InitializeImplementation()
    {
        actionComponents = Owner().Batch<ActionComponent>();
    }

    void ActionCurator::WorkImplementation(Stage& stage)
    {
        for (auto& loop : actionComponents)
        {
            auto runningScript = loop.script->RunningForThis();
            if (!runningScript)
                continue;

            runningScript->Resume();
        }

        if (!fieldSet.IsEmpty())
            for (auto& loop : actionComponents)
                loop.FireFieldEntered();

        if (!fieldUnset.IsEmpty())
            for (auto& loop : actionComponents)
                loop.FireFieldLeft();
    }
}