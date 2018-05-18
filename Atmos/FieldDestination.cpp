
#include "FieldDestination.h"

#include "Entity.h"
#include "Field.h"
#include "WorldManager.h"
#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(FieldDestination) : INSCRIPTION_TABLE_GET_MEM(dir), INSCRIPTION_TABLE_GET_MEM(pos), INSCRIPTION_TABLE_GET_MEM(id)
    {}

    FieldDestination::FieldDestination(const Direction &dir, const GridPosition &pos, FieldID id) : dir(dir), pos(pos), id(id)
    {}
}

namespace inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::FieldDestination)
        INSCRIPTION_TABLE_ADD(dir)
        INSCRIPTION_TABLE_ADD(pos)
        INSCRIPTION_TABLE_ADD(id)
    INSCRIPTION_TABLE_END
}