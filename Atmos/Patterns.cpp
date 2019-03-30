
#include "Patterns.h"
#include "StatusEffect.h"

namespace Atmos
{
    GraphicalPatternEntry::GraphicalPatternEntry(SpriteReference sprite) : sprite(sprite)
    {}

    bool GraphicalPatternEntry::operator==(const GraphicalPatternEntry &arg) const
    {
        return sprite == arg.sprite;
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GraphicalPatternEntry)
    {
        scribe(sprite);
    }
}