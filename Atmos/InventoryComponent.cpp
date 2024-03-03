
#include "InventoryComponent.h"
#include "Entity.h"

#include "Item.h"
#include "CombatComponent.h"

#include "CurrentField.h"

#include <Inscription\Memory.h>
#include <Inscription\UnorderedMap.h>

namespace Atmos
{
    namespace Ent
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(InventoryComponent)
        {
            scribe(container);
        }

        InventoryComponent::InventoryComponent(const InventoryComponent &arg) : Component(arg), container(arg.container)
        {}

        InventoryComponent::InventoryComponent(InventoryComponent &&arg) : Component(std::move(arg)), container(std::move(arg.container))
        {}

        InventoryComponent& InventoryComponent::operator=(const InventoryComponent &arg)
        {
            Component::operator=(arg);
            container = arg.container;
            return *this;
        }

        InventoryComponent& InventoryComponent::operator=(InventoryComponent &&arg)
        {
            Component::operator=(std::move(arg));
            container = std::move(arg.container);
            return *this;
        }

        bool InventoryComponent::operator==(const InventoryComponent &arg) const
        {
            return container == arg.container;
        }

        bool InventoryComponent::operator!=(const InventoryComponent &arg) const
        {
            return !(*this == arg);
        }

        ItemStack* InventoryComponent::Add(const Name &name, const ItemStack::CountT &count)
        {
            if (count == 0)
                return nullptr;

            return Add(ItemStack(*Registry<Item>::Find(name), count));
        }

        ItemStack* InventoryComponent::Add(ItemStack &&add)
        {
            Combine(add);
            if (add.GetCount() == 0)
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

        void InventoryComponent::Combine(ItemStack &stack)
        {
            if (stack.GetCount() == 0)
                return;

            for (auto &loop : container)
            {
                if (loop.GetName() == stack.GetName() && loop.GetCount() < ItemStack::maxCount)
                {
                    auto prevCount = loop.GetCount();
                    loop.IncrementCount(stack.GetCount());
                    stack.DecrementCount(loop.GetCount() - prevCount);
                }

                if (stack.GetCount() == 0)
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

        bool InventoryComponent::IsHere(ItemStack &check) const
        {
            for (auto &loop : container)
            {
                if (loop == check)
                    return true;
            }

            return false;
        }

        void InventoryComponent::Move(ItemStack &stack, InventoryComponent &moveTo)
        {
            for (auto loop = container.begin(); loop != container.end(); ++loop)
            {
                if (&*loop == &stack)
                {
                    moveTo.Add(std::move(*loop));
                    container.Remove(loop);
                    return;
                }
            }
        }

        bool InventoryComponent::AttemptConsume(ItemStack &stack)
        {
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
        }

        bool InventoryComponent::AttemptEquip(EquipSlot slot, ItemStack &stack)
        {
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
        }

        size_t InventoryComponent::GetTotalCount() const
        {
            size_t ret = 0;

            for (auto &loop : container)
                ret += loop.GetCount().Get();

            return ret;
        }
    }
}