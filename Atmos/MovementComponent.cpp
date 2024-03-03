
#include "MovementComponent.h"

#include "ObjectManager.h"

#include "EntityPositionSystem.h"

namespace Atmos
{
    namespace Entity
    {
        MovementComponent::MovementComponent(ObjectManager& manager, EntityReference reference) :
            Component(manager, reference)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(MovementComponent) :
            INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        void MovementComponent::Enable()
        {
            enabled = true;
        }

        void MovementComponent::Disable()
        {
            enabled = false;
        }

        bool MovementComponent::IsMoving() const
        {
            return movementModulatorCreator->IsRunning() || changeDirectionModulatorCreator->IsRunning() || Manager()->FindSystem<PositionSystem>()->IsMoving(owner.Get());
        }

        bool MovementComponent::CanMove() const
        {
            return enabled && !IsMoving();
        }

        ObjectTypeDescription MovementComponent::TypeDescription() const
        {
            return ObjectTraits<MovementComponent>::TypeDescription();
        }

        void MovementComponent::SetupScripts()
        {
            movementModulatorCreator->owner = this;
            changeDirectionModulatorCreator->owner = this;
        }
    }

    const ObjectTypeName ObjectTraits<Entity::MovementComponent>::typeName = "MovementComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::MovementComponent)
    {
        INSCRIPTION_TABLE_ADD(enabled);
        INSCRIPTION_TABLE_ADD(movementModulatorCreator);
        INSCRIPTION_TABLE_ADD(changeDirectionModulatorCreator);
    }
}