
#include "ModulatorController.h"

#include "Modulator.h"

namespace Atmos
{
    namespace Modulator
    {
        bool Controller::IsIn(const Observer &check) const
        {
            for (auto &loop : observers)
                if (loop == check)
                    return true;

            return false;
        }

        Controller::ID Controller::Attach(const Observer &attach)
        {
            if (IsIn(attach))
                return observers.nullID;

            return observers.Add(attach);
        }

        void Controller::Detach(ID detach)
        {
            observers.Remove(detach);
        }

        void Controller::Detach(const Observer &detach)
        {
            for (auto loop = observers.begin(); loop != observers.end(); ++loop)
            {
                if (detach == *loop)
                {
                    observers.Remove(loop);
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
                    observers.Remove(loop);
                    return;
                }
            }
        }

        Observer Controller::Find(ID find) const
        {
            auto found = observers.Find(find);
            if (found == observers.end())
                return Observer();

            return *found;
        }

        void Controller::Work()
        {
            for (auto loop = observers.begin(); loop != observers.end();)
            {
                if ((*loop)->Work())
                    loop = observers.Remove(loop);
                else
                    ++loop;
            }
        }
    }
}