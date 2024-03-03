#include "GeneralComponent.h"

namespace Atmos::Entity
{
    void GeneralComponent::SetPosition(const Position &set)
    {
        position = set;
    }

    Grid::Position GeneralComponent::PositionInFront() const
    {
        auto positionInFront(position);

        switch (direction.Get())
        {
        case Direction::Value::Up:
            --positionInFront.y;
            break;
        case Direction::Value::Down:
            ++positionInFront.y;
            break;
        case Direction::Value::Left:
            --positionInFront.x;
            break;
        case Direction::Value::Right:
            ++positionInFront.x;
            break;
        default:
            break;
        }

        return positionInFront;
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

    const GeneralComponent::StorageObject* GeneralComponent::FindStorage(const Name &find) const
    {
        auto found = storage.find(find);
        if (found == storage.end())
            return nullptr;

        return &found->second;
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
}

namespace Arca
{
    const TypeName Traits<::Atmos::Entity::GeneralComponent>::typeName = "GeneralComponent";
}