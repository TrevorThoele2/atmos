#pragma once

#include "EntityComponent.h"

#include "Item.h"
#include "ItemStack.h"

#include "RandomAccessSequence.h"
#include "Flags.h"
#include "ObjectMenuTraits.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class InventoryComponent : public Component<InventoryComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            typedef RandomAccessSequence<ItemStack> Container;
            Container container;

            InventoryComponent() = default;
            InventoryComponent(const InventoryComponent &arg);
            InventoryComponent(InventoryComponent &&arg);
            InventoryComponent& operator=(const InventoryComponent &arg);
            InventoryComponent& operator=(InventoryComponent &&arg);

            bool operator==(const InventoryComponent &arg) const;
            bool operator!=(const InventoryComponent &arg) const;

            ItemStack* Add(const Name &name, const ItemStack::CountT &count);
            ItemStack* Add(ItemStack &&add);
            ItemStack* Find(Container::ID position);
            // This will push the stack into other identical stacks until all other stacks are full or this stack is out
            void Combine(ItemStack &stack);
            void Remove(Container::ID index);
            bool IsEmpty() const;
            bool IsHere(ItemStack &check) const;

            void Move(ItemStack &stack, InventoryComponent &moveTo);
            bool AttemptConsume(ItemStack &stack);
            bool AttemptEquip(EquipSlot slot, ItemStack &stack);
        };

        ENTITY_COMPONENT_MAP_DECLARE("Inventory", InventoryComponent)
    }

    template<>
    struct ObjectMenuTraits<Ent::InventoryComponent>
    {
        static Ent::InventoryComponent::Container& RetrieveContainer(Ent::InventoryComponent &list) { return list.container; }
        static ItemStack& Unpack(Ent::InventoryComponent::Container::iterator itr) { return *itr; }
        static const ItemStack& Unpack(Ent::InventoryComponent::Container::const_iterator itr) { return *itr; }
    };
}