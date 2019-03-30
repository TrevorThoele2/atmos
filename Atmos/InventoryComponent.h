#pragma once

#include "nEntityComponent.h"

#include "ItemStack.h"

#include "EquipSlot.h"
#include "RandomAccessSequence.h"

#include "ObjectSerialization.h"

namespace Atmos
{
    namespace Ent
    {
        class nInventoryComponent : public nEntityComponent
        {
        public:
            typedef RandomAccessSequence<ItemStack> Container;
            Container container;
        public:
            typedef TypedObjectReference<nItem> ItemReference;
        public:
            nInventoryComponent(EntityReference reference);
            nInventoryComponent(const nInventoryComponent& arg) = default;
            nInventoryComponent(const ::Inscription::Table<nInventoryComponent>& table);

            ItemStack* Add(ItemReference itemSource, const ItemStack::Count& count);
            ItemStack* Add(ItemStack&& add);
            ItemStack* Find(Container::ID position);
            // This will push the stack into other identical stacks until all other stacks are full or this stack is out
            void Combine(ItemStack& stack);
            void Remove(Container::ID index);
            bool IsEmpty() const;
            bool IsHere(ItemStack& check) const;

            void Move(ItemStack& stack, TypedObjectReference<nInventoryComponent> moveTo);
            bool AttemptConsume(ItemStack& stack);
            bool AttemptEquip(EquipSlot slot, ItemStack& stack);

            size_t GetTotalCount() const;

            ObjectTypeDescription TypeDescription() const override;
        private:
            INSCRIPTION_ACCESS;
        };
    }

    template<>
    struct ObjectTraits<Ent::nInventoryComponent> : ObjectTraitsBase<Ent::nInventoryComponent>
    {
        static const ObjectTypeName typeName;
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::Ent::nInventoryComponent)
    {
    public:
        static void AddMembers(TableT& table);
    };
}