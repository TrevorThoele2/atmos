
#include "FieldDestination.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(FieldDestination)
    {
        scribe(dir);
        scribe(pos);
        scribe(id);
    }

    FieldDestination::FieldDestination(const Direction &dir, const GridPosition &pos, FieldID id) : dir(dir), pos(pos), id(id)
    {}
}