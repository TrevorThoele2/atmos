
#include "GeneralComponent.h"

#include <Inscription/UnorderedSet.h>

namespace Atmos
{
    namespace Entity
    {
        GeneralComponent::GeneralComponent(ObjectManager& manager, EntityReference reference) :
            Component(manager, reference), solid(false)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(GeneralComponent) : INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        void GeneralComponent::SetPosition(const PositionT &set)
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

    const ObjectTypeName ObjectTraits<Entity::GeneralComponent>::typeName = "GeneralComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::GeneralComponent)
    {
        INSCRIPTION_TABLE_ADD(name);
        INSCRIPTION_TABLE_ADD(niceName);
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(direction);
        INSCRIPTION_TABLE_ADD(solid);
        INSCRIPTION_TABLE_ADD(tags);
    }

    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Entity::GeneralComponent)
    {
        if (scribe.IsOutput())
        {
            auto& outputScribe = *scribe.AsOutput();

            ::Inscription::ContainerSize size(obj.storage.size());
            outputScribe.Save(size);

            for (auto& loop : obj.storage)
                outputScribe.Save(loop.second);
        }
        else // INPUT
        {
            auto& inputScribe = *scribe.AsInput();

            ::Inscription::ContainerSize size;
            inputScribe.Load(size);

            while (size-- > 0)
            {
                ManagedT::StorageObject storageObject;
                inputScribe.Load(storageObject);

                obj.storage.emplace(storageObject.name, std::move(storageObject));
            }
        }
    }
}