
#include "GroundItemHandler.h"

#include "MainGame.h"
#include "Battle.h"

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GroundItemHandler::Pile)
    {
        scribe(items);
    }

    GroundItemHandler::Pile::Pile() : sprite("bag.png", 1, Atmos::Color())
    {}

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GroundItemHandler)
    {
        scribe(piles);
    }

    GroundItemHandler::GroundItemHandler(Field &field) : FieldComponent(field)
    {}

    GroundItemHandler::GroundItemHandler(Field &field, GroundItemHandler &&arg) : FieldComponent(field), piles(std::move(arg.piles))
    {}

    GroundItemHandler& GroundItemHandler::operator=(GroundItemHandler &&arg)
    {
        piles = std::move(arg.piles);
        return *this;
    }

    void GroundItemHandler::DropItem(const GridPosition &position, ItemStack &stack)
    {
        if (stack.GetCount() == 0)
            return;

        auto found = piles.find(position);
        // If no pile exists here...
        if (found == piles.end())
        {
            // Create a new pile here
            found = piles.emplace(position, Pile()).first;
        }

        if (mainGameState.IsTop())
        {
            // Concatenate the item into existing stacks if possible
            for (auto &loop : found->second.items)
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

            found->second.items.push_back(stack);
        }
        else if(battleState.IsTop())
        {
            // Create a new stack for each count of item
            ItemStack::CountT loop;
            while (stack.GetCount() > 0)
            {
                found->second.items.push_back(ItemStack(stack.GetWrapped(), ItemStack::CountT(1)));
                stack.DecrementCount();
            }
        }
    }

    std::vector<ItemStack> GroundItemHandler::PickUpItems(const GridPosition &position)
    {
        typedef std::vector<ItemStack> RetT;
        RetT ret;

        // If can't find a pile at this position, leave
        auto found = piles.find(position);
        if (found == piles.end())
            return ret;

        if (mainGameState.IsTop())
        {
            // Return the entire pile while destroying it
            for (auto loop = found->second.items.begin(); loop != found->second.items.end();)
            {
                ret.push_back(std::move(*loop));
                loop = found->second.items.erase(loop);
            }

            piles.erase(found);
        }
        else if (battleState.IsTop())
        {
            // Return the front object (creates a FIFO)
            ret.push_back(std::move(*found->second.items.begin()));
            if (found->second.items.empty())
                piles.erase(found);
        }

        return ret;
    }
}