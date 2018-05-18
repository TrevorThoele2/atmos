#pragma once

#include "FieldComponent.h"
#include "ItemStack.h"
#include "GridPosition.h"

#include "Serialization.h"

namespace Atmos
{
    // Handles items which are on the ground in a field
    class GroundItemHandler : public FieldComponent
    {
    private:
        // Represents a pile of items
        class Pile
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            Sprite sprite;
            std::vector<ItemStack> items;

            Pile();
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        std::unordered_map<GridPosition, Pile> piles;
    public:
        GroundItemHandler(Field &field);
        GroundItemHandler(Field &field, GroundItemHandler &&arg);
        GroundItemHandler& operator=(GroundItemHandler &&arg);

        // In the main game, will concatenate all identical stacks
        // In a battle, will always drop a new stack
        void DropItem(const GridPosition &position, ItemStack &stack);
        // In the main game, will pick up everything on the tile (expect potentially more than one item stack)
        // In a battle, will only pick up a single item (expect only one item stack)
        std::vector<ItemStack> PickUpItems(const GridPosition &position);
    };
}