#include "AIComponent.h"

namespace Atmos::Entity
{
    AIComponent::AIComponent(ObjectManager& manager, EntityReference owner) :
        Component(manager, owner)
    {
        SetupScripts();
    }

    AIComponent::AIComponent(const ::Inscription::BinaryTableData<AIComponent>& data) :
        Component(std::get<0>(data.bases)), script(data.script)
    {
        SetupScripts();
    }

    ObjectTypeDescription AIComponent::TypeDescription() const
    {
        return ObjectTraits<AIComponent>::TypeDescription();
    }

    void AIComponent::SetupScripts()
    {
        script->owner = this;
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::AIComponent>::typeName = "AIComponent";
}

namespace Inscription
{
    Scribe<::Atmos::Entity::AIComponent, BinaryArchive>::Table::Table()
    {
        AddDataEntry(DataEntry::Auto(&ObjectT::script, &DataT::script));
    }
}