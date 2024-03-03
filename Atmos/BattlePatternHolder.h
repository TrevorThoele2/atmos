#pragma once

#include <vector>

#include "RangePattern.h"

#include "Sprite.h"
#include "CombatChain.h"
#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class BattlePatternHolder
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    public:
        class Piece
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            typedef FixedPoint64 Power;
        public:
            Sprite selectionSprite;
            // Basically a percentage rating
            Power power;
            CombatChain chain;

            Piece() = default;
            Piece(const Sprite &selectionSprite, Power power, const CombatChain &chain);
            Piece(const Piece &arg) = default;
            Piece(Piece &&arg);
            Piece& operator=(Piece &&arg);
            Piece& operator=(const Piece &arg) = default;
            bool operator==(const Piece &arg) const;
            bool operator!=(const Piece &arg) const;
        };
    public:
        typedef std::vector<Piece> PieceRegistry;
        PieceRegistry registry;

        typedef RelativeRangePattern<Piece*> PatternT;
        PatternT pieces;

        BattlePatternHolder() = default;
        BattlePatternHolder(const BattlePatternHolder &arg) = default;
        BattlePatternHolder& operator=(const BattlePatternHolder &arg) = default;

        bool operator==(const BattlePatternHolder &arg) const;
        bool operator!=(const BattlePatternHolder &arg) const;
    };
}