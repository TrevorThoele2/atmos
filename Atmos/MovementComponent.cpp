#include "MovementComponent.h"

#include "ObjectManager.h"

#include "EntityPositionSystem.h"

namespace Atmos::Entity
{
    MovementComponent::MovementComponent(ObjectManager& manager, EntityReference reference) :
        Component(manager, reference)
    {}

    MovementComponent::MovementComponent(const ::Inscription::BinaryTableData<MovementComponent>& data) :
        Component(std::get<0>(data.bases))
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
        return movementModulatorCreator->IsRunning() ||
            changeDirectionModulatorCreator->IsRunning() ||
            Manager()->FindSystem<PositionSystem>()->IsMoving(owner.Get());
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

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::MovementComponent>::typeName = "MovementComponent";
}

namespace Inscription
{
    Scribe<::Atmos::Entity::MovementComponent, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::enabled, &DataT::enabled),
            DataEntry::Auto(&ObjectT::movementModulatorCreator, &DataT::movementModulatorCreator),
            DataEntry::Auto(&ObjectT::changeDirectionModulatorCreator, &DataT::changeDirectionModulatorCreator) });
    }
}