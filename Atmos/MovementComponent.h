#pragma once

#include "ObjectReference.h"

#include "EntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos::Entity
{
    class MovementComponent : public Component
    {
    public:
        typedef TypedObjectReference<ScriptInstance> ScriptInstance;
    public:
        bool enabled;
    public:
        ScriptInstance movementModulatorCreator;
        ScriptInstance changeDirectionModulatorCreator;
    public:
        MovementComponent(ObjectManager& manager, EntityReference reference);
        MovementComponent(const MovementComponent& arg) = default;
        MovementComponent(const ::Inscription::BinaryTableData<MovementComponent>& data);

        void Enable();
        void Disable();

        bool IsMoving() const;
        bool CanMove() const;

        ObjectTypeDescription TypeDescription() const override;
    private:
        void SetupScripts();
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Atmos
{
    template<>
    struct ObjectTraits<Entity::MovementComponent> : ObjectTraitsBase<Entity::MovementComponent>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<Entity::Component> bases = {};
    };
}

namespace Inscription
{
    template<>
    struct TableData<::Atmos::Entity::MovementComponent, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::Entity::MovementComponent, BinaryArchive>
    {
        bool enabled;
        ObjectT::ScriptInstance movementModulatorCreator;
        ObjectT::ScriptInstance changeDirectionModulatorCreator;
    };

    template<>
    class Scribe<::Atmos::Entity::MovementComponent, BinaryArchive> :
        public ObjectScribe<::Atmos::Entity::MovementComponent, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };
}