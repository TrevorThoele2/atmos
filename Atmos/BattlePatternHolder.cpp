
#include "BattlePatternHolder.h"

#include <Inscription/Vector.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(BattlePatternHolder)
    {
        scribe(registry);
        scribe(pieces);
    }

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(BattlePatternHolder::Piece)
    {
        scribe(selectionSprite);
        scribe(power);
        scribe(chain);
    }

    BattlePatternHolder::Piece::Piece(const Sprite &selectionSprite, Power power, const CombatChain &chain) : selectionSprite(selectionSprite), power(power), chain(chain)
    {}

    BattlePatternHolder::Piece::Piece(Piece &&arg) : selectionSprite(std::move(arg.selectionSprite)), power(std::move(arg.power)), chain(std::move(arg.chain))
    {}

    BattlePatternHolder::Piece& BattlePatternHolder::Piece::operator=(Piece &&arg)
    {
        selectionSprite = std::move(arg.selectionSprite);
        power = std::move(arg.power);
        chain = std::move(arg.chain);
        return *this;
    }

    bool BattlePatternHolder::Piece::operator==(const Piece &arg) const
    {
        return selectionSprite == arg.selectionSprite && power == arg.power && chain == arg.chain;
    }

    bool BattlePatternHolder::Piece::operator!=(const Piece &arg) const
    {
        return !(*this == arg);
    }

    bool BattlePatternHolder::operator==(const BattlePatternHolder &arg) const
    {
        return registry == arg.registry && pieces == arg.pieces;
    }

    bool BattlePatternHolder::operator!=(const BattlePatternHolder &arg) const
    {
        return !(*this == arg);
    }
}