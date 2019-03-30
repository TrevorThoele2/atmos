#pragma once

#include "BattleActionManager.h"
#include "BattlePattern.h"

namespace Atmos
{
    /*
    namespace Battle
    {
        class ActionMovement : public ActionManager
        {
        private:
            class RangePattern : public Pattern<Sprite>
            {
            private:
                void SetPiecePosition(const GridPosition &center, const RelativeGridPosition &relPos, ValueT &value) override;
            };

            RangePattern rangePattern;

            void StartImpl() override;
            void StopImpl() override;
            bool CanStartImpl() const override;
            void OnDeselectEntityImpl() override;

            void OnActionActiveImpl(const Input::Action &arg) override;
            void OnActionPressedImpl(const Input::Action &arg) override;

            void ConstructRangePattern();
        public:
        };
    }
    */
}