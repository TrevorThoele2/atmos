
#include "Patterns.h"
#include "StatusEffect.h"

namespace Atmos
{
    INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(GraphicalPatternEntry) : INSCRIPTION_TABLE_GET_MEM(sprite)
    {}

    GraphicalPatternEntry::GraphicalPatternEntry(Sprite &&sprite) : sprite(std::move(sprite))
    {}

    bool GraphicalPatternEntry::operator==(const GraphicalPatternEntry &arg) const
    {
        return sprite == arg.sprite;
    }
}

namespace inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::GraphicalPatternEntry)
        INSCRIPTION_TABLE_ADD(sprite)
    INSCRIPTION_TABLE_END
}