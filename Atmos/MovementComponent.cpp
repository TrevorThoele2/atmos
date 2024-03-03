
#include "MovementComponent.h"

#include "ObjectManager.h"

#include "nEntityPositionSystem.h"

namespace Atmos
{
    namespace Ent
    {
        nMovementComponent::nMovementComponent(EntityReference reference) : nEntityComponent(reference)
        {}

        nMovementComponent::nMovementComponent(const ::Inscription::Table<nMovementComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        void nMovementComponent::Enable()
        {
            enabled = true;
        }

        void nMovementComponent::Disable()
        {
            enabled = false;
        }

        bool nMovementComponent::IsMoving() const
        {
            return movementModulatorCreator->IsRunning() || changeDirectionModulatorCreator->IsRunning() || Manager()->FindSystem<nEntityPositionSystem>()->IsMoving(owner.Get());
        }

        bool nMovementComponent::CanMove() const
        {
            return enabled && !IsMoving();
        }

        ObjectTypeDescription nMovementComponent::TypeDescription() const
        {
            return ObjectTraits<nMovementComponent>::TypeDescription();
        }

        void nMovementComponent::SetupScripts()
        {
            movementModulatorCreator->owner = this;
            changeDirectionModulatorCreator->owner = this;
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(nMovementComponent)
        {
            scribe(enabled);
            scribe(movementModulatorCreator);
            scribe(changeDirectionModulatorCreator);
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nMovementComponent>::typeName = "MovementComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nMovementComponent)
    {
        INSCRIPTION_TABLE_ADD(enabled);
        INSCRIPTION_TABLE_ADD(movementModulatorCreator);
        INSCRIPTION_TABLE_ADD(changeDirectionModulatorCreator);
    }
}