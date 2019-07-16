#include "GeneralComponent.h"

#include <Inscription/ContainerSize.h>
#include <Inscription/UnorderedSetScribe.h>

namespace Atmos::Entity
{
    GeneralComponent::GeneralComponent(ObjectManager& manager, EntityReference reference) :
        Component(manager, reference), solid(false)
    {}

    GeneralComponent::GeneralComponent(const ::Inscription::BinaryTableData<GeneralComponent>& data) :
        Component(std::get<0>(data.bases)), name(data.name), niceName(data.niceName), position(data.position),
        direction(data.direction), solid(data.solid), tags(data.tags)
    {}

    void GeneralComponent::SetPosition(const Position &set)
    {
        position = set;
    }

    GridPosition GeneralComponent::GetPositionInFront() const
    {
        GridPosition posInFront(position);

        switch (direction.Get())
        {
        case Direction::Value::UP:
            --posInFront.y;
            break;
        case Direction::Value::DOWN:
            ++posInFront.y;
            break;
        case Direction::Value::LEFT:
            --posInFront.x;
            break;
        case Direction::Value::RIGHT:
            ++posInFront.x;
            break;
        }

        return posInFront;
    }

    void GeneralComponent::SetSolid(bool set)
    {
        solid = set;
    }

    bool GeneralComponent::IsSolid() const
    {
        return solid;
    }

    GeneralComponent::StorageObject* GeneralComponent::AddStorage(const StorageObject &add)
    {
        return &storage.emplace(add.name, add).first->second;
    }

    GeneralComponent::StorageObject* GeneralComponent::AddStorage(StorageObject &&add)
    {
        return &storage.emplace(add.name, std::move(add)).first->second;
    }

    void GeneralComponent::RemoveStorage(const Name &remove)
    {
        storage.erase(remove);
    }

    void GeneralComponent::RemoveStorage(const StorageObject &remove)
    {
        RemoveStorage(remove.name);
    }

    GeneralComponent::StorageObject* GeneralComponent::FindStorage(const Name &find)
    {
        auto found = storage.find(find);
        if (found == storage.end())
            return nullptr;

        return &found->second;
    }

    GeneralComponent::StorageObject* GeneralComponent::FindStorage(const StorageObject &find)
    {
        return FindStorage(find.name);
    }

    const GeneralComponent::StorageObject* GeneralComponent::FindStorage(const Name &find) const
    {
        auto found = storage.find(find);
        if (found == storage.end())
            return nullptr;

        return &found->second;
    }

    const GeneralComponent::StorageObject* GeneralComponent::FindStorage(const StorageObject &find) const
    {
        return FindStorage(find.name);
    }

    bool GeneralComponent::HasStorage(const Name &check) const
    {
        return storage.find(check) != storage.end();
    }

    bool GeneralComponent::HasStorage(const StorageObject &check) const
    {
        return HasStorage(check.name);
    }

    void GeneralComponent::TagAs(const Tag &add)
    {
        tags.emplace(add);
    }

    void GeneralComponent::RemoveTag(const Tag &remove)
    {
        tags.erase(remove);
    }

    bool GeneralComponent::IsTaggedAs(const Tag &check) const
    {
        return tags.find(check) != tags.end();
    }

    ObjectTypeDescription GeneralComponent::TypeDescription() const
    {
        return ObjectTraits<GeneralComponent>::TypeDescription();
    }
}

namespace Atmos
{
    const ObjectTypeName ObjectTraits<Entity::GeneralComponent>::typeName = "GeneralComponent";
}

namespace Inscription
{
    Scribe<::Atmos::Entity::GeneralComponent, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::name, &DataT::name),
            DataEntry::Auto(&ObjectT::niceName, &DataT::niceName),
            DataEntry::Auto(&ObjectT::position, &DataT::position),
            DataEntry::Auto(&ObjectT::direction, &DataT::direction),
            DataEntry::Auto(&ObjectT::solid, &DataT::solid),
            DataEntry::Auto(&ObjectT::tags, &DataT::tags) });
    }

    void Scribe<::Atmos::Entity::GeneralComponent, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            ContainerSize size(object.storage.size());
            archive(size);

            for (auto& loop : object.storage)
                archive(loop.second);
        }
        else // INPUT
        {
            ContainerSize size;
            archive(size);

            while (size-- > 0)
            {
                ObjectT::StorageObject storageObject;
                archive(storageObject);

                object.storage.emplace(storageObject.name, std::move(storageObject));
            }
        }
    }
}