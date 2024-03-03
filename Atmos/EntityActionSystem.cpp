#include "EntityActionSystem.h"

#include "RunningScript.h"

#include "WorldSystem.h"

namespace Atmos::Entity
{
    ActionSystem::ActionSystem(ObjectManager& manager) : ObjectSystem(manager)
    {
        actionComponents = manager.Batch<ActionComponent>();
    }

    void ActionSystem::InitializeImpl()
    {
        auto worldSystem = Manager()->FindSystem<World::WorldSystem>();
        worldSystem->Get()->eventFieldSet.Subscribe(&ActionSystem::OnFieldSet, *this);
        worldSystem->Get()->eventFinalizeField.Subscribe(&ActionSystem::OnFieldUnset, *this);
    }

    void ActionSystem::WorkImpl()
    {
        for (auto& loop : Manager()->Batch<ActionComponent>())
        {
            auto runningScript = loop->script->RunningForThis();
            if (!runningScript.IsOccupied())
                continue;

            runningScript->Resume();
        }
    }

    void ActionSystem::OnFieldSet(World::Field& field)
    {
        for (auto& loop : actionComponents)
            loop->FireFieldEntered();
    }

    void ActionSystem::OnFieldUnset(World::Field* field)
    {
        if (!field)
            return;

        for (auto& loop : actionComponents)
            loop->FireFieldLeft();
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Entity::ActionSystem, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::Atmos::ObjectSystem>(object, archive);
    }
}