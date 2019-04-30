
#include "InventoryComponent.h"

namespace Atmos
{
    namespace Entity
    {
        InventoryComponent::InventoryComponent(ObjectManager& manager, EntityReference reference) :
            Component(manager, reference)
        {}

        InventoryComponent::InventoryComponent(const ::Inscription::Table<InventoryComponent>& table) : INSCRIPTION_TABLE_GET_BASE(Component)
        {}

        ItemStack* InventoryComponent::Add(ItemReference itemSource, const ItemStack::Count& count)
        {
            if (count == 0)
                return nullptr;

            return Add(ItemStack(itemSource, count));
        }

        ItemStack* InventoryComponent::Add(ItemStack&& add)
        {
            Combine(add);
            if (add.count == 0)
                return nullptr;

            container.AddBack(std::move(add));
            return &container.back();
        }

        ItemStack* InventoryComponent::Find(Container::ID position)
        {
            auto found = container.Find(position);
            if (found == container.end())
                return nullptr;

            return &*found;
        }

        void InventoryComponent::Combine(ItemStack& stack)
        {
            if (stack.count == 0)
                return;

            for (auto& loop : container)
            {
                if (loop.itemSource == stack.itemSource && loop.count < ItemStack::maxCount)
                {
                    auto prevCount = loop.count;
                    loop.count += stack.count;
                    stack.count -= loop.count - prevCount;
                }

                if (stack.count == 0)
                    return;
            }
        }

        void InventoryComponent::Remove(Container::ID index)
        {
            container.Remove(index);
        }

        bool InventoryComponent::IsEmpty() const
        {
            return container.empty();
        }

        bool InventoryComponent::IsHere(ItemStack& check) const
        {
            for (auto& loop : container)
            {
                if (loop == check)
                    return true;
            }

            return false;
        }

        void InventoryComponent::Move(ItemStack& stack, TypedObjectReference<InventoryComponent> moveTo)
        {
            for (auto loop = container.begin(); loop != container.end(); ++loop)
            {
                if (&*loop == &stack)
                {
                    moveTo->Add(std::move(*loop));
                    container.Remove(loop);
                    return;
                }
            }
        }

        bool InventoryComponent::AttemptConsume(ItemStack& stack)
        {
            /*
            if (!stack->HasConsumableAspect())
                return false;

            stack->consumableAspect->Affect(GetOwnerEntity());
            stack.DecrementCount();
            for (auto loop = container.begin(); loop != container.end(); ++loop)
            {
                if (loop->GetCount() == 0)
                    container.erase(loop);
            }

            return true;
            */

            return true;
        }

        bool InventoryComponent::AttemptEquip(EquipSlot slot, ItemStack &stack)
        {
            /*
            if (!stack->HasEquippableAspect())
                return false;

            if (!stack->equippableAspect->equipRegion.Has(slot))
                return false;

            auto combat = GetCurrentEntities()->FindComponent<CombatComponent>(GetOwnerEntity());
            if (!combat)
                return false;

            if (!combat->CanEquip(slot))
                return false;

            combat->Equip(slot, *stack);
            stack.DecrementCount();
            return true;
            */
            return true;
        }

        size_t InventoryComponent::GetTotalCount() const
        {
            size_t ret = 0;

            for (auto& loop : container)
                ret += loop.count.Value();

            return ret;
        }

        ObjectTypeDescription InventoryComponent::TypeDescription() const
        {
            return ObjectTraits<InventoryComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Entity::InventoryComponent>::typeName = "InventoryComponent";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::Entity::InventoryComponent)
    {
        INSCRIPTION_TABLE_ADD(container);
    }
}