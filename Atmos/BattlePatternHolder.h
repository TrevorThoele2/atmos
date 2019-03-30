#pragma once

#include <vector>

#include "RangePattern.h"

#include "ScriptInstance.h"
#include "Sprite.h"
#include "FixedPoint.h"

#include "Serialization.h"

namespace Atmos
{
    class BattlePatternHolder
    {
    public:
        class Piece
        {
        public:
            typedef TypedObjectReference<nSprite> SpriteReference;
            SpriteReference selectionSprite;
        public:
            typedef TypedObjectReference<ScriptInstance> ScriptReference;
            ScriptReference action;
        public:
            typedef FixedPoint64 Power;
            Power powerPercentage;
        public:
            Piece() = default;
            Piece(const Piece &arg) = default;
            Piece& operator=(const Piece &arg) = default;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        };
    public:
        typedef std::vector<Piece> PieceRegistry;
        PieceRegistry registry;

        typedef RelativeRangePattern<Piece*> PatternT;
        PatternT pieces;
    public:
        BattlePatternHolder() = default;
        BattlePatternHolder(const BattlePatternHolder &arg) = default;
        BattlePatternHolder& operator=(const BattlePatternHolder &arg) = default;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}