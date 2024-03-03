
#include "Movement.h"

namespace Atmos
{
    namespace affecter
    {
        void MovementEngine2D::ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta)
        {
            typedef Position2D::ValueT ValueT;
            func.Execute(Position2D(static_cast<ValueT>((ending.GetX() * delta) + (starting.GetX() * (1 - delta))), static_cast<ValueT>((ending.GetY() * delta) + (starting.GetY() * (1 - delta)))));
        }

        bool MovementEngine2D::IsActiveImpl() const
        {
            return !GetTimer().HasReachedGoal();
        }

        void MovementEngine2D::OnDone(const FunctionT &func)
        {
            func.Execute(ending);
        }

        MovementEngine2D::MovementEngine2D(FunctionT &&function, const Position2D &starting, const Position2D &ending) : Engine(std::move(function)), starting(starting), ending(ending)
        {}

        void MovementEngine3D::ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta)
        {
            typedef Position3D::ValueT ValueT;
            func.Execute(Position3D(static_cast<ValueT>((ending.GetX() * delta) + (starting.GetX() * (1 - delta))), static_cast<ValueT>((ending.GetY() * delta) + (starting.GetY() * (1 - delta))), static_cast<ValueT>((ending.GetZ() * delta) + (starting.GetZ() * (1 - delta)))));
        }

        bool MovementEngine3D::IsActiveImpl() const
        {
            return !GetTimer().HasReachedGoal();
        }

        void MovementEngine3D::OnDone(const FunctionT &func)
        {
            func.Execute(ending);
        }

        MovementEngine3D::MovementEngine3D(FunctionT &&function, const Position3D &starting, const Position3D &ending) : Engine(std::move(function)), starting(starting), ending(ending)
        {}
    }
}