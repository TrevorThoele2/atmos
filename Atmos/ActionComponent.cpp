#include "ActionComponent.h"

namespace Atmos::Entity
{
    ActionComponent::ActionComponent(ObjectManager& manager, EntityReference owner) :
        Component(manager, owner), activator(Activator::UseOn)
    {
        SetupScripts();
    }

    ActionComponent::ActionComponent(const ::Inscription::BinaryTableData<ActionComponent>& data) :
        Component(std::get<0>(data.bases)), activator(data.activator)
    {
        SetupScripts();
    }

    void ActionComponent::FireMovedInto()
    {
        if (activator == Activator::EnterTile)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireAttemptMovedInto()
    {
        if (activator == Activator::AttemptEnterTile)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireUseOn()
    {
        if (activator == Activator::UseOn)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireUseInto()
    {
        if (activator == Activator::UseInto)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireFieldEntered()
    {
        if (activator == Activator::EnterField)
            script->ExecuteDeferred();
    }

    void ActionComponent::FireFieldLeft()
    {
        if (activator == Activator::LeaveField)
            script->ExecuteDeferred();
    }

    ObjectTypeDescription ActionComponent::TypeDescription() const
    {
        return ObjectTraits<ActionComponent>::TypeDescription();
    }

    void ActionComponent::SetupScripts()
    {
        script->owner = this;
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::ActionComponent>::typeName = "ActionComponent";
}

namespace Inscription
{
    Scribe<::Atmos::Entity::ActionComponent, BinaryArchive>::Table::Table()
    {
        AddDataEntry(DataEntry::Auto(&ObjectT::activator, &DataT::activator));
    }
}