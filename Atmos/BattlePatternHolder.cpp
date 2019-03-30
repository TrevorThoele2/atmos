
#include "BattlePatternHolder.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(BattlePatternHolder::Piece)
    {
        scribe(selectionSprite);
        scribe(action);
        scribe(powerPercentage);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(BattlePatternHolder)
    {
        scribe(registry);
        scribe(pieces);
    }
}