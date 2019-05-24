#pragma once

#include "ObjectReference.h"

#include "EntityComponent.h"

#include "ScriptInstance.h"

namespace Atmos
{
    namespace Entity
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
            INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(MovementComponent);

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

    template<>
    struct ObjectTraits<Entity::MovementComponent> : ObjectTraitsBase<Entity::MovementComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::MovementComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}