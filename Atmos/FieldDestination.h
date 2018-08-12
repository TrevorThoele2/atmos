
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
        INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(FieldDestination, private);
    private:
        FieldDestination() = delete;
    public:
        Direction dir;
        GridPosition pos;
        FieldID id;

        FieldDestination(const Direction &dir, const GridPosition &pos, FieldID id);
        FieldDestination(const FieldDestination &arg) = default;
        FieldDestination& operator=(const FieldDestination &arg) = default;
    };
}

namespace Inscription
{
    template<>
    class Inscripter<::Atmos::FieldDestination> : public InscripterBase<::Atmos::FieldDestination>
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
    };
}