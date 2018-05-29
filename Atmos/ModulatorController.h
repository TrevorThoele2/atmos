#pragma once

#include <vector>
#include "ModulatorObserver.h"
#include "IDManager.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorBase;

        class Controller
        {
        public:
            typedef IDManagerBase::ID ID;
        private:
            IDManager<std::unordered_map<ID, Observer>> observers;
            bool IsIn(const Observer &check) const;
        public:
            // Modulators which are just being attached are classified as being staged; you must start them explicitly
            ID Attach(const Observer &attach);
            void Detach(ID detach);
            void Detach(const Observer &detach);
            void Detach(const ModulatorBase &detach);
            Observer Find(ID find) const;
            void Work();

            bool IsModulatorWorkingObject(void *obj) const;
        };
    }
}