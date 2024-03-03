
#pragma once

namespace Atmos
{
    enum class EquipSlot
    {
        NONE = 0,
        MAINHAND = 1 << 0,
        OFFHAND = 1 << 1,
        HEAD = 1 << 2,
        BODY = 1 << 3,
        LEGS = 1 << 4,
        FEET = 1 << 5,
        HANDS = 1 << 6,
        BACK = 1 << 7,
        AMMO = 1 << 8,
        SHARD = 1 << 9
    };
}