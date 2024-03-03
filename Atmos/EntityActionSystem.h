#pragma once

#include "ObjectSystem.h"
#include "ObjectManager.h"

#include "ActionComponent.h"

#include "Field.h"

namespace Atmos::Entity
{
    class ActionSystem : public ObjectSystem
    {
    public:
        ActionSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
        void WorkImpl() override;
    private:
        ObjectBatch<ActionComponent> actionComponents;
    private:
        void OnFieldSet(World::Field& field);
        void OnFieldUnset(World::Field* field);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::ActionSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::ActionSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}