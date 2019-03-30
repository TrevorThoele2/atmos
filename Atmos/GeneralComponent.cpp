
#include "GeneralComponent.h"

#include <Inscription/UnorderedSet.h>

namespace Atmos
{
    namespace Ent
    {
        nGeneralComponent::nGeneralComponent(EntityReference reference) : nEntityComponent(reference), solid(false)
        {}

        nGeneralComponent::nGeneralComponent(const ::Inscription::Table<nGeneralComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        void nGeneralComponent::SetPosition(const PositionT &set)
        {
            position = set;
        }

        GridPosition nGeneralComponent::GetPositionInFront() const
        {
            GridPosition posInFront(position);

            switch (direction.Get())
            {
            case Direction::ValueT::UP:
                --posInFront.y;
                break;
            case Direction::ValueT::DOWN:
                ++posInFront.y;
                break;
            case Direction::ValueT::LEFT:
                --posInFront.x;
                break;
            case Direction::ValueT::RIGHT:
                ++posInFront.x;
                break;
            }

            return posInFront;
        }

        void nGeneralComponent::SetSolid(bool set)
        {
            solid = set;
        }

        bool nGeneralComponent::IsSolid() const
        {
            return solid;
        }

        nGeneralComponent::StorageObject* nGeneralComponent::AddStorage(const StorageObject &add)
        {
            return &storage.emplace(add.name, add).first->second;
        }

        nGeneralComponent::StorageObject* nGeneralComponent::AddStorage(StorageObject &&add)
        {
            return &storage.emplace(add.name, std::move(add)).first->second;
        }

        void nGeneralComponent::RemoveStorage(const Name &remove)
        {
            storage.erase(remove);
        }

        void nGeneralComponent::RemoveStorage(const StorageObject &remove)
        {
            RemoveStorage(remove.name);
        }

        nGeneralComponent::StorageObject* nGeneralComponent::FindStorage(const Name &find)
        {
            auto found = storage.find(find);
            if (found == storage.end())
                return nullptr;

            return &found->second;
        }

        nGeneralComponent::StorageObject* nGeneralComponent::FindStorage(const StorageObject &find)
        {
            return FindStorage(find.name);
        }

        const nGeneralComponent::StorageObject* nGeneralComponent::FindStorage(const Name &find) const
        {
            auto found = storage.find(find);
            if (found == storage.end())
                return nullptr;

            return &found->second;
        }

        const nGeneralComponent::StorageObject* nGeneralComponent::FindStorage(const StorageObject &find) const
        {
            return FindStorage(find.name);
        }

        bool nGeneralComponent::HasStorage(const Name &check) const
        {
            return storage.find(check) != storage.end();
        }

        bool nGeneralComponent::HasStorage(const StorageObject &check) const
        {
            return HasStorage(check.name);
        }

        void nGeneralComponent::TagAs(const Tag &add)
        {
            tags.emplace(add);
        }

        void nGeneralComponent::RemoveTag(const Tag &remove)
        {
            tags.erase(remove);
        }

        bool nGeneralComponent::IsTaggedAs(const Tag &check) const
        {
            return tags.find(check) != tags.end();
        }

        ObjectTypeDescription nGeneralComponent::TypeDescription() const
        {
            return ObjectTraits<nGeneralComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nGeneralComponent>::typeName = "GeneralComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nGeneralComponent)
    {
        INSCRIPTION_TABLE_ADD(name);
        INSCRIPTION_TABLE_ADD(niceName);
        INSCRIPTION_TABLE_ADD(position);
        INSCRIPTION_TABLE_ADD(direction);
        INSCRIPTION_TABLE_ADD(solid);
        INSCRIPTION_TABLE_ADD(tags);
    }

    INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(::Atmos::Ent::nGeneralComponent)
    {
        if (scribe.IsOutput())
        {
            ::Inscription::ContainerSize size(obj.storage.size());
            scribe.Save(size);

            for (auto& loop : obj.storage)
                scribe.Save(loop.second);
        }
        else // INPUT
        {
            ::Inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                ManagedT::StorageObject storageObject;
                scribe.Load(storageObject);

                obj.storage.emplace(storageObject.name, std::move(storageObject));
            }
        }
    }
}