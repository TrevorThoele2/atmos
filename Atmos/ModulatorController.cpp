
#include "ModulatorController.h"

#include "Modulator.h"

namespace Atmos
{
    namespace Modulator
    {
        void Controller::Attach(const Observer &attach)
        {
            if (IsWorking(attach))
                return;

            observers.push_back(attach);
        }

        void Controller::Detach(const Observer &detach)
        {
            for (auto loop = observers.begin(); loop != observers.end(); ++loop)
            {
                if (detach == *loop)
                {
                    observers.erase(loop);
                    return;
                }
            }
        }

        void Controller::Detach(const ModulatorBase &detach)
        {
            for (auto loop = observers.begin(); loop != observers.end(); ++loop)
            {
                if (&detach == loop->Get())
                {
                    observers.erase(loop);
                    return;
                }
            }
        }

        void Controller::Work()
        {
            for (auto &loop : observers)
                loop->Work();
        }

        bool Controller::IsWorking(const Observer &check) const
        {
            for (auto &loop : observers)
            {
                if (check == loop)
                    return true;
            }

            return false;
        }
    }
}