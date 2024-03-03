
#include "InventoryComponent.h"

namespace Atmos
{
    namespace Ent
    {
        nInventoryComponent::nInventoryComponent(EntityReference reference) : nEntityComponent(reference)
        {}

        nInventoryComponent::nInventoryComponent(const ::Inscription::Table<nInventoryComponent>& table) : INSCRIPTION_TABLE_GET_BASE(nEntityComponent)
        {}

        ItemStack* nInventoryComponent::Add(ItemReference itemSource, const ItemStack::Count& count)
        {
            if (count == 0)
                return nullptr;

            return Add(ItemStack(itemSource, count));
        }

        ItemStack* nInventoryComponent::Add(ItemStack&& add)
        {
            Combine(add);
            if (add.count == 0)
                return nullptr;

            container.AddBack(std::move(add));
            return &container.back();
        }

        ItemStack* nInventoryComponent::Find(Container::ID position)
        {
            auto found = container.Find(position);
            if (found == container.end())
                return nullptr;

            return &*found;
        }

        void nInventoryComponent::Combine(ItemStack& stack)
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

        void nInventoryComponent::Remove(Container::ID index)
        {
            container.Remove(index);
        }

        bool nInventoryComponent::IsEmpty() const
        {
            return container.empty();
        }

        bool nInventoryComponent::IsHere(ItemStack& check) const
        {
            for (auto& loop : container)
            {
                if (loop == check)
                    return true;
            }

            return false;
        }

        void nInventoryComponent::Move(ItemStack& stack, TypedObjectReference<nInventoryComponent> moveTo)
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

        bool nInventoryComponent::AttemptConsume(ItemStack& stack)
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

        bool nInventoryComponent::AttemptEquip(EquipSlot slot, ItemStack &stack)
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

        size_t nInventoryComponent::GetTotalCount() const
        {
            size_t ret = 0;

            for (auto& loop : container)
                ret += loop.count.Value();

            return ret;
        }

        ObjectTypeDescription nInventoryComponent::TypeDescription() const
        {
            return ObjectTraits<nInventoryComponent>::TypeDescription();
        }
    }

    const ObjectTypeName ObjectTraits<Ent::nInventoryComponent>::typeName = "InventoryComponent";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Ent::nInventoryComponent)
    {
        INSCRIPTION_TABLE_ADD(container);
    }
}