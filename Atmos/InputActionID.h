#pragma once

namespace Atmos::Input
{
    enum class ActionID
    {
        NONE,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        USE,
        NAVIGATE_MENU_UP,
        NAVIGATE_MENU_DOWN,
        NAVIGATE_MENU_LEFT,
        NAVIGATE_MENU_RIGHT,
        CHANGE_SELECTED_CHARACTER_LEFT,
        CHANGE_SELECTED_CHARACTER_RIGHT,
        INVENTORY,
        END_TURN,
        ATTACK,
        OPEN_SPELLS,
        CANCEL,
        STATS
    };
}