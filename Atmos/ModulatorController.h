#pragma once

#include <vector>

#include "ModulatorObserver.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorBase;

        class Controller
        {
        private:
            std::vector<Observer> observers;
        public:
            void Attach(const Observer &attach);
            void Detach(const Observer &detach);
            void Detach(const ModulatorBase &detach);
            void Work();
            bool IsWorking(const Observer &check) const;
        };
    }
}