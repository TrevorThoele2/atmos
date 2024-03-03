#pragma once

namespace Atmos
{
    namespace Ini
    {
        enum class ID
        {
            WINDOWED = 1,
            RESOLUTION,
            FPS,
            VSYNC,

            MASTER_SOUND = VSYNC + 2,

            MOVE_LEFT = MASTER_SOUND + 2,
            MOVE_UP,
            MOVE_RIGHT,
            MOVE_DOWN,
            USE,
            MENU_LEFT,
            MENU_UP,
            MENU_RIGHT,
            MENU_DOWN,
            SELECT_CHARACTER_LEFT,
            SELECT_CHARACTER_RIGHT,
            INVENTORY,
            END_TURN,
            ATTACK,
            SPELLS,
            CANCEL,
            STATS
        };
    }
}