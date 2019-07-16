#include "SenseComponent.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Entity
{
    SenseComponent::SenseComponent(ObjectManager& manager, EntityReference reference) :
        Component(manager, reference), enabled(true)
    {}

    SenseComponent::SenseComponent(const ::Inscription::BinaryTableData<SenseComponent>& data) :
        Component(std::get<0>(data.bases))
    {}

    void SenseComponent::Enable(bool enable)
    {
        enabled = enable;
        SyncObjects();
    }

    void SenseComponent::Disable()
    {
        Enable(false);
    }

    bool SenseComponent::IsEnabled() const
    {
        return enabled;
    }

    void SenseComponent::SyncObjects()
    {
        if (enabled)
        {
            for (auto& loop : sprites)
            {
                //GetCurrentRenderFragments()->Add(*loop.source);
                loop.source->enabled = true;
            }

            for (auto& loop : sounds)
            {
                loop.source->enabled = true;
            }
        }
        else
        {
            for (auto& loop : sprites)
            {
                //GetCurrentRenderFragments()->Remove(*loop.source);
                loop.source->enabled = false;
            }

            for (auto& loop : sounds)
            {
                loop.source->enabled = false;
            }
        }
    }

    ObjectTypeDescription SenseComponent::TypeDescription() const
    {
        return ObjectTraits<SenseComponent>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::SenseComponent>::typeName = "SenseComponent";
}

namespace Inscription
{
    Scribe<::Atmos::Entity::SenseComponent, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::position, &DataT::position),
            DataEntry::Auto(&ObjectT::sprites, &DataT::sprites),
            DataEntry::Auto(&ObjectT::sounds, &DataT::sounds),
            DataEntry::Auto(&ObjectT::enabled, &DataT::enabled) });
    }

    void Scribe<::Atmos::Entity::SenseComponent, BinaryArchive>::Table::ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsInput())
            object.SyncObjects();
    }
}