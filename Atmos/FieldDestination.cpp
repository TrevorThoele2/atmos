
#include "FieldDestination.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    FieldDestination::FieldDestination(const Direction& direction, const GridPosition& position, FieldID id) :
        direction(direction), position(position), id(id)
    {}

    INSCRIPTION_BINARY_SERIALIZE_FUNCTION_DEFINE(FieldDestination)
    {
        scribe(direction);
        scribe(position);
        scribe(id);
    }
}