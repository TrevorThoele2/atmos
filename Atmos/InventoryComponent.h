#pragma once

#include "EntityComponent.h"

#include "ItemStack.h"

#include "EquipSlot.h"
#include "RandomAccessSequence.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Entity
    {
        class InventoryComponent : public Component
        {
        public:
            typedef RandomAccessSequence<ItemStack> Container;
            Container container;
        public:
            typedef TypedObjectReference<nItem> ItemReference;
        public:
            InventoryComponent(ObjectManager& manager, EntityReference reference);
            InventoryComponent(const InventoryComponent& arg) = default;
            InventoryComponent(const ::Inscription::Table<InventoryComponent>& table);

            ItemStack* Add(ItemReference itemSource, const ItemStack::Count& count);
            ItemStack* Add(ItemStack&& add);
            ItemStack* Find(Container::ID position);
            // This will push the stack into other identical stacks until all other stacks are full or this stack is out
            void Combine(ItemStack& stack);
            void Remove(Container::ID index);
            bool IsEmpty() const;
            bool IsHere(ItemStack& check) const;

            void Move(ItemStack& stack, TypedObjectReference<InventoryComponent> moveTo);
            bool AttemptConsume(ItemStack& stack);
            bool AttemptEquip(EquipSlot slot, ItemStack& stack);

            size_t GetTotalCount() const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Entity::InventoryComponent> : ObjectTraitsBase<Entity::InventoryComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Entity::InventoryComponent)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}