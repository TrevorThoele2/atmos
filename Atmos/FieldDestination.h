
#pragma once

#include "FieldID.h"
#include "GridPosition.h"
#include "Direction.h"
#include "Serialization.h"

namespace Atmos
{
    class Tile;
    class Field;
    class FieldDestination
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
    public:
        Direction dir;
        GridPosition pos;
        FieldID id;

        FieldDestination() = default;
        FieldDestination(const Direction &dir, const GridPosition &pos, FieldID id);
        FieldDestination(const FieldDestination &arg) = default;
        FieldDestination& operator=(const FieldDestination &arg) = default;
    };
}