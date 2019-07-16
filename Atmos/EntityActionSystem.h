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
    private:
        void InitializeImpl() override;
        void WorkImpl() override;
    private:
        ObjectBatch<ActionComponent> actionComponents;
    private:
        void OnFieldSet(Field& field);
        void OnFieldUnset(Field* field);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::ActionSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::ActionSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}