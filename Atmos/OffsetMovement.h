#pragma once

/*
#include "ObjectOffsetHandle.h"
#include <Affecter/Engine.h>

namespace Atmos
{
    namespace affecter
    {
        template<class Position>
        class OffsetMovementEngine3D : public ::affecter::Engine<const ObjectOffset<Position>&>
        {
        public:
            typedef Position PositionT;
            typedef ObjectOffset<PositionT> OffsetT;
        private:
            OffsetT starting, ending;
            void ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta) override;
            bool IsActiveImpl() const override;
            void OnDone(const FunctionT &func) override;
        protected:
            OffsetMovementEngine3D(FunctionT &&function, const OffsetT &starting, const OffsetT &ending);
        };

        template<class Position>
        void OffsetMovementEngine3D<Position>::ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta)
        {
            typedef typename OffsetT::ValueT ValueT;
            func.Execute(PositionT(static_cast<ValueT>((ending.x * delta) + (starting.x * (1 - delta))), static_cast<ValueT>((ending.y * delta) + (starting.y * (1 - delta))), static_cast<ValueT>((ending.z * delta) + (starting.z * (1 - delta)))));
        }

        template<class Position>
        bool OffsetMovementEngine3D<Position>::IsActiveImpl() const
        {
            return !GetTimer().HasReachedGoal();
        }

        template<class Position>
        void OffsetMovementEngine3D<Position>::OnDone(const FunctionT &func)
        {
            func.Execute(ending);
        }

        template<class Position>
        OffsetMovementEngine3D<Position>::OffsetMovementEngine3D(FunctionT &&function, const OffsetT &starting, const OffsetT &ending) : Engine(std::move(function)), starting(starting), ending(ending)
        {}
    }
}
*/