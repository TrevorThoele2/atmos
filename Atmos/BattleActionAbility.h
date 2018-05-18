#pragma once

#include "BattleActionManager.h"
#include "BattlePattern.h"

namespace Atmos
{
    namespace Battle
    {
        class ActionAbility : public ActionManager
        {
        private:
            class AttackPattern : public Pattern<BattlePatternHolder::Piece>
            {
            private:
                void SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value) override;
            };

            class RangePattern : public Pattern<Sprite>
            {
            private:
                void SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value) override;
            };
        private:
            RangePattern rangePattern;
            AttackPattern attackPattern;

            void StartImpl() override;
            void StopImpl() override;
            bool CanStartImpl() const override;
            void OnDeselectEntityImpl() override;

            void OnActionPressedImpl(const Input::Action &arg) override;
        public:
            ActionAbility();
        };
    }
}