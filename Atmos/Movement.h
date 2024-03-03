
#pragma once

#include "Position2D.h"
#include "Position3D.h"
#include <Affecter\Engine.h>

namespace Atmos
{
    namespace affecter
    {
        class MovementEngine2D : public ::affecter::Engine<const Position2D&>
        {
        private:
            Position2D starting, ending;
            void ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta) override;
            bool IsActiveImpl() const override;
            void OnDone(const FunctionT &func) override;
        protected:
            MovementEngine2D(FunctionT &&function, const Position2D &starting, const Position2D &ending);
        };

        class MovementEngine3D : public ::affecter::Engine<const Position3D&>
        {
        private:
            Position3D starting, ending;
            void ExecuteImpl(const FunctionT &func, ::affecter::DeltaT delta) override;
            bool IsActiveImpl() const override;
            void OnDone(const FunctionT &func) override;
        protected:
            MovementEngine3D(FunctionT &&function, const Position3D &starting, const Position3D &ending);
        };
    }
}